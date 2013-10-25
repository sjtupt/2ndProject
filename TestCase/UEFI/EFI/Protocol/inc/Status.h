/**
 * $Id: Status.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   Status.h
 *  @brief  This header file defines the STATUS code and type
 *
 *  @internal
 *  $RCSfile: Status.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/Status.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _STATUS_H
#define _STATUS_H


/******************************************************************************
*   ENUM    :   STATUS
******************************************************************************/
/**
 *  @brief
 *
 *****************************************************************************/
typedef enum
{
    /* Common Section */
    STATUS_OK = 0,      /* STATUS_OK = 0 */
    STATUS_FAIL,
    STATUS_BUSY,

    /* No device or device error */
    STATUS_DEVICE_NACK,

    /* Arbitration Lost */
    STATUS_DEVICE_AL,

    /* Driver section */
    STATUS_DRIVER_BEGIN = 200,

    /* Channel Common Section */
    STATUS_CHANNEL_BEGIN = 300,

    /* IPMB Channel Section */
    STATUS_IPMB_CHANNEL_BEGIN = 400,

    /* KCS Channel Section */
    STATUS_KCS_CHANNEL_BEGIN = 500,

    /* LAN Channel Section */
    STATUS_LAN_CHANNEL_BEGIN = 600,

    /* UART Channel Section */
    STATUS_UART_CHANNEL_BEGIN = 600

    /* Misc Channel Section */

    /* append your section from here */
} STATUS;

#endif /* _STATUS_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: Status.h,v $
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
