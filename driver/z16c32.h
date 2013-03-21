/********************************************************************/
/*  MP-WAN Developers Toolkit 1995				    */
/*		Copyright Sealevel Systems Inc. 1995		    */
/*   Z16C32.H							    */
/*	This header file defines the internal registers and bit     */
/*	positions of the Zilog Z16C32 Integrated Universal	    */
/*	Serial Controller (IUSC) as they relate to the MP-WAN	    */
/*	adapter.						    */
/********************************************************************/

#ifndef __Z16C32_H__
#define __Z16C32_H__

/********************************************************************/
/*  The registers on the IUSC are divided into a Serial Unit and a  */
/*  DMA Unit for Transmit and Receive.                              */
/********************************************************************/
#define     DMA_T	0x00	/* DMA Transmit Unit                */
#define     DMA_R	0x40	/* DMA Receive Unit                 */
#define     SERIAL_U	0x80	/* Serial Unit                      */

/********************************************************************/
/*  Z16C32 Bus Configuration Register	:   This register is written*/
/*	on the first access to the 16C32 and its value controls how */
/*	the IUSC interacts the rest of the card and the system bus. */
/********************************************************************/
#define     BCR   (SERIAL_U | 0x000)	/* Bus Configuration Register  */

/********************************************************************/
/*  Z16C32 Serial Unit Registers				    */
/********************************************************************/
#define     CCAR  (SERIAL_U | 0x000)	/* Channel Command/Address Reg */
#define     CMR   (SERIAL_U | 0x002)	/* Channel Mode Reg            */
#define     CCSR  (SERIAL_U | 0x004)	/* Channel Command / Status Reg */
#define     CCR   (SERIAL_U | 0x006)	/* Channel Control Reg         */
#define     PSR   (SERIAL_U | 0x008)	/* Port Status Reg             */
#define     PCR   (SERIAL_U | 0x00a)	/* Port Control Reg            */
#define     TMDR  (SERIAL_U | 0x00c)	/* Test Mode Data Reg          */
#define     TMCR  (SERIAL_U | 0x00e)	/* Test Mode Control Reg       */
#define     CMCR  (SERIAL_U | 0x010)	/* Clock Mode Control Reg      */
#define     HCR   (SERIAL_U | 0x012)	/* Hardware Configuration Reg  */
#define     IVR   (SERIAL_U | 0x014)	/* Interrupt Vector Reg        */
#define     IOCR  (SERIAL_U | 0x016)	/* Input / Output Reg          */
#define     ICR   (SERIAL_U | 0x018)	/* Interrupt Control Reg       */
#define     DCCR  (SERIAL_U | 0x01a)	/* Daisy-Chain Control Reg     */
#define     MISR  (SERIAL_U | 0x01c)	/* Misc Interrupt Status Reg   */
#define     SICR  (SERIAL_U | 0x01e)	/* Status Interrupt Control Reg */
#define     RDR   (SERIAL_U | 0x020)	/* Receive Data Reg            */
#define     RMR   (SERIAL_U | 0x022)	/* Receive Mode Reg            */
#define     RCSR  (SERIAL_U | 0x024)	/* Receive Command / Status Reg */
#define     RICR  (SERIAL_U | 0x026)	/* Recieve Interrupt Control Reg */
#define     RSR   (SERIAL_U | 0x028)	/* Receive Sync Reg            */
#define     RCLR  (SERIAL_U | 0x02a)	/* Receive Count Limit Reg     */
#define     RCCR  (SERIAL_U | 0x02c)	/* Receive Character Count Reg */
#define     TC0R  (SERIAL_U | 0x02e)	/* Time Constant 0 Reg         */
#define     TDR   (SERIAL_U | 0x030)	/* Transmit Data Reg           */
#define     TMR   (SERIAL_U | 0x032)	/* Transmit Mode Reg           */
#define     TCSR  (SERIAL_U | 0x034)	/* Transmit Command / Status Reg */
#define     TICR  (SERIAL_U | 0x036)	/* Transmit Interrupt Control Reg */
#define     TSR   (SERIAL_U | 0x038)	/* Transmit Sync Reg           */
#define     TCLR  (SERIAL_U | 0x03a)	/* Transmit Count Limit Reg    */
#define     TCCR  (SERIAL_U | 0x03c)	/* Transmit Character Count Reg */
#define     TC1R  (SERIAL_U | 0x03e)	/* Time Constant 1 Reg         */

/********************************************************************/
/*  Z16C32 DMA Unit Registers					   					*/
/********************************************************************/
#define     DCAR  (DMA_T | 0x000)	/* DMA Command / Address         */
#define     TDMR  (DMA_T | 0x002)	/* Transmit DMA Mode Reg         */
#define     DCR   (DMA_T | 0x006)	/* DMA Control Reg               */
#define     DACR  (DMA_T | 0x008)	/* DMA Array Count Reg           */
#define     BDCR  (DMA_T | 0x012)	/* Burst / Dwell Control Reg     */
#define     DIVR  (DMA_T | 0x014)	/* DMA Interrupt Vector Reg      */
#define     DICR  (DMA_T | 0x018)	/* DMA Interrupt Control Reg     */
#define     CDIR  (DMA_T | 0x01a)	/* Clear DMA Interrupt Reg       */
#define     SDIR  (DMA_T | 0x01c)	/* Set DMA Interrupt Reg         */
#define     TDIAR (DMA_T | 0x01e)	/* Transmit DMA Interrupt Arm Reg */
#define     TBCR  (DMA_T | 0x02a)	/* Transmit Byte Count Reg       */
#define     TARL  (DMA_T | 0x02c)	/* Transmit Address Reg (Lower)   */
#define     TARU  (DMA_T | 0x02e)	/* Transmit Address Reg (Upper)   */
#define     NTBCR (DMA_T | 0x03a)	/* Next Transmit Byte Count Reg   */
#define     NTARL (DMA_T | 0x03c)	/* Next Transmit Address Reg (Lower) */
#define     NTARU (DMA_T | 0x03e)	/* Next Transmit Address Reg (Upper) */
#define     RDMR  (DMA_R | 0x002)	/* Receive DMA Mode Reg          */
#define     RDIAR (DMA_R | 0x01e)	/* Receive DMA Interrupt Arm Reg  */
#define     RBCR  (DMA_R | 0x02a)	/* Receive Byte Count Reg        */
#define     RARL  (DMA_R | 0x02c)	/* Receive Address Reg (Lower)    */
#define     RARU  (DMA_R | 0x02e)	/* Receive Address Reg (Upper)    */
#define     NRBCR (DMA_R | 0x03a)	/* Next Receive Byte Count Reg    */
#define     NRARL (DMA_R | 0x03c)	/* Next Receive Address Reg (Lower) */
#define     NRARU (DMA_R | 0x03e)	/* Next Receive Address Reg (Upper) */

/********************************************************************/
/*	Burst / Dwell Control Reg   (BDCR)			    */
/********************************************************************/
/* FIELD "MaxTransfers" - Numerical values only */

/* FIELD "MaxClocks" - Numerical values only */

/********************************************************************/
/*	Bus Configuration Reg	(BCR)				    */
/********************************************************************/
/* FIELD "SepAdd8BitBus" */
#define    SepAdd8BitBus_No		 0x0
#define    SepAdd8BitBus_Yes		 0x8000

/* FIELD "TriStateAllPins" */
#define  TriStateAllPins_No		 0x0
#define  TriStateAllPins_Yes		 0x4000

/* FIELD "IntAckMode" */
#define       IntAckMode_Status 	 0x0
#define       IntAckMode_SinglePulse	 0x10
#define       IntAckMode_DoublePulse	 0x30

/* FIELD "BRQ_Signal" */
#define       BRQ_Signal_OpenDrain	 0x0
#define       BRQ_Signal_TotemPole	 0x8

/* FIELD "BusWidth" */
#define 	BusWidth_8BitBus	 0x0
#define 	BusWidth_16BitBus	 0x4

/* FIELD "IRQ_Signal" */
#define       IRQ_Signal_TotemPole	 0x0
#define       IRQ_Signal_OpenDrain	 0x2

/* FIELD "ShiftedAddr" */
#define      ShiftedAddr_ShiftLeftMode	 0x0
#define      ShiftedAddr_ShiftRightMode  0x1

/********************************************************************/
/*	Channel Command / Address Reg  (CCAR)			    */
/********************************************************************/
/* FIELD "ChannelCommand" */
#define   ChannelCommand_NullCommand	0x0
#define   ChannelCommand_ResetHighIUS	0x1000
#define   ChannelCommand_TrgChanLoadDMA 0x2000
#define   ChannelCommand_TriggerRxDMA	0x2800
#define   ChannelCommand_TriggerTxDMA	0x3000
#define   ChannelCommand_TrigRx_TxDMA	0x3800
#define   ChannelCommand_RxFIFO_Purge	0x4800
#define   ChannelCommand_TxFIFO_Purge	0x5000
#define   ChannelCommand_Rx_Purge	0xc800
#define   ChannelCommand_RxTxFIFO_Purge 0x5800
#define   ChannelCommand_LoadRxCharCt	0x6800
#define   ChannelCommand_LoadTxCharCt	0x7000
#define   ChannelCommand_LoadRxTxChCt	0x7800
#define   ChannelCommand_LoadTC0	0x8800
#define   ChannelCommand_LoadTC1	0x9000
#define   ChannelCommand_LoadTC0_TC1	0x9800
#define   ChannelCommand_SerlDataLSB1st 0xa000
#define   ChannelCommand_SerlDataMSB1st 0xa800
#define   ChannelCommand_SelctD15_8_1st 0xb000
#define   ChannelCommand_SelctD7_0_1st	0xb800

/* FIELD "ChannelReset" */
#define     ChannelReset_No		0x0
#define     ChannelReset_Yes		0x400

/* FIELD "ModeControl" */
#define      ModeControl_NormalOp	0x0
#define      ModeControl_AutoEcho	0x100
#define      ModeControl_ExtLocalLpback 0x200
#define      ModeControl_IntLocalLpback 0x300

/* FIELD "DMA_ChannelLoad" */
#define  DMA_ChannelLoad_Terminate	0x0
#define  DMA_ChannelLoad_Continue	0x80

/* FIELD "AccessSize" */
#define       AccessSize_Word		0x0
#define       AccessSize_Byte		0x40

/* FIELD "RegAddress" */
#define       RegAddress_CCAR		0x0
#define       RegAddress_CMR		0x2
#define       RegAddress_CCSR		0x4
#define       RegAddress_CCR		0x6
#define       RegAddress_TMDR		0xc
#define       RegAddress_TMCR		0xe
#define       RegAddress_CMCR		0x10
#define       RegAddress_HCR		0x12
#define       RegAddress_IVR		0x14
#define       RegAddress_IOCR		0x16
#define       RegAddress_ICR		0x18
#define       RegAddress_DCCR		0x1a
#define       RegAddress_MISR		0x1c
#define       RegAddress_SICR		0x1e
#define       RegAddress_RDR		0x20
#define       RegAddress_RMR		0x22
#define       RegAddress_RCSR		0x24
#define       RegAddress_RICR		0x26
#define       RegAddress_RSR		0x28
#define       RegAddress_RCLR		0x2a
#define       RegAddress_RCCR		0x2c
#define       RegAddress_TC0R		0x2e
#define       RegAddress_TDR		0x30
#define       RegAddress_TMR		0x32
#define       RegAddress_TCSR		0x34
#define       RegAddress_TICR		0x36
#define       RegAddress_TSR		0x38
#define       RegAddress_TCLR		0x3a
#define       RegAddress_TCCR		0x3c
#define       RegAddress_TC1R		0x3e

/* FIELD "AccessByteOn" */
#define     AccessByteOn_Lower		0x0
#define     AccessByteOn_Upper		0x1

