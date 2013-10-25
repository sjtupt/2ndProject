/**
 * $Id: SysTaskTable.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   SysTaskTable.h
 *  @brief  Defines System Task Creation Table structure and const
 *
 *  @internal
 *  $RCSfile: SysTaskTable.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SysTaskTable.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _SYSTASKTABLE_H
#define _SYSTASKTABLE_H

#include "../../../cdf/ostd/inc/type.h"
#include "../../../cdf/tx__/inc/tx_api.h"


typedef VOID (*TASK_ENTRY_FUNC)(ULONG);

/******************************************************************************
*   STRUCT      :   sTaskCreateTableEntry
******************************************************************************/
/**
 *  @brief   task creation table entry
 *
 *****************************************************************************/
typedef struct
{
    /** The task name */
    CHAR            TaskName[6];

    /** The task function entry address */
    TASK_ENTRY_FUNC TaskFunction;

    /** The task ID */
    UINT32          u32TaskID;

    /** The stack size of the task */
    UINT32          u32StackSize;

    /** The task priority */
    UINT8           u8Priority;

    /** The task preempt threshold */
    UINT8           u8PreemptThreshold;

    /** The task time slice */
    UINT32          u32TimeSlice;

    /** Should auto start the task or not */
    UINT8           u8AutoStart;
} sTaskCreateTableEntry;


#endif /* _SYSTASKTABLE_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: SysTaskTable.h,v $
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
