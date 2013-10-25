/**
 * $Id: CmdByteIndex.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *        Copyright &copy; 2001-2005 OSA Technologies. All Rights Reserved.
 *</em> </center>
 *----------------------------------------------------------------------------\n
 *  MODULES     impi \n
 *----------------------------------------------------------------------------\n
 *  @file   CmdByteIndex.h
 *  @brief  Command byte index definitions
 *
 *          This file is for the use of command handler. The index is for the
 *          convenience to map IPMI spec. to convert 1-base spec. index and
 *          0-base array index.
 *
 *  @internal
 *  $RCSfile: CmdByteIndex.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/CmdByteIndex.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _CMDBYTEINDEX_H
#define _CMDBYTEINDEX_H


/* Command request byte index */
#define IPMI_REQ_BYTE_01    0
#define IPMI_REQ_BYTE_02    1
#define IPMI_REQ_BYTE_03    2
#define IPMI_REQ_BYTE_04    3
#define IPMI_REQ_BYTE_05    4
#define IPMI_REQ_BYTE_06    5
#define IPMI_REQ_BYTE_07    6
#define IPMI_REQ_BYTE_08    7
#define IPMI_REQ_BYTE_09    8
#define IPMI_REQ_BYTE_10    9
#define IPMI_REQ_BYTE_11    10
#define IPMI_REQ_BYTE_12    11
#define IPMI_REQ_BYTE_13    12
#define IPMI_REQ_BYTE_14    13
#define IPMI_REQ_BYTE_15    14
#define IPMI_REQ_BYTE_16    15
#define IPMI_REQ_BYTE_17    16
#define IPMI_REQ_BYTE_18    17
#define IPMI_REQ_BYTE_19    18
#define IPMI_REQ_BYTE_20    19
#define IPMI_REQ_BYTE_21    20
#define IPMI_REQ_BYTE_22    21
#define IPMI_REQ_BYTE_23    22
#define IPMI_REQ_BYTE_24    23
#define IPMI_REQ_BYTE_25    24
#define IPMI_REQ_BYTE_26    25
#define IPMI_REQ_BYTE_27    26
#define IPMI_REQ_BYTE_28    27
#define IPMI_REQ_BYTE_29    28
#define IPMI_REQ_BYTE_30    29
#define IPMI_REQ_BYTE_31    30
#define IPMI_REQ_BYTE_32    31
#define IPMI_REQ_BYTE_33    32
#define IPMI_REQ_BYTE_34    33
#define IPMI_REQ_BYTE_35    34
#define IPMI_REQ_BYTE_36    35
#define IPMI_REQ_BYTE_37    36
#define IPMI_REQ_BYTE_38    37
#define IPMI_REQ_BYTE_39    38
#define IPMI_REQ_BYTE_40    39
#define IPMI_REQ_BYTE_41    40
#define IPMI_REQ_BYTE_42    41

/* Command response byte index */
#define IPMI_RSP_BYTE_02    0
#define IPMI_RSP_BYTE_03    1
#define IPMI_RSP_BYTE_04    2
#define IPMI_RSP_BYTE_05    3
#define IPMI_RSP_BYTE_06    4
#define IPMI_RSP_BYTE_07    5
#define IPMI_RSP_BYTE_08    6
#define IPMI_RSP_BYTE_09    7
#define IPMI_RSP_BYTE_10    8
#define IPMI_RSP_BYTE_11    9
#define IPMI_RSP_BYTE_12    10
#define IPMI_RSP_BYTE_13    11
#define IPMI_RSP_BYTE_14    12
#define IPMI_RSP_BYTE_15    13
#define IPMI_RSP_BYTE_16    14
#define IPMI_RSP_BYTE_17    15
#define IPMI_RSP_BYTE_18    16
#define IPMI_RSP_BYTE_19    17
#define IPMI_RSP_BYTE_20    18
#define IPMI_RSP_BYTE_21    19
#define IPMI_RSP_BYTE_22    20
#define IPMI_RSP_BYTE_23    21
#define IPMI_RSP_BYTE_24    22
#define IPMI_RSP_BYTE_25    23
#define IPMI_RSP_BYTE_26    24
#define IPMI_RSP_BYTE_27    25
#define IPMI_RSP_BYTE_28    26
#define IPMI_RSP_BYTE_29    27
#define IPMI_RSP_BYTE_30    28
#define IPMI_RSP_BYTE_31    29
#define IPMI_RSP_BYTE_32    30
#define IPMI_RSP_BYTE_33    31
#define IPMI_RSP_BYTE_34    32
#define IPMI_RSP_BYTE_35    33
#define IPMI_RSP_BYTE_36    34
#define IPMI_RSP_BYTE_37    35
#define IPMI_RSP_BYTE_38    36
#define IPMI_RSP_BYTE_39    37
#define IPMI_RSP_BYTE_40    38
#define IPMI_RSP_BYTE_41    39
#define IPMI_RSP_BYTE_42    40


#endif /* _CMDBYTEINDEX_H */


/* End of code */


/***************************************************************************
 *
 *   $Log: CmdByteIndex.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.1  2005/03/28 05:59:34  bitman
 *   Project: sanmarco
 *   BugID: none
 *   Comment: updated to IPMI v2.0
 *
 *   Revision 1.1.2.1  2005/02/23 13:25:02  wattwu
 *   [Change Note ID]:10445
 *
 *   [Internal APIs]
 *   [External APIs]
 *   [Brief]
 *   Integrate Tag_00_0001_0032_0000 HWD codes into Chicago product tree.
 *
 *   Revision 1.2  2005/02/22 04:34:18  ayu
 *   [Change Note ID]:10426
 *   [Brief]
 *   Modify command byte index naming.
 *
 *   Revision 1.1  2005/02/21 08:19:19  ayu
 *   [Change Note ID]:10417
 *   [Brief]
 *   Define command byte index.
 *
 *
***************************************************************************/
