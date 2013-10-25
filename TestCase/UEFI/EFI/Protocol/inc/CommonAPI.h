/**
 * $Id: CommonAPI.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
 *
 *<center>
 *               OSA Technologies, Inc. Proprietary Information
 * \n<em>
 *      This software is supplied under the terms of a license agreement or
 *      nondisclosure agreement with OSA Technologies, Inc. and may not copied,
 *      disseminated or distributed except in accordance with the terms of that
 *      agreement.
 *
 *      2001 Gateway Place, Suite 520W, San Jose, California, 95110 U.S.A.
 * \n
 *                  US phone: 408.436.6333
 * \n
 *                  email: ip_manager@osatechnologies.com
 * \n
 *
 *        Copyright &copy; 2001-2005 OSA Technologies. All Rights Reserved.
 *</em> </center>
 *----------------------------------------------------------------------------\n
 *  MODULES     ipmi \n
 *----------------------------------------------------------------------------\n
 *  @file   CommonAPI.h
 *  @brief  This header file defines the common functions used by many modules
 *
 *  @internal
 *  $RCSfile: CommonAPI.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/CommonAPI.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _COMMONAPI_H
#define _COMMONAPI_H

#include <syslog.h>
#include "../../../cdf/tx__/inc/tx_api.h"
#include "../../../cdf/ostd/inc/type.h"
#include "IPMIType.h"
#include "Status.h"

/* OSA Generic Queue function return code */
#define OSAQUEUE_RET_OK         0x00
#define OSAQUEUE_RET_ERR        0x09
#define OSAQUEUE_RET_EMPTY      0x0A
#define OSAQUEUE_RET_FULL       0x0B
#define OSAQUEUE_RET_TIEMOUT    0x04

/* OSA Generic Queue access waiting option */
#define OSAQUEUE_NO_WAIT        0x00000000
#define OSAQUEUE_WAIT_FOREVER   0xFFFFFFFF

/* define standard self test answer code */
#define SELFTEST_ANS_NO_ERR         0x55
#define SELFTEST_ANS_NOT_IMP        0x56
#define SELFTEST_ANS_DEIVCE_ERR     0x57
#define SELFTEST_ANS_FATAL_ERR      0x58
#define SELFTEST_ANS_RESERVED       0xFF

/* define the standard self test SELFTEST_ANS_DEIVCE_ERR error bit */
#define SELFTEST_BAD_OB             0x01
#define SELFTEST_BAD_BB             0x02
#define SELFTEST_BAD_PS             0x04
#define SELFTEST_SDR_EMPTY          0x08
#define SELFTEST_BAD_IPMB           0x10
#define SELFTEST_BAD_FRU            0x20
#define SELFTEST_BAD_SDR            0x40
#define SELFTEST_BAD_SEL            0x80

/* define OSA or OEM self test type code */
#define SELFTEST_OSA_ERR_TYPE       0x80
#define SELFTEST_OEM_ERR_TYPE       0xC0

/* there are 2 groups of error code, 32 bits for each group,
   so it can indicate 64 errors for both OSA and OEM errors */
#define SELFTEST_ERROR_GORUP_0      0x00
#define SELFTEST_ERROR_GORUP_1      0x01

/* define the OSA self test SELFTEST_ANS_OSA_ERR error bit, group 0 */
#define SELFTEST_NO_PWR_UNIT_SDR    0x00000001
#define SELFTEST_NO_WATCHODOG_SDR   0x00000002
#define SELFTEST_FAN_INIT_FAIL      0x00000004
#define SELFTEST_PEF_INIT_FAIL      0x00000008
#define SELFTEST_GPIO_INIT_FAIL     0x00000010
#define SELFTEST_NIC_INIT_FAIL      0x00000020

/******************************************************************************
*   STRUCT      :   sOSAGenericQueue
******************************************************************************/
/**
 *  @brief   OSA Genric free entry size queue control
 *
 *****************************************************************************/
typedef struct
{
    /** The index queue */
    TX_QUEUE        txIndexQueue;

    /** The memory pool to store each entry */
    UINT8           *pu8ItemPool;

    /** Need to do mutex control or not */
    UINT8           u8MutexControl;

    /** Binary semaphore for mutex control */
    TX_SEMAPHORE    txPoolSemaphore;

    /** Single entry size */
    UINT8           u8ItemSize;

    /** Total entry number in the queue */
    UINT8           u8ItemNo;
} sOSAGenericQueue;


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
UINT8 GetControllerAddress(void);

