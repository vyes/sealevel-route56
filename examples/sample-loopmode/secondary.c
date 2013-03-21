/*----------------------------------------------------------------------------
*
* Slave.c
*
* Slave station (Secondary) of a LoopMode connection.
*
* Purpose:
* -------
*
* Generates and queues transactions at a specified interval. These
* transactions are simple timestamps used to gauge turn-around times in this
* LoopMode test.
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
* When a Secondary station starts in this LoopMode test, it waits for general
* polls and transmits a Set Initialization Mode (SIM) packet. When the Primary
* station sees a Secondary station's SIM request, it will respond with a
* SIM Response. Upon seeing the SIM response, the Secondary station knows that
* the Primary is ready to communicate with the Secondary station.
*
* At regular intervals, a Secondary station will allocate and initialize a
* new transaction. A sequential transaction number is stored in this new
* transaction as well as the Win32 SYSTEMTIME returned from GetLocalTime().
*
* This new transaction is then put in a queue of transactions waiting to be
* transmitted to the primary. Upon receipt of a Poll sequence from the primary,
* if the Secondary station has any queued transactions, they are transmitted
* to the primary. After each queued transaction is successfully transmitted,
* the transaction is moved from this "Queued for transmission" list to a
* "Waiting for Response" list.
*
* Received Information Frames (responses to our transactions) are matched
* against entries in the "Wait for Response" list. If a match is found (and
* there should always be a match), the transaction is considered complete and
* removed from the "Waiting for Response" list.
*
* When the Primary station is ready to shutdown, it will broadcast DISC
* requests to all stations. A Secondary station should transmit a DISC
* response to inform the Primary that the Secondary station knows the Primary
* is going off-line.
*-----------------------------------------------------------------------------*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>

#include "common.h"

#include "secondary.h"
#include "profile.h"

#define	NEW_TRANSACTION_TIMER 0
#define	TIMER2	1
#define	TIMER3	2
#define	TIMER4	3
#define	TIMER5	4


APP_DATA AppData;

static void signal_handler(int signum);

/* Program entry point.
 */
int main(int argc, char **argv)
{
	openlog( argv[0], LOG_PID | LOG_PERROR, LOG_USER );
	syslog( LOG_INFO, "%s starting\n", argv[0] );
	
	if ( argc != 2 ) {
		Log( "usage: %s profile-file\n", argv[0] );
		return -1;
	}
	
	if (GetProfile(&AppData, argv[1])) {
		if ( AllocateResources (&AppData) &&
		     AllocatePort(&AppData) )
			SecondaryLoopTest( &AppData ); 
			
		DeallocatePort( &AppData );
		DeallocateResources( &AppData );
	}

	return 0;
}

