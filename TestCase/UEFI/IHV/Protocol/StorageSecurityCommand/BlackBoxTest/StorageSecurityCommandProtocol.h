/*++
  The material contained herein is not a license, either        
  expressly or impliedly, to any intellectual property owned    
  or controlled by any of the authors or developers of this     
  material or to any contribution thereto. The material         
  contained herein is provided on an "AS IS" basis and, to the  
  maximum extent permitted by applicable law, this information  
  is provided AS IS AND WITH ALL FAULTS, and the authors and    
  developers of this material hereby disclaim all other         
  warranties and conditions, either express, implied or         
  statutory, including, but not limited to, any (if any)        
  implied warranties, duties or conditions of merchantability,  
  of fitness for a particular purpose, of accuracy or           
  completeness of responses, of results, of workmanlike         
  effort, of lack of viruses and of lack of negligence, all     
  with regard to this material and any contribution thereto.    
  Designers must not rely on the absence or characteristics of  
  any features or instructions marked "reserved" or             
  "undefined." The Unified EFI Forum, Inc. reserves any         
  features or instructions so marked for future definition and  
  shall have no responsibility whatsoever for conflicts or      
  incompatibilities arising from future changes to them. ALSO,  
  THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,  
  QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR            
  NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY        
  CONTRIBUTION THERETO.                                         
                                                                
  IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR  
  ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR     
  THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST      
  PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,        
  CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER   
  UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY  
  WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS       
  DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF     
  THE POSSIBILITY OF SUCH DAMAGES.                              
                                                                
  Copyright 2006, 2007, 2008, 2009, 2010, 2011 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2011, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  StorageSecurityCommandProtocol.h

Abstract:

  Storage Security Command I/O Protocol (define according to the EFI Spec 2.3.1 )

--*/


#ifndef __IHV_STORAGE_SECURITY_COMMAND_H__
#define __IHV_STORAGE_SECURITY_COMMAND_H__


//
// Global ID for the EFI Storage Security Command Protocol
//
#define EFI_STORAGE_SECURITY_COMMAND_PROTOCOL_GUID \
  { \
    0xC88B0B6D, 0x0DFC, 0x49A7, {0x9C, 0xB4, 0x49, 0x07, 0x4B, 0x4C, 0x3A, 0x78 } \
  }


typedef struct _EFI_STORAGE_SECURITY_COMMAND_PROTOCOL  EFI_STORAGE_SECURITY_COMMAND_PROTOCOL;