/********************************************************************/
/*	Channel Command / Status Reg (CCSR)			    */
/********************************************************************/
/* FIELD "RCC_FIFO_Overflw" */
#define RCC_FIFO_Overflw_No		 0x0
#define RCC_FIFO_Overflw_Yes		 0x8000

/* FIELD "RCC_FIFO_Avail" */
#define   RCC_FIFO_Avail_No		 0x0
#define   RCC_FIFO_Avail_Yes		 0x4000

/* FIELD "RCC_FIFO_Clear" */
#define   RCC_FIFO_Clear_NullCommand	 0x0
#define   RCC_FIFO_Clear_Reset		 0x2000

/* FIELD "DPLL_SyncStat" */
#define    DPLL_SyncStat_AttemptingSync  0x0
#define    DPLL_SyncStat_AchievedSync	 0x1000

/* FIELD "DPLL_SyncCmd" */
#define     DPLL_SyncCmd_NullCommand	 0x0
#define     DPLL_SyncCmd_ReSync 	 0x1000

/* FIELD "ClksMissLatch" */
#define    ClksMissLatch_CorrctEncodDat  0x0
#define    ClksMissLatch_Miss2ConscCell  0x800

/* FIELD "ClksMissLatchCmd" */
#define ClksMissLatchCmd_NullCommand	 0x0
#define ClksMissLatchCmd_Clear		 0x800

/* FIELD "ClkMissLatch" */
#define     ClkMissLatch_CorrctEncodDat  0x0
#define     ClkMissLatch_MissBitCell	 0x400

/* FIELD "ClkMissLatchCmd" */
#define  ClkMissLatchCmd_NullCommand	 0x0
#define  ClkMissLatchCmd_Clear		 0x400

/* FIELD "DPLL_AdjSyncEdge" */
#define DPLL_AdjSyncEdge_BothEdges	 0x0
#define DPLL_AdjSyncEdge_RisingOnly	 0x100
#define DPLL_AdjSyncEdge_FallingOnly	 0x200
#define DPLL_AdjSyncEdge_AdjSyncInhibit  0x300

/* FIELD "OnLoopSlaveMono" */
#define  OnLoopSlaveMono_Inactive	 0x0
#define  OnLoopSlaveMono_TxmitterActive  0x80

/* FIELD "OnLoopHDLC_Loop" */
#define  OnLoopHDLC_Loop_Inactive	 0x0
#define  OnLoopHDLC_Loop_ChannelInLoop	 0x80

/* FIELD "LoopSending" */
#define      LoopSending_No		 0x0
#define      LoopSending_Yes		 0x40

/* FIELD "PORT0_1BypassCTR" */
#define PORT0_1BypassCTR_No		 0x0
#define PORT0_1BypassCTR_Yes		 0x20	/*Zilog EPM Error: 0x80 */

/* FIELD "HDLC_TxLast" */
#define      HDLC_TxLast_8Bits		 0x0
#define      HDLC_TxLast_1Bit		 0x4
#define      HDLC_TxLast_2Bits		 0x8
#define      HDLC_TxLast_3Bits		 0xc
#define      HDLC_TxLast_4Bits		 0x10
#define      HDLC_TxLast_5Bits		 0x14
#define      HDLC_TxLast_6Bits		 0x18
#define      HDLC_TxLast_7Bits		 0x1c

/********************************************************************/
/*	Channel Control Reg (CCR)				    */
/********************************************************************/
/* FIELD "TxCtrlBlockXfer" */
#define  TxCtrlBlockXfer_NoControlBlock  0x0
#define  TxCtrlBlockXfer_1WordCtrlBlock  0x4000
#define  TxCtrlBlockXfer_2WordCtrlBlock  0x8000

/* FIELD "Wait4TxTrig" */
#define      Wait4TxTrig_No		 0x0
#define      Wait4TxTrig_Yes		 0x2000

/* FIELD "TxFlgPreamble" */
#define    TxFlgPreamble_Disable	 0x0
#define    TxFlgPreamble_Enable 	 0x1000

/* FIELD "TxShavedBitLen" - Numerical values only */

/* FIELD "TxPreambleLength" */
#define TxPreambleLength_8Bits		 0x0
#define TxPreambleLength_16Bits 	 0x400
#define TxPreambleLength_32Bits 	 0x800
#define TxPreambleLength_64Bits 	 0xc00

/* FIELD "TxPreamblPattern" */
#define TxPreamblePattern_AllZeroes	 0x0
#define TxPreamblePattern_AllOnes	 0x100
#define TxPreamblePattern_Alternate1_0	 0x200
#define TxPreamblePattern_Alternate0_1	 0x300

/* FIELD "RxStatBlockXfer" */
#define  RxStatBlockXfer_NoStatusBlock	 0x0
#define  RxStatBlockXfer_1WordStatBlock  0x40
#define  RxStatBlockXfer_2WordStatBlock  0x80

/* FIELD "RxDmaTrigger" */
#define     RxDmaTrigger_Continue	 0x0
#define     RxDmaTrigger_Inhibit	 0x20

/********************************************************************/
/*	Channel Mode Reg (CMR)					    */
/********************************************************************/
/* FIELD "TxMode" */
#define 	  TxMode_Asynchronous	 0x0
#define 	  TxMode_Isochronous	 0x200
#define 	  TxMode_AsyncCodeViol	 0x300
#define 	  TxMode_Monosync	 0x400
#define 	  TxMode_Bisync 	 0x500
#define 	  TxMode_HDLC		 0x600
#define 	  TxMode_TranspBisync	 0x700
#define 	  TxMode_NBIP		 0x800
#define 	  TxMode_802_3		 0x900
#define 	  TxMode_SlavedMonosync  0xc00
#define 	  TxMode_HDLC_Loop	 0xe00

/* FIELD "RxMode" */
#define 	  RxMode_Asynchronous	 0x0
#define 	  RxMode_ExternalSync	 0x1
#define 	  RxMode_Isochronous	 0x2
#define 	  RxMode_AsyncCodeViol	 0x3
#define 	  RxMode_Monosync	 0x4
#define 	  RxMode_Bisync 	 0x5
#define 	  RxMode_HDLC		 0x6
#define 	  RxMode_TranspBisync	 0x7
#define 	  RxMode_NBIP		 0x8
#define 	  RxMode_802_3		 0x9

/* FIELD "TxStopBitsAsync" */
#define  TxStopBitsAsync_One		 0x0
#define  TxStopBitsAsync_Two		 0x4000
#define  TxStopBitsAsync_OneShaved	 0x8000
#define  TxStopBitsAsync_TwoShaved	 0xc000

/* FIELD "TxClockRateAsync" */
#define TxClockRateAsync_DataRate16X	 0x0
#define TxClockRateAsync_DataRate32X	 0x1000
#define TxClockRateAsync_DataRate64X	 0x2000

/* FIELD "RxClockRateAsync" */
#define RxClockRateAsync_DataRate16X	 0x0
#define RxClockRateAsync_DataRate32X	 0x10
#define RxClockRateAsync_DataRate64X	 0x20

/* FIELD "TxStopBitsIso" */
#define    TxStopBitsIso_One		 0x0
#define    TxStopBitsIso_Two		 0x4000

/* FIELD "TxStopBitsACV" */
#define    TxStopBitsACV_One		 0x0
#define    TxStopBitsACV_Two		 0x4000
#define    TxStopBitsACV_None		 0x8000

/* FIELD "TxExtendedWord" */
#define   TxExtendedWord_No		 0x0
#define   TxExtendedWord_Yes		 0x2000

/* FIELD "CV_Polarity" */
#define      CV_Polarity_ZeroCode	 0x0
#define      CV_Polarity_OneCode	 0x1000

/* FIELD "RxExtendedWord" */
#define   RxExtendedWord_No		 0x0
#define   RxExtendedWord_Yes		 0x10

/* FIELD "TxCRC_OnUnderMon" */
#define TxCRC_OnUnderMon_No		 0x0
#define TxCRC_OnUnderMon_Yes		 0x8000

/* FIELD "TxPreambleEnMon" */
#define	TxPreambleEnMon_No		 0x0
#define  TxPreambleEnMon_Yes		 0x2000

/* FIELD "TxSyncCharMon" */
#define    TxSyncCharMon_8BitsChar	 0x0
#define    TxSyncCharMon_MatchTxBitChar  0x1000

/* FIELD "RxSyncStripMon" */
#define   RxSyncStripMon_No		 0x0
#define   RxSyncStripMon_Yes		 0x20

/* FIELD "RxSyncCharMon" */
#define    RxSyncCharMon_8BitsChar	 0x0
#define    RxSyncCharMon_MatchRxBitChar  0x10

/* FIELD "TxUnderIdleBi" */
#define    TxUnderIdleBi_Syn1		 0x0
#define    TxUnderIdleBi_Syn0_Syn1	 0x4000
#define    TxUnderIdleBi_CRC_Syn1	 0x8000
#define    TxUnderIdleBi_CRC_Syn0_Syn1	 0xc000

/* FIELD "TxPreambleEnBi" */
#define   TxPreambleEnBi_No		 0x0
#define   TxPreambleEnBi_Yes		 0x2000

/* FIELD "TxSyncCharBi" */
#define     TxSyncCharBi_8BitsChar	 0x0
#define     TxSyncCharBi_MatchTxBitChar  0x1000

/* FIELD "RxSyncStripBi" */
#define    RxSyncStripBi_No		 0x0
#define    RxSyncStripBi_Yes		 0x20

/* FIELD "RxSyncCharBi" */
#define     RxSyncCharBi_8BitsChar	 0x0
#define     RxSyncCharBi_MatchRxBitChar  0x10

/* FIELD "TxUnderCondHDLC" */
#define  TxUnderCondHDLC_Abort		 0x0
#define  TxUnderCondHDLC_ExtendedAbort	 0x4000
#define  TxUnderCondHDLC_Flag		 0x8000
#define  TxUnderCondHDLC_CRC_Flag	 0xc000

/* FIELD "TxPreambleEnHDLC" */
#define TxPreambleEnHDLC_No		 0x0
#define TxPreambleEnHDLC_Yes		 0x2000

/* FIELD "SharZeroFlagHDLC" */
#define SharZeroFlagHDLC_No		 0x0
#define SharZeroFlagHDLC_Yes		 0x1000

/* FIELD "RxLogContEnable" */
#define  RxLogContEnable_No		 0x0
#define  RxLogContEnable_Yes		 0x80

/* FIELD "Rx16BitControl" */
#define   Rx16BitControl_No		 0x0
#define   Rx16BitControl_Yes		 0x40

/* FIELD "RxAddSearchMode" */
#define  RxAddSearchMode_Disabled	 0x0
#define  RxAddSearchMode_1ByteNoCntl	 0x10
#define  RxAddSearchMode_1BytePlusCntl	 0x20
#define  RxAddSearchMode_ExtendPlusCntl  0x30

/* FIELD "TxUnderCondTrBi" */
#define  TxUnderCondTrBi_SYN		 0x0
#define  TxUnderCondTrBi_DLE_SYN	 0x4000
#define  TxUnderCondTrBi_CRC_SYN	 0x8000
#define  TxUnderCondTrBi_CRC_DLE_SYN	 0xc000

/* FIELD "TxPreambleEnTrBi" */
#define TxPreambleEnTrBi_No		 0x0
#define TxPreambleEnTrBi_Yes		 0x2000

/* FIELD "TxCtrlCharCoding" */
#define TxCtrlCharCoding_ASCII		 0x0
#define TxCtrlCharCoding_EDBDIC 	 0x1000

/* FIELD "RxCtrlCharCoding" */
#define RxCtrlCharCoding_ASCII		 0x0
#define RxCtrlCharCoding_EDBDIC 	 0x10