/* Main Secondary station LoopMode test. Wait for intialization to complete,
 * then send a series of transactions to the Primary station for processing.
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int SecondaryLoopTest( PAPP_DATA pAppData )
{
	struct	itimerval itimer;
	struct	sigaction sa;
	int	StartRxAbortCount;
	
	Log( "SecondaryLoopTest() entry\n");

	/*--------------------------------------------------
	 * install signal handlers
	 *--------------------------------------------------*/
	memset( &sa, 0, sizeof(struct sigaction) );
	sa.sa_handler = &signal_handler;	
	sa.sa_flags = SA_RESTART;

	if ( sigaction( SIGALRM, &sa, NULL ) ||
		sigaction( SIGINT, &sa, NULL ) ) {
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

	if ( setitimer( ITIMER_REAL, &itimer, 0 ) ){
		Log( "failed to set interval timer: %s\n", strerror(errno) );
		return FALSE;
	} 

	/*--------------------------------------------------
	 * get starting rxabort count
	 *--------------------------------------------------*/

	if ( Rt56GetStats( &AppData, &AppData.icount ) != 0 ) {
		Log("Rt56GetStatus() failed!\n");
		return FALSE;
	} else
		StartRxAbortCount = AppData.icount.rxabort;

	/*--------------------------------------------------
	 * Prepare for Secondary Station LoopMode operation:
	 *
	 * 1. Enable receiver to start a receive operation.
	 *
	 * 2. Set transmit idle pattern and enable the
	 *    transmitter. When configured for HDLC/SDLC
	 *    LoopMode, enabling the transmitter initiates
	 * 	a Enter Loop request.
	 *
	 *    Until the station receives a GoAhead sequence,
	 *    all transmits will fail with a status of 21
	 *    (ERROR_NOT_READY)
	 *
	 * NOTE:
	 *
	 * Even though this test program allows the user to
	 * specify a Transmit Idle Pattern in the
	 * configuration profile, all Secondary Stations in
	 * an HDLC/SDLC Loop Mode Configuration should always
	 * Idle Flags.
	 *--------------------------------------------------*/
	if ( !EnableReceiver( pAppData, TRUE ) ||
			!SetIdle( pAppData, pAppData->TxIdle ) ||
			!EnableTransmitter( pAppData, TRUE ) )
	{
		return FALSE;
	}

	/*--------------------------------------------------
	 * wait for RxAbort count to change - indicating
	 * receipt of an Abort/GoAhead sequence
	 *--------------------------------------------------*/

	Log("Waiting for loop mode insertion to complete...\n");

	while ( StartRxAbortCount == AppData.icount.rxabort ) {
		struct timeval tv;
		
		tv.tv_sec = 0;
		tv.tv_usec = 100000;
		select(0,NULL,NULL,NULL,&tv); /* delay 100ms between checks */

		if ( CheckUserExitRequest( pAppData ) ) {
			Log(" ** User Requested Exit...\n");
			return FALSE;	
		}

		if ( Rt56GetStats( &AppData, &AppData.icount ) != 0 ) {
			Log("Rt56GetStatus() failed!\n");
			return FALSE;
		}	
	}

	Log("Secondary station is ready...\n");

	/*--------------------------------------------------
	 * Main work loop, process received data frames
	 * and generate/queue transactions when appropriate
	 *--------------------------------------------------*/
	for(;;) {
		if ( CheckUserExitRequest( pAppData ) ) {
			Log(" ** User Requested Exit...\n");
			break;
		}

		CheckReceive( pAppData, pAppData->RxTimeout, ProcessRxFrame );

		/*--------------------------------------------------
		 * Check for new transactions to queue for
		 * transmission to primary
		 *--------------------------------------------------*/
		if ( pAppData->bInitialized )
			CheckForNewTransactions( pAppData );
	}

	/*--------------------------------------------------
	 * Loop exitted, disable receiver and return
	 *
	 * NOTE:
	 * if the HDLC Port is closed, the adapter will be
	 * reset and the adapter will no longer continue to
	 * repeat/echo data, essentially halting all
	 * traffic at that workstation.
	 *--------------------------------------------------*/
	EnableReceiver( pAppData, FALSE );

	return TRUE;
}

/* Process/Parse received data frame
 *
 * input:
 *
 *  	pAppData	pointer to application data
 * 	pRcvRequest	pointer to receive data structure
 *
 * returns:
 *
 * 	TRUE to have caller CheckReceive() continue processing received frames
 * 	FALSE otherwise.
 */
