#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#include "common.h"

struct termios oldterm;
struct termios newterm;
int	old_disc;

#if !defined(N_HDLC)
#define N_HDLC 13
#endif

static int write_timed(int fd, void *buf, int size, int timeout, int *bUserAbort );
static int read_timed(int fd, void *buf, int size, int timeout, int *bUserAbort );

/* output message to console and log file
 *
 * input:	fmt	output format string
 *		...	variable number of output arguments
 *
 * returns:	0 if success, otherwise, error code
 */
void Log( char *fmt, ... )
{
	struct	timeval tv;
	struct	tm *ptm;
	char	TimeOfDay[20];
	va_list	marker;

	static	int first_time = 1;
	static	int stdout_tty = 0;

	if ( first_time )
	{
		stdout_tty = isatty( fileno(stdout) );
		first_time = 0;
	}

	va_start( marker, fmt );

	gettimeofday( &tv, NULL );
	ptm = localtime( (time_t*)&tv.tv_sec );

	sprintf(TimeOfDay, "%02u:%02u:%02u.%06lu ",
		ptm->tm_hour, ptm->tm_min, ptm->tm_sec, tv.tv_usec );

	printf( TimeOfDay );
	vprintf( fmt, marker );

	if ( !stdout_tty )
	{
		fprintf( stderr, TimeOfDay );
		vfprintf( stderr, fmt, marker );
	} 

	va_end( marker );
}

/* Get device statistics
 * input:	pAppData	pointer to application data
 * returns:	0 if successful, otherwise, error code
 */
