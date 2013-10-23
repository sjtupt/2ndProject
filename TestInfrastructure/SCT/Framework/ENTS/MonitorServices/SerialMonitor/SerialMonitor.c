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
                                                                
  Copyright 2006 - 2012 Unified EFI, Inc. All  
  Rights Reserved, subject to all existing rights in all        
  matters included within this Test Suite, to which United      
  EFI, Inc. makes no claim of right.                            
                                                                
  Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR>   
   
--*/
/*++

Module Name:

  SerialMonitor.c

Abstract:

  Serial Port Monitor services implementations.

--*/

#include "Efi.h"
#include "EntsLib.h"
#include "EfiDriverLib.h"
#include "SerialMonitor.h"

#define SIGNAL_TAG_LENGTH 2
#define HANDLE_IN_USE     2

//
// global variables
//
CHAR8                     gSerialSignalStart[SIGNAL_TAG_LENGTH] = { 0x55, 0x55 };
CHAR8                     gSerialSignalEnd[SIGNAL_TAG_LENGTH]   = { 0x57, 0x57 };

#define SERIAL_BUFFER_OUT_MAX 4096
CHAR8                     SerialBufferOut[SERIAL_BUFFER_OUT_MAX];

EFI_ENTS_MONITOR_PROTOCOL *gSerialMonitorInterface = NULL;

//
// Local Functions Declaration
//
BOOLEAN
IsComdBegin (
  IN CHAR8                          *Buffer,
  IN UINTN                          Size
  );

BOOLEAN
IsComdEnd (
  IN CHAR8                          *Buffer,
  IN UINTN                          Size
  );

EFI_STATUS
GetByte (
  IN EFI_SERIAL_IO_PROTOCOL   *SerialIo,
  IN OUT UINTN                *BufferSize,
  OUT    CHAR8                *Buffer
  );

EFI_STATUS
SerialMonitorUnload (
  IN EFI_HANDLE                ImageHandle
  );

EFI_DRIVER_ENTRY_POINT (SerialMonitorEntryPoint)
//
// External functions implementations
//
EFI_STATUS
SerialMonitorEntryPoint (
  IN EFI_HANDLE                ImageHandle,
  IN EFI_SYSTEM_TABLE          *SystemTable
  )
/*++

Routine Description:

  Entry point of SerialMonitor.
  
Arguments:

  ImageHandle           - The image handle.
  SystemTable           - The system table.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS                Status;
  EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;

  EfiInitializeDriverLib (ImageHandle, SystemTable);
  EfiInitializeEntsLib (ImageHandle, SystemTable);

  gBS->HandleProtocol (
        ImageHandle,
        &gEfiLoadedImageProtocolGuid,
        (VOID *) &LoadedImage
        );

  LoadedImage->Unload = SerialMonitorUnload;

  Status = gBS->AllocatePool (
                  EfiBootServicesData,
                  sizeof (EFI_ENTS_MONITOR_PROTOCOL),
                  (VOID **)&gSerialMonitorInterface
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  gSerialMonitorInterface->MonitorName      = ENTS_SERIAL_MONITOR_NAME;
  gSerialMonitorInterface->MonitorIo        = NULL;
  gSerialMonitorInterface->InitMonitor      = InitSerial;
  gSerialMonitorInterface->ResetMonitor     = ResetSerial;
  gSerialMonitorInterface->MonitorListener  = SerialListener;
  gSerialMonitorInterface->MonitorSender    = SerialSender;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gEfiEntsMonitorProtocolGuid,
                  gSerialMonitorInterface,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    goto Error;
  }

  return EFI_SUCCESS;

Error:
  if (gSerialMonitorInterface != NULL) {
    gBS->FreePool (gSerialMonitorInterface);
  }

  return Status;
}

EFI_STATUS
SerialMonitorUnload (
  IN EFI_HANDLE                ImageHandle
  )
/*++

Routine Description:

  Unload SerialMonitor.

Arguments:

  ImageHandle           - The image handle.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS  Status;

  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gEfiEntsMonitorProtocolGuid,
                  gSerialMonitorInterface,
                  NULL
                  );

  if (gSerialMonitorInterface != NULL) {
    gBS->FreePool (gSerialMonitorInterface);
  }

  return Status;
}

EFI_STATUS
InitSerial (
  IN EFI_ENTS_MONITOR_PROTOCOL     *This
  )
/*++

Routine Description:

  Initialize Serial port.

Arguments:

  This  - Pointer to the EFI_ENTS_MONITOR_PROTOCOL instance.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS              Status;
  EFI_SERIAL_IO_PROTOCOL  *SerialIo;
  UINTN                   NoHandle;
  EFI_HANDLE              *HandleBuffer;

  SerialIo = NULL;

  Status = EntsLibLocateHandle (
            ByProtocol,
            &gEfiSerialIoProtocolGuid,
            NULL,
            &NoHandle,
            &HandleBuffer
            );

  if (NoHandle >= HANDLE_IN_USE) {
    gBS->HandleProtocol (
          HandleBuffer[HANDLE_IN_USE - 1],
          &gEfiSerialIoProtocolGuid,
          &SerialIo
          );
  } else {
    Status = gBS->LocateProtocol (
                    &gEfiSerialIoProtocolGuid,
                    NULL,
                    &SerialIo
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  Status = SerialIo->SetAttributes (
                      SerialIo,
                      9600,
                      0,  // ReceiveFifoDepth
                      0,  // Timeout
                      0,  // Parity
                      8,  // DataBits
                      1   // StopBits
                      );

  if (EFI_ERROR (Status)) {
    EntsPrint (L"Set Serial attribute 9600 - %r\n", Status);
    return Status;
  }

  SerialIo->Reset (SerialIo);

  This->MonitorIo = (VOID *) SerialIo;

  EntsFreePool(HandleBuffer);

  return EFI_SUCCESS;
}

EFI_STATUS
ResetSerial (
  IN EFI_ENTS_MONITOR_PROTOCOL     *This
  )
/*++

Routine Description:

  Reset Serial port.

Arguments:

  This  - Pointer to the EFI_ENTS_MONITOR_PROTOCOL instance.

Returns:

  EFI_SUCCESS - Operation succeeded.
  Others      - Some failure happened.

--*/
{
  EFI_STATUS              Status;
  EFI_SERIAL_IO_PROTOCOL  *SerialIo;

  SerialIo = This->MonitorIo;

  Status = SerialIo->SetAttributes (
                      SerialIo,
                      115200,
                      0,  // ReceiveFifoDepth
                      0,  // Timeout
                      0,  // Parity
                      8,  // DataBits
                      1   // StopBits
                      );

  if (EFI_ERROR (Status)) {
    EntsPrint (L"Set Serial attribute 115200 - %r\n", Status);
  }

  SerialIo->Reset (SerialIo);
  This->MonitorIo = NULL;

  return Status;
}