int ProcessRxFrame( PAPP_DATA pAppData, unsigned char * pRxFrame, int RxFrameSize )
{
	if ( RxFrameSize == 2 && pRxFrame[CTRL_FIELD] == DISC ) {
		if ( pAppData->bInitialized ) {
			/* If we are initialized, respond with a DISC response */
			Log("Recognize Disconnect request, shutting down ...\n");
			if ( SendDiscResponse( pAppData ) )
				pAppData->bInitialized = FALSE;
		} else
			Log("Recognize Disconnect request, already shutting down ...\n");
		return FALSE;
	}

	/*--------------------------------------------------
	 * If not initialized, check for general or specific
	 * polls and send SIM (Set Initialized Mode) request
	 *--------------------------------------------------*/
	if ( !pAppData->bInitialized ) {
	
		if ( RxFrameSize == 2 && IS_FINAL( pRxFrame[CTRL_FIELD] ) &&
				IS_RR( pRxFrame[CTRL_FIELD] ) )
		{
			Log("Recognize General Poll, transmitting SIM Request\n");
			SendSIMRequest( pAppData );
			return FALSE;
		}
		
		if ( RxFrameSize == 3 && IS_SIM( pRxFrame[CTRL_FIELD] ) ) {
			Log("Recognize SIM response, ready for processing\n");

			/*--------------------------------------------------
			 * Primary responds to SIM request with a 3 byte
			 * packet:
			 *
			 * 	Addr SIM Addr
			 *
			 * Set initialized and reset timers/counters for
			 * new transactions
			 *--------------------------------------------------*/
			pAppData->bInitialized = TRUE;
			pAppData->Timers[NEW_TRANSACTION_TIMER] = pAppData->NewTransactionInterval;
			pAppData->SendCount = 0;

			return FALSE;
		}
		
		Log("Unrecognized packet while waiting to be initialized\n" );
		return TRUE;
	}

	if ( RxFrameSize == 2 && IS_POLL( pRxFrame[CTRL_FIELD] ) ) {
		/* this is a poll frame, respond with requests or poll response */
		DoPollResponse( pAppData );
		return FALSE;
	}

	 
	if ( (pRxFrame[CTRL_FIELD] & 0x01) == 0 ) {
	
		PAPP_TRANSACTION pTransaction = (PAPP_TRANSACTION)&pRxFrame[INFO_FIELD];
		
		/* this is an information frame, reconcile if it is a response */

		if ( !pTransaction->Request )
			ReconcileTransaction( pAppData, pTransaction );

		/*--------------------------------------------------
		 * if received frame has FINAL bit set, return FALSE
		 * to stop processing received frames, otherwise
		 * return TRUE to continue processing received frames
		 *--------------------------------------------------*/
		return pRxFrame[CTRL_FIELD] & FINAL ? FALSE : TRUE ;
	}

	/*--------------------------------------------------
	 * Frame was not processed - return TRUE to continue
	 * processing received frames
	 *--------------------------------------------------*/
	return TRUE;
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int SendSIMRequest()
*
* Transmit a Set Initialization Mode (SIM) request to the primary
*
*
* Notes:
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
int SendSIMRequest( PAPP_DATA pAppData )
{
	pAppData->TxBufferSize = 0;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = pAppData->Rt56Params.addr_filter;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = SIM;
	Log("Sending to SIM request... \n" );

	/*--------------------------------------------------
	 * send frame
	 *--------------------------------------------------*/
	return LoopModeSendLastFrame(pAppData);
}

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int DoPollResponse()
*
* We have received a Poll from the primary. Determine if we have any
* transactions queued for transmission to the primnary. If so, send them
* now, otherwise, transmit a simple Poll response
*
* Notes:
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
int DoPollResponse( PAPP_DATA pAppData )
{
	if ( pAppData->pQueuedTransactionList == NULL ) {
		Log("DoPollResponse() - no queued transactions, send Poll Response\n");
		
		pAppData->TxBufferSize = 0;
		pAppData->pTxBuffer[pAppData->TxBufferSize++] = pAppData->Rt56Params.addr_filter;

		/* respond with RR including Nr value */
		pAppData->pTxBuffer[pAppData->TxBufferSize++] = NR | RR | FINAL;

		return LoopModeSendLastFrame(pAppData);
	}

	/* send queued transaction frames */
	
	while ( pAppData->pQueuedTransactionList ) {
		int bResult = TRUE;
		PAPP_TRANSACTION pTransaction = pAppData->pQueuedTransactionList;

		Log("DoPollResponse() - preparing to transmit transaction %d\n",
				pTransaction->TransactionNumber );

		pAppData->TxBufferSize = 0;

		/* add address field */
		
		pAppData->pTxBuffer[pAppData->TxBufferSize++] =
				pAppData->Rt56Params.addr_filter;

		/* add sequence numbers and final bit to control field */
		
		pAppData->pTxBuffer[pAppData->TxBufferSize] = NR | NS ;
		if ( pTransaction->pNext == NULL )
			pAppData->pTxBuffer[pAppData->TxBufferSize] |= FINAL;
		++pAppData->TxBufferSize;

		/* add information field (transaction contents */
		
		memcpy( &pAppData->pTxBuffer[pAppData->TxBufferSize],
			pTransaction, sizeof(APP_TRANSACTION) );
		pAppData->TxBufferSize += sizeof(APP_TRANSACTION);

		if ( pTransaction->pNext )
			bResult = SendFrame(pAppData);
		else
			bResult = LoopModeSendLastFrame(pAppData);

		if ( bResult ) {
			Log("DoPollResponse() - transmit frame accepted, "
				"moving transaction to pending list\n");		
			MoveQueuedTransactionToPendingList( pAppData );
			
			/* adjust send sequence number */
			pAppData->SendCount = ++pAppData->SendCount & 0x07;
		} else {
			Log("DoPollResponse() - transmit frame not accepted, "
					"try again later\n");
			return FALSE;
		}
	}

	return TRUE;
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* int SendDiscResponse()
*
* Sends a disconnect response to the primary station
*
* Notes:
*
* input:
*
* 	PAPP_DATA pAppData                  // pointer to application data
*
* returns:
*
* 	TRUE if successful, FALSE otherwise
*
*-----------------------------------------------------------------------------*/
int SendDiscResponse( PAPP_DATA pAppData )
{
	pAppData->TxBufferSize = 0;

	pAppData->pTxBuffer[pAppData->TxBufferSize++] = pAppData->Rt56Params.addr_filter;
	pAppData->pTxBuffer[pAppData->TxBufferSize++] = DISC;

	/*--------------------------------------------------
	 * send frame
	 *--------------------------------------------------*/
	return LoopModeSendLastFrame( pAppData );
}

/* Queue a new transaction to send to primary if transaction timer expired
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int CheckForNewTransactions( PAPP_DATA pAppData )
{
	PAPP_TRANSACTION pNewTransaction;
	
	if ( pAppData->Timers[NEW_TRANSACTION_TIMER] != 0 )
		return TRUE;
		
	/* transaction timer expired */

	if ( pAppData->NumTransactions &&
		pAppData->LastTransactionNumber >= pAppData->NumTransactions ) 
	{
		if ( !pAppData->ShowedLimitHitMessage ) {
			pAppData->ShowedLimitHitMessage = 1;
			Log("CheckForNewTransactions() - user specified limit of %u transactions\n"
				"has been hit, no more transactions will be sent to primary\n",
				pAppData->NumTransactions );
		}
		return TRUE;
	}

	/* allocate a new transaction */
	
	pNewTransaction = malloc( sizeof(APP_TRANSACTION) );
	if ( pNewTransaction == NULL ) {
		Log("CheckForNewTransactions() - out of memory\n");
		return FALSE;
	}
	
	/* initialize transaction */
	
	memset( pNewTransaction, 0, sizeof(APP_TRANSACTION) );

	pNewTransaction->Addr = pAppData->Rt56Params.addr_filter;
	pNewTransaction->Request = TRUE;
	pNewTransaction->TransactionNumber = ++pAppData->LastTransactionNumber;
	gettimeofday( &pNewTransaction->CreationTimeStamp, NULL );

	AddToQueuedTransactionList( pAppData, pNewTransaction );

	/* reset transaction timer */
	pAppData->Timers[NEW_TRANSACTION_TIMER] = pAppData->NewTransactionInterval;

	return TRUE;
}

/* Add transaction to QueuedTransaction List (those waiting
 * to be transmitted to the primary for processing)
 *
 * input:
 *
 * 	pAppData	pointer to application data
 * 	pTransaction	transaction to add to list
 *
 * returns: nothing
 */
void AddToQueuedTransactionList( PAPP_DATA pAppData, PAPP_TRANSACTION pTransaction )
{
	pTransaction->pNext = NULL;
	if ( pAppData->pQueuedTransactionList == NULL ) {
		/* List is empty, add as the one and only entry */
		pAppData->pQueuedTransactionList = pTransaction;
	} else {
		/* List is not empty, add entry to the end of the list */
		((PAPP_TRANSACTION)pAppData->pLastQueuedTransactionList)->pNext = pTransaction;
	}

	/* set new list tail */
	pAppData->pLastQueuedTransactionList = pTransaction;

	++pAppData->NumQueuedTransactions;

	Log("Transaction %u queued for transmission to primary, %u queued\n",
			pTransaction->TransactionNumber,
			pAppData->NumQueuedTransactions );
}

/* Remove and return head of QueuedTransactionList
 *
 * input:	pAppData	pointer to application data
 * returns:	pointer to list head if available, otherwise NULL
 */
PAPP_TRANSACTION RemoveQueuedTransationListHead( PAPP_DATA pAppData )
{
	PAPP_TRANSACTION	pTransaction = NULL;

	if ( pAppData->pQueuedTransactionList ) {
	
		pTransaction = pAppData->pQueuedTransactionList;
		pTransaction->pNext = NULL;
		pAppData->pQueuedTransactionList = pTransaction->pNext;
		--pAppData->NumQueuedTransactions;

		Log( "%u transactions queued for transmission to primary\n",
			pAppData->NumQueuedTransactions );
	}

	return pTransaction;
}

/* Adds the specified transaction to the PendingTransaction List (those
 * waiting for a response from the primary)
 *
 * input:
 *
 * 	pAppData	pointer to application data
 * 	pTransaction	transaction to add to list
 *
 * returns: nothing
 */
void AddToPendingTransactionList( PAPP_DATA pAppData, PAPP_TRANSACTION pTransaction )
{
	pTransaction->pNext = NULL;
	
	/* set new head if list empty, or add to tail of list */
	if ( pAppData->pPendingTransactionList == NULL )
		pAppData->pPendingTransactionList = pTransaction;
	else
		((PAPP_TRANSACTION)pAppData->pLastPendingTransactionList)->pNext = pTransaction;

	/* set new list tail */
	pAppData->pLastPendingTransactionList = pTransaction;
	++pAppData->NumPendingTransactions;

	Log("Transaction %u waiting for response from primary, %u transactions outstanding\n",
		pTransaction->TransactionNumber, pAppData->NumPendingTransactions );
}

/* Move head of queued list (waiting to send) to tail of
 * pending list (sent but not acknowledged).
 *
 * input:	pAppData	pointer to application data
 * returns:	TRUE if successful, FALSE otherwise
 */
int MoveQueuedTransactionToPendingList( PAPP_DATA pAppData )
{
	PAPP_TRANSACTION pTransaction = RemoveQueuedTransationListHead(pAppData);
	if ( pTransaction ) {
		AddToPendingTransactionList( pAppData, pTransaction );
		return TRUE;
	}
	Log("MoveQueuedTransactionToPendingList() - nothing to move!\n");
	return FALSE;
}

/* Reconcile a received transaction with those transactions in the
 * TransactionPendingList
 *
 * input:	pAppData	pointer to application data
 * 		pTransaction	transaction to reconcile
 *
 * returns:	TRUE if successful, FALSE otherwise
 */
int ReconcileTransaction( PAPP_DATA pAppData, PAPP_TRANSACTION pTransaction )
{
	PAPP_TRANSACTION pList = pAppData->pPendingTransactionList;
	PAPP_TRANSACTION pLast = NULL;

	/* Note time of transaction completion */
	gettimeofday(&pTransaction->CompletionTimeStamp, NULL);

	/* search for transaction in pending list */
	
	for (; pList != NULL; pList = pList->pNext) {
	
		/* Match TransactionNumber and CreationTimeStamp */

		if ( pList->TransactionNumber == pTransaction->TransactionNumber &&
			pList->CreationTimeStamp.tv_sec ==
				pTransaction->CreationTimeStamp.tv_sec &&
			pList->CreationTimeStamp.tv_usec ==
				pTransaction->CreationTimeStamp.tv_usec )
		{
			DumpTransaction("Completed transaction", pTransaction);

			/* found transaction, remove from list */
				
			if ( pList == pAppData->pPendingTransactionList )
				pAppData->pPendingTransactionList = pList->pNext;
			else
				pLast->pNext = pList->pNext;
				
			if (pLast && pLast->pNext == NULL)
				pAppData->pLastPendingTransactionList = pLast;
			
			free( pList );

			Log("%u transactions waiting for response from primary\n",
				--pAppData->NumPendingTransactions );

			return TRUE;
		}
		
		DumpTransaction("WARNING, transaction still pending:", pList);
			
		pLast = pList;
	}

	DumpTransaction("No matching found", pTransaction);
	return FALSE;
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
		break;

	case SIGINT :
		/* CTRL-C abort signal */
		++AppData.bExitApplication ;
		break;
	}
}
