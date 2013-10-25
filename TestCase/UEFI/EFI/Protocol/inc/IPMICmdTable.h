/**
 * $Id: IPMICmdTable.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   IPMICmdTable.h
 *  @brief  This header file defines the IPMI command table and search function
 *
 *  @internal
 *  $RCSfile: IPMICmdTable.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/IPMICmdTable.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _IPMICMDTABLE_H
#define _IPMICMDTABLE_H

#include "../../../cdf/ostd/inc/type.h"
#include "IPMIMsg.h"

/***************************************************************************
 * Constant value definitions
 ***************************************************************************/

/* define the message handle function prototype*/
typedef UINT8 (* IPMIRequestHandleFunctionType)(sIPMIMsg *, UINT8 *, UINT8 *);
typedef void (* IPMIResponseHandleFunctionType)(sIPMIMsg *);
typedef void (* IPMIRequestTimeOutHandleFunctionType) \
             (sIPMIMsg *, ChannelHandle);

/***************************************************************************
 * Macro Function definitions
 ***************************************************************************/
#define SEARCHKEY(n, m)      ((n << 8) | m)

/* Command owners  -- by BZhou for SanMarco */
#define CMDOWNER_BMC		0x01
#define CMDOWNER_RAC		0x02
#define CMDOWNER_BMCRAC	(CMDOWNER_BMC | CMDOWNER_RAC) // 0x03

/******************************************************************************
*   STRUCT      :   sRequestHandleTableEntry
******************************************************************************/
/**
 *  @brief   request  handle table entry
 *
 *****************************************************************************/
typedef struct
{
    /** search key, High byte is NetFn (even), low byte is CmdID */
    UINT16                          u16NetFunCmd;

    /** the command privilege */
    Privilege                       PrivilegeLevel;

    /** address of command handle function */
    IPMIRequestHandleFunctionType   RequestHandleFunction;

   /** the command owner: CMDOWNER_BMC, CMDOWNER_RAC, CMDOWNER_BMCRAC */
   UINT8					owner;
} sRequestHandleTableEntry;


/******************************************************************************
*   STRUCT      :   sResponseHandleTableEntry
******************************************************************************/
/**
 *  @brief   response handle table entry
 *
 *****************************************************************************/
typedef struct
{
    /** search key, High byte is NetFn (even), low byte is CmdID */
    UINT16                          u16NetFunCmd;

    /** address of response handle function */
    IPMIResponseHandleFunctionType  ResponseHandleFunction;
} sResponseHandleTableEntry;


/******************************************************************************
*   STRUCT      :   sRequestTimeOutHandleTableEntry
******************************************************************************/
/**
 *  @brief   request time out handle table entry
 *
 *****************************************************************************/
typedef struct
{
    /** search key, High byte is NetFn (even), low byte is CmdID */
    UINT16                                  u16NetFunCmd;

    /** address of request time out handle function */
    IPMIRequestTimeOutHandleFunctionType    RequestTimeOutHandleFunction;
} sRequestTimeOutHandleTableEntry;


/******************************************************************************
*                   Function Prototype Declaration
******************************************************************************/
/* search function */
BOOLEAN RequestHandleTableSearch(
                                  UINT8 u8NetFn,
                                  UINT8 u8Cmd,
                                  sRequestHandleTableEntry *psEntry
                                );


BOOLEAN ResponseHandleTableSearch(
                                   UINT8 u8NetFn,
                                   UINT8 u8Cmd,
                                   sResponseHandleTableEntry *psEntry
                                 );


BOOLEAN RequestTimeOutHandleTableSearch(
                                        UINT8 u8NetFn,
                                        UINT8 u8Cmd,
                                        sRequestTimeOutHandleTableEntry *psEntry
                                       );


/* default handle functions */
UINT8 OverPrivilegeRequestHandle(
                                  sIPMIMsg *psMsg,
                                  UINT8 *pu8RspDataLen,
                                  UINT8 au8RspData[]
                                );


UINT8 NotSupportRequestHandle(
                               sIPMIMsg *psMsg,
                               UINT8 *pu8RspDataLen,
                               UINT8 au8RspData[]
                             );


void  DefaultRequestTimeOutHandle(
                                   sIPMIMsg *psTimeOutMsg,
                                   ChannelHandle hDestChannel
                                 );


void  DefaultResponseHandle(
                             sIPMIMsg *psMsg
                           );

#endif /* _IPMICMDTABLE_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: IPMICmdTable.h,v $
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