/* FIELD "AddressDataBit9" */
#define  AddressDataBit9_DataByte	 0x0
#define  AddressDataBit9_AddressByte	 0x8000

/* FIELD "TxParityOnData" */
#define   TxParityOnData_No		 0x0
#define   TxParityOnData_Yes		 0x4000

/* FIELD "TxClockRate" */
#define      TxClockRate_16X		 0x0
#define      TxClockRate_32X		 0x1000
#define      TxClockRate_64X		 0x2000

/* FIELD "RxParityOnData" */
#define   RxParityOnData_No		 0x0
#define   RxParityOnData_Yes		 0x40

/* FIELD "RxClockRate" */
#define      RxClockRate_16X		 0x0
#define      RxClockRate_32X		 0x10
#define      RxClockRate_64X		 0x20

/* FIELD "TxCRC_OnUnder802" */
#define TxCRC_OnUnder802_WithoutCRC	 0x0
#define TxCRC_OnUnder802_WithCRC	 0x8000

/* FIELD "RxAddSearch" */
#define      RxAddSearch_No		 0x0
#define      RxAddSearch_Yes		 0x10

/* FIELD "TxCRC_OnUnderSM" */
#define  TxCRC_OnUnderSM_WithoutCRC	 0x0
#define  TxCRC_OnUnderSM_WithCRC	 0x8000

/* FIELD "TxActiveOnRxSync" */
#define TxActiveOnRxSync_No		 0x0
#define TxActiveOnRxSync_Yes		 0x2000

/* FIELD "TxSyncCharSM" */
#define     TxSyncCharSM_8BitsChar	 0x0
#define     TxSyncCharSM_MatchTxBitChar  0x1000

/* FIELD "TxUnderCondLoop" */
#define  TxUnderCondLoop_Abort		 0x0
#define  TxUnderCondLoop_ExtendedAbort	 0x4000
#define  TxUnderCondLoop_Flag		 0x8000
#define  TxUnderCondLoop_CRC_Flag	 0xc000

/* FIELD "TxActiveOnPoll" */
#define   TxActiveOnPoll_No		 0x0
#define   TxActiveOnPoll_Yes		 0x2000

/* FIELD "SharZeroFlagLoop" */
#define SharZeroFlagLoop_No		 0x0
#define SharZeroFlagLoop_Yes		 0x1000

/********************************************************************/
/*	Clear DMA Interrupt Reg (CDIR)				    */
/********************************************************************/
/* FIELD "Reserve1_CDIR" - Numerical values only */

/* FIELD "ResetDMA_RxIUS" */
#define   ResetDMA_RxIUS_NullCommand	 0x0
#define   ResetDMA_RxIUS_Reset		 0x200

/* FIELD "ResetDMA_TxIUS" */
#define   ResetDMA_TxIUS_NullCommand	 0x0
#define   ResetDMA_TxIUS_Reset		 0x100

/* FIELD "Reserve2_CDIR" - Numerical values only */

/* FIELD "ResetDMA_RxIP" */
#define    ResetDMA_RxIP_NullCommand	 0x0
#define    ResetDMA_RxIP_Reset		 0x2

/* FIELD "ResetDMA_TxIP" */
#define    ResetDMA_TxIP_NullCommand	 0x0
#define    ResetDMA_TxIP_Reset		 0x1

/********************************************************************/
/*	Clock Mode Control Reg (CMCR)				    */
/********************************************************************/
/* FIELD "CTR1_ClkSource" */
#define   CTR1_ClkSource_Disabled	 0x0
#define   CTR1_ClkSource_Port1_Pin	 0x4000
#define   CTR1_ClkSource_RxC_Pin	 0x8000
#define   CTR1_ClkSource_TxC_Pin	 0xc000

/* FIELD "CTR0_ClkSource" */
#define   CTR0_ClkSource_Disabled	 0x0
#define   CTR0_ClkSource_Port0_Pin	 0x1000
#define   CTR0_ClkSource_RxC_Pin	 0x2000
#define   CTR0_ClkSource_TxC_Pin	 0x3000

/* FIELD "BRG1_ClkSource" */
#define   BRG1_ClkSource_CTR0_Output	 0x0
#define   BRG1_ClkSource_CTR1_Output	 0x400
#define   BRG1_ClkSource_RxC_Pin	 0x800
#define   BRG1_ClkSource_TxC_Pin	 0xc00

/* FIELD "BRG0_ClkSource" */
#define   BRG0_ClkSource_CTR0_Output	 0x0
#define   BRG0_ClkSource_CTR1_Output	 0x100
#define   BRG0_ClkSource_RxC_Pin	 0x200
#define   BRG0_ClkSource_TxC_Pin	 0x300

/* FIELD "DPLL_ClkSource" */
#define   DPLL_ClkSource_BRG0_Output	 0x0
#define   DPLL_ClkSource_BRG1_Output	 0x40
#define   DPLL_ClkSource_RxC_Pin	 0x80
#define   DPLL_ClkSource_TxC_Pin	 0xc0

/* FIELD "TxClkSource" */
#define      TxClkSource_Disabled	 0x0
#define      TxClkSource_RxC_Pin	 0x8
#define      TxClkSource_TxC_Pin	 0x10
#define      TxClkSource_DPLL_Output	 0x18
#define      TxClkSource_BRG0_Output	 0x20
#define      TxClkSource_BRG1_Output	 0x28
#define      TxClkSource_CTR0_Output	 0x30
#define      TxClkSource_CTR1_Output	 0x38

/* FIELD "RxClkSource" */
#define      RxClkSource_Disabled	 0x0
#define      RxClkSource_RxC_Pin	 0x1
#define      RxClkSource_TxC_Pin	 0x2
#define      RxClkSource_DPLL_Output	 0x3
#define      RxClkSource_BRG0_Output	 0x4
#define      RxClkSource_BRG1_Output	 0x5
#define      RxClkSource_CTR0_Output	 0x6
#define      RxClkSource_CTR1_Output	 0x7

/********************************************************************/
/*	Daisy Chain Control Reg (DCCR)				    */
/********************************************************************/
/* FIELD "IUS_Command" */
#define      IUS_Command_NullCommand	 0x0
#define      IUS_Command_NullCommand_	 0x4000
#define      IUS_Command_ResetIUS	 0x8000
#define      IUS_Command_SetIUS 	 0xc000

/* FIELD "RxStatusIUS" */
#define      RxStatusIUS_No		 0x0
#define      RxStatusIUS_Yes		 0x2000

/* FIELD "RxDataIUS" */
#define        RxDataIUS_No		 0x0
#define        RxDataIUS_Yes		 0x1000

/* FIELD "TxStatusIUS" */
#define      TxStatusIUS_No		 0x0
#define      TxStatusIUS_Yes		 0x800

/* FIELD "TxDataIUS" */
#define        TxDataIUS_No		 0x0
#define        TxDataIUS_Yes		 0x400

/* FIELD "IO_StatusIUS" */
#define     IO_StatusIUS_No		 0x0
#define     IO_StatusIUS_Yes		 0x200

/* FIELD "DeviceStatusIUS" */
#define  DeviceStatusIUS_No		 0x0
#define  DeviceStatusIUS_Yes		 0x100

/* FIELD "IP_Command" */
#define       IP_Command_NullCommand	 0x0
#define       IP_Command_ResetIP_IUS	 0x40
#define       IP_Command_ResetIP	 0x80
#define       IP_Command_SetIP		 0xc0

/* FIELD "RxStatusIP" */
#define       RxStatusIP_No		 0x0
#define       RxStatusIP_Yes		 0x20

/* FIELD "RxDataIP" */
#define 	RxDataIP_No		 0x0
#define 	RxDataIP_Yes		 0x10

/* FIELD "TxStatusIP" */
#define       TxStatusIP_No		 0x0
#define       TxStatusIP_Yes		 0x8

/* FIELD "TxDataIP" */
#define 	TxDataIP_No		 0x0
#define 	TxDataIP_Yes		 0x4

/* FIELD "IO_StatusIP" */
#define      IO_StatusIP_No		 0x0
#define      IO_StatusIP_Yes		 0x2

/* FIELD "DeviceStatusIP" */
#define   DeviceStatusIP_No		 0x0
#define   DeviceStatusIP_Yes		 0x1

/********************************************************************/
/*	DMA Array Count Reg (DACR)				    */
/********************************************************************/
/* FIELD "ReservedDACR" - Numerical values only */

/* FIELD "RxDMAFetchStat" */
#define   RxDMAFetchStat_SglBufOrPipe	 0x0
#define   RxDMAFetchStat_FetchPending	 0xf0
#define   RxDMAFetchStat_BufLnkAdrByte1  0xe0
#define   RxDMAFetchStat_BufLnkAdrByte2  0xd0
#define   RxDMAFetchStat_BufLnkAdrByte3  0xc0
#define   RxDMAFetchStat_BufLnkAdrByte4  0xb0
#define   RxDMAFetchStat_ByteCountByte1  0xa0
#define   RxDMAFetchStat_ByteCountByte2  0x90
#define   RxDMAFetchStat_CountByte1Clr	 0x80
#define   RxDMAFetchStat_CountByte2Clr	 0x70
#define   RxDMAFetchStat_RSBByte1Stored  0x60
#define   RxDMAFetchStat_RSBStatStored	 0x50
#define   RxDMAFetchStat_RSBByte3Stored  0x40
#define   RxDMAFetchStat_RSBRCCStored	 0x30
#define   RxDMAFetchStat_Byte11Cleared	 0x20
#define   RxDMAFetchStat_Byte12Cleared	 0x10

/* FIELD "TxDMAFetchStat" */
#define   TxDMAFetchStat_SglBufOrPipe	 0x0
#define   TxDMAFetchStat_FetchPending	 0xf
#define   TxDMAFetchStat_BufLnkAdrByte1  0xe
#define   TxDMAFetchStat_BufLnkAdrByte2  0xd
#define   TxDMAFetchStat_BufLnkAdrByte3  0xc
#define   TxDMAFetchStat_BufLnkAdrByte4  0xb
#define   TxDMAFetchStat_ByteCountByte1  0xa
#define   TxDMAFetchStat_ByteCountByte2  0x9
#define   TxDMAFetchStat_CountByte1Clr	 0x8
#define   TxDMAFetchStat_CountByte2Clr	 0x7
#define   TxDMAFetchStat_TCBByte1	 0x6
#define   TxDMAFetchStat_TCBControlWord  0x5
#define   TxDMAFetchStat_TCBByte3	 0x4
#define   TxDMAFetchStat_TCBTCCWord	 0x3
#define   TxDMAFetchStat_Byte11Read	 0x2
#define   TxDMAFetchStat_Byte12Read	 0x1

/********************************************************************/
/*	DMA Command / Address Reg (DCAR)			    */
/********************************************************************/
/* FIELD "DMA_ChannelCmd" */
#define   DMA_ChannelCmd_NullCommand	 0x0
#define   DMA_ChannelCmd_ResetChannel	 0x1000
#define   DMA_ChannelCmd_StartChannel	 0x2000
#define   DMA_ChannelCmd_StartContChanl  0x3000
#define   DMA_ChannelCmd_PauseChannel	 0x4000
#define   DMA_ChannelCmd_AbortChannel	 0x5000
#define   DMA_ChannelCmd_StartInitChanl  0x7000
#define   DMA_ChannelCmd_ResetHighstIUS  0x8000
#define   DMA_ChannelCmd_ResetBoth	 0x9000
#define   DMA_ChannelCmd_StartBoth	 0xa000
#define   DMA_ChannelCmd_StartContBoth	 0xb000
#define   DMA_ChannelCmd_PauseBoth	 0xc000
#define   DMA_ChannelCmd_AbortBoth	 0xd000
#define   DMA_ChannelCmd_StartInitBoth	 0xf000

