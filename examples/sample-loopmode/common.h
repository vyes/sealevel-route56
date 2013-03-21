#if !defined(_COMMON_H_INCLUDED)
#define _COMMON_H_INCLUDED

#include <sys/times.h>
#include <sys/time.h>
#if defined(__FreeBSD__)
#include "../route56.h"
#else
#include <linux/types.h>
#include "route56.h"
#endif


#define	MAX_IFRAME_SIZE	4096
#define	NUM_APP_TIMERS	5		/* allow for up to 5 timers */

/*----------------------------------------------------------------------------
* structure used by primary station for R56_RECEIVE_REQUEST list
*-----------------------------------------------------------------------------*/
typedef	struct	_RX_TRANSACTION
{
	int	RxFrameSize;
	unsigned char *pRxFrame;
	struct	_RX_TRANSACTION *pNext;    /* pointer to next frame in list */

} RX_TRANSACTION, *PRX_TRANSACTION;

/*----------------------------------------------------------------------------
* Common Transaction structure
*-----------------------------------------------------------------------------*/
typedef	struct	_APP_TRANSACTION
{
	unsigned char Addr;			/* source station address */
	unsigned char Request;			/* 0 = request, 1 = response */
	unsigned int TransactionNumber;		/* transaction number */
	struct timeval	CreationTimeStamp;	   /* time when transaction was created */
	struct timeval	PrimaryReceiptTimeStamp;  /* time when primary station received request  */
	struct timeval	PrimaryTimeStamp;       /* time when primary processed this transaction */

	struct timeval	CompletionTimeStamp;	   /* time when secondary completed processing 	*/
													/* this transaction 									*/

	struct	_APP_TRANSACTION *pNext;	/* pointer to next in list of APP_TRANSACTIONS */

} APP_TRANSACTION, *PAPP_TRANSACTION;


/*----------------------------------------------------------------------------
* information used to primary to manage secondary stations
*-----------------------------------------------------------------------------*/
typedef struct _CLIENT
{
	unsigned char	Addr;
	unsigned char	Nr;
	unsigned char	Ns;

	struct _CLIENT *pNext;

} CLIENT, *PCLIENT;


/*----------------------------------------------------------------------------
* Generic Primary/Secondary stations application information.
*-----------------------------------------------------------------------------*/
typedef	struct	_APP_DATA
{
	char	PortName[20];									/* HDLC Port Name */
	int	hDevice;                   /* handle to open HDLC Port */

	int		bExitApplication;          /* set to true if ESC key detected */

	unsigned int		RxTimeout;                 /* Receive timeout (milliseconds)*/
	unsigned int		TxTimeout;                 /* Transmit timeout (milliseconds) */

	unsigned int		PollInterval;				   /* Primary poll interval (seconds) */

	unsigned int		NewTransactionInterval;	   /* Rate at which Secondary creates new */
													/* transactions (seconds)              */

	int		bPrimary;	               /* TRUE = station is primary, FALSE = station is secondary */
	unsigned int		TxIdle;	                  /* TX Idle pattern to use */

	R56_PARAMS	Rt56Params;             /* R56_PARAM structure used to configure */
													/* the MGHDLC Port								*/
	unsigned char	*pTxBuffer;                    /* Common transmit buffer */
	unsigned int	TxBufferSize;                 /* Transmit buffer size */
	unsigned int	TxStatus;                     /* Transmit Status */

	unsigned char *pRxBuffer; 							/* extract receive frames from driver		*/

	unsigned int	Timers[NUM_APP_TIMERS];       /* simple timer counters */
	unsigned int	ElapsedMS;							/* elapsed time in milliseconds */

	unsigned char	ReceiveCount;
	unsigned char	SendCount;

	int	bInitialized;                 /* client initialized? */

	unsigned int	NumTransactions;			      /* Max number of client transactions to process	*/
													/* 0 = no limit 											*/
	int	ShowedLimitHitMessage;

	/*--------------------------------------------------
	 * Components used the secondary stations to generate
	 * and keep track of outbound and pending transactions
	 *--------------------------------------------------*/
	unsigned int	LastTransactionNumber;

	/*--------------------------------------------------
	 * For Primary station: list of received transactions
	 * waiting to be processed and returned to their
	 * owners
	 *
	 * For Secondary stations: list of transaction waiting
	 * to be sent to the primary station for processing
	 *--------------------------------------------------*/
	int					NumQueuedTransactions;
	void *					pQueuedTransactionList;
	void *					pLastQueuedTransactionList;

	/*--------------------------------------------------
	 * For Secondary stations: list of transactions already
	 * sent to the primary station and waiting for responses
	 *--------------------------------------------------*/
	int					NumPendingTransactions;
	void *					pPendingTransactionList;
	void *					pLastPendingTransactionList;


	/*--------------------------------------------------
	 * for Primary stations, list of clients
	 *--------------------------------------------------*/
	PCLIENT				pClientList;

	struct	r56_icount	last_icount;
	struct	r56_icount	icount;

} APP_DATA, *PAPP_DATA ;