int Rt56GetStats(PAPP_DATA pAppData, struct r56_icount *picounts )
{
	int rc = ioctl(pAppData->hDevice, R56_IOCGSTATS, picounts );
	if (rc < 0) {
		Log("unable to get statistics: (%d) %s\n",
			errno, strerror(errno) );
	}
	return rc;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* void DumpFrame()
*
* This routine simply outputs the specified buffer as hex bytes with the
* associated ASCII character representations. Undisplayable characters
* are display as '.'
*
* Notes:
*
* input:
*
* 	char *pPrefix                       // output string prefix (such as "Rx"/"Tx")
* 	unsigned char * pBuffer                       // pointer to buffer for output
* 	int BufferLength                    // size of this buffer
*
* returns:
*
* 	nothing
*-----------------------------------------------------------------------------*/
void DumpFrame( char *pPrefix, unsigned char * pBuffer, int BufferLength )
{
	int	Counter = 0;
	int	ByteCounter = 0;
	char	HexBuffer[ 16*3 + 1 ];
	char	*pHexBuffer = HexBuffer;

	char	AscBuffer[ 16 + 1 ];
	char	*pAscBuffer = AscBuffer;

	while ( BufferLength )
	{
		pHexBuffer += sprintf( pHexBuffer, " %02x", *pBuffer );
		*pAscBuffer++ = *pBuffer >= ' ' && *pBuffer <= '~' ? *pBuffer : '.' ;
		*pAscBuffer = '\0';

		++Counter;
		++pBuffer;
		--BufferLength;

		if ( Counter >= 16 )
		{
			Log(" %s (%08x) %-*s | %-*s |\n",
					pPrefix,
					ByteCounter,
					sizeof(HexBuffer)-1, HexBuffer,
					sizeof(AscBuffer)-1, AscBuffer );

			ByteCounter += Counter;
			pHexBuffer = HexBuffer;
			pAscBuffer = AscBuffer;
			Counter = 0;
		}
	}

	if ( Counter )
	{
		Log(" %s (%08x) %-*s | %-*s |\n",
				pPrefix,
				ByteCounter,
				sizeof(HexBuffer)-1, HexBuffer,
				sizeof(AscBuffer)-1, AscBuffer );
	}

	Log("\n");
}

/* Allocate common resources required by the application
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int AllocateResources( PAPP_DATA pAppData )
{
	Log("AllocatingResources() ...\n");

	pAppData->pTxBuffer = malloc( MAX_IFRAME_SIZE );
	pAppData->pRxBuffer = malloc( MAX_IFRAME_SIZE );

	if ( pAppData->pTxBuffer == NULL ||
		pAppData->pRxBuffer == NULL )
	{
		Log( "AllocateResources() - malloc() failed\n");
		return FALSE;
	}

	return TRUE;
}

/* Deallocate common resources required by the application
 *
 * input:	pAppData	pointer to application data
 * returns:	Always TRUE
 */
int DeallocateResources( PAPP_DATA pAppData )
{
	Log("DeallocateResources()...\n");

	if ( pAppData->pTxBuffer )
	{
		free( pAppData->pTxBuffer );
		pAppData->pTxBuffer = NULL;
	}

	if ( pAppData->pRxBuffer )
	{
		free( pAppData->pRxBuffer );
		pAppData->pRxBuffer = NULL;
	}

	return TRUE;
}

/* Opens the specified MGHDLC Port and prepares it for use
 *
 * input:	pAppData	pointer to application data
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int AllocatePort( PAPP_DATA pAppData )
{
	struct termios testterm;
	int rc;
#if defined(__FreeBSD__)
	int hdlc_disc = HDLCDISC;	/* HDLC line discipline index */
#else
	int hdlc_disc = N_HDLC;
#endif
	Log("AllocatePort() ...\n");

	/* open the specified device
	 * use O_NONBLOCK so open returns immediately without
	 * regard to the state of the modem signal DCD 
	 */
	pAppData->hDevice = open(pAppData->PortName,O_RDWR | O_NONBLOCK,0);
	if (pAppData->hDevice < 0) {
		Log("failed to open %s: (%d) %s\n",
			pAppData->PortName,errno, strerror(errno) );
		return errno;
	}

	/* get current line discipline */
	rc = ioctl(pAppData->hDevice, TIOCGETD, &old_disc);
	if(rc < 0) {
		Log("ERROR, can't get line discipline error=%d\n",errno);
		goto Rt56OpenError;
	}

	/* set device to HDLCDISC line discipline */
	rc = ioctl(pAppData->hDevice, TIOCSETD, &hdlc_disc);
	if(rc < 0) {
		Log("ERROR, can't set line discipline error=%d\n",errno);
		goto Rt56OpenError;
	}
	
	/* get and save current port attributes */
	tcgetattr(pAppData->hDevice,&oldterm);
	newterm = oldterm;
	
	cfsetospeed(&newterm,B9600);
	cfsetispeed(&newterm,B9600);
	
	/* turn off tty processing to allow raw binary HDLC data */
	/* to be exchanged through the tty interface */
	newterm.c_lflag &= ~(ICANON|ECHO|ECHOCTL|ECHONL|ISIG|NOFLSH|IEXTEN);
#if !defined(__FreeBSD__)	
	newterm.c_oflag &= ~(ONLCR|OPOST|OLCUC|ONOCR|OCRNL|ONLRET);
	newterm.c_iflag &= ~(ICRNL|INPCK|ISTRIP|IUCLC|IXOFF|IXON|IGNCR);
#else
	newterm.c_oflag &= ~(ONLCR|OPOST|OXTABS|ONOEOT);
	newterm.c_iflag &= ~(ICRNL|INPCK|ISTRIP|IXOFF|IXON|IGNCR);
#endif
	
	/* setup control flags to prevent automatic manipulation of the */
	/* modem signals, which is done manually below with ioctl calls */
	newterm.c_cflag &= ~(HUPCL|CRTSCTS);
	newterm.c_cflag |= CLOCAL|CREAD;
	
	/* setup read to return when at least one byte available */
	newterm.c_cc[VMIN] = 1;
	newterm.c_cc[VTIME] = 0;

	/* set new port attributes */	
	tcsetattr(pAppData->hDevice,TCSANOW,&newterm);
	
	/* verify port attributes */
	tcgetattr(pAppData->hDevice,&testterm);
	if (testterm.c_iflag != newterm.c_iflag ||
	    testterm.c_oflag != newterm.c_oflag ||
	    testterm.c_lflag != newterm.c_lflag ||
	    testterm.c_cflag != newterm.c_cflag) {
		Log("can't set port attributes\n");
		goto Rt56OpenError;
	}

	/* set device to blocking mode for reads and writes */	
	fcntl(pAppData->hDevice, F_SETFL, 
		fcntl(pAppData->hDevice,F_GETFL) & ~O_NONBLOCK);

	if ( !SetParams( pAppData ) )
		goto Rt56OpenError;
		
	return TRUE;

Rt56OpenError:
	close( pAppData->hDevice );
	return FALSE;
}

