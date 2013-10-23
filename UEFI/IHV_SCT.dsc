#
# The material contained herein is not a license, either      
# expressly or impliedly, to any intellectual property owned  
# or controlled by any of the authors or developers of this   
# material or to any contribution thereto. The material       
# contained herein is provided on an "AS IS" basis and, to the
# maximum extent permitted by applicable law, this information
# is provided AS IS AND WITH ALL FAULTS, and the authors and  
# developers of this material hereby disclaim all other       
# warranties and conditions, either express, implied or       
# statutory, including, but not limited to, any (if any)      
# implied warranties, duties or conditions of merchantability,
# of fitness for a particular purpose, of accuracy or         
# completeness of responses, of results, of workmanlike       
# effort, of lack of viruses and of lack of negligence, all   
# with regard to this material and any contribution thereto.  
# Designers must not rely on the absence or characteristics of
# any features or instructions marked "reserved" or           
# "undefined." The Unified EFI Forum, Inc. reserves any       
# features or instructions so marked for future definition and
# shall have no responsibility whatsoever for conflicts or    
# incompatibilities arising from future changes to them. ALSO,
# THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
# QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR          
# NON-INFRINGEMENT WITH REGARD TO THE TEST SUITE AND ANY      
# CONTRIBUTION THERETO.                                       
#                                                             
# IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THIS MATERIAL OR
# ANY CONTRIBUTION THERETO BE LIABLE TO ANY OTHER PARTY FOR   
# THE COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST    
# PROFITS, LOSS OF USE, LOSS OF DATA, OR ANY INCIDENTAL,      
# CONSEQUENTIAL, DIRECT, INDIRECT, OR SPECIAL DAMAGES WHETHER 
# UNDER CONTRACT, TORT, WARRANTY, OR OTHERWISE, ARISING IN ANY
# WAY OUT OF THIS OR ANY OTHER AGREEMENT RELATING TO THIS     
# DOCUMENT, WHETHER OR NOT SUCH PARTY HAD ADVANCE NOTICE OF   
# THE POSSIBILITY OF SUCH DAMAGES.                            
#                                                             
# Copyright 2006 - 2012 Unified EFI, Inc. All
# Rights Reserved, subject to all existing rights in all      
# matters included within this Test Suite, to which United    
# EFI, Inc. makes no claim of right.                          
#                                                             
# Copyright (c) 2010 - 2012, Intel Corporation. All rights reserved.<BR> 
#
#
#/*++
#
# Module Name:
#
#    IHV_SCT.dsc
#
# Abstract:
#
#   This is a build description file used to build the test modules for IHV SCT
#
# Notes:
#
#   The info in this file is broken down into sections. The start of a section
#   is designated by a "[" in the first column. So the [=====] separater ends
#   a section.
#
#--*/

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = IHVSct
  PLATFORM_GUID                  = 81ae3da4-3e91-4611-9015-eabc96b5e556
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/IHVSct
  SUPPORTED_ARCHITECTURES        = IA32|X64|IPF
  BUILD_TARGETS                  = DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  EDK_GLOBAL SHELL_SOURCE         =EdkCompatibilityPkg/Other/Maintained/Application/Shell

################################################################################
#
# SKU Identification section - list of all SKU IDs supported by this
#                              Platform.
#
################################################################################
[SkuIds]
  0|DEFAULT              # The entry: 0|DEFAULT is reserved and always required.

