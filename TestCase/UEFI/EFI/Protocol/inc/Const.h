/**
 * $Id: Const.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *  @file   Const.h
 *  @brief  This header file define the constant value of IPMI spec
 *
 *  @internal
 *  $RCSfile: Const.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/Const.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef _CONST_H
#define _CONST_H

/* define max IPMI NetFn pair */
#define MAX_NETFN_PAIR_NO   32

/* define IPMI comands support bitfield present in how many byte */
#define IPMICMD_BITFIELD_BYTES              16

/* define IPMI command sub-functions support bitfield present in how many byte */
#define IPMICMD_SUBFN_BITFIELD_BYTES        4

#define OSA_IANA_SIZE       3
/* Define Net Function Codes (IPMI 1.5 Table 5-1),
   Odd net function code represents response,
   Even net function code represents request */
#define    NF_CHS           0x00
#define    NF_BRG           0x02
#define    NF_SEN           0x04
#define    NF_APP           0x06
#define    NF_FWE           0x08
#define    NF_STO           0x0A
#define    NF_TRN           0x0C
#define    NF_SOL           0x34
/* FIXME: temperoly changed from 0x3E to 0x2E, the same as NF_DEL */
#define    NF_OSA           0x2E
#define 	 NF_OEM	   0x30
#define    NF_DEL		   0x2E


/* Define Completion Codes (IPMI 1.5 Table 5-2) */
#define     COMPCODE_NORMAL                     0x00
#define     COMPCODE_NODE_BUSY                  0xC0
#define     COMPCODE_CMD_UNKNOWN                0xC1
#define     COMPCODE_CMD_INVALID_FOR_LUN        0xC2
#define     COMPCODE_TIMEOUT                    0xC3
#define     COMPCODE_OUT_OF_SPACE               0xC4
#define     COMPCODE_RESERVATION_CANCELLED      0xC5
#define     COMPCODE_REQ_DATA_TRUNCATED         0xC6
#define     COMPCODE_REQ_DATA_LEN_INVALID       0xC7
#define     COMPCODE_REQ_LEN_LIMIT_EXCEEDED     0xC8
#define     COMPCODE_PARAM_OUT_OF_RANGE         0xC9
#define     COMPCODE_REQ_BYTE_CNT_ERR           0xCA
#define     COMPCODE_OBJ_ABSENT                 0xCB
#define     COMPCODE_REQ_FIELD_INVALID          0xCC
#define     COMPCODE_CMD_ILL_FOR_SEN            0xCD
#define     COMPCODE_RESP_UNAVAILABLE           0xCE
#define     COMPCODE_REQ_DUPLICATE              0xCF
#define     COMPCODE_SDR_REPO_IN_UPD            0xD0
#define     COMPCODE_FW_IN_UPD                  0xD1
#define     COMPCODE_INIT_IN_PROGRESS           0xD2
#define     COMPCODE_DEST_UNAVAILABLE           0xD3
#define     COMPCODE_WRONG_PRIV                 0xD4

/* FIREWALL-specific completion code: attemp to execute unavailable command */
#define     COMPCODE_CMD_UNAVAILABLE            0xD4
/* FIREWALL-specific completion code: attemp to execute unavailable command sub-function */
#define     COMPCODE_CMD_SUBFN_UNAVAILABLE      0xD6

#define     COMPCODE_CUR_NOT_SUPPORT            0xD5
#define     COMPCODE_UNSPECIFIED                0xFF

/* FRU-specific completion code: write-protected offset */
#define     COMPCODE_FRU_WR_PROTECTED_OFS       0x80

/* FIREWALL-specific completion code: attempt to enable an unsupported or disable un-configurable command */
#define     COMPCODE_FIREWALL_SET_ENABLE_UNCFGCMD       0x80
/* FIREWALL-specific completion code: attempt to enable an unsupported or disable un-configurable command */
#define     COMPCODE_FIREWALL_SET_ENABLE_UNCFGSUBFN     0x80




/** Define Channel Number (IPMI 1.5 Table 6-1), the number is used in IPMI
    message, not internal channel ID */
