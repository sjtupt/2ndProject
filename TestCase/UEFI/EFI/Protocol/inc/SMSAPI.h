/**
 * $Id: SMSAPI.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *        Copyright &copy; 2001-2004 OSA Technologies. All Rights Reserved.
 *</em> </center>
 *----------------------------------------------------------------------------\n
 * MODULES      ipmi \n
 *----------------------------------------------------------------------------\n
 *  @file   SMSAPI.h
 *  @brief  Define the API, data structure of SMS APIs
 *
 *  @internal
 *  $RCSfile: SMSAPI.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SMSAPI.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _SMSPAPI_H
#define _SMSPAPI_H

#include "../../../cdf/tx__/inc/tx_api.h"
#include "../../../cdf/ostd/inc/type.h"
#include "Status.h"

/* define the SMS Message Flag IDs */
#define SMS_MSG_FLG_RX_MSG_Q        0
#define SMS_MSG_FLG_EVENT_MSG_BUF   1
#define SMS_MSG_FLG_WATCHDOG        3
#define SMS_MSG_FLG_OEM0            5
#define SMS_MSG_FLG_OEM1            6
#define SMS_MSG_FLG_OEM2            7

#define MAX_SMS_MSG_FLG_NO          8

/* SMS interface relative functions */
typedef STATUS  (* Set_SMS_ATN_Func) (UINT8);

UINT8 InitSMSAPI(void);

void Register_SMS_ATN_Service(
                               Set_SMS_ATN_Func pServiceFunction
                             );

void SetSMSMessageFlag(
                        UINT8 u8FlagID
                      );

void ClearSMSMessageFlag(
                          UINT8 u8FlagID
                        );

void ClearSMSMessageFlagEx(
                            UINT8 au8Flags[]
                          );

void GetSMSMessageFlags(
                         UINT8 au8Flags[]
                       );

void SyncSMS_ATN_Status(void);

#endif /* _SMSPAPI_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: SMSAPI.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1.1.1  2004/12/13 09:35:30  jhwang
 *   Initialization of IPMI 2.0 core tree
 *
 *
 *****************************************************************************/

