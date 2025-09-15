/*---------------------------------------------------------------------------*
  Project:  MDM API
  File:     mdm.h

  Copyright 2000 Nintendo.  All rights reserved.

  These coded instructions, statements, and computer programs contain
  proprietary information of Nintendo of America Inc. and/or Nintendo
  Company Ltd., and are protected by Federal copyright law.  They may
  not be disclosed to third parties or copied or duplicated in any form,
  in whole or in part, without the prior written consent of Nintendo.

  $Log: /Dolphin/include/dolphin/mdm.h $
    
    1     9/12/00 1:17p Shiki
    Initial check-in.

    1     ?/??/00 ?:?? ikedae
    Initial check-in.
  $NoKeywords: $
 *---------------------------------------------------------------------------*/


#pragma once

// public structure

struct MDMStatus
{
    u32     status;
};

typedef struct MDMStatus MDMStatus;
typedef void (*MDMCallback)(s32 para);

// pointers should be 32byte alligned whenever possible, for better performance.
// size also to be multiply of 32 byte

enum
{
    MDM_DIAL_TONE = 1,
    MDM_DIAL_PULSE_10,
    MDM_DIAL_PULSE_20
};

#define MDM_OK			1
#define MDM_CONNEXANT	1

enum {
	MDM_CN_V90 = 1,
	MDM_CN_FLEX,
	MDM_CN_V34,
	MDM_CN_V32B,
	MDM_CN_V22B,
	MDM_CN_PIAFS
	};
	
enum {
	MDM_EC_V42 = 1,
	MDM_EC_MNP,
	MDM_EC_DIRECT
	};

enum {
	MDM_CM_V42B = 1,
	MDM_CM_MNP5,
	MDM_CM_NONE,
	MDM_CM_BOTH
	};

// MDM API error code (negative value)
#define MDM_ERR_NOMODEM			-1
#define MDM_ERR_NOSUCHMODE		-2
#define MDM_ERR_STATE			-3
#define MDM_ERR_SENDBUSY		-4
#define MDM_ERR_RECVBUSY		-5
#define MDM_ERR_NOTCONNECTED	-6

// modem status register
#define MDM_MSR_DCD		0x80
#define MDM_MSR_RI		0x40
#define MDM_MSR_DSR		0x20
#define MDM_MSR_CTS		0x10
#define MDM_MSR_DDCD	0x08
#define MDM_MSR_DRI		0x04
#define MDM_MSR_DDSR	0x02
#define MDM_MSR_DCTS	0x01

// country code
#define MDM_CC_JAPAN	"00"
#define MDM_CC_US		"B5"
// when used in other countries, country code should be set
// according to "AT+GCI=" parameter.


#if 0
extern "C" {
#endif
extern s32 MDMInit(char *countrycode);
extern void MDMDestroy(void);

extern s32	MDMDial        (char *dialstr, s32 dialmode, MDMCallback cb);
extern void MDMAnswer      (MDMCallback cb);     // answer to the RING 
extern void MDMHangUp      (void);

extern void MDMChangeThreshold (s32 txth, s32 rxth);
extern void MDMSetFwt(s32 fwt);
extern BOOL MDMSend        (u8 *buf, s32 len, MDMCallback sndCallback);

extern BOOL MDMSendBusy(void);

extern s32 MDMRecv        (u8 *buf, s32 maxlen, MDMCallback rcvCallback);

extern BOOL MDMGetStatus   (MDMStatus *mdmStatus);

extern s32 MDMConnectMode(s32 mode);
extern s32 MDMErrorCorrectMode(s32 mode);
extern s32 MDMCompressMode(s32 mode);

// for better independecy from the specific modem model, AT command should not
// be issued as possible

// some CONNEXANT modem specific procedures are from here
extern s32 MDMATCommand   (char *atcmd);
extern BOOL MDMATBusy(void);
extern BOOL MDMGetStatus(MDMStatus *status);
extern u8 MDMGetRawStatus (void);   // represent LED of usual modem
                                    // this status should not be used for flow
                                    // control etc.
extern u8 MDMGetESR(void);
#if 0
}
#endif


#define MDM_DEFAULT_RXTH		480
#define MDM_DEFAULT_TXTH		33		// 32 + 1   because FIFOCOUNT < THRESHOLD