#define CHANNEL_NUM_PRIMARY_IPMB     0x00
#define CHANNEL_NUM_CHANNEL1         0x01
#define CHANNEL_NUM_CHANNEL2         0x02
#define CHANNEL_NUM_CHANNEL3         0x03
#define CHANNEL_NUM_CHANNEL4         0x04
#define CHANNEL_NUM_CHANNEL5         0x05
#define CHANNEL_NUM_CHANNEL6         0x06
#define CHANNEL_NUM_CHANNEL7         0x07
#define CHANNEL_NUM_RESERVED1        0x08
#define CHANNEL_NUM_RESERVED2        0x09
#define CHANNEL_NUM_RESERVED3        0x0A
#define CHANNEL_NUM_RESERVED4        0x0B
#define CHANNEL_NUM_RESERVED5        0x0C
#define CHANNEL_NUM_RESERVED6        0x0D
#define CHANNEL_NUM_PRESENT          0x0E
#define CHANNEL_NUM_SYSTEM           0x0F
#define CHANNEL_NUM_INVALID          0xFF


/* Define Channel Protocol Type (IPMI 1.5 Table 6-2) */
#define CHANNEL_PROTOCOL_TYPE_IPMB          0x01
#define CHANNEL_PROTOCOL_TYPE_ICMB          0x02
#define CHANNEL_PROTOCOL_TYPE_SMBUS         0x04
#define CHANNEL_PROTOCOL_TYPE_KCS           0x05
#define CHANNEL_PROTOCOL_TYPE_SMIC          0x06
#define CHANNEL_PROTOCOL_TYPE_BT10          0x07
#define CHANNEL_PROTOCOL_TYPE_BT15          0x08
#define CHANNEL_PROTOCOL_TYPE_TMODE         0x09
#define CHANNEL_PROTOCOL_TYPE_OEM1          0x1C
#define CHANNEL_PROTOCOL_TYPE_OEM2          0x1E
#define CHANNEL_PROTOCOL_TYPE_OEM3          0x1F
#define CHANNEL_PROTOCOL_TYPE_INVALID       0xFF


/* Define Channel Medium Type (IPMI 1.5 Table 6-3) */
#define CHANNEL_MEDIUM_TYPE_IPMB            0x01
#define CHANNEL_MEDIUM_TYPE_ICMB10          0x01
#define CHANNEL_MEDIUM_TYPE_ICMB09          0x03
#define CHANNEL_MEDIUM_TYPE_LAN8023         0x04
#define CHANNEL_MEDIUM_TYPE_RS232           0x05
#define CHANNEL_MEDIUM_TYPE_MISC_LAN        0x06
#define CHANNEL_MEDIUM_TYPE_PCISMBUS        0x07
#define CHANNEL_MEDIUM_TYPE_SMBUS10         0x08
#define CHANNEL_MEDIUM_TYPE_SMBUS20         0x09
#define CHANNEL_MEDIUM_TYPE_USB1            0x0A
#define CHANNEL_MEDIUM_TYPE_USB2            0x0B
#define CHANNEL_MEDIUM_TYPE_SYSTEM          0x0C
#define CHANNEL_MEDIUM_TYPE_INVALID         0xFF

/* define channel access mode */
#define CHANNELMODE_DISABLED                0x00
#define CHANNELMODE_PRE_BOOT                0x01
#define CHANNELMODE_ALWAYS                  0x02
#define CHANNELMODE_SHARED                  0x03

/* Define IPMB LUN (IPMI 1.5 Table 7-1) */
#define LUN_BMC     0x00
#define LUN_OEM1    0x01
#define LUN_SMS     0x02
#define LUN_OEM2    0x03


/* Define IPMI Command ID (IPMI 1.5 Table G-1) */
/* Global Commands */
#define IPMICMD_GET_DEVICE_ID                   0x01
#define IPMICMD_BROADCAST_GET_DEVICE_ID         0x01
#define IPMICMD_COLD_RESET                      0x02
#define IPMICMD_WARM_RESET                      0x03
#define IPMICMD_GET_SELF_TEST_RESULT            0x04
#define IPMICMD_MANUFACTURING_TEST_ON           0x05
#define IPMICMD_SET_ACPI_POWER_STATE            0x06
#define IPMICMD_GET_ACPI_POWER_STATE            0x07
#define IPMICMD_GET_DEVICE_GUID                 0x08

/* Firmware Firewall Configuration Commands */
#define IPMICMD_GET_NETFN_SUPPORT               0x09
#define IPMICMD_GET_COMMAND_SUPPORT             0x0A
#define IPMICMD_GET_COMMAND_SUBFN_SUPPORT       0x0B
#define IPMICMD_GET_CONFIGURABLE_COMMANDS       0x0C
#define IPMICMD_GET_CONFIGURABLE_COMMAND_SUBFN  0x0D