/* Close/Reset the specified MGHDLC Port
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int DeallocatePort( PAPP_DATA pAppData )
{
	int rc = 0;
	Log("DeallocatePort()...\n");

	if ( pAppData->hDevice != -1 )
	{
		/* restore port attributes */
		tcsetattr(pAppData->hDevice,TCSANOW,&oldterm);

		/* restore original line discipline */
		rc = ioctl(pAppData->hDevice, TIOCSETD, &old_disc);
		if(rc < 0) {
			Log("can't restore line discipline: (%d) %s\n",
				errno, strerror(errno) );
		}

		close( pAppData->hDevice );
		pAppData->hDevice = -1;
	}
	return rc == 0 ? TRUE : FALSE;
}

/* Enable/Disable the port's transmitter
 * input:	pAppData	pointer to application data
 *		bEnable		TRUE to enable, FALSE to disable
 * returns:	TRUE if successful, FALSE otherwise
 */
int EnableTransmitter( PAPP_DATA pAppData, int bEnable )
{
	int rc;
	Log("EnableTransmitter(%s)\n", bEnable ? "TRUE" : "FALSE" );
#if defined(__FreeBSD__)	
	rc = ioctl(pAppData->hDevice, R56_IOCTXENABLE, &bEnable );
#else
	rc = ioctl(pAppData->hDevice, R56_IOCTXENABLE, bEnable );
#endif
	if(rc < 0) {
		Log("can't %s transmitter: (%d) %s\n", 
			bEnable?"enable":"disable", errno,strerror(errno) );
	}
	return rc == 0 ? TRUE : FALSE;
}

/* SendFrame()
 *
 * Transmits the frame contained in the pTxBuffer as specified in the input
 * application data structure.
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int SendFrame(PAPP_DATA pAppData)
{
	int rc;
	
	DumpFrame( "TX:", pAppData->pTxBuffer, pAppData->TxBufferSize );
	
	rc = write_timed( pAppData->hDevice, 
			pAppData->pTxBuffer, 
			pAppData->TxBufferSize, 
			pAppData->TxTimeout,	
			&pAppData->bExitApplication );
			
	if (rc<=0) {
		if (rc == 0)
			Log("timeout on send frame\n");
		else		
			Log("error on send frame: (%d) %s\n",
				errno, strerror(errno) ); 
		return FALSE;
	}
	Log("send frame successful\n");
	return TRUE;
}

/* Transmit frame contained in pAppData->pTxBuffer and inform driver
 * to release line after transmission is complete.
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int LoopModeSendLastFrame(PAPP_DATA pAppData)
{
	int rc;
	int FrameSent;
	
	FrameSent = SendFrame(pAppData);	
	
	/* Inform driver to 'release' line by having TxD echo
	 * RxD when current transmission is complete.
	 */
	Log("LoopModeSendLastFrame() - giving driver last frame notification\n");
	rc = ioctl(pAppData->hDevice, R56_IOCLOOPTXDONE, 0);
	if(rc < 0) {
		Log("failed to set LoopMode send done: (%d) %s\n", 
			errno, strerror(errno) );
	}
	
	return FrameSent;
}

