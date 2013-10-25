/**
 * $Id: IPMIType.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   IPMIType.h
 *  @brief  Defines data type for global used in this project
 *
 *  @internal
 *  $RCSfile: IPMIType.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/IPMIType.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _IPMITYPE_H
#define _IPMITYPE_H

#include "../../../cdf/ostd/inc/type.h"

#define INVALID_SEQNO           0xFF
#define INVALID_CHANNEL_HANDLE  0xFF


/** combine NetFn and LUN in one byte,
    Bit 7..2 is NetFn,
    Bit 1..0 is LUN    */
typedef UINT8   NetFnLUNCouple;


/** combine IPMB SeqNo and LUN in one byte,
    Bit 7..2 is IPMB SeqNo,
    Bit 1..0 is LUN         */
typedef UINT8   SeqNoLUNCouple;


/** define the channel acess handle,
    Bit 7..4 is channel number ( refer to IPMI spec 6.3),
    Bit 3..0 is channel session handle (at most 4 sessions per channel at the
    same time) */
typedef UINT8   ChannelHandle;


/** encapsulate the privilege level type */
typedef UINT8   Privilege;


#endif /* _IPMITYPE_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: IPMIType.h,v $
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