/* FIELD "Channel" */
#define 	 Channel_Tx		 0x0
#define 	 Channel_Rx		 0x200

/* FIELD "MasterBusReqEn" */
#define   MasterBusReqEn_No		 0x0
#define   MasterBusReqEn_Yes		 0x100

/* FIELD "DMA_RxTx" */
#define 	DMA_RxTx_TxReg		 0x0
#define 	DMA_RxTx_RxReg		 0x80

/* FIELD "DMA_AccessSize" */
#define   DMA_AccessSize_Word		 0x0
#define   DMA_AccessSize_Byte		 0x40

/* FIELD "DMA_RegAddress" */
#define   DMA_RegAddress_DCAR		 0x0
#define   DMA_RegAddress_TDMR_RDMR	 0x2
#define   DMA_RegAddress_DCR		 0x6
#define   DMA_RegAddress_DACR		 0x8
#define   DMA_RegAddress_BDCR		 0x12
#define   DMA_RegAddress_DIVR		 0x14
#define   DMA_RegAddress_DICR		 0x18
#define   DMA_RegAddress_CDIR		 0x1a
#define   DMA_RegAddress_SDIR		 0x1c
#define   DMA_RegAddress_TDIAR_RDIAR	 0x1e
#define   DMA_RegAddress_TBCR_RBCR	 0x2a
#define   DMA_RegAddress_TARL_RARL	 0x2c
#define   DMA_RegAddress_TARU_RARU	 0x2e
#define   DMA_RegAddress_NTBCR_NRBCR	 0x3a
#define   DMA_RegAddress_NTARL_NRARL	 0x3c
#define   DMA_RegAddress_NTARU_NRARU	 0x3e

/* FIELD "DMA_AccessByteOn" */
#define DMA_AccessByteOn_Lower		 0x0
#define DMA_AccessByteOn_Upper		 0x1

/********************************************************************/
/*	DMA Control Reg (DCR)					    */
/********************************************************************/
/* FIELD "ChannelPriority" */
#define  ChannelPriority_Tx		 0x0
#define  ChannelPriority_Rx		 0x4000
#define  ChannelPriority_Alternating	 0x8000

/* FIELD "PreEmptEnable" */
#define    PreEmptEnable_No		 0x0
#define    PreEmptEnable_Yes		 0x2000

/* FIELD "BinaryValueOrder" */
#define BinaryValueOrder_Z80_Intel	 0x0
#define BinaryValueOrder_Z8000_Motorola  0x1000

/* FIELD "ReArbTime" */
#define        ReArbTime_TwoPerGrant	 0x0
#define        ReArbTime_OnePerSerReq	 0x400
#define        ReArbTime_OnePerGrant	 0x800

/* FIELD "ReservedDCR" */
#define      ReservedDCR_MustBeZero	 0x0

/* FIELD "MinOff39" */
#define 	MinOff39_7_ClockPeriod	 0x0
#define 	MinOff39_39_ClockPeriod  0x20

/* FIELD "DC_SD_Out" */
#define        DC_SD_Out_No		 0x0
#define        DC_SD_Out_Yes		 0x10

/* FIELD "Wait_1" */
#define 	  Wait_1_No		 0x0
#define 	  Wait_1_Yes		 0x8

/* FIELD "UAS_Frequency" */
#define    UAS_Frequency_WhenNecessary	 0x0
#define    UAS_Frequency_OnAllCycles	 0x4

/* FIELD "AddrSeg" */
#define 	 AddrSeg_32_Bits	 0x0
#define 	 AddrSeg_16_Bits	 0x2
#define 	 AddrSeg_24_Bits	 0x3

/********************************************************************/
/*	DMA Interrupt Control Reg (DICR)			    */
/********************************************************************/
/* FIELD "DMA_MasterIntEn" */
#define  DMA_MasterIntEn_No		 0x0
#define  DMA_MasterIntEn_Yes		 0x8000

/* FIELD "DMA_LowChainInt" */
#define  DMA_LowChainInt_Enabled	 0x0
#define  DMA_LowChainInt_Disabled	 0x4000

/* FIELD "DMA_IntVector" */
#define    DMA_IntVector_Enabled	 0x0
#define    DMA_IntVector_Disabled	 0x2000

/* FIELD "DMA_VectInclStat" */
#define DMA_VectInclStat_No		 0x0
#define DMA_VectInclStat_Yes		 0x1000

/* FIELD "ReservedDICR" - Numerical values only */

/* FIELD "DMA_Rx_IE" */
#define        DMA_Rx_IE_No		 0x0
#define        DMA_Rx_IE_Yes		 0x2

/* FIELD "DMA_Tx_IE" */
#define        DMA_Tx_IE_No		 0x0
#define        DMA_Tx_IE_Yes		 0x1

/********************************************************************/
/*	DMA Interrupt Vector Reg (DIVR)				    */
/********************************************************************/
/* FIELD "DMA_ModifVector" - Numerical values only */

/* FIELD "DMA_IntCode" */
#define      DMA_IntCode_None		 0x0
#define      DMA_IntCode_TxIntPending	 0x400
#define      DMA_IntCode_RxIntPending	 0x600

/* FIELD "DMA_IntVectField" - Numerical values only */

/********************************************************************/
/*	Hardware Configuration Reg (HCR)			    */
/********************************************************************/
/* FIELD "CTR0_Divisor" */
#define     CTR0_Divisor_32X		 0x0
#define     CTR0_Divisor_16X		 0x4000
#define     CTR0_Divisor_8X		 0x8000
#define     CTR0_Divisor_4X		 0xc000

/* FIELD "CTR1_DivisorCTR0" */
#define CTR1_DivisorCTR0_32X		 0x0
#define CTR1_DivisorCTR0_16X		 0x4000
#define CTR1_DivisorCTR0_8X		 0x8000
#define CTR1_DivisorCTR0_4X		 0xc000

/* FIELD "CTR1_DivisorSel" */
#define  CTR1_DivisorSel_SameAsCTR0	 0x0
#define  CTR1_DivisorSel_SameAsDPLL	 0x2000

/* FIELD "AcceptCodeViol" */
#define   AcceptCodeViol_No		 0x0
#define   AcceptCodeViol_Yes		 0x1000

/* FIELD "DPLL_Divisor" */
#define     DPLL_Divisor_32X		 0x0
#define     DPLL_Divisor_16X		 0x400
#define     DPLL_Divisor_8X		 0x800

/* FIELD "CTR1_DivisorDPLL" */
#define CTR1_DivisorDPLL_32X		 0x0
#define CTR1_DivisorDPLL_16X		 0x400
#define CTR1_DivisorDPLL_8X		 0x800
#define CTR1_DivisorDPLL_4X		 0xc00

/* FIELD "DPLL_Mode" */
#define        DPLL_Mode_Disabled	 0x0
#define        DPLL_Mode_NRZ_NRZI	 0x100
#define        DPLL_Mode_BiphasMarkSpac  0x200
#define        DPLL_Mode_BiphaseLevel	 0x300

/* FIELD "BRG1_SingleCont" */
#define  BRG1_SingleCont_ContinuousMode  0x0
#define  BRG1_SingleCont_SinglCycleMode  0x20

/* FIELD "BRG1_Enable" */
#define      BRG1_Enable_No		 0x0
#define      BRG1_Enable_Yes		 0x10

/* FIELD "BRG0_SingleCont" */
#define  BRG0_SingleCont_ContinuousMode  0x0
#define  BRG0_SingleCont_SinglCycleMode  0x2

/* FIELD "BRG0_Enable" */
#define      BRG0_Enable_No		 0x0
#define      BRG0_Enable_Yes		 0x1

/********************************************************************/
/*	Input / Output Control Reg (IOCR)			    */
/********************************************************************/
/* FIELD "CTS_PinControl" */
#define   CTS_PinControl_CTS_Input	 0x0
#define   CTS_PinControl_CTS_Input_	 0x4000
#define   CTS_PinControl_Output0	 0x8000
#define   CTS_PinControl_Output1	 0xc000

/* FIELD "DCD_PinControl" */
#define   DCD_PinControl_DCD_Input	 0x0
#define   DCD_PinControl_DCD_SYNC_Input  0x1000
#define   DCD_PinControl_Output0	 0x2000
#define   DCD_PinControl_Output1	 0x3000

/* FIELD "TxREQ_PinControl" */
#define TxREQ_PinControl_TriStateOutput  0x0
#define TxREQ_PinControl_TxRequstOutput  0x400
#define TxREQ_PinControl_Output0	 0x800
#define TxREQ_PinControl_Output1	 0xc00

/* FIELD "RxREQ_PinControl" */
#define RxREQ_PinControl_TriStateOutput  0x0
#define RxREQ_PinControl_RxRequstOutput  0x100
#define RxREQ_PinControl_Output0	 0x200
#define RxREQ_PinControl_Output1	 0x300

/* FIELD "TxD_PinControl" */
#define   TxD_PinControl_TxDataOutput	 0x0
#define   TxD_PinControl_TriStateOutput  0x40
#define   TxD_PinControl_Output0	 0x80
#define   TxD_PinControl_Output1	 0xc0

/* FIELD "TxC_PinControl" */
#define   TxC_PinControl_InputPin	 0x0
#define   TxC_PinControl_TxClkOutput	 0x8
#define   TxC_PinControl_TxBytClkOutput  0x10
#define   TxC_PinControl_TxCompltOutput  0x18
#define   TxC_PinControl_BRG0_Output	 0x20
#define   TxC_PinControl_BRG1_Output	 0x28
#define   TxC_PinControl_CTR1_Output	 0x30
#define   TxC_PinControl_DPLL_TxOutput	 0x38

/* FIELD "RxC_PinControl" */
#define   RxC_PinControl_InputPin	 0x0
#define   RxC_PinControl_RxClkOutput	 0x1
#define   RxC_PinControl_RxBytClkOutput  0x2
#define   RxC_PinControl_SYNC_Output	 0x3
#define   RxC_PinControl_BRG0_Output	 0x4
#define   RxC_PinControl_BRG1_Output	 0x5
#define   RxC_PinControl_CTR0_Output	 0x6
#define   RxC_PinControl_DPLL_RxOutput	 0x7

/********************************************************************/
/*	Interrupt Control Reg (ICR)				    */
/********************************************************************/
/* FIELD "MasterIntEnable" */
#define  MasterIntEnable_No		 0x0
#define  MasterIntEnable_Yes		 0x8000

/* FIELD "LowerChainIntCtl" */
#define LowerChainIntCtl_Enabled	 0x0
#define LowerChainIntCtl_Disabled	 0x4000

/* FIELD "IntVectControl" */
#define   IntVectControl_Enabled	 0x0
#define   IntVectControl_Disabled	 0x2000

/* FIELD "VectIncludeStat" */
#define  VectIncludeStat_No		 0x0
#define  VectIncludeStat_Yes		 0x1000

/* FIELD "VIS_Level" */
#define        VIS_Level_EnblAllSrcs	 0x0
#define        VIS_Level_EnblAllSrcs_	 0x200
#define        VIS_Level_IO_Stat_Above	 0x400
#define        VIS_Level_TxData_Above	 0x600
#define        VIS_Level_TxStat_Above	 0x800
#define        VIS_Level_RxData_Above	 0xa00
#define        VIS_Level_RxStat_Above	 0xc00
#define        VIS_Level_None		 0xe00

/* FIELD "IE_Command" */
#define       IE_Command_NullCommand	 0x0
#define       IE_Command_NullCommand_	 0x40
#define       IE_Command_ResetIE	 0x80
#define       IE_Command_SetIE		 0xc0

/* FIELD "RxStatusIE" */
#define       RxStatusIE_No		 0x0
#define       RxStatusIE_Yes		 0x20

/* FIELD "RxDataIE" */
#define 	RxDataIE_No		 0x0
#define 	RxDataIE_Yes		 0x10