EFI_STATUS
SerialListener (
  IN EFI_ENTS_MONITOR_PROTOCOL     *This,
  IN OUT UINTN                     *Size,
  OUT CHAR16                       **Buffer
  )
/*++

Routine Description:

  This func is to read data from serial port.

Arguments:

  This    - Pointer to the EFI_ENTS_MONITOR_PROTOCOL instance.
  Size    - To indicate buffer length
  Buffer  - A buffer to return data to. It must be null before entering this func.

Returns:

  EFI_SUCCESS           - Operation succeeded.
  EFI_INVALID_PARAMETER - Parameter invalid.
  Others                - Some failure happened.

--*/
{
  EFI_STATUS              Status;
  EFI_SERIAL_IO_PROTOCOL  *SerialIo;

  SerialIo = This->MonitorIo;

  if ((Size == NULL) || (Buffer == NULL) || (*Buffer != NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status  = EFI_SUCCESS;

  *Size   = 0;

  //
  // Find start signal
  //
  do {
    Status = GetByte (SerialIo, Size, SerialBufferOut);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (IsComdBegin (SerialBufferOut, *Size)) {
      EntsSetMem (SerialBufferOut, *Size, 0);
      *Size = 0;
      break;
    }
  } while (1);

  //
  // Retrieve serial port until sig end is got
  //
  do {
    Status = GetByte (SerialIo, Size, SerialBufferOut);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    if (IsComdEnd (SerialBufferOut, *Size)) {
      EntsSetMem (SerialBufferOut + (*Size) - SIGNAL_TAG_LENGTH, SIGNAL_TAG_LENGTH, 0);
      *Size -= SIGNAL_TAG_LENGTH;
      break;
    }

  } while (1);

  *Buffer = EntsAllocatePool(*Size + 1);
  if (*Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  Status = Char8ToChar16 (SerialBufferOut, *Size, *Buffer);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  EntsSetMem (SerialBufferOut, 1024, 0);

  return EFI_SUCCESS;
}

EFI_STATUS
SerialSender (
  IN EFI_ENTS_MONITOR_PROTOCOL     *This,
  IN CHAR16                        *Buffer
  )
/*++

Routine Description:

  This func is to write data to serial port.

Arguments:

  This    - Pointer to the EFI_ENTS_MONITOR_PROTOCOL instance.
  Buffer  - A buffer to return data to. It must be null before entering this func.

Returns:

  EFI_SUCCESS          - Operation succeeded.
  EFI_ACCESS_DENIED    - Cannot send out packet in state SendOutPacket.
  EFI_OUT_OF_RESOURCES - Memory allocation failed.
  Others               - Some failure happened.
  
--*/
{
  EFI_STATUS              Status;
  UINTN                   BufferSize;
  CHAR8                   *BufferTmp;
  EFI_SERIAL_IO_PROTOCOL  *SerialIo;

  SerialIo  = This->MonitorIo;
  Status    = EFI_SUCCESS;

  //
  // Convert CHAR16 string to CHAR8 string
  //
  BufferSize = EntsStrLen(Buffer);
  BufferTmp = EntsAllocatePool(BufferSize + 1);
  if (BufferTmp == NULL) {
  	return EFI_OUT_OF_RESOURCES;
  }
  Status    = Char16ToChar8 (Buffer, BufferTmp, BufferSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // write data: Adding SigStart/SigEnd tag
  //
  BufferSize  = SIGNAL_TAG_LENGTH;
  Status      = SerialIo->Write (SerialIo, &BufferSize, gSerialSignalStart);
  if (EFI_ERROR (Status)) {
    EFI_ENTS_STATUS ((L"SerialWrite device Error\n"));
    EntsPrint (L"In Sender: can't write start sig Status - %r", Status);
    gBS->FreePool (BufferTmp);
    return Status;
  }

  BufferSize  = EntsStrLen (Buffer) + 1;
  Status      = SerialIo->Write (SerialIo, &BufferSize, BufferTmp);
  gBS->FreePool (BufferTmp);
  if (EFI_ERROR (Status)) {
    EFI_ENTS_STATUS ((L"SerialWrite device Error\n"));
    EntsPrint (L"In Sender: can't write data - %r", Status);
    return Status;
  }

  BufferSize  = SIGNAL_TAG_LENGTH;
  Status      = SerialIo->Write (SerialIo, &BufferSize, gSerialSignalEnd);
  if (EFI_ERROR (Status)) {
    EFI_ENTS_STATUS ((L"SerialWrite device Error\n"));
    EntsPrint (L"In Sender: can't write end sig. Status - %r", Status);
    return Status;
  }

  BufferSize  = 1;
  Status      = SerialIo->Write (SerialIo, &BufferSize, L"\n");
  if (EFI_ERROR (Status)) {
    EFI_ENTS_STATUS ((L"SerialWrite device Error\n"));
    EntsPrint (L"In Sender: can't write data - %r", Status);
    return Status;
  }

  Status = SerialIo->Reset (SerialIo);
  if (EFI_ERROR (Status)) {
    EntsPrint (L"in Sender:Error when reset serialIo");
  }

  return Status;
}

//
// Internal functions implementations
//
BOOLEAN
IsComdBegin (
  IN CHAR8                          *Buffer,
  IN UINTN                          Size
  )
/*++

Routine Description:

  This func is to search a start signal at the end of a buffer.

Arguments:

  Buffer  - The buffer to search.
  Size    - The data length of the buffer.

Returns:

  TRUE    - Operation succeeded.
  FALSE   - Operation failed.

--*/
{
  if (Size < SIGNAL_TAG_LENGTH) {
    return FALSE;
  }

  if (EntsCompareMem (Buffer + Size - SIGNAL_TAG_LENGTH, gSerialSignalStart, SIGNAL_TAG_LENGTH) == 0) {
    EntsPrint ((L"\nget sig begin\n"));
    return TRUE;
  }

  return FALSE;
}

BOOLEAN
IsComdEnd (
  IN CHAR8                          *Buffer,
  IN UINTN                          Size
  )
/*++

Routine Description:

  This func is to search a end signal at the end of a buffer.

Arguments:

  Buffer  - The buffer to search.
  Size    - The data length of the buffer.

Returns:

  TRUE    - Operation succeeded.
  FALSE   - Operation failed.

--*/
{
  if (Size < SIGNAL_TAG_LENGTH) {
    return FALSE;
  }

  if (EntsCompareMem (Buffer + Size - SIGNAL_TAG_LENGTH, gSerialSignalEnd, SIGNAL_TAG_LENGTH) == 0) {
    EntsPrint ((L"\nget sig end\n"));
    return TRUE;
  }

  return FALSE;
}

EFI_STATUS
GetByte (
  IN EFI_SERIAL_IO_PROTOCOL   *SerialIo,
  IN OUT UINTN                *BufferSize,
  OUT CHAR8                   *Buffer
  )
/*++

Routine Description:

  This func is to read one byte from serialIO port, and append this byte to a buffer.

Arguments:

  SerialIo    - Interface of SerialIo Protocol
  BufferSize  - Pointer of the size of the prelocated buffer. the size will be changed after reading
  Buffer      - To append the byte got from serial port

Returns:

  EFI_SUCCESS           - Operation succeeded.
  EFI_INVALID_PARAMETER - Parameter invalid.
  Others                - Some failure happened.


--*/
{
  EFI_STATUS  Status;
  UINTN       Size;
  CHAR8       BufferTmp;

  if ((Buffer == NULL) || (BufferSize == NULL)) {
    EFI_ENTS_STATUS ((L"Invalid parameter for GetByte"));
    return EFI_INVALID_PARAMETER;
  }

  Size = 1;
  while (1) {
    Status = SerialIo->Read (SerialIo, &Size, &BufferTmp);
    if (0 == Size) {
      Size = 1;
      continue;
    }

    if (Status == EFI_SUCCESS) {
      break;
    } else {
      EntsPrint (L"GetByte Statue:%r\n", Status);
    }

    if (Status == EFI_DEVICE_ERROR) {
      EFI_ENTS_STATUS ((L"SerialRead device Error\n"));
      EntsPrint (L"getByte: device_error\n");
      return Status;
    }
  }

  EntsCopyMem (Buffer + (*BufferSize), &BufferTmp, Size);
  *BufferSize += Size;

  return EFI_SUCCESS;
}
