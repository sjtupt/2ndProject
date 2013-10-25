/**
 * $Id: Config.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  MODULES     ipmi (intelligent platform management interface) \n
 *----------------------------------------------------------------------------\n
 *  @file   Config.h
 *  @brief  This file specifies the global defines for logical devices
 *
 *  @internal
 *  $RCSfile: Config.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/Config.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/
#ifndef CONFIG_H
#define CONFIG_H

/* Define Logical Device Class */
#define     FRU_CLASS          0
#define     SDR_CLASS          1
#define     SEL_CLASS          2
#define     IPMB_CLASS         3
#define     HOST_CLASS         4
#define     LAN_CLASS          5
#define     SER_MODEM_CLASS    6
#define     GPIO_CLASS         7
#define     ADC_CLASS          8
#define     FAN_CLASS          9
#define     LED_CLASS          10

/* Define FRU instance */
#define     FRU0              (0x0000 | (FRU_CLASS << 8))
#define     FRU1              (0x0001 | (FRU_CLASS << 8))
#define     FRU2              (0x0002 | (FRU_CLASS << 8))
#define     FRU3              (0x0003 | (FRU_CLASS << 8))
#define     FRU4              (0x0004 | (FRU_CLASS << 8))

/* Define SDR instance */
#define     SDR               (0x0000 | (SDR_CLASS << 8))

/* Define SEL instance */
#define     SEL               (0x0000 | (SEL_CLASS << 8))

/* Define GPIO instance */
#define     GPIO_ID0          (0x0000 | (GPIO_CLASS << 8))
#define     GPIO_ID1          (0x0001 | (GPIO_CLASS << 8))
#define     GPIO_ID2          (0x0002 | (GPIO_CLASS << 8))
#define     GPIO_ID3          (0x0003 | (GPIO_CLASS << 8))
#define     GPIO_ID4          (0x0004 | (GPIO_CLASS << 8))
#define     GPIO_ID5          (0x0005 | (GPIO_CLASS << 8))
#define     GPIO_ID6          (0x0006 | (GPIO_CLASS << 8))
#define     GPIO_ID7          (0x0007 | (GPIO_CLASS << 8))
#define     GPIO_ID8          (0x0008 | (GPIO_CLASS << 8))
#define     GPIO_ID9          (0x0009 | (GPIO_CLASS << 8))
#define     GPIO_ID10         (0x000A | (GPIO_CLASS << 8))
#define     GPIO_ID11         (0x000B | (GPIO_CLASS << 8))
#define     GPIO_ID12         (0x000C | (GPIO_CLASS << 8))
#define     GPIO_ID13         (0x000D | (GPIO_CLASS << 8))
#define     GPIO_ID14         (0x000E | (GPIO_CLASS << 8))
#define     GPIO_ID15         (0x000F | (GPIO_CLASS << 8))

/* Define ADC instance */
#define     ADC_ID0           (0x0000 | (ADC_CLASS << 8))
#define     ADC_ID1           (0x0001 | (ADC_CLASS << 8))
#define     ADC_ID2           (0x0002 | (ADC_CLASS << 8))
#define     ADC_ID3           (0x0003 | (ADC_CLASS << 8))
#define     ADC_ID4           (0x0004 | (ADC_CLASS << 8))
#define     ADC_ID5           (0x0005 | (ADC_CLASS << 8))
#define     ADC_ID6           (0x0006 | (ADC_CLASS << 8))
#define     ADC_ID7           (0x0007 | (ADC_CLASS << 8))

/* Define IPMB instance */
#define     IPMB              (0x0000 | (IPMB_CLASS << 8))

/* Define Host instance */
#define     HOST              (0x0000 | (HOST_CLASS << 8))

/* Define LAN instance */
#define     LAN0              (0x0000 | (LAN_CLASS << 8))
#define     LAN1              (0x0001 | (LAN_CLASS << 8))
#define     LAN2              (0x0002 | (LAN_CLASS << 8))

/* Define SerialModem instance */
#define     SERIAL_0          (0x0000 | (SER_MODEM_CLASS << 8))
#define     SERIAL_1          (0x0001 | (SER_MODEM_CLASS << 8))

/* Define Fan instance */
#define     FAN0              (0x0000 | (FAN_CLASS << 8))
#define     FAN1              (0x0001 | (FAN_CLASS << 8))
#define     FAN2              (0x0002 | (FAN_CLASS << 8))
#define     FAN3              (0x0003 | (FAN_CLASS << 8))
#define     CPU_FAN0          (0x0004 | (FAN_CLASS << 8))
#define     CPU_FAN1          (0x0005 | (FAN_CLASS << 8))
#define     CPU_FAN2          (0x0006 | (FAN_CLASS << 8))
#define     CPU_FAN3          (0x0007 | (FAN_CLASS << 8))
#define     PWR_SUPPLY_FAN0   (0x0008 | (FAN_CLASS << 8))
#define     PWR_SUPPLY_FAN1   (0x0009 | (FAN_CLASS << 8))
#define     PWR_SUPPLY_FAN2   (0x000A | (FAN_CLASS << 8))
#define     PWR_SUPPLY_FAN3   (0x000B | (FAN_CLASS << 8))
#define     SYSTEM_FAN0       (0x000C | (FAN_CLASS << 8))
#define     SYSTEM_FAN1       (0x000D | (FAN_CLASS << 8))
#define     DRIVE_FAN0        (0x000E | (FAN_CLASS << 8))
#define     DRIVE_FAN1        (0x000F | (FAN_CLASS << 8))

/* LED instance */
#define     LED0              (0x0000 | (LED_CLASS << 8))
#define     LED1              (0x0001 | (LED_CLASS << 8))
#define     LED2              (0x0002 | (LED_CLASS << 8))
#define     LED3              (0x0003 | (LED_CLASS << 8))
#define     MFT_ON_LED        (0x0004 | (LED_CLASS << 8))
#define     DRIVE_FAULT_LED   (0x0005 | (LED_CLASS << 8))
#define     FAN_FAULT_LED     (0x0006 | (LED_CLASS << 8))
#define     POWER_FAULT_LED   (0x0007 | (LED_CLASS << 8))
#define     FAULT_COOLING_LED (0x0008 | (LED_CLASS << 8))

#endif  /* CONFIG_H */


/* End of code */


/******************************************************************************
 *
 *   $Log: Config.h,v $
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
