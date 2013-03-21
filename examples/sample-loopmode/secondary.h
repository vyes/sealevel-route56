#if !defined(_SECONDARY_H_INCLUDED)
#define _SECONDARY_H_INCLUDED

extern int  main(int argc,char **argv );
extern int  SecondaryLoopTest(struct _APP_DATA *pAppData);
extern int  ProcessRxFrame(struct _APP_DATA *pAppData,unsigned char *, int );
extern int  SendSIMRequest(struct _APP_DATA *pAppData);
extern int  DoPollResponse(struct _APP_DATA *pAppData);
extern int  SendDiscResponse(struct _APP_DATA *pAppData);
extern unsigned int  TimerThread(void *pArg);
extern int  CheckForNewTransactions(struct _APP_DATA *pAppData);
extern void  AddToQueuedTransactionList(struct _APP_DATA *pAppData,struct _APP_TRANSACTION *pTransaction);
extern struct _APP_TRANSACTION * RemoveQueuedTransationListHead(struct _APP_DATA *pAppData);
extern void  AddToPendingTransactionList(struct _APP_DATA *pAppData,struct _APP_TRANSACTION *pTransaction);
extern int  MoveQueuedTransactionToPendingList(struct _APP_DATA *pAppData);
extern int  ReconcileTransaction(struct _APP_DATA *pAppData,struct _APP_TRANSACTION *pTransaction);

#endif