#define	BROADCAST_ADDR		0xff        /* broadcast address */

#define	DISC					0x53		   /* Disconnect */
#define	SIM					0x17			/* Set initialization mode */
#define	UI						0x13			/* Unnumbered information  */
#define	POLL					0x10			/* Poll/Final  */
#define	FINAL 				0x10			/* Poll/Final  */
#define	RR						0x01			/* Receiver Ready */
#define	RNR					0x05        /* Receiver Not Ready */
#define	REJ					0x09        /* Reject */

#define	NR	((unsigned char)pAppData->ReceiveCount << 5)
#define	NS ((unsigned char)pAppData->SendCount << 1 )

#define	IS_POLL(c)	( (c)&POLL )
#define	IS_FINAL(c)	( (c)&FINAL )
#define	IS_RR(c)		( ((c)&0x0f) == RR )
#define	IS_RNR(c)	( ((c)&0x0f) == RNR )
#define	IS_REJ(c)	( ((c)&0x0f) == REJ )

#define	IS_DISC(c)	( (c)==(unsigned char)DISC )
#define	IS_SIM(c)	( (c)==(unsigned char)SIM )
#define	IS_UI(c)	( (c)==(unsigned char)UI )


#define	ADDR_FIELD		0	            /* offset 0 of any Frame is the Address Field */
#define	CTRL_FIELD		1					/* offset 1 of any Frame is the Control Field */
#define	INFO_FIELD		2	            /* offset 2 of any Frame starts Transaction Information */



#define	LOG		Log
#define	DISPLAY	Display

/*----------------------------------------------------------------------------
* common.c prototypes
*-----------------------------------------------------------------------------*/

void Log(char *fmt, ...);
int Rt56GetStats(PAPP_DATA pAppData, struct r56_icount *picounts);
void DumpFrame(char *pPrefix, unsigned char *pBuffer, int BufferLength);
int AllocateResources(PAPP_DATA pAppData);
int DeallocateResources(PAPP_DATA pAppData);
int AllocatePort(PAPP_DATA pAppData);
int DeallocatePort(PAPP_DATA pAppData);
int EnableTransmitter(PAPP_DATA pAppData, int bEnable);
int SendFrame(PAPP_DATA pAppData);
int LoopModeSendLastFrame(PAPP_DATA pAppData);
int CancelTransmit(PAPP_DATA pAppData);
int EnableReceiver(PAPP_DATA pAppData, int bEnable);
int CheckReceive(PAPP_DATA pAppData, unsigned int MaxIdleTime, int (*RxCallback)(PAPP_DATA pAppData, unsigned char *pRxBuffer, int BufferSize));
int SetParams(PAPP_DATA pAppData);
int SetIdle(PAPP_DATA pAppData, unsigned int IdleMode);
int CancelReceive(PAPP_DATA pAppData);
void DumpTransaction(char *pComment, PAPP_TRANSACTION pTransaction);
int CheckUserExitRequest(PAPP_DATA pAppData);
void ResetUserExitRequest(PAPP_DATA pAppData);
int wait_all_sent(int fd,int timeout,int *bUserAbort);
#endif
