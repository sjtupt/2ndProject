/**
 * $Id: CmdTableAPI.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  MODULES     impi \n
 *----------------------------------------------------------------------------\n
 *  @file   CmdTableAPI.h
 *  @brief  Command Table API
 *
 *  @internal
 *  $RCSfile: CmdTableAPI.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/CmdTableAPI.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _CMDTABLEAPI_H
#define _CMDTABLEAPI_H

#include "../../../cdf/ostd/inc/type.h"
#include "IPMICmdTable.h"

BOOLEAN GetCommandDefaultHandleFunction(
                                         UINT8 u8NetFn,
                                         UINT8 u8Cmd,
                                         sRequestHandleTableEntry *psEntry
                                       );

#endif /* _CMDTABLEAPI_H */

/* End of code */


/***************************************************************************
 *
 *   $Log: CmdTableAPI.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1.1.1  2004/12/13 09:35:30  jhwang
 *   Initialization of IPMI 2.0 core tree
 *
 *
***************************************************************************/
