/*----------------------------------------------------------------------------
*
* Primary.c
*
* Primary Station of a LoopMode connection.
*
* Purpose:
* -------
*
* Interact with one or more Secondary stations and queues received transactions
* for later processing. Controls flow of information between all stations in
* the local Loop Mode setup.
*
* Note:
* ----
*
* The protocol used by this test program is merely for demonstration purposes
* only and in no way tries to mimic any established protocol. There is very
* little error checking or recovery management performed in this sample.
*
* Summary:
* -------
*
* When the Primary station starts, it does so in a normal HDLC mode (Non-Loop
* Mode) in order to control the flow of data around the Local Loop stations.
*
* The Primary stations generally sends General Polls and responds to all
* received data frames. When a Secondary station comes on-line, it should
* transmit a Set Initialization Mode (SIM) request to the Primary station.
* The Primary will then allocate a block of memory used to manage that
* Secondary station. If the allocation is successful, the Primary will
* transmit a SIM response to the Secondary station to inform it that the
* Primary is ready to communicate with it.
*
* Normally, the Primary station will only receive Poll responses from the
* Secondary stations if they have nothing to transmit to the Primary. If a
* Secondary station has data to transmit, it will do so. The Primary stations
* assumes all received Information Frames are transactions which will be
* queued to be process later.
*
* At regular intervals, the Primary station will Poll the Secondary stations
* if it has not transactions queued and waiting for a response. Otherwise,
* the Primary will transmit all queued transactions to the original Secondary
* stations.
*
* When the user hits the <ESC>, the Primary station starts a shutdown sequence.
* It broadcasts a DISC request to all Secondary stations. Each Secondary
* station should respond with a DISC response to indicate it has accepted the
* request and is ready to go offline. As the Primary stations receives the
* Secondary stations DISC responses, the block of information used to manage
* that workstation is deallocated. When all Secondary stations have been
* deallocated, the Primary station exists.
*
*-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <termios.h>
#include <sys/ioctl.h>

#include "common.h"

#include "primary.h"
#include "profile.h"

#define	POLL_TIMER		0
#define	TIMER2	1
#define	TIMER3	2
#define	TIMER4	3
#define	TIMER5	4

APP_DATA	AppData;

static void signal_handler(int signum);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* main()
*
*
* Notes:
*
* input:
*
* returns:
*
*-----------------------------------------------------------------------------*/
int main( int argc, char **argv )
{
	setvbuf( stdout, NULL, _IONBF, 0 );

	openlog( argv[0], LOG_PID | LOG_PERROR, LOG_USER );
	syslog( LOG_INFO, "%s starting\n", argv[0] );

	if ( argc != 2 )
	{
		Log( "usage: %s profile-file\n",
			argv[0] );
		return -1;
	}
	else if ( GetProfile( &AppData, argv[1] ) )
	{
		/*--------------------------------------------------
		 * Allocate memory, object resources and prepare
		 * HDLC port for use
		 *--------------------------------------------------*/
		if ( AllocateResources ( &AppData ) &&
			AllocatePort( &AppData ) )
		{
			/*--------------------------------------------------
			 * Setup complete, continue with Secondary loopmode
			 * test
			 *--------------------------------------------------*/
			PrimaryLoopTest( &AppData );
		}

		/*--------------------------------------------------
		 * Release resources
		 *--------------------------------------------------*/
		DeallocatePort( &AppData );
		DeallocateResources( &AppData );
	}

	return 0;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int LoopTestPrimary()
*
* Do Primary Loop Test - wait for transaction frames to be received, queue
* then for processing and return when processing complete
*
* Notes:
*
* input:
*
* 	PAPP_DATA pAppData	// pointer to application data
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
int PrimaryLoopTest( PAPP_DATA pAppData )
{
	int	bResult = TRUE;
	struct	itimerval itimer;
	struct	sigaction sa;
	
	Log( "PrimaryLoopTest() entry\n");

	/*--------------------------------------------------
	 * install signal handlers
	 *--------------------------------------------------*/
	memset( &sa, 0, sizeof(struct sigaction) );
	sa.sa_handler = &signal_handler;	

	if ( sigaction( SIGALRM, &sa, NULL ) ||
		sigaction( SIGINT, &sa, NULL ) )
	{
		Log("failed to set sigactions: %s\n", strerror(errno) );
		return FALSE;
	}

	/*--------------------------------------------------
	 * set interval timer for 100ms (100000us).
	 *--------------------------------------------------*/
	itimer.it_value.tv_sec = 0;
	itimer.it_value.tv_usec = 100000;
	itimer.it_interval.tv_sec = 0;
	itimer.it_interval.tv_usec = 100000;

	if ( setitimer( ITIMER_REAL, &itimer, 0 ) )
	{
		Log( "failed to set interval timer: %s\n", strerror(errno) );
		return FALSE;
	} 
	
	/* Do not wait for received frames longer than the poll interval */
	/* PollInterval is in seconds, RxTimeout is in milliseconds */
	if ( pAppData->PollInterval < (pAppData->RxTimeout/1000) )
		pAppData->RxTimeout = pAppData->PollInterval * 1000;
		
	/* Prepare for Primary Station operation:
	 *
	 * Enable receiver to start a receive operation.
	 * Set transmit idle pattern
	 * Enable the transmitter.
	 *
	 * NOTE:
	 * Even though this test program allows the user to
	 * specify a Transmit Idle Pattern in the
	 * configuration profile, the Primary Station in
	 * an HDLC/SDLC Loop Mode Configuration should always
	 * Idle Ones
	 */
	if ( !EnableReceiver( pAppData, TRUE ) ||
		!SetIdle( pAppData, pAppData->TxIdle ) ||
		!EnableTransmitter( pAppData, TRUE ) )
	{
		return FALSE;
	}

	Log("Primary station is ready ...\n");

	while ( TRUE ) {
		if ( CheckUserExitRequest( pAppData ) ) {
			Log(" ** User Requested Exit...\n");
			break;
		}

		/* check for and process received frames */
		CheckReceive( pAppData, pAppData->RxTimeout, ProcessRxFrames );

		/* Check for any transmit activity required */
		CheckTransmit( pAppData );
	}

	/* Go offline - broadcast DISC and wait for response */
	DoDisc( pAppData );

	/* Disable receiver and return */
	EnableReceiver( pAppData, FALSE );

	return bResult;
}

/* Checks for and performs any transmit operations required
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int CheckTransmit( PAPP_DATA pAppData )
{
	if ( pAppData->Timers[POLL_TIMER] )
		return TRUE;
		
	/* transmit timer expired */

	if (pAppData->pQueuedTransactionList != NULL)
	 	SendTransactionResponses( pAppData );
	else
		SendGeneralPoll( pAppData );

	pAppData->Timers[POLL_TIMER] = pAppData->PollInterval;
	return TRUE;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int ProcessRxFrames()
*
* Callback function used to process/parse received data frame
*
* Notes:
*
* ProcessRxFrames() will always return TRUE to caller CheckReceive() to indicate
* receive process should continue.
*
* CheckReceive() will continue to process received frames as long as the
* call back function returns TRUE.
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
* 	PR56_RECEIVE_REQUEST pRcvRequest   // pointer to receive data structure
*
* returns:
*
*	always returns TRUE to keep processing received frames
*
*-----------------------------------------------------------------------------*/
int ProcessRxFrames( PAPP_DATA pAppData, unsigned char *pRxFrame, int RxFrameSize )
{
	if ( RxFrameSize == 2 && IS_SIM( pRxFrame[CTRL_FIELD] ) ) {
		/* Received Set Initialization Mode from Secondary
		 * station - add to list of existing clients
		 * and send a SIM response if successful
		 */
		if ( AllocateClient( pAppData, pRxFrame[ADDR_FIELD] ) )
			SendSIMResponse( pAppData, pRxFrame[ADDR_FIELD] );
		return TRUE;
	}

	if ( RxFrameSize == 2 && IS_DISC( pRxFrame[CTRL_FIELD] ) ) {
		Log("Received DISC response from %x\n", pRxFrame[ADDR_FIELD] );
		DeallocateClient( pAppData, pRxFrame[ADDR_FIELD] );
		return TRUE;
	}

	/*--------------------------------------------------
	 * Check for Information Frame and process
	 *--------------------------------------------------*/
	 
	if ( ( pRxFrame[CTRL_FIELD] & 0x01 ) == 0 )
	{
		PAPP_TRANSACTION pTransaction = (PAPP_TRANSACTION)&pRxFrame[INFO_FIELD];
		PCLIENT	pClient = GetClientInfo( pAppData, pRxFrame[ADDR_FIELD] );

		if ( pClient == NULL ) {
			Log("ProcessRxFrames() - station %x unknown\n", pRxFrame[ADDR_FIELD] );
			return TRUE;
		}

		/* Process only requests. Ignore received responses that
		 * have traversed the loop back to the primary
		 */
		if ( pTransaction->Request )
			QueueTransaction( pAppData, pRxFrame, RxFrameSize );
		else
			Log("ProcessRxFrames() - ignoring response frame that has looped back\n");

		return TRUE;
	}

	/*--------------------------------------------------
	 * Frame was not processed - return TRUE to continue
	 * processing received frames
	 *--------------------------------------------------*/
	return TRUE;
}

/* Add a new client with the specified address to the
 * list of active clients.
 *
 * input:	pAppData	pointer to application data
 *		Addr		secondary station's address
 * 
 * returns:	TRUE if successful, FALSE otherwise
 */
int AllocateClient( PAPP_DATA pAppData, unsigned char Addr )
{
	PCLIENT pList = pAppData->pClientList;
	PCLIENT pNewNode;

	Log("AllocateClient() - request to add %x\n", Addr );

	/* search for client in current list */
	
	while ( pList ) {
		if ( pList->Addr == Addr ) {
			Log("AllocateClient() - already in list, reset and return\n");
			pList->Nr = pList->Ns = 0;
			return TRUE;
		}
		pList = pList->pNext;
	}

	/* allocate new client */
	
	pNewNode = (PCLIENT)malloc( sizeof(CLIENT) );
	if ( !pNewNode ) {
		Log("AllocateClient() - out of memory\n");
		return FALSE;
	}
	
	/* initialize new client */
	
	memset( pNewNode, 0, sizeof(CLIENT) );
	pNewNode->Addr = Addr;

	/* add new client to tail of client list */
	
	if ( pAppData->pClientList ) {
		pList = pAppData->pClientList;
		while( pList->pNext )
			pList = pList->pNext;
		pList->pNext = pNewNode;
	} else
		pAppData->pClientList = pNewNode;

	return TRUE;
}

/* Remove a client with the specified address from the list  
 * of active clients.
 *
 * input:	pAppData	pointer to application data
 *		Addr		secondary station's address
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int DeallocateClient( PAPP_DATA pAppData, unsigned char Addr )
{
	PCLIENT pList = pAppData->pClientList;
	PCLIENT	pLast = NULL;

	Log("DeallocateClient() - request to remove %x\n", Addr );

	/* find client address in list */
	
	while ( pList ) {
		if ( pList->Addr == Addr ) {
			/* Found entry, remove from list */
			if ( pList == pAppData->pClientList )
				pAppData->pClientList = pList->pNext;
			else
				pLast->pNext = pList->pNext;

			Log("DeallocateClient() - removed %x (%#x)\n", pList->Addr, (int)pList);
			free(pList);
			return TRUE;
		}
		pLast = pList;
		pList = pList->pNext;
	}

	Log("DeallocateClient() - entry %x not found\n", Addr);
	return TRUE;
}

