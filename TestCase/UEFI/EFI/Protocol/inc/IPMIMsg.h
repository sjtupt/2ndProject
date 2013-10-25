/**
 * $Id: IPMIMsg.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   IPMIMsg.h
 *  @brief  This header file defines the IPMI message structure
 *
 *  @internal
 *  $RCSfile: IPMIMsg.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/IPMIMsg.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _IPMIMSG_H
#define _IPMIMSG_H

#include "../../../cdf/ostd/inc/type.h"
#include "IPMIType.h"

#define IPMI_MAX_MSG_SIZE                   0x28


/******************************************************************************
*   STRUCT      :   sIPMIMsg
******************************************************************************/
/**
 *  @brief   IPMI message structure in IPMI message queue which consists of
 *           the IPMI message header, data and channel control info
 *
 *****************************************************************************/
typedef struct
{
    /** The handle to access src channel information */
    ChannelHandle       hSrcChannelHandle;

    /** The current privilege of the channel session */
    Privilege           PrivilegeLevel;

    /** The responder's address */
    UINT8               u8RsA;

    /** The Net function and responder's LUN */
    NetFnLUNCouple      u8NetFnRsLUN;

    /** The Requester's Address */
    UINT8               u8RqA;

    /** The sequence number and requester's LUN */
    SeqNoLUNCouple      u8RqSeqNoRqLUN;

    /** The IPMI command ID */
    UINT8               u8Cmd;

    /** The IPMI command data length */
    UINT8               u8CmdDataLen;

#ifdef SYS_PROFILE_MSG_RSP
    /** The start execution time of this message */
    UINT32              u32StartTime;
#endif

    /** the IPMI command data (including completion code) */
    UINT8               au8CmdData[IPMI_MAX_MSG_SIZE];
    
} sIPMIMsg;

#endif /* _IPMIMSG_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: IPMIMsg.h,v $
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
