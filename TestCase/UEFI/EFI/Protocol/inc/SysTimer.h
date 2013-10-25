/**
 * $Id: SysTimer.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 * MODULES      isel (IPMI SEL Commands)
 *----------------------------------------------------------------------------\n
 *  @file   SysTimer.h
 *  @brief  typedef's, constants, and function prototype's which is internal
 *          used in SEL module
 *
 *  @internal
 *  $RCSfile: SysTimer.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SysTimer.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef SYSTIMER_H
#define SYSTIMER_H

#include "../../../cdf/ostd/inc/type.h"
#include "Status.h"
// by bzhou
#include <time.h>


/******************************************************************************
*   STRUCT  :   SysTimerHookFuncTag
******************************************************************************/
/**
 *  @brief  In this data structure, contains two function prototype for hook
 *          functions.
 *
 *****************************************************************************/
struct SysTimerHookFuncTag
{
    /** This hook function is called when initialize the system timer */
    UINT32 (* ONSYSTIMERINIT)(void);

    /** This hook function is called when set the system timer */
    STATUS (* ONSYSTIMERSET)(UINT32);
};


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
UINT8 SysTimerInit(void);
STATUS SysTimerSet(
                    UINT32 u32TimeStamp
                  );
STATUS SysTimerGet(
                    UINT32 *pu32TimeStamp
                  );

void SysTimerRegisterHookFunc(
                                struct SysTimerHookFuncTag *psSysTimerHookFunc
                             );


#endif  /* SYSTIMER_H */

/* End of code */


/***************************************************************************
 *
 *   $Log: SysTimer.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1.1.1  2004/12/13 09:35:30  jhwang
 *   Initialization of IPMI 2.0 core tree
 *
 *
 **************************************************************************/