/* Return pointer to CLIENT structure for specified client address
 *
 * input:	pAppData	pointer to application data
 * 		Addr		secondary station's address
 *
 * returns:	pointer to CLIENT if found, otherwise NULL
 */
PCLIENT GetClientInfo( PAPP_DATA pAppData, unsigned char Addr )
{
	PCLIENT pList = pAppData->pClientList;
	while ( pList ) {
		if ( pList->Addr == Addr )
			break;
		pList = pList->pNext;
	}

	return pList;
}

/* Send a response to Secondary station acknowledging receipt of SIM request
 *
 * A secondary station transmits a 2 byte packet: ADDR SIM
 *
 * as a request to initialize, the primary transmits a 3 byte response
 *
 * input:	pAppData	pointer to application data
 *		Addr		secondary station's address
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int SendSIMResponse( PAPP_DATA pAppData, unsigned char Addr )
{
	pAppData->TxBufferSize = 0;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = Addr;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = SIM;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = Addr;
	Log("Sending SIM response to %x ... \n", Addr );

	return SendFrame(pAppData);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int DoDisc()
*
* Perform DISC (Disconnect) request. Broadcast DISC requests and as each
* Secondary Station responds, remove it from our list of clients.
*
* Notes:
*
* When all clients have responded, our DISC processing is complete.
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
*
* returns:
*
* 	TRUE if succcessful, FALSE if an error occurs
*
*-----------------------------------------------------------------------------*/
int DoDisc( PAPP_DATA pAppData )
{
	pAppData->Timers[POLL_TIMER] = pAppData->PollInterval;

	/*--------------------------------------------------
	 * Reset user exit request flag
	 *--------------------------------------------------*/
	ResetUserExitRequest( pAppData );

	while ( TRUE )
	{
		if ( pAppData->Timers[POLL_TIMER] == 0 )
		{
			/*--------------------------------------------------
			 * Time to broadcase a DISC request to all stations
	 		 *--------------------------------------------------*/
			pAppData->TxBufferSize = 0;
			pAppData->pTxBuffer[pAppData->TxBufferSize++] = BROADCAST_ADDR;
			pAppData->pTxBuffer[pAppData->TxBufferSize++] = DISC;

			Log("Broadcasting DISC request... \n" );

			/*--------------------------------------------------
			 * Send frame
			 *--------------------------------------------------*/
			if ( !SendFrame( pAppData ) )
				return FALSE;

			pAppData->Timers[POLL_TIMER] = pAppData->PollInterval;
		}

		/*--------------------------------------------------
		 * Check for Rx frames
		 *--------------------------------------------------*/
		CheckReceive( pAppData, pAppData->RxTimeout, ProcessRxDiscFrames );

		/*--------------------------------------------------
		 * Determine if all stations have responded (if the
		 * client list is empty). If so, exit routine
		 *--------------------------------------------------*/
		if ( pAppData->pClientList == NULL )
		{
			Log("All stations ready to go offline\n");
			return TRUE;
		}

		/*--------------------------------------------------
		 * Check for user requested exit
		 *--------------------------------------------------*/
		if ( CheckUserExitRequest( pAppData ) )
		{
			Log(" ** User Requested Exit...\n");
			return FALSE;
		}
	}

}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int ProcessRxDiscFrames()
*
* Callback function to processes all received frames during DISC processing
*
* Notes:
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
* 	PR56_RECEIVE_REQUEST pRcvRequest   // pointer to receive data structure
*
* returns:
*
* 	always returns TRUE to continue processing received frames
*
*-----------------------------------------------------------------------------*/
int ProcessRxDiscFrames( PAPP_DATA pAppData, unsigned char *pRxFrame, int RxFrameSize )
{
	if( RxFrameSize == 2 && IS_DISC( pRxFrame[CTRL_FIELD] ) )
	{
		/*--------------------------------------------------
		 * DISC Response from a Secondary stations, remove
		 * from active client list
		 *--------------------------------------------------*/
		Log("Received DISC response from %x\n", pRxFrame[ADDR_FIELD] );
		DeallocateClient( pAppData, pRxFrame[ADDR_FIELD] );
	}

	return TRUE;
}