/* BMC Watchdog Timer Commands*/
#define IPMICMD_RESET_WATCHDOG_TIMER            0x22
#define IPMICMD_SET_WATCHDOG_TIMER              0x24
#define IPMICMD_GET_WATCHDOG_TIMER              0x25

/* BMC Device and Messaging Commands */
#define IPMICMD_SET_BMC_GLOBAL_ENABLES          0x2E
#define IPMICMD_GET_BMC_GLOBAL_ENABLES          0x2F
#define IPMICMD_CLEAR_MSG_FLGS                  0x30
#define IPMICMD_GET_MSG_FLAGS                   0x31
#define IPMICMD_ENABLE_MSG_CHANNEL_RECEIVE      0x32
#define IPMICMD_GET_MSG                         0x33
#define IPMICMD_SEND_MSG                        0x34
#define IPMICMD_READ_EVENT_MSG_BUF              0x35
#define IPMICMD_GET_BT_INTERFACE_CAP            0x36
#define IPMICMD_GET_SYS_GUID                    0x37
#define IPMICMD_GET_CHANNEL_AUTH_CAP            0x38
#define IPMICMD_GET_SESSION_CHALLENGE           0x39
#define IPMICMD_ACTIVATE_SESSION                0x3A
#define IPMICMD_SET_SESSION_PRIVILEGE_LEVEL     0x3B
#define IPMICMD_CLOSE_SESSION                   0x3C
#define IPMICMD_GET_SESSION_INFO                0x3D
#define IPMICMD_GET_AUTHCODE                    0x3F
#define IPMICMD_SET_CHANNEL_ACCESS              0x40
#define IPMICMD_GET_CHANNEL_ACCESS              0x41
#define IPMICMD_GET_CHANNEL_INFO_CMD            0x42
#define IPMICMD_SET_USER_ACCESS_CMD             0x43
#define IPMICMD_GET_USER_ACCESS_CMD             0x44
#define IPMICMD_SET_USER_NAME                   0x45
#define IPMICMD_GET_USER_NAME_CMD               0x46
#define IPMICMD_SET_USER_PASSWORD_CMD           0x47
#define IPMICMD_ACTIVATE_PAYLOAD                0x48
#define IPMICMD_DEACTIVATE_PAYLOAD              0x49
#define IPMICMD_GET_PAYLOAD_ACT_STATUS          0x4A
#define IPMICMD_GET_PAYLOAD_INS_INFO            0x4B
#define IPMICMD_SET_USER_PAYLOAD_ACCESS         0x4C
#define IPMICMD_GET_USER_PAYLOAD_ACCESS         0x4D
#define IPMICMD_GET_CHANNEL_PAYLOAD_SUPPORT     0x4E
#define IPMICMD_GET_CHANNEL_PAYLOAD_VER         0x4F
#define IPMICMD_GET_CHANNEL_OEM_PAYLOAD_INFO    0x50
#define IPMICMD_MASTER_WRITE_READ               0x52
#define IPMICMD_GET_CHANNEL_CIPHER_SUITES       0x54
#define IPMICMD_SUSPEND_RESUME_PAYLOAD_ENCRYPT  0x55
#define IPMICMD_SET_CHANNEL_SECURITY_KEYS       0x56

/* Firmware Firewall Configuration Commands */
#define IPMICMD_SET_COMMAND_ENABLES             0x60
#define IPMICMD_GET_COMMAND_ENABLES             0x61
#define IPMICMD_SET_COMMAND_SUBFN_ENABLES       0x62
#define IPMICMD_GET_COMMAND_SUBFN_ENABLES       0x63
#define IPMICMD_GET_OEM_NETFN_IANA_SUPPORT      0x64

