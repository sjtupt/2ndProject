/**
 * $Id: SysInit.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   SysInit.h
 *  @brief  This header file define the program initialize functions
 *
 *  @internal
 *  $RCSfile: SysInit.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SysInit.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _SYSINIT_H
#define _SYSINIT_H

#include "../../../cdf/ostd/inc/type.h"
#include "../../../cdf/tx__/inc/tx_api.h"


#define SYSINIT_FILEID_OSA      10000
#define SYSINIT_FILEID_OEM      20000


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
UINT8 SystemInit(
                  void *first_unused_memory
                );

void  SystemInitErrorReport(
                             UINT32 u32FileID,
                             UINT32 u32FunctionID,
                             UINT32 u32Pos,
                             UINT32 u32ErrorCode
                           );

/****************************************************************************
* FUNCTION      :   CalcMemSizeFunction
*--------------------------------------------------------------------------
* DESCRIPTION   :   Calc the required memory size of the resource allocator
*
* FUNCTION TYPE :   Global function
*
* Context/Scope :
*                   These function will called by system initialize framework,
*                   please intser your calac function in the
*                   G_aCalcMemSizeFunctionTable
*
*--------------------------------------------------------------------------
* PARAMETERS    :
*       None
*
* RETURNS       :   required memory size
****************************************************************************/
typedef UINT32 (* CalcMemSizeFunction)(UINT8);

/****************************************************************************
* FUNCTION      :   SysInitFuncion
*--------------------------------------------------------------------------
* DESCRIPTION   :   Init function of each module
*
* FUNCTION TYPE :   Global function
*
* Context/Scope :
*                   These function will called by system initialize framework,
*                   please intser your initialize function in
*                   the G_aSysPreInitFunctionTable
*--------------------------------------------------------------------------
* PARAMETERS    :
*       None
*
* RETURNS       :   0 if success, otherwise fail
****************************************************************************/
typedef UINT8 (* SysInitFuncion)(void);



#endif /* _SYSINIT_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: SysInit.h,v $
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