/* Queue a received request for later processing
 *
 * input:	pAppData	pointer to application data
 *		pRcvRequest	pointer to receive data structure
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int QueueTransaction( PAPP_DATA pAppData, unsigned char *pRxFrame, int RxFrameSize )
{
	PRX_TRANSACTION pRxTransaction = malloc(sizeof(RX_TRANSACTION));

	/*--------------------------------------------------
	 * Record timestamp used to indicate receipt time
	 * at the primary station
	 *
	 * RxFrame:
	 *
	 *  	DataBuffer[0] = Addr
	 * 	DataBuffer[1] = CtrlField
	 * 	DataBuffer[2] = start of transaction buffer
	 *--------------------------------------------------*/

	PAPP_TRANSACTION pTransaction =
			(PAPP_TRANSACTION)&pRxFrame[INFO_FIELD] ;

	gettimeofday( &pTransaction->PrimaryReceiptTimeStamp, NULL );

	DumpTransaction( "Queuing transaction request", pTransaction );

	/*--------------------------------------------------
	 * If allocation for a new RX_TRANSACTION structure
	 * was successful, allocate another buffer to
	 * hold a copy of the input R56_RECEIVE_REQUEST
	 * structure.
	 *--------------------------------------------------*/
	if ( pRxTransaction ) {
		memset( pRxTransaction, 0, sizeof(RX_TRANSACTION)  );
		pRxTransaction->pRxFrame = malloc(RxFrameSize);
	}

	if ( !pRxTransaction || !pRxTransaction->pRxFrame ) {
		Log("QueueTransaction() - out of memory\n");
		if ( pRxTransaction->pRxFrame )
			free( pRxTransaction->pRxFrame );
		if ( pRxTransaction )
			free( pRxTransaction );
		return FALSE;
	}

	/*--------------------------------------------------
	 * All allocations were successful, copy the input
	 * R56_RECEIVE_REQUEST structure to the new
	 * RX_TRANSACTION buffer
	 *--------------------------------------------------*/
	memcpy( pRxTransaction->pRxFrame, pRxFrame, RxFrameSize );

	/* Add to queued transaction list */
	
 	if ( pAppData->pQueuedTransactionList == NULL )
 		pAppData->pQueuedTransactionList = pRxTransaction;
 	else
 		((PRX_TRANSACTION)pAppData->pLastQueuedTransactionList)->pNext = pRxTransaction;
 	pAppData->pLastQueuedTransactionList = pRxTransaction;
 	++pAppData->NumQueuedTransactions;

 	Log("Transaction queued for processing, %u queued\n",
 			pAppData->NumQueuedTransactions );

	return TRUE;
}