/* Chassis Device Commands */
#define IPMICMD_GET_CHASSIS_CAP                 0x00
#define IPMICMD_GET_CHASSIS_STATUS              0x01
#define IPMICMD_CHASSIS_CONTROL                 0x02
#define IPMICMD_CHASSIS_RESET                   0x03
#define IPMICMD_CHASSIS_IDENTIFY                0x04
#define IPMICMD_SET_CHASSIS_CAP                 0x05
#define IPMICMD_SET_POWER_RESTORE_POLICY        0x06
#define IPMICMD_SYSTEM_RESTART_CAUSE            0x07
#define IPMICMD_SET_SYSTEM_BOOT_OPTION          0x08
#define IPMICMD_GET_SYSTEM_BOOT_OPTION          0x09
#define IPMICMD_SET_FRONT_PANEL_ENABLE          0x0A
#define IPMICMD_SET_PWR_CYCLE_INTERVAL          0x0B
#define IPMICMD_GET_POH_COUNTER                 0x0F

/* Event Commands */
#define IPMICMD_SET_EVENT_RECEIVER              0x00
#define IPMICMD_GET_EVENT_RECEIVER              0x01
#define IPMICMD_PLATFORM_EVENT                  0x02

/* PEF and Alerting Commands */
#define IPMICMD_GET_PEF_CAP                     0x10
#define IPMICMD_ARM_PEF_POSTPONE_TIMER          0x11
#define IPMICMD_SET_PEF_CONFIG_PARAM            0x12
#define IPMICMD_GET_PEF_CONFIG_PARAM            0x13
#define IPMICMD_SET_LAST_PROCESSED_EVENT_ID     0x14
#define IPMICMD_GET_LAST_PROCESSED_EVENT_ID     0x15
#define IPMICMD_ALERT_IMMEDIATE                 0x16
#define IPMICMD_PET_ACK                         0x17

/* Sensor Device Commands */
#define IPMICMD_GET_DEVICE_SDR_INFO             0x20
#define IPMICMD_GET_DEVICE_SDR                  0x21
#define IPMICMD_RESERVE_DEVICE_SDR_REPOSITORY   0x22
#define IPMICMD_GET_SENSOR_READING_FACTORS      0x23
#define IPMICMD_SET_SENSOR_HYSTERESIS           0x24
#define IPMICMD_GET_SENSOR_HYSTERESIS           0x25
#define IPMICMD_SET_SENSOR_THRESHOLD            0x26
#define IPMICMD_GET_SENSOR_THRESHOLD            0x27
#define IPMICMD_SET_SENSOR_EVENT_ENABLE         0x28
#define IPMICMD_GET_SENSOR_EVENT_ENABLE         0x29
#define IPMICMD_REARM_SENSOR_EVENTS             0x2A
#define IPMICMD_GET_SENSOR_EVENT_STATUS         0x2B
#define IPMICMD_GET_SENSOR_READING              0x2D
#define IPMICMD_SET_SENSOR_TYPE                 0x2E
#define IPMICMD_GET_SENSOR_TYPE                 0x2F

/* FRU Device Commands */
#define IPMICMD_GET_FRU_INVENTORY_AREA_INFO     0x10
#define IPMICMD_READ_FRU_DATA                   0x11
#define IPMICMD_WRITE_FRU_DATA                  0x12

/* SDR Device Commands */
#define IPMICMD_GET_SDR_REPOSITORY_INFO         0x20
#define IPMICMD_GET_SDR_REPOSITORY_ALLOC_INFO   0x21
#define IPMICMD_RESERVE_SDR_REPOSITORY          0x22
#define IPMICMD_GET_SDR                         0x23
#define IPMICMD_ADD_SDR                         0x24
#define IPMICMD_PARTIAL_ADD_SDR                 0x25
#define IPMICMD_DEL_SDR                         0x26
#define IPMICMD_CLEAR_SDR_REPOSITORY            0x27
#define IPMICMD_GET_SDR_REPOSITORY_TIME         0x28
#define IPMICMD_SET_SDR_REPOSITORY_TIME         0x29
#define IPMICMD_ENTER_SDR_REPOSITORY_UPDATEMODE 0x2A
#define IPMICMD_EXIT_SDR_REPOSITORY_UPDATEMODE  0x2B
#define IPMICMD_RUN_INIT_AGENT                  0x2C

/* SEL Device Commands */
#define IPMICMD_GET_SEL_INFO                    0x40
#define IPMICMD_GET_SEL_ALLOC_INFO              0x41
#define IPMICMD_RESERVE_SEL                     0x42
#define IPMICMD_GET_SEL_ENTRY                   0x43
#define IPMICMD_ADD_SEL_ENTRY                   0x44
#define IPMICMD_PARTIAL_ADD_SEL_ENTRY           0x45
#define IPMICMD_DEL_SEL_ENTRY                   0x46
#define IPMICMD_CLEAR_SEL                       0x47
#define IPMICMD_GET_SEL_TIME                    0x48
#define IPMICMD_SET_SEL_TIME                    0x49
#define IPMICMD_GET_AUX_LOG_STATUS              0x5A
#define IPMICMD_SET_AUX_LOG_STATUS              0x5B