[BuildOptions]
  *_*_IA32_CC_FLAGS    = /D EFI_SPECIFICATION_VERSION=0x0002001F /D TIANO_RELEASE_VERSION=0x00080006 /D EFI32 /wd4133  /D MDE_CPU_IA32
  *_*_IA32_ASM_FLAGS   = /DEFI32
  *_*_IA32_VFRPP_FLAGS = /D EFI_SPECIFICATION_VERSION=0x0002001F /D TIANO_RELEASE_VERSION=0x00080006 /D EFI32
  *_*_IA32_APP_FLAGS   = /D EFI_SPECIFICATION_VERSION=0x0002001F /D TIANO_RELEASE_VERSION=0x00080006 /D EFI32
  *_*_IA32_PP_FLAGS    = /D EFI_SPECIFICATION_VERSION=0x0002001F /D TIANO_RELEASE_VERSION=0x00080006 /D EFI32

  *_*_X64_CC_FLAGS    = /D EFI_SPECIFICATION_VERSION=0x0002001F  /D TIANO_RELEASE_VERSION=0x00080006 /D EFIX64 /wd4133  /D MDE_CPU_X64
  *_*_X64_ASM_FLAGS   = /DEFIX64
  *_*_X64_VFRPP_FLAGS = /D EFI_SPECIFICATION_VERSION=0x0002001F  /D TIANO_RELEASE_VERSION=0x00080006 /D EFIX64
  *_*_X64_APP_FLAGS   = /D EFI_SPECIFICATION_VERSION=0x0002001F  /D TIANO_RELEASE_VERSION=0x00080006 /D EFIX64
  *_*_X64_PP_FLAGS    = /D EFI_SPECIFICATION_VERSION=0x0002001F  /D TIANO_RELEASE_VERSION=0x00080006 /D EFIX64
  
  *_*_IPF_CC_FLAGS = /Od /D EFI_SPECIFICATION_VERSION=0x0002001F /D TIANO_RELEASE_VERSION=0x00080006  /D EFIX64 /D MDE_CPU_IPF

[Libraries]

EdkCompatibilityPkg/Foundation/Library/CompilerStub/CompilerStubLib.inf
EdkCompatibilityPkg/Foundation/Guid/EdkGuidLib.inf
EdkCompatibilityPkg/Foundation/Framework/Guid/EdkFrameworkGuidLib.inf
EdkCompatibilityPkg/Foundation/Efi/Guid/EfiGuidLib.inf
EdkCompatibilityPkg/Foundation/Library/EfiCommonLib/EfiCommonLib.inf
EdkCompatibilityPkg/Foundation/Cpu/Pentium/CpuIA32Lib/CpuIA32Lib.inf

EdkCompatibilityPkg/Foundation/Protocol/EdkProtocolLib.inf
EdkCompatibilityPkg/Foundation/Framework/Protocol/EdkFrameworkProtocolLib.inf
EdkCompatibilityPkg/Foundation/Efi/Protocol/EfiProtocolLib.inf
EdkCompatibilityPkg/Foundation/Core/Dxe/ArchProtocol/ArchProtocolLib.inf
EdkCompatibilityPkg/Foundation/Library/Dxe/EfiDriverLib/EfiDriverLib.inf
EdkCompatibilityPkg/Foundation/Library/RuntimeDxe/EfiRuntimeLib/EfiRuntimeLib.inf
EdkCompatibilityPkg/Foundation/Library/Dxe/Print/PrintLib.inf
EdkCompatibilityPkg/Foundation/Library/Dxe/Graphics/Graphics.inf
EdkCompatibilityPkg/Foundation/Library/EdkIIGlueLib/Library/BaseLib/BaseLib.inf

EdkCompatibilityPkg/Other/Maintained/Application/Shell/Library/EfiShellLib.inf

SctPkg/Protocol/TestProtocolLib.inf
SctPkg/Library/EfiTestLib/EfiTestLib.inf
SctPkg/Library/EfiTestUtilityLib/EfiTestUtilityLib.inf

SctPkg/TestInfrastructure/SCT/Framework/ENTS/EasLib/EntsLib.inf
SctPkg/TestCase/RIVL/NetworkProtocolLib/NetworkProtocolLib.inf
SctPkg/Protocol/EntsProtocol/EntsProtocolLib.inf
SctPkg/Protocol/Eftp/EftpProtocolLib.inf
SctPkg/TestInfrastructure/SCT/Framework/ENTS/EasDispatcher/Eas.inf
SctPkg/Library/NetLib/Network/NetLib.inf
[Libraries.IPF]
EdkCompatibilityPkg/Foundation/Cpu/Itanium/CpuIa64Lib/CpuIA64Lib.inf