/* enables/disables the receiver.
 *
 * input:	pAppData	pointer to application data
 *		bEnable		TRUE to enable receive, FALSE to disable
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int EnableReceiver( PAPP_DATA pAppData, int bEnable )
{
#if defined(__FreeBSD__)
	int rc = ioctl(pAppData->hDevice, R56_IOCRXENABLE, &bEnable);
#else
	int rc = ioctl(pAppData->hDevice, R56_IOCRXENABLE, bEnable);
#endif
	if(rc < 0) {
		Log("can't %s receiver: (%d) %s\n", 
			bEnable?"enable":"disable", errno, strerror(errno) );
		return FALSE;
	}

	return TRUE; 
}

/* CheckReceive()
 *
 * Pass received frames to a specified callback function for
 * processing until a specified amount of time has passed or
 * an error is returned by the callback.
 *
 * input:
 *
 * 	pAppData	pointer to application data
 * 	timeout		max idle time allowed (in milliseconds)
 *	RxCallback	callback for RxFrames
 *
 * returns:
 *
 * 	TRUE if successful, FALSE otherwise
 */
int CheckReceive(PAPP_DATA pAppData, unsigned int timeout, int (*RxCallback)(PAPP_DATA pAppData, unsigned char * pRxBuffer, int BufferSize) )
{
	int rc;
	struct timeval tv,tvstart,tvcur,tvorig;
	
	/* save original timeout */
	tvorig.tv_sec  = timeout/1000; 
	tvorig.tv_usec = (timeout%1000) * 1000;
	tv = tvorig;
	gettimeofday(&tvstart,NULL);

	for(;;) {
		/* wait for a received frame */
		rc = read_timed( pAppData->hDevice, pAppData->pRxBuffer, MAX_IFRAME_SIZE,
				tv.tv_sec*1000 + tv.tv_usec/1000, &pAppData->bExitApplication );
		if (rc < 0) {
			Log("error on rcv frame: (%d) %s\n", errno, strerror(errno));
			return FALSE;
		}
		if (rc == 0)
			break;	/* timeout expired */
			
		/* Filter out received broadcast frames for primary station,
	 	 * these are primary transmit packets that have looped back around.
	 	 */
		if (!pAppData->bPrimary || pAppData->pRxBuffer[ADDR_FIELD] != BROADCAST_ADDR) {
			DumpFrame("RX:", pAppData->pRxBuffer, rc);

			/* Pass frame to call back function */
			if (RxCallback && 
		     	     !(RxCallback)(pAppData, pAppData->pRxBuffer, rc))
				return FALSE;
		}

		/* adjust remaining time */
		gettimeofday(&tvcur,NULL);
		timersub(&tvcur,&tvstart,&tv);
		if (timercmp(&tv,&tvorig,>))
			break;
			
		timersub(&tvorig,&tv,&tv);
	}
	return TRUE;
}

/* Set device parameters
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int SetParams ( PAPP_DATA pAppData )
{
	/* make ioctl call to set current route56 parameters */
	int rc = ioctl(pAppData->hDevice,R56_IOCSPARAMS,&pAppData->Rt56Params);
	if (rc < 0) 
	{
		Log("failed to set HDLC parameters on device %s"
			" failed: (%d) %s\n",
			pAppData->PortName, errno, strerror(errno) );
	}

	return rc == 0 ? TRUE : FALSE ;
}

