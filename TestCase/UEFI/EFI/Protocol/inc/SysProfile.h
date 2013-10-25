/**
 * $Id: SysProfile.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   SysProfile.h
 *  @brief  This header file defines the profile functions used by many modules
 *
 *  @internal
 *  $RCSfile: SysProfile.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SysProfile.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _SYS_PROFILE_H
#define _SYS_PROFILE_H

#include "../../../cdf/ostd/inc/type.h"

#define FWTIMERSTAMP_INVALID                            0xFFFFFFFF

#define PROFILE_INVALID                                 0xFFFFFFFF


UINT32 ProfileGetTimerStamp(
                             void
                           );

UINT32 ProfileGetMaxTaskStackSize(
                                   UINT32 u32ThreadID
                                 );

void ProfileThreadResume(
                          void
                        );

void ProfileThreadSuspend(
                           void
                         );

UINT32 ProfileGetMaxTaskExecTime(
                                  UINT32 u32ThreadID
                                );

void ProfileClearMaxTaskExecTime(
                                  void
                                );

UINT8 ProfileGetChnlMaxRspTime(
                                UINT8 u8ChannelID, 
                                UINT8 *pu8Resp
                              );

void ProfileLogChnlRspTime(
                            UINT8 u8ChannelID, 
                            UINT8 u8NetFn, 
                            UINT8 u8CmdID, 
                            UINT32 u32Time
                          );

#endif

/* End of code */


/*****************************************************************************
 *
 *   $Log: SysProfile.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1.1.1  2004/12/13 09:35:30  jhwang
 *   Initialization of IPMI 2.0 core tree
 *
 *
 ****************************************************************************/