/* LAN Device Commands */
#define IPMICMD_SET_LAN_CONFIG_PARAM            0x01
#define IPMICMD_GET_LAN_CONFIG_PARAM            0x02
#define IPMICMD_SUSPEND_BMC_ARPS                0x03
#define IPMICMD_GET_IPUDPRMCP_STATISTICS        0x04

/* Serial/Modem Device Commands */
#define IPMICMD_SET_MODEM_CONFIG                		0x10
#define IPMICMD_GET_MODEM_CONFIG                		0x11
#define IPMICMD_SET_MODEM_MUX                   			0x12
#define IPMICMD_GET_TAP_RSP_CODES               		0x13
#define IPMICMD_SET_PPPUDP_PROXY_TRANS_DATA     	0x14
#define IPMICMD_GET_PPPUDP_PROXY_TRANS_DATA     	0x15
#define IPMICMD_SEND_PPPUDP_PROXY_PACKET        	0x16
#define IPMICMD_GET_PPPUDP_PROXY_RECV_DATA      	0x17
#define IPMICMD_MODEM_CONNECTION_ACTIVE         	0x18
#define IPMICMD_CALLBACK                        				0x19
#define IPMICMD_SET_USR_CALLBACK_OPTIONS        	0x1A
#define IPMICMD_GET_USR_CALLBACK_OPTIONS        	0x1B
/* IPMI 2.0 Errata Proposed Commands */
#define IPMICMD_SET_SERIAL_ROUTING_MUX			0x1C
#define IPMICMD_SOL_ACTIVATING					0x20
/* SOL 1.0 Commands */
#define IPMICMD_SOL10_SET_SOL_CONFIGURATION     	0x21
#define IPMICMD_SOL10_GET_SOL_CONFIGURATION     	0x22
/* IPMI 2.0 Errata Proposed Commands */
#define IPMCMD_FORWARDED_CMD_CMDS				0x30
#define IPMICMD_SET_FORWARDED_CMDS				0x31
#define IPMICMD_GET_FORWARDED_CMDS				0x32
#define IPMICMD_ENABLE_FORWARD_CMDS				0x33
#define IPMICMD_SET_SENSOR_READING            		0x30


/* Bridge Management Commands (ICMB) */
#define IPMICMD_GET_BRIDGE_STATE                0x00
#define IPMICMD_SET_BRIDGE_STATE                0x01
#define IPMICMD_GET_ICMB_ADDRESS                0x02
#define IPMICMD_SET_ICMB_ADDRESS                0x03
#define IPMICMD_SET_BRIDGE_PROXY_ADDRESS        0x04
#define IPMICMD_GET_BRIDGE_STATISTICS           0x05
#define IPMICMD_GET_ICMB_CAP                    0x06
#define IPMICMD_CLEAR_BRIDGE_STATISTICS         0x08
#define IPMICMD_GET_BRIDGE_PROXY_ADDRESS        0x09
#define IPMICMD_GET_ICMB_CONNECTOR_INFO         0x0A
#define IPMICMD_GET_ICMB_CONNECTOR_ID           0x0B
#define IPMICMD_SEND_ICMB_CONNECTION_ID         0x0C

/* Discovery Commands (ICMB) */
#define IPMICMD_PREPARE_FOR_DISCOVERY           0x10
#define IPMICMD_GET_ADDRESSES                   0x11
#define IPMICMD_SET_DISCOVERED                  0x12
#define IPMICMD_GET_CHASSIS_DEVICE_ID           0x13
#define IPMICMD_SET_CHASSIS_DEVICE_ID           0x14

/* Bridging Commands (ICMB) */
#define IPMICMD_BRIDGE_REQUEST                  0x20
#define IPMICMD_BRIDGE_MESSAGE                  0x21

