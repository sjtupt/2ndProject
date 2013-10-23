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

  IPsecConfigDataIp4Transport.c

Abstract:

  IPsec Config Transport Mode Setting Data from the UEFI 2.3 specification.

--*/

#include "Efi.h"
#include "IPsecConfigProtocol.h"

EFI_IP_ADDRESS_INFO LocalAddressIp4Transport;
EFI_IP_ADDRESS_INFO RemoteAddressIp4Transport;


//
// Currently Inbound & OutBound use the same EncKey to keep encryption & decryption keys equal 
//
CHAR8  InBoundEncKeyIp4Transport[128]   = "ipv4readylogo3descbcout1"; 
CHAR8  InBoundAuthKeyIp4Transport[128]  = "ipv4readylogsha1out1";
CHAR8  OutBoundEncKeyIp4Transport[128]  = "ipv4readylogo3descbcout1";
CHAR8  OutBoundAuthKeyIp4Transport[128] = "ipv4readylogsha1out1";



//
// In Bound & OutBound transport mode SPD selector 
//
EFI_IPSEC_CONFIG_SELECTOR InBoundSpdSelectorIp4Transport =
{ 
   1,                                 // LocalAddressCount
   &RemoteAddressIp4Transport,        // LocalAddress
   1,                                 // RemoteAddressCount;
   &LocalAddressIp4Transport,         // RemoteAddress
   0xFFFF,                            // NextLayerProtocol
   0,                                 // LocalPort
   65535,                             // LocalPortRange
   0,                                 // RemotePort
   65535                              // RemotePortRange
};

EFI_IPSEC_CONFIG_SELECTOR OutBoundSpdSelectorIp4Transport = 
{ 
   1,                                 // LocalAddressCount
   &LocalAddressIp4Transport,         // LocalAddress
   1,                                 // RemoteAddressCount;
   &RemoteAddressIp4Transport,        // RemoteAddress
   0xFFFF,                            // NextLayerProtocol
   0,                                 // LocalPort
   65535,                             // LocalPortRange
   0,                                 // RemotePort
   65535                              // RemotePortRange
};

EFI_IPSEC_CONFIG_SELECTOR InBoundSadSelectorIp4Transport;
EFI_IPSEC_CONFIG_SELECTOR OutBoundSadSelectorIp4Transport;


EFI_IPSEC_PROCESS_POLICY InBoundProcessingPolicyIp4Transport = 
{
  FALSE,                  // ExtSeqNum 
  FALSE,                  // SeqOverflow
  FALSE,                  // FragCheck
               
  {                       // SaLifetime 
    0xFFFFFFFF,      // ByteCount;
    0xFFFFFFFF,      // SoftLifetime;
    0xFFFFFFFF,      // HardLifetime
  },            

  EfiIPsecTransport,      // Mode
  NULL,                   // TunnelOption
  EfiIPsecESP,            // Proto
  IKE_AALG_SHA1HMAC,      // AuthAlgoId
  IKE_EALG_3DESCBC        // EncAlgoId
};

EFI_IPSEC_PROCESS_POLICY OutBoundProcessingPolicyIp4Transport = 
{
  FALSE,                  // ExtSeqNum 
  FALSE,                  // SeqOverflow
  FALSE,                  // FragCheck
               
  {                       // SaLifetime 
    0xFFFFFFFF,      //ByteCount;
    0xFFFFFFFF,      // SoftLifetime;
    0xFFFFFFFF,      // HardLifetime
  },            

  EfiIPsecTransport,      // Mode
  NULL,                   // TunnelOption
  EfiIPsecESP,            // Proto
  IKE_AALG_SHA1HMAC,      // AuthAlgoId
  IKE_EALG_3DESCBC        // EncAlgoId
};


//
// InBound & OutBound SAD
//
EFI_IPSEC_SA_DATA2 InBoundSadIp4Transport = 
{
  EfiIPsecTransport,                                  // Mode
  0,                                                  // SNCount
  10,                                                 // AntiReplayWindows
  {                                                   // AlgoInfo
                   // AhAlgoInfo
    0,             // AuthAlgoId
    0,             // KeyLength
    NULL           // Key            
  }, 
  {                                                   // SaLifetime 
    0xFFFFFFFF,    // ByteCount
    0xFFFFFFFF,    // SoftLifetime
    0xFFFFFFFF,    // HardLifetime
  }, 
  1000,                                               // PathMTU
  &(InBoundSpdSelectorIp4Transport.SpdSelector),      // SpdSelector
  TRUE,                                               // ManualSet
                                                      // TunnelSourceAddress
                                                      // TunnelDestinationAddress 
};


EFI_IPSEC_SA_DATA2 OutBoundSadIp4Transport = 
{
  EfiIPsecTransport,                                 // Mode
  0,                                                 // SNCount
  10,                                                // AntiReplayWindows
  {                                                  // AlgoInfo
                   // AhAlgoInfo
    0,             // AuthAlgoId
    0,             // KeyLength
    NULL           // Key            
  }, 
  {                                                  //SaLifetime 
    0xFFFFFFFF,    // ByteCount
    0xFFFFFFFF,    // SoftLifetime
    0xFFFFFFFF,    // HardLifetime
  },
  1000,                  // PathMTU
  &(OutBoundSpdSelectorIp4Transport.SpdSelector),    // SpdSelector
  TRUE,                                              // ManualSet
                                                     // TunnelSourceAddress
                                                     // TunnelDestinationAddress
};


//
// InBound & OutBound SPD Transport Mode
//
EFI_IPSEC_SPD_DATA OutBoundSpdIp4Transport =
{
  "OutboundTestIp4TransportMode",                    // Name
  0x0,                                               // PackageFlag
  EfiIPsecOutBound,                                  // TrafficDirection
  EfiIPsecActionProtect,                             // Action
  &OutBoundProcessingPolicyIp4Transport,             // ProcessingPolicy
  1,                                                 // SaIdCount
  {                  
    {                                      // SaId[0]
      0,                            // SPI
      EfiIPsecESP                   // Proto
                                    // DestAddres
    },
  },
};


EFI_IPSEC_SPD_DATA InBoundSpdIp4Transport = 
{
  "InboundTestIp4TransportMode",                       // Name
  0x0,                                                 // PackageFlag
  EfiIPsecInBound,                                     // TrafficDirection
  EfiIPsecActionProtect,                               // Action
  &InBoundProcessingPolicyIp4Transport,                // ProcessingPolicy
  1,                                                   // SaIdCount
  {                  
    {                                     // SaId[0]
      0,                            // SPI
      EfiIPsecESP                   // Proto
                                    // DestAddres
    },
  },
};



