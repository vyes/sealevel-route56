#if !defined(_PRIMARY_H_INCLUDED)
#define	_PRIMARY_H_INCLUDED

#include "common.h"

extern int main(int argc,char **argv );
extern int PrimaryLoopTest(struct _APP_DATA *pAppData);
extern int CheckTransmit(struct _APP_DATA *pAppData);
extern int ProcessRxFrames(struct _APP_DATA *pAppData,unsigned char *pRxFrame, int RxFrameSize);
extern int AllocateClient(struct _APP_DATA *pAppData,unsigned char Addr);
extern int DeallocateClient(struct _APP_DATA *pAppData,unsigned char Addr);
extern struct _CLIENT *GetClientInfo(struct _APP_DATA *pAppData,unsigned char Addr);
extern int SendSIMResponse(struct _APP_DATA *pAppData,unsigned char Addr);
extern int DoDisc(struct _APP_DATA *pAppData);
extern int ProcessRxDiscFrames(struct _APP_DATA *pAppData,unsigned char *pRxFrame, int RxFrameSize);
extern unsigned int TimerThread(void *pArg);
extern int QueueTransaction(struct _APP_DATA *pAppData,unsigned char *pRxFrame, int RxFrameSize);
extern void RemoveQueuedTransationListHead(struct _APP_DATA *pAppData);
extern int TxReady(struct _APP_DATA *pAppData);
extern int SendGeneralPoll(struct _APP_DATA *pAppData);
extern int SendTransactionResponses(struct _APP_DATA *pAppData);

#endif