//
// Prototype for the EFI Storage Security Command protocol
//
/**
  Send a security protocol command to a device that receives data and/or the result
  of one or more commands sent by SendData.

  The ReceiveData function sends a security protocol command to the given MediaId.
  The security protocol command sent is defined by SecurityProtocolId and contains
  the security protocol specific data SecurityProtocolSpecificData. The function
  returns the data from the security protocol command in PayloadBuffer.

  For devices supporting the SCSI command set, the security protocol command is sent
  using the SECURITY PROTOCOL IN command defined in SPC-4.

  For devices supporting the ATA command set, the security protocol command is sent
  using one of the TRUSTED RECEIVE commands defined in ATA8-ACS if PayloadBufferSize
  is non-zero.

  If the PayloadBufferSize is zero, the security protocol command is sent using the
  Trusted Non-Data command defined in ATA8-ACS.

  If PayloadBufferSize is too small to store the available data from the security
  protocol command, the function shall copy PayloadBufferSize bytes into the
  PayloadBuffer and return EFI_WARN_BUFFER_TOO_SMALL.

  If PayloadBuffer or PayloadTransferSize is NULL and PayloadBufferSize is non-zero,
  the function shall return EFI_INVALID_PARAMETER.

  If the given MediaId does not support security protocol commands, the function shall
  return EFI_UNSUPPORTED. If there is no media in the device, the function returns
  EFI_NO_MEDIA. If the MediaId is not the ID for the current media in the device,
  the function returns EFI_MEDIA_CHANGED.

  If the security protocol fails to complete within the Timeout period, the function
  shall return EFI_TIMEOUT.

  If the security protocol command completes without an error, the function shall
  return EFI_SUCCESS. If the security protocol command completes with an error, the
  function shall return EFI_DEVICE_ERROR.

  @param  This                           Indicates a pointer to the calling context.
  @param  MediaId                      ID of the medium to receive data from.
  @param  Timeout                     The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                      if the time required to execute the receive data command
                                      is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize          Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command. The caller is responsible for having
                                       either implicit or explicit ownership of the buffer.
  @param  PayloadTransferSize          A pointer to a buffer to store the size in bytes of the
                                       data written to the payload data buffer.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The PayloadBufferSize was too small to store the available
                                       data from the device. The PayloadBuffer contains the truncated data.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer or PayloadTransferSize is NULL and
                                       PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_STORAGE_SECURITY_RECEIVE_DATA)(
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
  IN UINT32                                   MediaId,
  IN UINT64                                   Timeout,
  IN UINT8                                    SecurityProtocolId,
  IN UINT16                                   SecurityProtocolSpecificData,
  IN UINTN                                    PayloadBufferSize,
  OUT VOID                                    *PayloadBuffer,
  OUT UINTN                                   *PayloadTransferSize
  );

/**
  Send a security protocol command to a device.

  The SendData function sends a security protocol command containing the payload
  PayloadBuffer to the given MediaId. The security protocol command sent is
  defined by SecurityProtocolId and contains the security protocol specific data
  SecurityProtocolSpecificData. If the underlying protocol command requires a
  specific padding for the command payload, the SendData function shall add padding
  bytes to the command payload to satisfy the padding requirements.

  For devices supporting the SCSI command set, the security protocol command is sent
  using the SECURITY PROTOCOL OUT command defined in SPC-4.

  For devices supporting the ATA command set, the security protocol command is sent
  using one of the TRUSTED SEND commands defined in ATA8-ACS if PayloadBufferSize
  is non-zero. If the PayloadBufferSize is zero, the security protocol command is
  sent using the Trusted Non-Data command defined in ATA8-ACS.

  If PayloadBuffer is NULL and PayloadBufferSize is non-zero, the function shall
  return EFI_INVALID_PARAMETER.

  If the given MediaId does not support security protocol commands, the function
  shall return EFI_UNSUPPORTED. If there is no media in the device, the function
  returns EFI_NO_MEDIA. If the MediaId is not the ID for the current media in the
  device, the function returns EFI_MEDIA_CHANGED.

  If the security protocol fails to complete within the Timeout period, the function
  shall return EFI_TIMEOUT.

  If the security protocol command completes without an error, the function shall return
  EFI_SUCCESS. If the security protocol command completes with an error, the function
  shall return EFI_DEVICE_ERROR.

  @param  This                           Indicates a pointer to the calling context.
  @param  MediaId                     ID of the medium to receive data from.
  @param  Timeout                     The timeout, in 100ns units, to use for the execution
                                       of the security protocol command. A Timeout value of 0
                                       means that this function will wait indefinitely for the
                                       security protocol command to execute. If Timeout is greater
                                       than zero, then this function will return EFI_TIMEOUT
                                      if the time required to execute the receive data command
                                      is greater than Timeout.
  @param  SecurityProtocolId           The value of the "Security Protocol" parameter of
                                       the security protocol command to be sent.
  @param  SecurityProtocolSpecificData The value of the "Security Protocol Specific" parameter
                                       of the security protocol command to be sent.
  @param  PayloadBufferSize          Size in bytes of the payload data buffer.
  @param  PayloadBuffer                A pointer to a destination buffer to store the security
                                       protocol command specific payload data for the security
                                       protocol command.

  @retval EFI_SUCCESS                  The security protocol command completed successfully.
  @retval EFI_UNSUPPORTED              The given MediaId does not support security protocol commands.
  @retval EFI_DEVICE_ERROR             The security protocol command completed with an error.
  @retval EFI_NO_MEDIA                 There is no media in the device.
  @retval EFI_MEDIA_CHANGED            The MediaId is not for the current media.
  @retval EFI_INVALID_PARAMETER        The PayloadBuffer is NULL and PayloadBufferSize is non-zero.
  @retval EFI_TIMEOUT                  A timeout occurred while waiting for the security
                                       protocol command to execute.

**/
typedef
EFI_STATUS
(EFIAPI *EFI_STORAGE_SECURITY_SEND_DATA) (
  IN EFI_STORAGE_SECURITY_COMMAND_PROTOCOL    *This,
  IN UINT32                                   MediaId,
  IN UINT64                                   Timeout,
  IN UINT8                                    SecurityProtocolId,
  IN UINT16                                   SecurityProtocolSpecificData,
  IN UINTN                                    PayloadBufferSize,
  IN VOID                                     *PayloadBuffer
);

///
/// The EFI_STORAGE_SECURITY_COMMAND_PROTOCOL is used to send security protocol
/// commands to a mass storage device. Two types of security protocol commands
/// are supported. SendData sends a command with data to a device. ReceiveData
/// sends a command that receives data and/or the result of one or more commands
/// sent by SendData.
///
/// The security protocol command formats supported shall be based on the definition
/// of the SECURITY PROTOCOL IN and SECURITY PROTOCOL OUT commands defined in SPC-4.
/// If the device uses the SCSI command set, no translation is needed in the firmware
/// and the firmware can package the parameters into a SECURITY PROTOCOL IN or SECURITY
/// PROTOCOL OUT command and send the command to the device. If the device uses a
/// non-SCSI command set, the firmware shall map the command and data payload to the
/// corresponding command and payload format defined in the non-SCSI command set
/// (for example, TRUSTED RECEIVE and TRUSTED SEND in ATA8-ACS).
///
/// The firmware shall automatically add an EFI_STORAGE_SECURITY_COMMAND_PROTOCOL
/// for any storage devices detected during system boot that support SPC-4, ATA8-ACS
/// or their successors.
///
struct _EFI_STORAGE_SECURITY_COMMAND_PROTOCOL {
  EFI_STORAGE_SECURITY_RECEIVE_DATA ReceiveData;
  EFI_STORAGE_SECURITY_SEND_DATA    SendData;
};

extern EFI_GUID gEfiStorageSecurityCommandProtocolGuid;

//
// Both ATA8-ACS & SPC-4 Receive Security Command mandatorily supports
//    0x0000 Supported security protocol list
//    0x0001 Certificate data
// ATA8-ACS & SPC-4 return same supported security protocol list datastructure
//
typedef struct  {
  UINT8                            Reserved1[6];
  UINT8                            SupportedSecurityListLength[2];
  UINT8                            SupportedSecurityProtocol[1];
} SUPPORTED_SECURITY_PROTOCOLS_PARAMETER_DATA;

#endif