/* FIELD "TxStatusIE" */
#define       TxStatusIE_No		 0x0
#define       TxStatusIE_Yes		 0x8

/* FIELD "TxDataIE" */
#define 	TxDataIE_No		 0x0
#define 	TxDataIE_Yes		 0x4

/* FIELD "IO_StatusIE" */
#define      IO_StatusIE_No		 0x0
#define      IO_StatusIE_Yes		 0x2

/* FIELD "DeviceStatusIE" */
#define   DeviceStatusIE_No		 0x0
#define   DeviceStatusIE_Yes		 0x1

/********************************************************************/
/*	Interrupt Vector Reg (IVR)				    */
/********************************************************************/
/* FIELD "ModifiedVector" - Numerical values only */

/* FIELD "InterruptCode" */
#define    InterruptCode_None		 0x0
#define    InterruptCode_DeviceStatus	 0x200
#define    InterruptCode_IO_Status	 0x400
#define    InterruptCode_TxData 	 0x600
#define    InterruptCode_TxStatus	 0x800
#define    InterruptCode_RxData 	 0xa00
#define    InterruptCode_RxStatus	 0xc00
#define    InterruptCode_NotUsed	 0xe00

/* FIELD "IntVectCtrlField" - Numerical values only */

/********************************************************************/
/*	Misc Interrupt Status Reg (MISR)			    */
/********************************************************************/
/* FIELD "RxC_TransStat" */
#define    RxC_TransStat_UnLatched	 0x0
#define    RxC_TransStat_Latched	 0x8000

/* FIELD "RxC_TransCmd" */
#define     RxC_TransCmd_NullCommand	 0x0
#define     RxC_TransCmd_ResetLatch	 0x8000

/* FIELD "RxC_PinStatus" */
#define    RxC_PinStatus_Zero		 0x0
#define    RxC_PinStatus_One		 0x4000

/* FIELD "TxC_TransStat" */
#define    TxC_TransStat_UnLatched	 0x0
#define    TxC_TransStat_Latched	 0x2000

/* FIELD "TxC_TransCmd" */
#define     TxC_TransCmd_NullCommand	 0x0
#define     TxC_TransCmd_ResetLatch	 0x2000

/* FIELD "TxC_PinStatus" */
#define    TxC_PinStatus_Zero		 0x0
#define    TxC_PinStatus_One		 0x1000

/* FIELD "RxREQ_TransStat" */
#define  RxREQ_TransStat_UnLatched	 0x0
#define  RxREQ_TransStat_Latched	 0x800

/* FIELD "RxREQ_TransCmd" */
#define   RxREQ_TransCmd_NullCommand	 0x0
#define   RxREQ_TransCmd_ResetLatch	 0x800

/* FIELD "RxREQ_PinStatus" */
#define  RxREQ_PinStatus_Zero		 0x0
#define  RxREQ_PinStatus_One		 0x400

/* FIELD "TxREQ_TransStat" */
#define  TxREQ_TransStat_UnLatched	 0x0
#define  TxREQ_TransStat_Latched	 0x200

/* FIELD "TxREQ_TransCmd" */
#define   TxREQ_TransCmd_NullCommand	 0x0
#define   TxREQ_TransCmd_ResetLatch	 0x200

/* FIELD "TxREQ_PinStatus" */
#define  TxREQ_PinStatus_Zero		 0x0
#define  TxREQ_PinStatus_One		 0x100

/* FIELD "DCD_TransStat" */
#define    DCD_TransStat_UnLatched	 0x0
#define    DCD_TransStat_Latched	 0x80

/* FIELD "DCD_TransCmd" */
#define     DCD_TransCmd_NullCommand	 0x0
#define     DCD_TransCmd_ResetLatch	 0x80

/* FIELD "DCD_PinStatus" */
#define    DCD_PinStatus_Zero		 0x0
#define    DCD_PinStatus_One		 0x40

/* FIELD "CTS_TransStat" */
#define    CTS_TransStat_UnLatched	 0x0
#define    CTS_TransStat_Latched	 0x20

/* FIELD "CTS_TransCmd" */
#define     CTS_TransCmd_NullCommand	 0x0
#define     CTS_TransCmd_ResetLatch	 0x20

/* FIELD "CTS_PinStatus" */
#define    CTS_PinStatus_Zero		 0x0
#define    CTS_PinStatus_One		 0x10

/* FIELD "RCC_OverStat" */
#define     RCC_OverStat_UnLatched	 0x0
#define     RCC_OverStat_Latched	 0x8

/* FIELD "RCC_OverCmd" */
#define      RCC_OverCmd_NullCommand	 0x0
#define      RCC_OverCmd_ResetLatch	 0x8

/* FIELD "DPLL_LatchStat" */
#define   DPLL_LatchStat_UnLatched	 0x0
#define   DPLL_LatchStat_Latched	 0x4

/* FIELD "DPLL_LatchCmd" */
#define    DPLL_LatchCmd_NullCommand	 0x0
#define    DPLL_LatchCmd_ResetLatch	 0x4

/* FIELD "BRG1_ZC_Stat" */
#define     BRG1_ZC_Stat_UnLatched	 0x0
#define     BRG1_ZC_Stat_Latched	 0x2

/* FIELD "BRG1_ZC_Cmd" */
#define      BRG1_ZC_Cmd_NullCommand	 0x0
#define      BRG1_ZC_Cmd_ResetLatch	 0x2

/* FIELD "BRG0_ZC_Stat" */
#define     BRG0_ZC_Stat_UnLatched	 0x0
#define     BRG0_ZC_Stat_Latched	 0x1

/* FIELD "BRG0_ZC_Cmd" */
#define      BRG0_ZC_Cmd_NullCommand	 0x0
#define      BRG0_ZC_Cmd_ResetLatch	 0x1

/********************************************************************/
/*	Next Receive Address Reg Lower (NRARL)			    */
/********************************************************************/
/* FIELD "RxDMA_NextLowAdd" - Numerical values only */

/********************************************************************/
/*	Next Receive Address Reg Upper (NRARU)			    */
/********************************************************************/
/* FIELD "RxDMAUppNexAdd" - Numerical values only */

/********************************************************************/
/*	Next Receive Byte Count Reg (NRBCR)			    */
/********************************************************************/
/* FIELD "RxDMA_NextCount" - Numerical values only */

/********************************************************************/
/*	Next Transmit Address Reg Lower (NTARL) 		    */
/********************************************************************/
/* FIELD "TxDMA_NextLowAdd" - Numerical values only */

/********************************************************************/
/*	Next Transmit Address Reg Upper (NTARU) 		    */
/********************************************************************/
/* FIELD "TxDMA_NextUppAdd" - Numerical values only */

/********************************************************************/
/*	Next Transmit Byte Count Reg (NTBCR)			    */
/********************************************************************/
/* FIELD "TxDMA_NextCount" - Numerical values only */

/********************************************************************/
/*	Port Control Reg (PCR)					    */
/********************************************************************/
/* FIELD "Port7_PinCtrl" */
#define    Port7_PinCtrl_Input		 0x0
#define    Port7_PinCtrl_TxCompleteOut	 0x4000
#define    Port7_PinCtrl_Output0	 0x8000
#define    Port7_PinCtrl_Output1	 0xc000

/* FIELD "Port6_PinCtrl" */
#define    Port6_PinCtrl_Input		 0x0
#define    Port6_PinCtrl_FrameSyncInput  0x1000
#define    Port6_PinCtrl_Output0	 0x2000
#define    Port6_PinCtrl_Output1	 0x3000

/* FIELD "Port5_PinCtrl" */
#define    Port5_PinCtrl_Input		 0x0
#define    Port5_PinCtrl_RxSyncOutput	 0x400
#define    Port5_PinCtrl_Output0	 0x800
#define    Port5_PinCtrl_Output1	 0xc00

/* FIELD "Port4_PinCtrl" */
#define    Port4_PinCtrl_Input		 0x0
#define    Port4_PinCtrl_TxTSA_GateOut	 0x100
#define    Port4_PinCtrl_Output0	 0x200
#define    Port4_PinCtrl_Output1	 0x300

/* FIELD "Port3_PinCtrl" */
#define    Port3_PinCtrl_Input		 0x0
#define    Port3_PinCtrl_RxTSA_GateOut	 0x40
#define    Port3_PinCtrl_Output0	 0x80
#define    Port3_PinCtrl_Output1	 0xc0

/* FIELD "Port2_PinCtrl" */
#define    Port2_PinCtrl_Input		 0x0
#define	   Port2_PinCtrl_DriverEnable    0x10	// JGY added 2/23/98
#define    Port2_PinCtrl_Output0	 0x20
#define    Port2_PinCtrl_Output1	 0x30

/* FIELD "Port1_PinCtrl" */
#define    Port1_PinCtrl_Input		 0x0
#define    Port1_PinCtrl_CTR1_Clock	 0x4
#define    Port1_PinCtrl_Output0	 0x8
#define    Port1_PinCtrl_Output1	 0xc

/* FIELD "Port0_PinCtrl" */
#define    Port0_PinCtrl_Input		 0x0
#define    Port0_PinCtrl_CTR0_Clock	 0x1
#define    Port0_PinCtrl_Output0	 0x2
#define    Port0_PinCtrl_Output1	 0x3

/********************************************************************/
/*	Port Status Reg (PSR)					    */
/********************************************************************/
/* FIELD "Port7_LatchState" */
#define Port7_LatchState_NoTransition	 0x0
#define Port7_LatchState_TransDetected	 0x8000

/* FIELD "Port7_LatchCmd" */
#define   Port7_LatchCmd_NullCommand	 0x0
#define   Port7_LatchCmd_ResetLatch	 0x8000

/* FIELD "Port7_Pin" */
#define        Port7_Pin_High		 0x0
#define        Port7_Pin_Low		 0x4000

/* FIELD "Port6_LatchState" */
#define Port6_LatchState_NoTransition	 0x0
#define Port6_LatchState_TransDetected	 0x2000

/* FIELD "Port6_LatchCmd" */
#define   Port6_LatchCmd_NullCommand	 0x0
#define   Port6_LatchCmd_ResetLatch	 0x2000

/* FIELD "Port6_Pin" */
#define        Port6_Pin_High		 0x0
#define        Port6_Pin_Low		 0x1000

/* FIELD "Port5_LatchState" */
#define Port5_LatchState_NoTransition	 0x0
#define Port5_LatchState_TransDetected	 0x800

/* FIELD "Port5_LatchCmd" */
#define   Port5_LatchCmd_NullCommand	 0x0
#define   Port5_LatchCmd_ResetLatch	 0x800

/* FIELD "Port5_Pin" */
#define        Port5_Pin_High		 0x0
#define        Port5_Pin_Low		 0x400

/* FIELD "Port4_LatchState" */
#define Port4_LatchState_NoTransition	 0x0
#define Port4_LatchState_TransDetected	 0x200

/* FIELD "Port4_LatchCmd" */
#define   Port4_LatchCmd_NullCommand	 0x0
#define   Port4_LatchCmd_ResetLatch	 0x200

/* FIELD "Port4_Pin" */
#define        Port4_Pin_High		 0x0
#define        Port4_Pin_Low		 0x100

/* FIELD "Port3_LatchState" */
#define Port3_LatchState_NoTransition	 0x0
#define Port3_LatchState_TransDetected	 0x80

/* FIELD "Port3_LatchCmd" */
#define   Port3_LatchCmd_NullCommand	 0x0
#define   Port3_LatchCmd_ResetLatch	 0x80

/* FIELD "Port3_Pin" */
#define        Port3_Pin_High		 0x0
#define        Port3_Pin_Low		 0x40

/* FIELD "Port2_LatchState" */
#define Port2_LatchState_NoTransition	 0x0
#define Port2_LatchState_TransDetected	 0x20