/* Remove the first item from the QueuedTransactionList
 *
 * input:	pAppData	pointer to application data
 * returns:	nothing
 */
void RemoveQueuedTransationListHead( PAPP_DATA pAppData )
{
	if (pAppData->pQueuedTransactionList) {
		PRX_TRANSACTION pTransaction = pAppData->pQueuedTransactionList;

		pAppData->pQueuedTransactionList = pTransaction->pNext;

		free( pTransaction->pRxFrame );
		free( pTransaction );

		--pAppData->NumQueuedTransactions;
	}

	Log( "%u transactions queued for responses\n",
		pAppData->NumQueuedTransactions );
}

int SendGeneralPoll( PAPP_DATA pAppData )
{
	pAppData->TxBufferSize = 0;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = BROADCAST_ADDR;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = (POLL | RR );
	return SendFrame( pAppData );
}

/* Send a response to each queued request transaction
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int SendTransactionResponses( PAPP_DATA pAppData )
{
	int bResetIdleMode = FALSE;

	if ( !pAppData->pQueuedTransactionList )
		return TRUE;

	if ( ((PRX_TRANSACTION)pAppData->pQueuedTransactionList)->pNext ) {
	
		/* There is more than one frame to send. Set to idle
		 * flags in between frame so primary retains control of
		 * the line until all frames sent.
		 */
		SetIdle(pAppData, HDLC_TXIDLE_FLAGS);
		bResetIdleMode = TRUE;
	}

	while ( pAppData->pQueuedTransactionList ) {
		PRX_TRANSACTION pQueuedTransaction =
				(PRX_TRANSACTION)pAppData->pQueuedTransactionList;
		unsigned char *	pRxFrame = pQueuedTransaction->pRxFrame;
		PAPP_TRANSACTION pTransaction = (PAPP_TRANSACTION)&pRxFrame[INFO_FIELD];
		
		PCLIENT pClient = GetClientInfo( pAppData, pRxFrame[ADDR_FIELD] );
		if ( pClient == NULL ) {
			DumpTransaction("Client unknown, request discarded", pTransaction );
			RemoveQueuedTransationListHead( pAppData );
			continue;
		}

		/* Insert primary's respond time into the transaction */
		gettimeofday( &pTransaction->PrimaryTimeStamp, NULL );

		/* mark frame as response */
		pTransaction->Request = FALSE;

		pAppData->TxBufferSize = 0;

		/* set address/control fields */
		
		pAppData->pTxBuffer[pAppData->TxBufferSize++] = pRxFrame[ADDR_FIELD];
		pAppData->pTxBuffer[pAppData->TxBufferSize++] = (pClient->Ns << 1) ;
		pClient->Ns = ++pClient->Ns & 0x07;

		/* copy transaction contents into info field */
		
		memcpy( &pAppData->pTxBuffer[pAppData->TxBufferSize],
			pTransaction, sizeof(APP_TRANSACTION) );

		pAppData->TxBufferSize += sizeof(APP_TRANSACTION) ;

		if ( SendFrame(pAppData) ) {
			DumpTransaction("Respose sent", pTransaction );
			RemoveQueuedTransationListHead( pAppData );
		} else
			break;	/* timeout/fail, try again later */
	}

	/*--------------------------------------------------
	 * Send a general poll to guaranty Secondary
	 * stations get a Poll/Final bit
	 *--------------------------------------------------*/
	SendGeneralPoll( pAppData );

	if ( bResetIdleMode ) {
		/* reset to idle ones, so secondaries can transmit */
		wait_all_sent(pAppData->hDevice,10000,&pAppData->bExitApplication);
		SetIdle( pAppData, HDLC_TXIDLE_ONES );
	}
	
	return TRUE;
}