/* Set transmit idle mode
 *
 * input:	pAppData	pointer to application data
 *		IdleMode	transmit idle mode
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int SetIdle( PAPP_DATA pAppData, unsigned int IdleMode )
{
	int rc;
	static char *achIdleModes[] = 
		{ "HDLC_TXIDLE_FLAGS", "HDLC_TXIDLE_ALT_ZEROS_ONES",
		  "HDLC_TXIDLE_ZEROS", "HDLC_TXIDLE_ONES",
		  "HDLC_TXIDLE_ALT_MARK_SPACE", "HDLC_TXIDLE_SPACE",
		  "HDLC_TXIDLE_MARK" };

	if ( IdleMode >= HDLC_TXIDLE_FLAGS && IdleMode <= HDLC_TXIDLE_MARK )
		Log("SetIdle() - Setting idle to %s\n", achIdleModes[IdleMode] );
	else
		Log("SetIdle() - Setting idle to undefined value of %#x\n", IdleMode );
#if defined(__FreeBSD__)
	rc = ioctl(pAppData->hDevice, R56_IOCSTXIDLE, &IdleMode );
#else
	rc = ioctl(pAppData->hDevice, R56_IOCSTXIDLE, IdleMode );
#endif
	if(rc < 0) 
	{
		Log("can't set idle mode: (%d) %s\n",
			errno, strerror(errno) );
	}
	return rc == 0 ? TRUE : FALSE;
}

/* format and display a test transaction structures
 *
 * input: 	pComment	pointer to prefix comment
 *		pTransaction	pointer to transaction to display
 */
void DumpTransaction( char *pComment, PAPP_TRANSACTION pTransaction )
{
	Log( "\n ** %s: Source %x (%s) - Transaction # %u\n"
			"    Created: %10lu.%06lu\n"
			"   Received: %10lu.%06lu\n"
			"   Returned: %10lu.%06lu\n"
			"  Completed: %10lu.%06lu\n",

			pComment,

			pTransaction->Addr,
			pTransaction->Request ? "Request" : "Response",
			pTransaction->TransactionNumber,

			pTransaction->CreationTimeStamp.tv_sec,
			pTransaction->CreationTimeStamp.tv_usec,
			
			pTransaction->PrimaryReceiptTimeStamp.tv_sec,
			pTransaction->PrimaryReceiptTimeStamp.tv_usec,
			
			pTransaction->PrimaryTimeStamp.tv_sec,
			pTransaction->PrimaryTimeStamp.tv_usec,
			
			pTransaction->CompletionTimeStamp.tv_sec,
			pTransaction->CompletionTimeStamp.tv_usec
	);
}

/* Check to see if user has requested that the application exit
 * determines if the user has requested application exit
 * by using CTRL-C (SIGINT)
 */
int CheckUserExitRequest( PAPP_DATA pAppData )
{
	return pAppData->bExitApplication;
}

/* Reset flag that signals user exit request
 */
void ResetUserExitRequest( PAPP_DATA pAppData )
{
	pAppData->bExitApplication = FALSE;
}

/* write_timed()
 * 
 * 	make write call to file/device with a timeout
 * 	This call blocks (if file has O_NONBLOCK cleared)
 * 	until write data is accepted or timeout has occurred.
 * 	
 * Arguments:
 * 
 * 	fd		handle to open file descriptor
 * 	buf		pointer to buffer with write data
 * 	size		size write data in bytes
 * 	timeout		timeout in milliseconds
 * 	
 * Return Value:
 * 
 * 	positive int = number of bytes accepted by driver
 * 	0 = write timeout
 * 	negative = error
 */
static int write_timed(int fd,void *buf, int size, int timeout, int *bUserAbort )
{
	struct timeval tv,tvstart,tvcur,tvorig;
	fd_set fds;
	int rc;
		
	/* save original timeout */
	tvorig.tv_sec  = timeout/1000; 
	tvorig.tv_usec = (timeout%1000) * 1000;
	tv = tvorig;
	gettimeofday(&tvstart,NULL);

	/* use select call to wait for data available */
		
	FD_ZERO(&fds);
	FD_SET(fd,&fds);
doagain:	
	/* use select() to wait for an indication that the
	 * driver can accept another frame in the time specfied.
	 * Check for EINTR since we are using signals for timing 
	 * purposes, and each signal interrupts out select() call 
	 * and sets errno to EINTR.
	 *
	 * If this happens, simply restart our call and wait until
	 * we get 0 (indicating timeout) or non-zero, indicating
	 * the driver can accept another frame  for transmit
	 */
	do {
		rc = select(fd+1,NULL,&fds,NULL,&tv);		
		if (rc < 0) {			
			gettimeofday(&tvcur,NULL);
			timersub(&tvcur,&tvstart,&tv);
			if (timercmp(&tv,&tvorig,>=) || *bUserAbort)
				rc = 0;
			else
				timersub(&tvorig,&tv,&tv);
		}
	
	} while (rc < 0 && errno == EINTR) ;

	if (rc>0) {
		rc = write(fd,buf,size);
		if (rc == 0)
			goto doagain;
	}
	
	return rc;
	
}	/* end of write_timed() */