/* FIELD "Port2_LatchCmd" */
#define   Port2_LatchCmd_NullCommand	 0x0
#define   Port2_LatchCmd_ResetLatch	 0x20

/* FIELD "Port2_Pin" */
#define        Port2_Pin_High		 0x0
#define        Port2_Pin_Low		 0x10

/* FIELD "Port1_LatchState" */
#define Port1_LatchState_NoTransition	 0x0
#define Port1_LatchState_TransDetected	 0x8

/* FIELD "Port1_LatchCmd" */
#define   Port1_LatchCmd_NullCommand	 0x0
#define   Port1_LatchCmd_ResetLatch	 0x8

/* FIELD "Port1_Pin" */
#define        Port1_Pin_High		 0x0
#define        Port1_Pin_Low		 0x4

/* FIELD "Port0_LatchState" */
#define Port0_LatchState_NoTransition	 0x0
#define Port0_LatchState_TransDetected	 0x2

/* FIELD "Port0_LatchCmd" */
#define   Port0_LatchCmd_NullCommand	 0x0
#define   Port0_LatchCmd_ResetLatch	 0x2

/* FIELD "Port0_Pin" */
#define        Port0_Pin_High		 0x0
#define        Port0_Pin_Low		 0x1

/********************************************************************/
/*	Receive Address Reg Lower (RARL)			    */
/********************************************************************/
/* FIELD "RxDMA_LowAdd" - Numerical values only */

/********************************************************************/
/*	Receive Address Reg Upper (RARU)			    */
/********************************************************************/
/* FIELD "RxDMA_UppAdd" - Numerical values only */

/********************************************************************/
/*	Receive Byte Count Reg (RBCR)				    */
/********************************************************************/
/* FIELD "RxDMA_Count" - Numerical values only */

/********************************************************************/
/*	Receive Character Count Reg (RCCR)			    */
/********************************************************************/
/* FIELD "RxCharCount" - Numerical values only */

/********************************************************************/
/*	Receive Command / Status Reg (RCSR)			    */
/********************************************************************/
/* FIELD "SecondByteInErr" */
#define  SecondByteInErr_No		 0x0
#define  SecondByteInErr_Yes		 0x8000

/* FIELD "FirstByteInErr" */
#define   FirstByteInErr_No		 0x0
#define   FirstByteInErr_Yes		 0x4000

/* FIELD "RxCommand" */
#define        RxCommand_NullCommand	 0x0
#define        RxCommand_PresetCRC	 0x2000
#define        RxCommand_EnterHuntMode	 0x3000
#define        RxCommand_SelectTSA	 0x4000
#define        RxCommand_SelectFilLevel  0x5000
#define        RxCommand_SelectIntLevel  0x6000
#define        RxCommand_SelectReqLevel  0x7000

/* FIELD "ResidueCode" */
#define      ResidueCode_8ValidBits	 0x0
#define      ResidueCode_1ValidBits	 0x200
#define      ResidueCode_2ValidBits	 0x400
#define      ResidueCode_3ValidBits	 0x600
#define      ResidueCode_4ValidBits	 0x800
#define      ResidueCode_5ValidBits	 0xa00
#define      ResidueCode_6ValidBits	 0xc00
#define      ResidueCode_7ValidBits	 0xe00

/* FIELD "ShortFrame" */
#define       ShortFrame_No		 0x0
#define       ShortFrame_Yes		 0x100

/* FIELD "CV_Pol" */
#define 	  CV_Pol_No		 0x0
#define 	  CV_Pol_Yes		 0x100

/* FIELD "ExitHuntLatch" */
#define    ExitHuntLatch_UnLatched	 0x0
#define    ExitHuntLatch_Latched	 0x80

/* FIELD "ExitHuntCmd" */
#define      ExitHuntCmd_NullCommand	 0x0
#define      ExitHuntCmd_ResetLatch	 0x80

/* FIELD "RxIdleLatch" */
#define      RxIdleLatch_UnLatched	 0x0
#define      RxIdleLatch_Latched	 0x40

/* FIELD "RxIdleCmd" */
#define        RxIdleCmd_NullCommand	 0x0
#define        RxIdleCmd_ResetLatch	 0x40

/* FIELD "RxBreakLatch" */
#define     RxBreakLatch_UnLatched	 0x0
#define     RxBreakLatch_Latched	 0x20

/* FIELD "RxBreakCmd" */
#define       RxBreakCmd_NullCommand	 0x0
#define       RxBreakCmd_ResetLatch	 0x20

/* FIELD "RxAbortLatch" */
#define     RxAbortLatch_UnLatched	 0x0
#define     RxAbortLatch_Latched	 0x20

/* FIELD "RxAbortCmd" */
#define       RxAbortCmd_NullCommand	 0x0
#define       RxAbortCmd_ResetLatch	 0x20

/* FIELD "RxBound_Latch" */
#define    RxBound_Latch_UnLatched	 0x0
#define    RxBound_Latch_Latched	 0x10

/* FIELD "RxBound_Cmd" */
#define      RxBound_Cmd_NullCommand	 0x0
#define      RxBound_Cmd_ResetLatch	 0x10

/* FIELD "CRC_FrameError" */
#define   CRC_FrameError_No		 0x0
#define   CRC_FrameError_Yes		 0x8

/* FIELD "ParityErrorLatch" */
#define ParityErrorLatch_UnLatched	 0x0
#define ParityErrorLatch_Latched	 0x4

/* FIELD "ParityErrorCmd" */
#define   ParityErrorCmd_NullCommand	 0x0
#define   ParityErrorCmd_ResetLatch	 0x4

/* FIELD "RxSpecificAbort" */
#define  RxSpecificAbort_Unlatched	 0x0
#define  RxSpecificAbort_Latched	 0x4

/* FIELD "RxSpecAbortCmd" */
#define   RxSpecAbortCmd_NullCommand	 0x0
#define   RxSpecAbortCmd_ResetLatch	 0x4

/* FIELD "RxOverrunLatch" */
#define   RxOverrunLatch_UnLatched	 0x0
#define   RxOverrunLatch_Latched	 0x2

/* FIELD "RxOverrunCmd" */
#define     RxOverrunCmd_NullCommand	 0x0
#define     RxOverrunCmd_ResetLatch	 0x2

/* FIELD "RxCharAvailable" */
#define  RxCharAvailable_FIFO_Empty	 0x0
#define  RxCharAvailable_FIFO_NotEmpty	 0x1

/********************************************************************/
/*	Receive Count Limit Reg (RCLR)				    */
/********************************************************************/
/* FIELD "RxCountLimit" - Numerical values only */

/********************************************************************/
/*	Receive Data Reg (RDR)					    */
/********************************************************************/
/* FIELD "RxDataReg" - Numerical values only */

/********************************************************************/
/*	Receive DMA Interrupt Arm Reg (RDIAR)			    */
/********************************************************************/
/* FIELD "ReservedRDIAR" - Numerical values only */

/* FIELD "RxEOA_EOL_IA" */
#define     RxEOA_EOL_IA_No		 0x0
#define     RxEOA_EOL_IA_Yes		 0x8

/* FIELD "RxEOB_IA" */
#define 	RxEOB_IA_No		 0x0
#define 	RxEOB_IA_Yes		 0x4

/* FIELD "RxHardAbortIA" */
#define    RxHardAbortIA_No		 0x0
#define    RxHardAbortIA_Yes		 0x2

/* FIELD "RxSoftAbortIA" */
#define    RxSoftAbortIA_No		 0x0
#define    RxSoftAbortIA_Yes		 0x1

/********************************************************************/
/*	Receive DMA Mode Reg (RDMR)				    */
/********************************************************************/
/* FIELD "RxChannelMode" */
#define    RxChannelMode_SingleBuffer	 0x0
#define    RxChannelMode_Pipelined	 0x4000
#define    RxChannelMode_Array		 0x8000
#define    RxChannelMode_LinkedList	 0xc000

/* FIELD "RxDMAAdvStatHdl" */
#define  RxDMAAdvStatHdl_No		 0x0
#define  RxDMAAdvStatHdl_Yes		 0x2000

/* FIELD "RxDMAWriteBack0" */
#define  RxDMAWriteBack0_No		 0x0
#define  RxDMAWriteBack0_Yes		 0x1000

/* FIELD "RxAddressMode" */
#define    RxAddressMode_Increment	 0x0
#define    RxAddressMode_Decrement	 0x400
#define    RxAddressMode_NoChange	 0x800

/* FIELD "RxTermEnable" */
#define     RxTermEnable_No		 0x0
#define     RxTermEnable_Yes		 0x200

/* FIELD "RxDMA_AccessSize" */
#define RxDMA_AccessSize_Word		 0x0
#define RxDMA_AccessSize_Byte		 0x100

/* FIELD "RxContinues" */
#define      RxContinues_No		 0x0
#define      RxContinues_Yes		 0x80

/* FIELD "RxGetLink" */
#define        RxGetLink_No		 0x0
#define        RxGetLink_Yes		 0x40

/* FIELD "RxBusy" */
#define 	  RxBusy_No		 0x0
#define 	  RxBusy_Yes		 0x20

/* FIELD "RxInit" */
#define 	  RxInit_No		 0x0
#define 	  RxInit_Yes		 0x10

/* FIELD "RxEOA_EOL" */
#define        RxEOA_EOL_No		 0x0
#define        RxEOA_EOL_Yes		 0x8

/* FIELD "RxEOB" */
#define 	   RxEOB_No		 0x0
#define 	   RxEOB_Yes		 0x4

/* FIELD "RxHardwareAbort" */
#define  RxHardwareAbort_No		 0x0
#define  RxHardwareAbort_Yes		 0x2

/* FIELD "RxSoftwareAbort" */
#define  RxSoftwareAbort_No		 0x0
#define  RxSoftwareAbort_Yes		 0x1

/********************************************************************/
/*	Receive Interrupt Control Reg (RICR)			    */
/********************************************************************/
/* FIELD "RxFIFO_FillLevel" - Numerical values only */

/* FIELD "RxFIFO_IntLevel" - Numerical values only */

/* FIELD "RxFIFO_DMA_Level" - Numerical values only */

/* FIELD "RxTimeSlotAssign" - Numerical values only */

/* FIELD "RxSlotOffset" */
#define     RxSlotOffset_NoOffset	 0x0
#define     RxSlotOffset_7_ClkOffset	 0x2000
#define     RxSlotOffset_6_ClkOffset	 0x4000
#define     RxSlotOffset_5_ClkOffset	 0x6000
#define     RxSlotOffset_4_ClkOffset	 0x8000
#define     RxSlotOffset_3_ClkOffset	 0xa000
#define     RxSlotOffset_2_ClkOffset	 0xc000
#define     RxSlotOffset_1_ClkOffset	 0xe000

/* FIELD "RxConcatSlots" */
#define    RxConcatSlots_NoSlot 	 0x0
#define    RxConcatSlots_1_Slot 	 0x200
#define    RxConcatSlots_2_Slots	 0x400
#define    RxConcatSlots_3_Slots	 0x600
#define    RxConcatSlots_4_Slots	 0x800
#define    RxConcatSlots_5_Slots	 0xa00
#define    RxConcatSlots_6_Slots	 0xc00
#define    RxConcatSlots_7_Slots	 0xe00
#define    RxConcatSlots_8_Slots	 0x1000
#define    RxConcatSlots_9_Slots	 0x1200
#define    RxConcatSlots_10_Slots	 0x1400
#define    RxConcatSlots_11_Slots	 0x1600
#define    RxConcatSlots_12_Slots	 0x1800
#define    RxConcatSlots_13_Slots	 0x1a00
#define    RxConcatSlots_14_Slots	 0x1c00
#define    RxConcatSlots_15_Slots	 0x1e00

