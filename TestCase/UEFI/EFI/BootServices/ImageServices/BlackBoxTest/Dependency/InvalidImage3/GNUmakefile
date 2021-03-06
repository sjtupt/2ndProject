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
# Copyright (c) 2011, 2012 ARM Ltd. All rights reserved.<BR>  
#
#
#/*++
#
# Module Name:
#
#   makefile
#
# Abstract:
#
#   This is the makefile for creating an invalid image file for testing.
#
#--*/

BASE_NAME=ImageServices_InvalidImage3
SOURCE_DIR=$(EFI_SOURCE)/SctPkg/TestCase/UEFI/EFI/BootServices/ImageServices/BlackBoxTest/Dependency/InvalidImage3
GENBIN=GenBin
DEPENDECY=$(SOURCE_DIR)/InvalidImage3.ini
UNAME = $(shell uname)

#Check if it is cygwin environment
ifeq ($(findstring CYGWIN,$(UNAME)),CYGWIN)
	GEN_BIN_PROFILE=`cygpath -m $(DEPENDECY)`
	TARGET=`cygpath -m $(BIN_DIR)/$(BASE_NAME)`
else
	GEN_BIN_PROFILE=$(DEPENDECY)
	TARGET=$(BIN_DIR)/$(BASE_NAME)
endif
#
# Define some useful macros, then include the master Efi toolchain setup
# file.
#
#BIN_DIR     = $(BUILD_DIR)/$(PROCESSOR)
#TOOLCHAIN   = TOOLCHAIN_$(PROCESSOR)

#!INCLUDE $(BUILD_DIR)/PlatformTools.env

#
# We generate the invalid image file from a profile
#
$(BIN_DIR)/$(BASE_NAME) : $(DEPENDECY)
	$(GENBIN) $(GEN_BIN_PROFILE) $(TARGET)

all : $(BIN_DIR)/$(BASE_NAME)

clean:
	$(RM) $(BIN_DIR)/$(BASE_NAME)