void GetSysGUID(
                 UINT8 *pu8GUID
               );

STATUS SetSysGUID(
                   UINT8 *pu8GUID
                 );

STATUS SetBMCSA(
                 UINT8 u8BMCSA
               );


STATUS GetBMCSA(
                 UINT8 *pu8BMCSA
               );

void SetControllerAddress(
                           UINT8 u8ControllerAddress
                         );

NetFnLUNCouple NetFnLUNCoupleConstruct(
                                        UINT8 u8NetFn,
                                        UINT8 u8LUN
                                      );

UINT8 NetFnLUNCoupleGetNetFn(
                              NetFnLUNCouple Couple
                            );

void NetFnLUNCoupleDecouple(
                             NetFnLUNCouple Couple,
                             UINT8 *pu8NetFn,
                             UINT8 *pu8LUN
                           );


SeqNoLUNCouple SeqNoLUNCoupleConstruct(
                                        UINT8 u8SeqNo,
                                        UINT8 u8LUN
                                      );

UINT8 SeqNoLUNCoupleGetSeqNo(
                              SeqNoLUNCouple Couple
                            );

void SeqNoLUNCoupleDecouple(
                             SeqNoLUNCouple Couple,
                             UINT8 *pu8SeqNo,
                             UINT8 *pu8LUN
                           );

/* OSA Generic Queue APIs */
UINT32 QueueCalcSize(
                      UINT8 u8ItemSize,
                      UINT8 u8ItemNo
                    );

UINT8 QueueCreate(
                   sOSAGenericQueue *psQueue,
                   char *pQueueName,
                   UINT8 u8ItemSize,
                   UINT8 u8ItemNo,
                   VOID *pQueue_start,
                   UINT32 u32QueueSize,
                   BOOLEAN bNeedMutextControl
                 );


UINT8 QueueFlush(
                  sOSAGenericQueue *psQueue
                );

UINT8 QueueReceive(
                    sOSAGenericQueue *psQueue,
                    VOID *pRecvItem,
                    UINT32 u32WaitTime
                  );

UINT8 QueueSend(
                 sOSAGenericQueue *psQueue,
                 VOID *pSrcItem,
                 BOOLEAN bNoWait,
                 BOOLEAN bToEnd
               );

/****  BMC Runtime log utilities    ****/
typedef enum {
	SYSLOG_LEVEL_QUIET = 0,
	SYSLOG_LEVEL_FATAL,
	SYSLOG_LEVEL_ERROR,
	SYSLOG_LEVEL_INFO,
	SYSLOG_LEVEL_VERBOSE,
	SYSLOG_LEVEL_DEBUG1,
	SYSLOG_LEVEL_DEBUG2,
	SYSLOG_LEVEL_DEBUG3,
	SYSLOG_LEVEL_INVALID
} LogLevel;

void Drac5_Log(LogLevel level, char *herefmt, char *func, int line, char *pMsg, ...);

