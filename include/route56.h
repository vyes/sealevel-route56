/*
 * Route56 Multiprotocol Serial Adapter Driver
 * Based on SyncLink Multiprotocol Serial Adapter Driver
 *
 * $Id: route56.h,v 1.4 2008/08/08 18:00:51 thomasw Exp $
 *
 * Copyright (C) 1998-2000 by Microgate Corporation
 *
 * Redistribution of this file is permitted under
 * the terms of the GNU Public License (GPL)
 */

#ifndef _ROUTE56_H_
#define _ROUTE56_H_
#define ROUTE56_H_VERSION 2.0

#define BOOLEAN int
#define TRUE 1
#define FALSE 0

#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000


#define HDLC_MAX_FRAME_SIZE	65535
#define MAX_ASYNC_TRANSMIT	4096
#define MAX_ASYNC_BUFFER_SIZE	4096

#define ASYNC_PARITY_NONE		0
#define ASYNC_PARITY_EVEN		1
#define ASYNC_PARITY_ODD		2
#define ASYNC_PARITY_SPACE		3

#define HDLC_FLAG_UNDERRUN_ABORT7	0x0000
#define HDLC_FLAG_UNDERRUN_ABORT15	0x0001
#define HDLC_FLAG_UNDERRUN_FLAG		0x0002
#define HDLC_FLAG_UNDERRUN_CRC		0x0004
#define HDLC_FLAG_SHARE_ZERO		0x0010
#define HDLC_FLAG_AUTO_CTS		0x0020
#define HDLC_FLAG_AUTO_DCD		0x0040
#define HDLC_FLAG_AUTO_RTS		0x0080
#define HDLC_FLAG_RXC_DPLL		0x0100
#define HDLC_FLAG_RXC_BRG		0x0200
#define HDLC_FLAG_RXC_TXCPIN		0x8000
#define HDLC_FLAG_RXC_RXCPIN		0x0000
#define HDLC_FLAG_TXC_DPLL		0x0400
#define HDLC_FLAG_TXC_BRG		0x0800
#define HDLC_FLAG_TXC_TXCPIN		0x0000
#define HDLC_FLAG_TXC_RXCPIN		0x0008
#define HDLC_FLAG_DPLL_DIV8		0x1000
#define HDLC_FLAG_DPLL_DIV16		0x2000
#define HDLC_FLAG_DPLL_DIV32		0x0000
#define HDLC_FLAG_HDLC_LOOPMODE		0x4000

#define HDLC_CRC_NONE			0
#define HDLC_CRC_16_CCITT		1
#define HDLC_CRC_32_CCITT		2
#define HDLC_CRC_MASK			0x00ff
#define HDLC_CRC_RETURN_EX		0x8000

#define RX_OK				0
#define RX_CRC_ERROR			1

#define HDLC_TXIDLE_FLAGS		0
#define HDLC_TXIDLE_ALT_ZEROS_ONES	1
#define HDLC_TXIDLE_ZEROS		2
#define HDLC_TXIDLE_ONES		3
#define HDLC_TXIDLE_ALT_MARK_SPACE	4
#define HDLC_TXIDLE_SPACE		5
#define HDLC_TXIDLE_MARK		6

#define HDLC_ENCODING_NRZ			0
#define HDLC_ENCODING_NRZB			1
#define HDLC_ENCODING_NRZI_MARK			2
#define HDLC_ENCODING_NRZI_SPACE		3
#define HDLC_ENCODING_NRZI			HDLC_ENCODING_NRZI_SPACE
#define HDLC_ENCODING_BIPHASE_MARK		4
#define HDLC_ENCODING_BIPHASE_SPACE		5
#define HDLC_ENCODING_BIPHASE_LEVEL		6
#define HDLC_ENCODING_DIFF_BIPHASE_LEVEL	7

#define HDLC_PREAMBLE_LENGTH_8BITS	0
#define HDLC_PREAMBLE_LENGTH_16BITS	1
#define HDLC_PREAMBLE_LENGTH_32BITS	2
#define HDLC_PREAMBLE_LENGTH_64BITS	3