/* FIELD "RxModeSwitch" */
#define     RxModeSwitch_TSA_Slot	 0x0
#define     RxModeSwitch_TSA_OffCount	 0x100

/* FIELD "ExitHuntIA" */
#define       ExitHuntIA_No		 0x0
#define       ExitHuntIA_Yes		 0x80

/* FIELD "RxIdleIA" */
#define 	RxIdleIA_No		 0x0
#define 	RxIdleIA_Yes		 0x40

/* FIELD "RxBreakAbortIA" */
#define   RxBreakAbortIA_No		 0x0
#define   RxBreakAbortIA_Yes		 0x20

/* FIELD "RxBound_IA" */
#define       RxBound_IA_No		 0x0
#define       RxBound_IA_Yes		 0x10

/* FIELD "StatusOn" */
#define 	StatusOn_Bytes		 0x0
#define 	StatusOn_Words		 0x8

/* FIELD "ParityErrorIA" */
#define    ParityErrorIA_No		 0x0
#define    ParityErrorIA_Yes		 0x4

/* FIELD "RxOverrunIA" */
#define      RxOverrunIA_No		 0x0
#define      RxOverrunIA_Yes		 0x2

/* FIELD "TC0R_ReadValue" */
#define   TC0R_ReadValue_TimeConstant	 0x0
#define   TC0R_ReadValue_CurrentValue	 0x1

/********************************************************************/
/*	Receive Mode Reg (RMR)					    */
/********************************************************************/
/* FIELD "RxDataDecode" */
#define     RxDataDecode_NRZ		 0x0
#define     RxDataDecode_NRZB		 0x2000
#define     RxDataDecode_NRZI_Mark	 0x4000
#define     RxDataDecode_NRZI_Space	 0x6000
#define     RxDataDecode_BiphaseMark	 0x8000
#define     RxDataDecode_BiphaseSpace	 0xa000
#define     RxDataDecode_BiphaseLevel	 0xc000
#define     RxDataDecode_DiffBiphasLevl  0xe000

/* FIELD "RxCRC_Polynomial" */
#define 	RxCRC_Polynomial_CRC_CCITT	0x0
#define 	RxCRC_Polynomial_CRC_16 	0x800
#define 	RxCRC_Polynomial_CRC_32 	0x1000

/* FIELD "RxCRC_PresetVal" */
#define 	RxCRC_PresetVal_Zeros		0x0
#define		RxCRC_PresetVal_Ones		0x400

/* FIELD "RxCRC_Enable" */
#define 	RxCRC_Enable_No			0x0
#define 	RxCRC_Enable_Yes		0x200

/* FIELD "RxAbortHandling" */
#define 	RxAbortHandling_No		0x0
#define 	RxAbortHandling_Yes		0x100

/* FIELD "RxParitySense" */
#define 	RxParitySense_Even		0x0
#define 	RxParitySense_Odd		0x40
#define 	RxParitySense_Space		0x80
#define 	RxParitySense_Mark		0xc0

/* FIELD "RxParity" */
#define 	RxParity_Disabled		0x0
#define 	RxParity_Enabled		0x20

/* FIELD "RxCharLength" */
#define		RxCharLength_8Bits		0x0
#define		RxCharLength_1Bits		0x4
#define     RxCharLength_2Bits			0x8
#define     RxCharLength_3Bits			0xc
#define     RxCharLength_4Bits			0x10
#define     RxCharLength_5Bits			0x14
#define     RxCharLength_6Bits			0x18
#define     RxCharLength_7Bits			0x1c

/* FIELD "RxEnable" */
#define 	RxEnable_DisablImmediat 	0x0
#define 	RxEnable_DisableAfterRx 	0x1
#define 	RxEnable_Enable 		0x2
#define 	RxEnable_EnableWithDCD		0x3

/********************************************************************/
/*	Receive Sync Reg (RSR)					    */
/********************************************************************/
/* FIELD "RxMonoSyncBits" - Numerical values only */

/* FIELD "RxBisyncSYN0" - Numerical values only */

/* FIELD "RxBisyncSYN1" - Numerical values only */

/* FIELD "RxAddress8" - Numerical values only */

/* FIELD "RxAddress16" - Numerical values only */

/********************************************************************/
/*	Set DMA Interrupt Reg (SDIR)				    */
/********************************************************************/
/* FIELD "Reserve1_SDIR" - Numerical values only */

/* FIELD "DMA_RxIUS" */
#define        DMA_RxIUS_No			0x0
#define        DMA_RxIUS_Yes			0x200

/* FIELD "SetDMA_RxIUS" */
#define     SetDMA_RxIUS_NullCommand		0x0
#define     SetDMA_RxIUS_Set			0x200

/* FIELD "DMA_TxIUS" */
#define        DMA_TxIUS_No			0x0
#define        DMA_TxIUS_Yes			0x100

/* FIELD "SetDMA_TxIUS" */
#define     SetDMA_TxIUS_NullCommand		0x0
#define     SetDMA_TxIUS_Set			0x100

/* FIELD "Reserve2_SDIR" - Numerical values only */

/* FIELD "DMA_RxIP" */
#define 	DMA_RxIP_No			0x0
#define 	DMA_RxIP_Yes			0x2

/* FIELD "SetDMA_RxIP" */
#define      SetDMA_RxIP_NullCommand		0x0
#define      SetDMA_RxIP_Set			0x2

/* FIELD "DMA_TxIP" */
#define 	DMA_TxIP_No			0x0
#define 	DMA_TxIP_Yes			0x1

/* FIELD "SetDMA_TxIP" */
#define      SetDMA_TxIP_NullCommand		0x0
#define      SetDMA_TxIP_Set			0x1

/********************************************************************/
/*	Status Interrupt Control Reg (SICR)			    */
/********************************************************************/
/* FIELD "RxC_Interrupts" */
#define   RxC_Interrupts_Disabled		0x0
#define   RxC_Interrupts_RisingEdgeOnly 	0x4000
#define   RxC_Interrupts_FallngEdgeOnly 	0x8000
#define   RxC_Interrupts_BothEdges		0xc000

/* FIELD "TxC_Interrupts" */
#define   TxC_Interrupts_Disabled		0x0
#define   TxC_Interrupts_RisingEdgeOnly 	0x1000
#define   TxC_Interrupts_FallngEdgeOnly 	0x2000
#define   TxC_Interrupts_BothEdges		0x3000

/* FIELD "RxREQ_Interrupts" */
#define RxREQ_Interrupts_Disabled		0x0
#define RxREQ_Interrupts_RisingEdgeOnly 	0x400
#define RxREQ_Interrupts_FallngEdgeOnly 	0x800
#define RxREQ_Interrupts_BothEdges		0xc00

/* FIELD "TxREQ_Interrupts" */
#define TxREQ_Interrupts_Disabled		0x0
#define TxREQ_Interrupts_RisingEdgeOnly 	0x100
#define TxREQ_Interrupts_FallngEdgeOnly 	0x200
#define TxREQ_Interrupts_BothEdges		0x300

/* FIELD "DCD_Interrupts" */
#define   DCD_Interrupts_Disabled		0x0
#define   DCD_Interrupts_RisingEdgeOnly 	0x40
#define   DCD_Interrupts_FallngEdgeOnly		0x80
#define   DCD_Interrupts_BothEdges		0xc0

/* FIELD "CTS_Interrupts" */
#define   CTS_Interrupts_Disabled		0x0
#define   CTS_Interrupts_RisingEdgeOnly 	0x10
#define   CTS_Interrupts_FallngEdgeOnly 	0x20
#define   CTS_Interrupts_BothEdges		0x30

/* FIELD "RCC_OverflowIA" */
#define   RCC_OverflowIA_No			0x0
#define   RCC_OverflowIA_Yes			0x8

/* FIELD "DPLL_SyncIA" */
#define      DPLL_SyncIA_No			0x0
#define      DPLL_SyncIA_Yes			0x4

/* FIELD "BRG1_ZC_IA" */
#define       BRG1_ZC_IA_No			0x0
#define       BRG1_ZC_IA_Yes			0x2

/* FIELD "BRG0_ZC_IA" */
#define       BRG0_ZC_IA_No			0x0
#define       BRG0_ZC_IA_Yes			0x1

/********************************************************************/
/*	Test Mode Control Reg (TMCR)				    */
/********************************************************************/
/* FIELD "TestRegisterAdd" */
#define  TestRegisterAdd_NullAddress		0x0
#define  TestRegisterAdd_HiByteShifters 	0x1
#define  TestRegisterAdd_CRC_Byte0		0x2
#define  TestRegisterAdd_CRC_Byte1		0x3
#define  TestRegisterAdd_RxFIFO_Write		0x4
#define  TestRegisterAdd_ClkMuxOutputs		0x5
#define  TestRegisterAdd_CTR0_CTR1count		0x6
#define  TestRegisterAdd_ClkMuxInputs		0x7
#define  TestRegisterAdd_DPLL_State		0x8
#define  TestRegisterAdd_LoByteShifters 	0x9
#define  TestRegisterAdd_CRC_Byte2		0xa
#define  TestRegisterAdd_CRC_Byte3		0xb
#define  TestRegisterAdd_TxFIFO_Read		0xc
#define  TestRegisterAdd_IO_DevStatLatc 	0xe
#define  TestRegisterAdd_IntDaisyChain		0xf
#define  TestRegisterAdd_RxCountHoldReg 	0x16

/********************************************************************/
/*	Test Mode Data Reg (TMDR)				    */
/********************************************************************/
/* FIELD "TestDataBits" - Numerical values only */

/********************************************************************/
/*	Time Constant 0 Reg (TC0R)				    */
/********************************************************************/
/* FIELD "TimeConst0" - Numerical values only */

/********************************************************************/
/*	Time Constant 1 Reg (TC1R)				    */
/********************************************************************/
/* FIELD "TimeConst1" - Numerical values only */

/********************************************************************/
/*	Transmit Address Reg Lower (TARL)			    */
/********************************************************************/
/* FIELD "TxDMA_LowAdd" - Numerical values only */

/********************************************************************/
/*	Transmit Address Reg Upper (TARU)			    */
/********************************************************************/
/* FIELD "TxDMA_UppAdd" - Numerical values only */

/********************************************************************/
/*	Transmit Byte Count Reg (TBCR)				    */
/********************************************************************/
/* FIELD "TxDMA_Count" - Numerical values only */

/********************************************************************/
/*	Transmit Character Count Reg (TCCR)			    */
/********************************************************************/
/* FIELD "TxCharCount" - Numerical values only */

/********************************************************************/
/*	Transmit Command / Status Reg (TCSR)			    */
/********************************************************************/
/* FIELD "TxCommand" */
#define        TxCommand_NullCommand	0x0
#define        TxCommand_PresetCRC	0x2000
#define        TxCommand_SelectTSA	0x4000
#define        TxCommand_SelectFilLevel	0x5000
#define        TxCommand_SelectIntLevel 0x6000
#define        TxCommand_SelectReqLevel 0x7000
#define        TxCommand_SendFramMessag 0x8000
#define        TxCommand_SendAbort	0x9000
#define        TxCommand_ResetDLE_Inhbt 0xc000
#define        TxCommand_SetDLE_Inhibit	0xd000
#define        TxCommand_ResetEOF_EOM	0xe000
#define        TxCommand_SetEOF_EOM	0xf000

/* FIELD "TxIdleLineCond" */
#define   TxIdleLineCond_SYNC_FlagNorml 0x0
#define   TxIdleLineCond_Alternating1_0 0x100
#define   TxIdleLineCond_AllZeros	0x200
#define   TxIdleLineCond_AllOnes	0x300
#define   TxIdleLineCond_AltrntMarkSpac 0x500
#define   TxIdleLineCond_SpaceIdle	0x600
#define   TxIdleLineCond_MarkIdle	0x700