/* Event Commands (ICMB) */
#define IPMICMD_GET_EVENT_COUNT                 0x30
#define IPMICMD_SET_EVENT_DESTINATION           0x31
#define IPMICMD_SET_EVENT_RECEPTION_STATE       0x32
#define IPMICMD_SEND_ICMB_EVENT_MSG             0x33
#define IPMICMD_GET_EVENT_DESTINAION            0x34
#define IPMICMD_GET_EVENT_RECEPTION_STATE       0x35

/* OEM Commands for Bridge NetFn */

/* Other Bridge Commands */
#define IPMICMD_ERROR_REPORT                    0xFF

/* OSA commands */
#define IPMICMD_OSA_DEVICE_LED_ON_OFF           0x01
#define IPMICMD_OSA_DEVICE_LED_FLASH            0x02
#define IPMICMD_OSA_DEVICE_SET_FAN_SPEED        0x03
#define IPMICMD_OSA_TERMINAL_SYS                0x07
#define IPMICMD_OSA_TERMINAL_SYS_DRAC           0x08
#define IPMICMD_OSA_GET_MEM_DATA                0x0B
#define IPMICMD_OSA_SET_MEM_DATA                0x0C
#define IPMICMD_OSA_RESET_TO_DEFAULT            0x0D
#define IPMICMD_OSA_OEM_CMD                     0xCC

/* IPMI 2.0 Errata Proposed Commands */
#define IPMICMD_SET_SYSTEM_INFO	 		0x58
#define IPMICMD_GET_SYSTEM_INFO	 		0x59

/* DELL Proposed Extended IPMI Commands To Support RAC */
#define IPMICMD_DELL_RESV_EXT_CONF    0x01
#define IPMICMD_DELL_GET_EXT_CONF      0x02
#define IPMICMD_DELL_SET_EXT_CONF      0x03
#define IPMICMD_DELL_POST_EVENT          0x04

/*FIRMWARE Commands */
#define IPMICMD_OSAOEM_GET_FLASH_UPDATE_INFO    0x01
#define IPMICMD_OSAOEM_ENTER_FLASH_UPDATE_MODE  0x02
#define IPMICMD_OSAOEM_FLASH_READ               0x03
#define IPMICMD_OSAOEM_FLASH_WRITE              0x04
#define IPMICMD_OSAOEM_FLASH_WRITE_VERIFY       0x05


/* Define IPMI privilege levels (IPMI 1.5 Table 6-5) */
#define PRIV_NON    0x00
#define PRIV_CBK    0x01    /**< Callback Priviliege */
#define PRIV_USR    0x02    /**< User Privilege */
#define PRIV_OPR    0x03    /**< Operator Priviliege */
#define PRIV_ADM    0x04    /**< Administrator Priviliege */
#define PRIV_OEM    0x05    /**< OEM Specified Priviliege */


/* Define IPMI Authentication Type (IPMI 1.5 Table 18-6) */
#define AUTH_TYPE_NON   0x00    /**< None Auth */
#define AUTH_TYPE_MD2   0x01    /**< MD2 */
#define AUTH_TYPE_MD5   0x02    /**< MD5 */
#define AUTH_TYPE_PWD   0x04    /**< Straight Password / Key */
#define AUTH_TYPE_OEM   0x05    /**< OEM Proprietary */


/* ACPI power state masks */
#define ACPI_STATE_S0               0x01
#define ACPI_STATE_S1               0x02
#define ACPI_STATE_S2               0x04
#define ACPI_STATE_S3               0x08
#define ACPI_STATE_S4               0x10
#define ACPI_STATE_S5               0x20
#define ACPI_STATE_ALL              0xFF
#define ACPI_STATE_NONE             0x00


#endif /* _CONST_H */

/* End of code */


