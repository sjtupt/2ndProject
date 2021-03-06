/**
 * $Id: SysEventRecord.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $
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
 *\n
 *                  US phone: 408.436.6333
 *\n
 *                  email: ip_manager@osatechnologies.com
 *\n
 *
 *        Copyright &copy; 2001-2005 OSA Technologies. All Rights Reserved.
 *</em> </center>
 *----------------------------------------------------------------------------\n
 *  MODULES     ipmi\n
 *----------------------------------------------------------------------------\n
 *  @file   SysEventRecord.h
 *  @brief  typedef's, constants, and function prototype's which is global to
 *          other modules
 *
 *  @internal
 *  $RCSfile: SysEventRecord.h,v $
 *  $Revision: 1.1.1.1 $
 *  $Date: 2005/08/23 08:23:04 $
 *  $Author: benwang $
 *  $Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/SysEventRecord.h,v $
 *  $Name:  $
 *----------------------------------------------------------------------------*/

#ifndef SYSEVENTRECORD_H
#define SYSEVENTRECORD_H

#include "../../../cdf/ostd/inc/type.h"

/******************************************************************************
*   STRUCT  :   sSysEventRecord
******************************************************************************/
/**
 *  @brief  Format of a System Event Log (SEL) Record. IPMI v1.5.  Typedeffing
 *          the format of a SEL Event Record. This is the stored information
 *          from Event Messages, as described in Section 23.4, Event Request
 *          Message Fields in IPMI Spec. v1.5.
 *
 *****************************************************************************/
typedef struct
{
    /** Record ID. Used for SEL Record access. The Record ID values 0000h
        and FFFFh have special meaning in the Event Access commands and
        must not be used as Record ID values for stored SEL Event Records. */
    UINT8 au8ID[2];

    /** <ul>
        Record Type.
        <li> [7:0] - Record Type:
                     <ul>
                     <li> 02h = system event record
                     <li> C0h-DFh = OEM timestamped, bytes 8-16 OEM defined.
                     <li> E0h-FFh = OEM non-timestamped, bytes 4-16 OEM
                          defined.
                     </ul>
        </ul> */
    UINT8 u8Type;

    /** Time when event was logged. LS byte first. */
    UINT8 au8Timestamp[4];

    /** <ul>
        RqSA & LUN if event was generated from IPMB. Software ID if event
        was generated from system software. <br>
        Byte 1 <br>
        <li> [7:1] - 7-bit I2C . Slave Address, or 7-bit system
                     software ID
        <li> [0] 0b = ID is IPMB Slave Address
                 1b = system software ID

        Byte 2 <br>
        <li> [7:4] - Channel number. Channel that event message was
                     received over. 0h if the event message was
                     received via the system interface, primary IPMB,
                     or internally generated by the BMC. (New for
                     IPMI v1.5. These bits were reserved in IPMI
                     v1.0)
        <li> [3:2] - reserved. Write as 00b.
        <li> [1:0] - IPMB device LUN if byte 1 holds Slave Address.
                     00b otherwise.
        </ul> */
    UINT8 au8GeneratorID[2];

    /** Event Message format version: <br>
        04h = events in IPMI v1.5 <br>
        03h = IPMI v1.0 Event Messages. <br>
        Note: the BMC must accept Platform Event request
        messages that are in IPMI v1.0 format (EvMRev = 03h)
        and log them as IPMI v1.5 Records by setting the EvMRev
        field to 04h and setting the Channel Number in the
        Generator ID field appropriately for the channel that
        the event was received from. */
    UINT8 u8EvMRev;

    /** Sensor Type Code for sensor that generated the event. */
    UINT8 u8SensorType;

    /** Number of sensor that generated the event. */
    UINT8 u8SensorNum;

    /** Event Dir | Event Type <br>
        Event Dir: <br>
        [7 ] - 0b = Assertion event; 1b = Deassertion event. <br><br>

        Event Type: <br>
        Type of trigger for the event, e.g. critical threshold going high,
        state asserted, etc. Also indicates class of the event. E.g. discrete,
        threshold, or OEM. The Event Type field is encoded using the
        Event/Reading Type Code. See IPMI Spec. v1.5, Section 36.1,
        "Event/Reading Type Codes".<br>
        [6:0] - Event Type Code */
    UINT8 u8EventAttr;

    /** See IPMI Spec. v1.5, Table 23-6, "Event Request Message" Event Data
        Field Contents. */
    UINT8 au8EventData[3];
} sSysEventRecord;

#endif  /* SYSEVENTRECORD_H */

/* End of code */