/* FIELD "TxPreambleSentLt" */
#define TxPreambleSentLt_UnLatched	0x0
#define TxPreambleSentLt_Latched	0x80

/* FIELD "TxPreambleSentCd" */
#define TxPreambleSentCd_NullCommand	0x0
#define TxPreambleSentCd_ResetLatch	0x80

/* FIELD "TxIdleSentLatch" */
#define  TxIdleSentLatch_UnLatched	0x0
#define  TxIdleSentLatch_Latched	0x40

/* FIELD "TxIdleSentCmd" */
#define    TxIdleSentCmd_NullCommand	0x0
#define    TxIdleSentCmd_ResetLatch	0x40

/* FIELD "TxAbortSentLatch" */
#define TxAbortSentLatch_UnLatched	0x0
#define TxAbortSentLatch_Latched	0x20

/* FIELD "TxAbortSentCmd" */
#define   TxAbortSentCmd_NullCommand	0x0
#define   TxAbortSentCmd_ResetLatch	0x20

/* FIELD "TxEOF_EOT_SentLt" */
#define TxEOF_EOT_SentLt_UnLatched	0x0
#define TxEOF_EOT_SentLt_Latched	0x10

/* FIELD "TxEOF_EOT_SentCd" */
#define TxEOF_EOT_SentCd_NullCommand	0x0
#define TxEOF_EOT_SentCd_ResetLatch	0x10

/* FIELD "TxCRC_SentLatch" */
#define  TxCRC_SentLatch_UnLatched	0x0
#define  TxCRC_SentLatch_Latched	0x8

/* FIELD "TxCRC_SentCmd" */
#define    TxCRC_SentCmd_NullCommand	0x0
#define    TxCRC_SentCmd_ResetLatch	0x8

/* FIELD "AllSent" */
#define 	 AllSent_No		0x0
#define 	 AllSent_Yes		0x4

/* FIELD "TxUnderrunLatch" */
#define  TxUnderrunLatch_UnLatched	0x0
#define  TxUnderrunLatch_Latched	0x2

/* FIELD "TxUnderrunCmd" */
#define    TxUnderrunCmd_NullCommand	0x0
#define    TxUnderrunCmd_ResetLatch	0x2

/* FIELD "TxBuffer" */
#define 	TxBuffer_NotEmpty	0x0
#define 	TxBuffer_Empty		0x1

/********************************************************************/
/*	Transmit Count Limit Reg (TCLR)    			    */
/********************************************************************/
/* FIELD "TxCountLimit" - Numerical values only */

/********************************************************************/
/*	Transmit Data Reg (TDR) 				    */
/********************************************************************/
/* FIELD "TxDataReg" - Numerical values only */

/********************************************************************/
/*	Transmit DMA Interrupt Arm Reg (TDIAR)			    */
/********************************************************************/
/* FIELD "ReservedTDIAR" - Numerical values only */

/* FIELD "TxEOA_EOL_IA" */
#define     TxEOA_EOL_IA_No		0x0
#define     TxEOA_EOL_IA_Yes		0x8

/* FIELD "TxEOB_IA" */
#define 	TxEOB_IA_No		0x0
#define 	TxEOB_IA_Yes		0x4

/* FIELD "TxHardAbortIA" */
#define    TxHardAbortIA_No		0x0
#define    TxHardAbortIA_Yes		0x2

/* FIELD "TxSoftAbortIA" */
#define    TxSoftAbortIA_No		0x0
#define    TxSoftAbortIA_Yes		0x1

/********************************************************************/
/*	Transmit DMA Mode Reg (TDMR)				    */
/********************************************************************/
/* FIELD "TxChannelMode" */
#define    TxChannelMode_SingleBuffer	0x0
#define    TxChannelMode_Pipelined	0x4000
#define    TxChannelMode_Array		0x8000
#define    TxChannelMode_LinkedList	0xc000

/* FIELD "TxDMAAdvStatHdl" */
#define  TxDMAAdvStatHdl_No		0x0
#define  TxDMAAdvStatHdl_Yes		0x2000

/* FIELD "TxDMAWriteBack0" */
#define  TxDMAWriteBack0_No		0x0
#define  TxDMAWriteBack0_Yes		0x1000

/* FIELD "TxAddressMode" */
#define    TxAddressMode_Increment	0x0
#define    TxAddressMode_Decrement	0x400
#define    TxAddressMode_NoChange	0x800

/* FIELD "TxTermEnable" */
#define     TxTermEnable_No		0x0
#define     TxTermEnable_Yes		0x200

/* FIELD "TxDMA_AccessSize" */
#define TxDMA_AccessSize_Word		0x0
#define TxDMA_AccessSize_Byte		0x100

/* FIELD "TxContinues" */
#define      TxContinues_No		0x0
#define      TxContinues_Yes		0x80

/* FIELD "TxGetLink" */
#define        TxGetLink_No		0x0
#define        TxGetLink_Yes		0x40

/* FIELD "TxBusy" */
#define 	  TxBusy_No		0x0
#define 	  TxBusy_Yes		0x20

/* FIELD "TxInit" */
#define 	  TxInit_No		0x0
#define 	  TxInit_Yes		0x10

/* FIELD "TxEOA_EOL" */
#define        TxEOA_EOL_No		0x0
#define        TxEOA_EOL_Yes		0x8

/* FIELD "TxEOB" */
#define 	   TxEOB_No		0x0
#define 	   TxEOB_Yes		0x4

/* FIELD "TxHardwareAbort" */
#define  TxHardwareAbort_No		0x0
#define  TxHardwareAbort_Yes		0x2

/* FIELD "TxSoftwareAbort" */
#define  TxSoftwareAbort_No		0x0
#define  TxSoftwareAbort_Yes		0x1

/********************************************************************/
/*	Transmit Interrupt Control Reg (TICR)			    */
/********************************************************************/
/* FIELD "TxFIFO_FillLevel" - Numerical values only */

/* FIELD "TxFIFO_IntLevel" - Numerical values only */

/* FIELD "TxFIFO_DMA_Level" - Numerical values only */

/* FIELD "TxTimeSlotAssign" - Numerical values only */

/* FIELD "TxSlotOffset" */
#define     TxSlotOffset_NoOffset	0x0
#define     TxSlotOffset_7_ClkOffset	0x2000
#define     TxSlotOffset_6_ClkOffset	0x4000
#define     TxSlotOffset_5_ClkOffset	0x6000
#define     TxSlotOffset_4_ClkOffset	0x8000
#define     TxSlotOffset_3_ClkOffset	0xa000
#define     TxSlotOffset_2_ClkOffset	0xc000
#define     TxSlotOffset_1_ClkOffset	0xe000

/* FIELD "TxConcatSlots" */
#define    TxConcatSlots_NoSlot 	0x0
#define    TxConcatSlots_1_Slot 	0x200
#define    TxConcatSlots_2_Slots	0x400
#define    TxConcatSlots_3_Slots	0x600
#define    TxConcatSlots_4_Slots	0x800
#define    TxConcatSlots_5_Slots	0xa00
#define    TxConcatSlots_6_Slots	0xc00
#define    TxConcatSlots_7_Slots	0xe00
#define    TxConcatSlots_8_Slots	0x1000
#define    TxConcatSlots_9_Slots	0x1200
#define    TxConcatSlots_10_Slots	0x1400
#define    TxConcatSlots_11_Slots	0x1600
#define    TxConcatSlots_12_Slots	0x1800
#define    TxConcatSlots_13_Slots	0x1a00
#define    TxConcatSlots_14_Slots	0x1c00
#define    TxConcatSlots_15_Slots	0x1e00

/* FIELD "TxModeSwitch" */
#define     TxModeSwitch_TSA_Slot	0x0
#define     TxModeSwitch_TSA_OffCount	0x100

/* FIELD "TxPreambleSentIA" */
#define TxPreambleSentIA_No		0x0
#define TxPreambleSentIA_Yes		0x80

/* FIELD "TxIdleSentIA" */
#define     TxIdleSentIA_No		0x0
#define     TxIdleSentIA_Yes		0x40

/* FIELD "TxAbortSentIA" */
#define    TxAbortSentIA_No		0x0
#define    TxAbortSentIA_Yes		0x20

/* FIELD "TxEOF_EOT_SentIA" */
#define TxEOF_EOT_SentIA_No		0x0
#define TxEOF_EOT_SentIA_Yes		0x10

/* FIELD "TxCRC_SentIA" */
#define     TxCRC_SentIA_No		0x0
#define     TxCRC_SentIA_Yes		0x8

/* FIELD "TxActionAtEOT" */
#define    TxActionAtEOT_GoForIt	0x0
#define    TxActionAtEOT_WaitForSendCmd 0x4

/* FIELD "TxUnderrunIA" */
#define     TxUnderrunIA_No		0x0
#define     TxUnderrunIA_Yes		0x2

/* FIELD "TC1R_ReadValue" */
#define   TC1R_ReadValue_TimeConstant	0x0
#define   TC1R_ReadValue_CurrentValue	0x1

/********************************************************************/
/*	Transmit Mode Reg (TMR) 				    */
/********************************************************************/
/* FIELD "TxDataEncoding" */
#define   TxDataEncoding_NRZ		0x0
#define   TxDataEncoding_NRZB		0x2000
#define   TxDataEncoding_NRZI_Mark	0x4000
#define   TxDataEncoding_NRZI_Space	0x6000
#define   TxDataEncoding_BiphaseMark	0x8000
#define   TxDataEncoding_BiphaseSpace	0xa000
#define   TxDataEncoding_BiphaseLevel	0xc000
#define   TxDataEncoding_DiffBiphasLevl	0xe000

/* FIELD "TxCRC_Polynomial" */
#define TxCRC_Polynomial_CRC_CCITT	0x0
#define TxCRC_Polynomial_CRC_16 	0x800
#define TxCRC_Polynomial_CRC_32 	0x1000

/* FIELD "TxCRC_PresetVal" */
#define  TxCRC_PresetVal_Zeros	 	0x0
#define  TxCRC_PresetVal_Ones	 	0x400

/* FIELD "TxCRC_Enable" */
#define     TxCRC_Enable_No		0x0
#define     TxCRC_Enable_Yes	 	0x200

/* FIELD "TxCRC_On_EOF_EOM" */
#define TxCRC_On_EOF_EOM_No		0x0
#define TxCRC_On_EOF_EOM_Yes	 	0x100

/* FIELD "TxParitySense" */
#define    TxParitySense_Even		0x0
#define    TxParitySense_Odd		0x40
#define    TxParitySense_Space		0x80
#define    TxParitySense_Mark		0xc0

/* FIELD "TxParity" */
#define 	TxParity_Disabled	0x0
#define 	TxParity_Enabled	0x20

/* FIELD "TxCharLength" */
#define     TxCharLength_8Bits		0x0
#define     TxCharLength_1Bits		0x4
#define     TxCharLength_2Bits		0x8
#define     TxCharLength_3Bits		0xc
#define     TxCharLength_4Bits		0x10
#define     TxCharLength_5Bits		0x14
#define     TxCharLength_6Bits		0x18
#define     TxCharLength_7Bits		0x1c

/* FIELD "TxEnable" */
#define 	TxEnable_DisablImmediat 0x0
#define 	TxEnable_DisableAfterTx 0x1
#define 	TxEnable_Enable 	0x2
#define 	TxEnable_EnableWithCTS	0x3

/********************************************************************/
/*	Transmit Sync Reg (TSR) 				    */
/********************************************************************/
/* FIELD "TxMonoSyncBits" - Numerical values only */

/* FIELD "TxBisyncSYN0" - Numerical values only */

/* FIELD "TxBisyncSYN1" - Numerical values only */

#endif //__Z16C32_H__
/********************************************************************/
/*  End of File 						    */
/********************************************************************/