#define HDLC_PREAMBLE_PATTERN_NONE	0
#define HDLC_PREAMBLE_PATTERN_ZEROS	1
#define HDLC_PREAMBLE_PATTERN_FLAGS	2
#define HDLC_PREAMBLE_PATTERN_10	3
#define HDLC_PREAMBLE_PATTERN_01	4
#define HDLC_PREAMBLE_PATTERN_ONES	5

#define R56_MODE_ASYNC		1
#define R56_MODE_HDLC		2
#define R56_MODE_RAW		6

#define R56_BUS_TYPE_ISA	1
#define R56_BUS_TYPE_EISA	2
#define R56_BUS_TYPE_PCI	5

#define R56_PCI_XTAL_SPEED 20000000
#define R56_ISA_XTAL_SPEED 14745600

#define RS232		0x02
#define RS485_TERM	0x04
#define RS485_TERM_NE	0x84
#define RS485_NOTERM	0x05
#define RS485_NOTERM_NE	0x85
#define LOOP_STD	0x0A
#define LOOP_DIFF	0x0B
#define RS530		0x0D
#define V35		0x0E
#define RS530A		0x0F

#define R56_INTERFACE_MASK     0xf
#define R56_INTERFACE_DISABLE  0
#define R56_INTERFACE_RS232    1
#define R56_INTERFACE_V35      2
#define R56_INTERFACE_RS422    3
#define R56_INTERFACE_RTS_EN   0x10
#define R56_INTERFACE_LL       0x20
#define R56_INTERFACE_RL       0x40
#define R56_INTERFACE_NE       0x80

typedef struct _R56_PARAMS
{
	/* Common */

	unsigned long	mode;		/* Asynchronous or HDLC */
	unsigned char	loopback;	/* internal loopback mode */
	unsigned char   interface;      /* RS-232/530/485/whatever */

	/* HDLC Only */

	unsigned short	flags;
	unsigned char	encoding;	/* NRZ, NRZI, etc. */
	unsigned long	clock_speed;	/* external clock speed in bits per second */
	unsigned char	addr_filter;	/* receive HDLC address filter, 0xFF = disable */
	unsigned short	crc_type;	/* None, CRC16-CCITT, or CRC32-CCITT */
	unsigned char	preamble_length;
	unsigned char	preamble;

	/* Async Only */

	unsigned long	data_rate;	/* bits per second */
	unsigned char	data_bits;	/* 7 or 8 data bits */
	unsigned char	stop_bits;	/* 1 or 2 stop bits */
	unsigned char	parity;		/* none, even, or odd */

} R56_PARAMS, *PR56_PARAMS;

#define R561_DEVICE_ID  0x5101   // The original highspeed sync card
#define R562_DEVICE_ID  0x5104   // Low profile
#define R563_DEVICE_ID  0x3514   // PC104+ version
#define R564_DEVICE_ID  0x9063   // Custom card, do we even need a driver?
#define R565_DEVICE_ID  0x5105   // Mini PCI version


/*
** device diagnostics status
*/

#define DiagStatus_OK				0
#define DiagStatus_AddressFailure		1
#define DiagStatus_AddressConflict		2
#define DiagStatus_IrqFailure			3
#define DiagStatus_IrqConflict			4
#define DiagStatus_DmaFailure			5
#define DiagStatus_DmaConflict			6
#define DiagStatus_PciAdapterNotFound		7
#define DiagStatus_CantAssignPciResources	8
#define DiagStatus_CantAssignPciMemAddr	9
#define DiagStatus_CantAssignPciIoAddr		10
#define DiagStatus_CantAssignPciIrq		11
#define DiagStatus_MemoryError			12

#define SerialSignal_DCD            0x01     /* Data Carrier Detect */
#define SerialSignal_TXD            0x02     /* Transmit Data */
#define SerialSignal_RI             0x04     /* Ring Indicator */
#define SerialSignal_RXD            0x08     /* Receive Data */
#define SerialSignal_CTS            0x10     /* Clear to Send */
#define SerialSignal_RTS            0x20     /* Request to Send */
#define SerialSignal_DSR            0x40     /* Data Set Ready */
#define SerialSignal_DTR            0x80     /* Data Terminal Ready */