#define LogFatal(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_FATAL, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogError(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_ERROR, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogInfo(pMsg...) 	\
	Drac5_Log(SYSLOG_LEVEL_INFO, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogVerbose(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_VERBOSE, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogDebug1(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_DEBUG1, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogDebug2(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_DEBUG2, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogDebug3(pMsg...)  \
	Drac5_Log(SYSLOG_LEVEL_DEBUG3, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

#define LogRac(pMsg...) \
	Drac5_Log(SYSLOG_LEVEL_INVALID, "%s:%d: ", __FUNCTION__, __LINE__, pMsg);

/* For compitable */
#define TRACE	LogInfo


UINT8 GetBitIndex32(
                     UINT32 u32Flag
                   );

INT8 MemCmp(
             UINT8 *pu8Src,
             UINT8 *pu8Dest,
             UINT8 u8Size
           );

void *MemAlloc(
                UINT8  u8MemID,
                UINT32 u32Size
              );

void *MemGetOEMMemStartAddress(void);

/* stanard self test report functions */
void SetStdSelfTestError(
                          UINT8 u8AnswerCode,
                          UINT8 u8ErrorCode
                        );

void GetStdSelfTestError(
                          UINT8 *pu8AnswerCode,
                          UINT8 *pu8ErrorCode
                        );

/* OSA or OEM defined self test report functions */
void SetSelfTestError(
                       UINT8 u8TypeCode,
                       UINT8 u8GroupNum,
                       UINT32 u32ErrorCode
                     );

void GetSelfTestError(
                       UINT8 u8TypeCode,
                       UINT32 au32ErrorCode[]
                     );
/* byte order convert functions */

/* convert byte order between LSB and MSB */
#define MSB2LSB32(u32MSB)       ((u32MSB << 24) \
                                 | ((u32MSB << 8) & 0x00FF0000) \
                                 | ((u32MSB >> 8) & 0x0000FF00) \
                                 | (u32MSB >> 24))

#define LSB2MSB32(u32LSB)       MSB2LSB32(u32LSB)

#define MSB2LSB16(u16MSB)       ((u16MSB >> 8) | (u16MSB << 8))
#define LSB2MSB16(u16LSB)       MSB2LSB16(u16LSB)

/* force convert host byte order to MSB or LSB byte stream */
void    HtoMSB32(
                  UINT32 u32Host,
                  UINT8 au8MSBData[]
                );

void    HtoLSB32(
                  UINT32 u32Host,
                  UINT8 au8LSBData[]
                );

void    HtoMSB16(
                  UINT16 u16Host,
                  UINT8 au8MSBData[]
                );

void    HtoLSB16(
                  UINT16 u16Host,
                  UINT8 au8LSBData[]
                );

UINT8   GetMSB16(
                  UINT16 u16X
                );

UINT8   GetLSB16(
                  UINT16 u16X
                );

UINT16  MakeUINT16(
                    UINT8 u8MSB,
                    UINT8 u8LSB
                  );

BOOLEAN IsSystemInterface(
                           UINT8 u8ChannelID
                         );

/* calculate CRC checksum */
UINT16 CRCCalculation(
                       UINT16 u16Length,
                       void *pStartPtr
                     );

void DigitToASCII(
                    UINT8 u8Digit,
                    char *u8DataBuf
                  );

#define AlignTo4(n)     (((n)%4) ? ((n) + (4 - ((n) % 4))) : (n))

/* force convert MSB/LSB byte stream to host byte order */
#define MSB2H32(au8MSBData) \
        ((((UINT32) au8MSBData[0]) << 24) \
         | (((UINT32) au8MSBData[1]) << 16) \
         | (((UINT32) au8MSBData[2]) << 8) \
         | au8MSBData[3])

#define LSB2H32(au8LSBData) \
        ((((UINT32) au8LSBData[3]) << 24) \
         | (((UINT32) au8LSBData[2]) << 16) \
         | (((UINT32) au8LSBData[1]) << 8) \
         | au8LSBData[0])

#define MSB2H16(au8MSBData) \
        ((((UINT16) au8MSBData[0]) << 8) \
         | au8MSBData[1])

#define LSB2H16(au8LSBData) \
        ((((UINT16) au8LSBData[1]) << 8) \
         | au8LSBData[0])

/* Manufacturing Test Mode */
void SetManufacturingTestMode(
                               BOOLEAN IsMTM
                             );

BOOLEAN IsInManufacturingTestMode(void);

UINT8 BinToHex(
                /** Pointer to binary data */
                UINT8 *pu8BinData,

                /** Binary data size */
                UINT8 u8BinDataSize,

                /** Pointer to hex-pair string data */
                UINT8 *pu8HexData
              );

extern STATUS Exist_RacStatusCache(void);

extern STATUS Read_RacStatusCache(UINT8 *pu8read);

extern STATUS Write_RacStatusCache(UINT8 u8write);

#endif /* _COMMONAPI_H */


/* End of code */


/******************************************************************************
 *
 *   $Log: CommonAPI.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1.2.2  2005/02/23 13:17:16  wattwu
 *   [Change Note ID]:10445
 *
 *   [Internal APIs]
 *   [External APIs]
 *   [Brief]
 *   Integrate Tag_00_0001_0032_0000 HWD codes into Chicago product tree.
 *
 *   Revision 1.1.2.1  2005/01/24 13:28:48  jho
 *   Integrate Tag_00_0001_0030_0001 HW-ID codes into Chicago product tree.
 *   Modify the copyright to 2005.
 *
 *   Revision 1.1  2005/01/13 03:20:57  jhwang
 *   Initial revision
 *
 *
 *****************************************************************************/