#
# These are the components that will be built by the master makefile
#

[Components]

#
# The default package
#
DEFINE PACKAGE=Default

#
# Components
#

#
# Following are the SCT suite & related drivers
#

#SctPkg/TestInfrastructure/SCT/Framework/Sct.inf
SctPkg/TestInfrastructure/SCT/Drivers/StandardTest/StandardTest.inf
SctPkg/TestInfrastructure/SCT/Drivers/TestProfile/TestProfile.inf
SctPkg/TestInfrastructure/SCT/Drivers/TestRecovery/TestRecovery.inf
SctPkg/TestInfrastructure/SCT/Drivers/TestLogging/TestLogging.inf

#
# Related SCT applications
#

SctPkg/Application/InstallSct/InstallSct.inf
SctPkg/Application/StallForKey/StallForKey.inf

#
# Test cases for UEFI/IHV SCT
#

SctPkg/TestCase/UEFI/IHV/Protocol/BlockIo/BlackBoxTest/IhvBlockIoBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/BlockIo2/BlackBoxTest/IhvBlockIo2BBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/BusSpecificDriverOverride/BlackBoxTest/IhvBusSpecificDriverOverrideBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/ComponentName2/BlackBoxTest/IhvComponentName2BBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/DevicePath/BlackBoxTest/IhvDevicePathBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/DiskIo/BlackBoxTest/IhvDiskIoBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/DriverBinding/BlackBoxTest/IhvDriverBindingBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/DriverDiagnostics2/BlackBoxTest/IhvDriverDiagnostics2BBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/Ebc/BlackBoxTest/IhvEbcBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/PlatformDriverOverride/BlackBoxTest/IhvPlatformDriverOverrideBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/PxeBaseCode/BlackBoxTest/IhvPxeBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/ScsiPassThru/BlackBoxTest/IhvScsiPassThruBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SerialIo/BlackBoxTest/IhvSerialIoBbTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimpleFileSystem/BlackBoxTest/IhvSimpleFileSystemBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimpleNetwork/BlackBoxTest/IhvSimpleNetworkBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimplePointer/BlackBoxTest/IhvSimplePointerBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimpleTextIn/BlackBoxTest/IhvSimpleTextInBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimpleTextInputEx/BlackBoxTest/IhvSimpleTextInputExBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/SimpleTextOut/BlackBoxTest/IhvSimpleTextOutBBTest_uefi.inf
SctPkg/TestCase/UEFI/IHV/Protocol/UnicodeCollation2/BlackBoxTest/IhvUnicodeCollation2BBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/Usb2Hc/BlackBoxTest/IhvUsb2HcTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/UsbIo/BlackBoxTest/IhvUsbIoTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/GraphicsOutput/BlackBoxTest/IhvGraphicsOutputBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/ExtScsiPassThru/BlackBoxTest/IhvExtScsiPassThruBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/ScsiIo/BlackBoxTest/IhvScsiIoBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/iScsiInitiatorName/BlackBoxTest/IhviScsiInitiatorNameBBTest.inf

SctPkg/TestCase/UEFI/IHV/Protocol/AbsolutePointer/BlackBoxTest/IhvAbsolutePointerBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/PlatformToDriverConfiguration/BlackBoxTest/IhvPlatformToDriverConfigurationBBTest.inf
SctPkg/TestCase/UEFI/IHV/Protocol/StorageSecurityCommand/BlackBoxTest/IhvStorageSecurityCommandBBTest.inf

#
# Dependency files for UEFI/IHV Test
#
SctPkg/TestCase/UEFI/IHV/Protocol/Ebc/BlackBoxTest/Dependency/EbcDriverBin/EbcDriverBin.inf

SctPkg/TestInfrastructure/SCT/Framework/Sct.inf