/* read_timed()
 * 
 * 	make read call to file/device with a timeout
 * 	This call blocks (if file has O_NONBLOCK cleared)
 * 	until read data is available or timeout has occurred.
 * 	
 * Arguments:
 * 
 * 	fd		handle to open file descriptor
 * 	buf		pointer to returned data buffer
 * 	size		size of buffer in bytes
 * 	timeout		timeout in milliseconds
 * 	
 * Return Value:
 * 
 * 	positive int = number of bytes returned
 * 	0 = read timeout
 * 	negative = read error
 */
static int read_timed(int fd,void *buf, int size, int timeout, int *bUserAbort )
{
	struct timeval tv,tvstart,tvcur,tvorig;
	fd_set fds;
	int rc;
	
	/* save original timeout */
	tvorig.tv_sec  = timeout/1000; 
	tvorig.tv_usec = (timeout%1000) * 1000;
	tv = tvorig;
	gettimeofday(&tvstart,NULL);
	
	/* use select call to wait for data available */

	FD_ZERO(&fds);
	FD_SET(fd,&fds);

	/* use select() to wait for an available frame for the
	 * time specified. Check for EINTR since we are using
	 * signals for timing purposes, and each signal 
	 * interrupts out select() call and sets errno to EINTR.
	 *
	 * If this happens, simply restart our call and wait until
	 * we get 0 (indicating timeout) or non-zero, indicating
	 * size of received frame available
	 */

	do {
		rc = select(fd+1,&fds,NULL,NULL,&tv);		
		if (rc < 0 && errno == EINTR) {
			gettimeofday(&tvcur,NULL);
			timersub(&tvcur,&tvstart,&tv);
			if (timercmp(&tv,&tvorig,>=) || *bUserAbort)
				rc = 0;
			else
				timersub(&tvorig,&tv,&tv);
		}
			
	} while (rc < 0 && errno == EINTR) ;
	
	if (rc>0)
		rc = read(fd,buf,size);
		
	return rc;
	
}	/* end of read_timed() */

/* wait_all_sent()
 * 
 * block until all send data has drained from the port
 * (send buffers empty and hardware send complete)
 * 	
 * Arguments:
 * 
 * 	fd		handle to open file descriptor
 * 	timeout		timeout in milliseconds
 *	bUserAbort	user abort flag to monitor
 * 	
 * Return Value:
 * 
 * 	0 if success
 * 	EINTR if timeout or user abort
 *	error code if error
 */
int wait_all_sent(int fd,int timeout,int *bUserAbort)
{
	struct timeval tv,tvstart,tvcur,tvorig;
	int rc;
	
	/* save original timeout */
	tvorig.tv_sec  = timeout/1000; 
	tvorig.tv_usec = (timeout%1000) * 1000;
	tv = tvorig;
	gettimeofday(&tvstart,NULL);
	
	do {
		rc = tcdrain(fd);
		if (rc < 0 && errno == EINTR) {
			gettimeofday(&tvcur,NULL);
			timersub(&tvcur,&tvstart,&tv);
			if (timercmp(&tv,&tvorig,>=) || *bUserAbort)
				return EINTR;
			else
				timersub(&tvorig,&tv,&tv);
		}
			
	} while (rc < 0 && errno == EINTR) ;
	
	return rc;
	
}	/* end of wait_all_sent() */