/*
 * Counters of the input lines (CTS, DSR, RI, CD) interrupts
 */
struct r56_icount {
	__u32	cts, dsr, rng, dcd, tx, rx;
	__u32	frame, parity, overrun, brk;
	__u32	buf_overrun;
	__u32	txok;
	__u32	txunder;
	__u32	txabort;
	__u32	txtimeout;
	__u32	rxshort;
	__u32	rxlong;
	__u32	rxabort;
	__u32	rxover;
	__u32	rxcrc;
	__u32	rxok;
	__u32	exithunt;
	__u32	rxidle;
};

struct gpio_desc {
	__u32 state;
	__u32 smask;
	__u32 dir;
	__u32 dmask;
};

#define DEBUG_LEVEL_DATA	1
#define DEBUG_LEVEL_ERROR 	2
#define DEBUG_LEVEL_INFO  	3
#define DEBUG_LEVEL_BH    	4
#define DEBUG_LEVEL_ISR		5

/*
** Event bit flags for use with R56WaitEvent
*/

#define Rt56Event_DsrActive	0x0001
#define Rt56Event_DsrInactive	0x0002
#define Rt56Event_Dsr		0x0003
#define Rt56Event_CtsActive	0x0004
#define Rt56Event_CtsInactive	0x0008
#define Rt56Event_Cts		0x000c
#define Rt56Event_DcdActive	0x0010
#define Rt56Event_DcdInactive	0x0020
#define Rt56Event_Dcd		0x0030
#define Rt56Event_RiActive	0x0040
#define Rt56Event_RiInactive	0x0080
#define Rt56Event_Ri		0x00c0
#define Rt56Event_ExitHuntMode	0x0100
#define Rt56Event_IdleReceived	0x0200

/* Private IOCTL codes:
 *
 * R56_IOCSPARAMS	set R56_PARAMS structure values
 * R56_IOCGPARAMS	get current R56_PARAMS structure values
 * R56_IOCSTXIDLE	set current transmit idle mode
 * R56_IOCGTXIDLE	get current transmit idle mode
 * R56_IOCTXENABLE	enable or disable transmitter
 * R56_IOCRXENABLE	enable or disable receiver
 * R56_IOCTXABORT	abort transmitting frame (HDLC)
 * R56_IOCGSTATS	return current statistics
 * R56_IOCWAITEVENT	wait for specified event to occur
 * R56_LOOPTXDONE	transmit in HDLC LoopMode done
 * R56_IOCSIF          set the serial interface type
 * R56_IOCGIF          get the serial interface type
 */
#define R56_MAGIC_IOC	        'm'
#define R56_IOCSPARAMS		_IOW(R56_MAGIC_IOC,0,struct _R56_PARAMS)
#define R56_IOCGPARAMS		_IOR(R56_MAGIC_IOC,1,struct _R56_PARAMS)
#define R56_IOCSTXIDLE		_IO(R56_MAGIC_IOC,2)
#define R56_IOCGTXIDLE		_IO(R56_MAGIC_IOC,3)
#define R56_IOCTXENABLE	_IO(R56_MAGIC_IOC,4)
#define R56_IOCRXENABLE	_IO(R56_MAGIC_IOC,5)
#define R56_IOCTXABORT		_IO(R56_MAGIC_IOC,6)
#define R56_IOCGSTATS		_IO(R56_MAGIC_IOC,7)
#define R56_IOCWAITEVENT	_IOWR(R56_MAGIC_IOC,8,int)
#define R56_IOCCLRMODCOUNT	_IO(R56_MAGIC_IOC,15)
#define R56_IOCLOOPTXDONE	_IO(R56_MAGIC_IOC,9)
#define R56_IOCSIF		_IO(R56_MAGIC_IOC,10)
#define R56_IOCGIF		_IO(R56_MAGIC_IOC,11)
#define R56_IOCSGPIO		_IOW(R56_MAGIC_IOC,16,struct gpio_desc)
#define R56_IOCGGPIO		_IOR(R56_MAGIC_IOC,17,struct gpio_desc)
#define R56_IOCWAITGPIO	_IOWR(R56_MAGIC_IOC,18,struct gpio_desc)

#endif /* _ROUTE56_H_ */
