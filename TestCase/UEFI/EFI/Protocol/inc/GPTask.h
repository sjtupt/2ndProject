/**
 * $Id: GPTask.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  MODULES     ipmi \n
 *----------------------------------------------------------------------------\n
 *  @file   GPTask.h
 *  @brief  Defines general purpose events handler Task and events table APIs
 *
 *  @internal
 *  $RCSfile: GPTask.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/GPTask.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _GPTASK_H
#define _GPTASK_H

#include "../../../cdf/tx__/inc/tx_api.h"
#include "../../../cdf/ostd/inc/type.h"
#include "Status.h"

#define HIGH_SPEED_GPTASK   0x00
#define LOW_SPEED_GPTASK    0x01

#define GPEVENT_FREE        0x00    /* no handle assgined to this event */
#define GPEVENT_ONCE        0x01    /* exe once and remove from event table */
#define GPEVENT_RESIDENT    0xFF    /* resident until cancel it */

/* This value is returned if register GPTask event fails */
#define INVALID_GPTASK_HANDLE  0xFFFFFFFF

typedef void (* GPEventHandleFunctionType)(UINT32);

/******************************************************************************
*   STRUCT      :   sGPEventHandle
******************************************************************************/
/**
 *  @brief   Genernal purpose event handle function information
 *
 *****************************************************************************/
typedef struct
{
    /** The type of handle function
        (GPEVENT_FREE, GPEVENT_ONCE, GPEVENT_RESIDENT) */
    UINT8                       u8Type;

    /** Handle function entry point */
    GPEventHandleFunctionType   HandleFunc;
} sGPEventHandle;


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
void GPHiSpeedEventHandlerTask(
                                ULONG thread_input
                              );

void GPLoSpeedEventHandlerTask(
                                ULONG thread_input
                              );

UINT32 RegisterGPEvent(
                        sGPEventHandle *psGPEventHandle,
                        UINT8 u8GPTaskType
                      );

STATUS SetGPTaskEvent(
                       UINT32 u32EventID,
                       UINT8 u8GPTaskType
                     );

void CancelGPEvent(
                    UINT32 u32EventID,
                    UINT8 u8GPTaskType
                  );

#endif /* _GPTASK_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: GPTask.h,v $
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