/******************************************************************************
 *
 *   $Log: Const.h,v $
 *   Revision 1.1.1.1  2005/08/23 08:23:04  benwang
 *   Project: sanmarco
 *   BugID: none
 *   Comment: Dell's Release Tree
 *
 *   Revision 1.2  2004/12/14 08:44:40  tchang
 *   Merge RMCP+ code
 *
 *   Revision 1.6  2004/04/06 06:07:33  albert
 *   [Change Note ID]:10112
 *
 *   [Internal APIs]
 *
 *   [External APIs]
 *
 *   [Brief]
 *   Implement SOL 2.0 over RMCP.
 *
 *   Revision 1.5  2004/03/01 16:50:29  dailybuild
 *   [Change Note ID]:1028
 *   [Brief]
 *   1.Update code Header
 *
 *   Revision 1.4  2004/02/28 18:16:13  dailybuild
 *   [Change Note ID]:1028
 *  [Brief]
 *  1. update the year from 2001-2003 to 2001-2004 for legal reason
 *  2. remove Taiwan phone contact from the header
 *
 *   Revision 1.3  2003/11/27 11:10:23  jerry
 *   Fix Doxygen and Coding standard error
 *
 *   Revision 1.2  2003/10/07 13:22:34  jerry
 *   merged from g03
 *
 *   Revision 1.1.1.1  2003/09/29 19:47:46  pchiang
 *
 *
 *   Revision 1.13  2003/09/18 08:12:58  tchang
 *   add 2 chassis commands definiton according to IPMI1.5 errata
 *
 *   Revision 1.12  2003/07/30 09:33:09  lchang
 *   Move channel access mode const to Const.h
 *
 *   Revision 1.11  2003/07/23 11:15:43  jlee
 *   Add OEM command to write data into the assigned memory address
 *
 *   Revision 1.10  2003/07/21 09:24:53  jlee
 *   Add OEM command CmdReadMemory to read back memory data
 *
 *   Revision 1.9  2003/07/18 09:49:44  albert
 *   Remove id constants of old OSA OEM commands .
 *   Add id constant of a integrated OSA OEM command
 *
 *   Revision 1.8  2003/07/10 10:51:02  albert
 *   add get BMC SA
 *
 *   Revision 1.7  2003/07/01 10:28:28  albert
 *   Add 'Set BMC slave address' OEM command.
 *
 *   Revision 1.6  2003/06/26 08:55:22  tchang
 *   add reset PS function
 *
 *   Revision 1.5  2003/05/23 08:02:41  albert
 *   Add ISOL
 *
 *   Revision 1.4  2003/04/21 06:20:55  albert
 *   Add IPMICMD_OSA_TERMINAL_SYS define
 *   for serial interface terminal mode ASCII-text commands.
 *
 *   Revision 1.3  2003/03/26 13:28:59  owu
 *   Migrating all freshest g02 files to g03.
 *
 *   Revision 1.17  2003/03/03 09:48:26  dchung
 *   reorder the OSA command number(just keeps number 0x03, and not modify
 *   the following numbers)
 *
 *   Revision 1.16  2003/02/27 13:50:51  dchung
 *   new files from Taiwan CVS server
 *
 *   Revision 1.3  2003/02/27 09:26:23  oleg
 *   Added const definitions of System ACPI Power States for use in
 *   the future enhancement of FW features.
 *
 *   Revision 1.14  2003/02/13 03:29:48  dchung
 *   remove IPMICMD_OSA_DEVICE_READ_AD and resort IPMI_OSA_*
 *
 *   Revision 1.13  2002/12/26 06:48:44  lchang
 *   provide get channel protocol and medium type API
 *
 *   Revision 1.12  2002/12/02 04:08:10  lchang
 *   Add OSA command to get sys GUID
 *
 *   Revision 1.11  2002/11/13 14:38:02  tchang
 *   remove SDR specific command return code to SDRCmds.h
 *
 *   Revision 1.10  2002/11/13 14:07:38  tchang
 *   Add one SDR specific command return code
 *    (COMPCODE_ADD_SDR_LENGTH_ERROR)
 *
 *   Revision 1.9  2002/11/04 06:49:47  tchang
 *   modify OSA Netfn from 0x2E to 0x3E
 *
 *   Revision 1.8  2002/11/01 05:42:17  lchang
 *   add comp code 0xD5
 *
 *   Revision 1.7  2002/10/11 06:19:14  tchang
 *   Add OSA command - Get FW version
 *
 *   Revision 1.6  2002/08/30 11:07:38  dchung
 *   After code review, build OK for National.
 *
 *   Revision 1.5  2002/08/15 20:38:32  lchang
 *   Add Auth Type Definition
 *
 *   Revision 1.4  2002/08/08 02:02:33  jchiang
 *   add the firmware command define.
 *
 *   Revision 1.3  2002/07/24 23:19:22  lchang
 *   Add Set Fan Speed OEM command
 *
 *   Revision 1.2  2002/07/17 19:57:54  lchang
 *   ended with a new line
 *
 *   Revision 1.1  2002/07/05 03:37:17  cwang
 *   OSA TW office create for temperary purpose @ 0705
 *
 *****************************************************************************/