/* signal handler for program
 *
 * We previously set iteration timer to occur every 100ms (or 100,000us).
 * Maintain elapsed time in milliseconds and seconds for application
 * timing purposes
 *
 * input:	signal number being processed
 */
static void signal_handler(int signum)
{
	switch( signum )
	{
	case SIGALRM :
		/* adjust elapsed time by timer interval */
		AppData.ElapsedMS += 100;

		/* Every 1000ms (second), process application timers */
		if ( !(AppData.ElapsedMS % 1000) ) {
			int i;
			for ( i = 0; i < NUM_APP_TIMERS; ++i ) {
				if ( AppData.Timers[i] )
					--AppData.Timers[i];
			}
		}

		if ( Rt56GetStats( &AppData, &AppData.icount ) == 0 )
		{
			if ( AppData.last_icount.txunder != AppData.icount.txunder )
			{
				AppData.last_icount.txunder = AppData.icount.txunder;
				Log(" ** txunder = %lu\n",
					AppData.icount.txunder);
			}

			if ( AppData.last_icount.txabort != AppData.icount.txabort )
			{
				AppData.last_icount.txabort = AppData.icount.txabort;
				Log(" ** txabort = %lu\n",
					AppData.icount.txabort);
			}

			if ( AppData.last_icount.txtimeout != AppData.icount.txtimeout )
			{
				AppData.last_icount.txtimeout = AppData.icount.txtimeout;
				Log(" ** rxtimeout = %lu\n",
					AppData.icount.txtimeout);
			}
			
			if ( AppData.last_icount.rxshort != AppData.icount.rxshort )
			{
				AppData.last_icount.rxshort = AppData.icount.rxshort;
				Log(" ** rxshort = %lu\n",
					AppData.icount.rxshort);
			}
			
			if ( AppData.last_icount.rxlong != AppData.icount.rxlong )
			{
				AppData.last_icount.rxlong = AppData.icount.rxlong;
				Log(" ** rxlong = %lu\n",
					AppData.icount.rxlong);
			}
			
			if ( AppData.last_icount.rxabort != AppData.icount.rxabort )
			{
				AppData.last_icount.rxabort = AppData.icount.rxabort;
				Log(" ** rxabort = %lu\n",
					AppData.icount.rxabort);
			}
			
			if ( AppData.last_icount.rxover != AppData.icount.rxover )
			{
				AppData.last_icount.rxover = AppData.icount.rxover;
				Log(" ** rxover = %lu\n",
					AppData.icount.rxover);
			}
			
			if ( AppData.last_icount.rxcrc != AppData.icount.rxcrc )
			{
				AppData.last_icount.rxcrc = AppData.icount.rxcrc;
				Log(" ** rxcrc = %lu\n",
					AppData.icount.rxcrc);
			}
			
			if ( AppData.last_icount.rxidle != AppData.icount.rxidle )
			{
				AppData.last_icount.rxidle = AppData.icount.rxidle;
				Log(" ** rxidle = %lu\n",
					AppData.icount.rxidle);
			}
		}
		break;

	case SIGINT :
		/* CTRL-C abort signal */
		++AppData.bExitApplication ;
		break;
	}
}
