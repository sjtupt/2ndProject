/** <small>$Id: system.h,v 1.1.1.1 2005/08/23 08:23:04 benwang Exp $</small>\n

  <center>
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

@file    system.h
@brief   delray system header  
     
  Each system header provides a standard set of symbolic 
  constants, macros, etc.  All software components should always 
  include SYSTEMSPACE_ROOT/sys/inc/system.h.  All software 
  components should always use whatever standard facilities 
  are implemented here.

  See the OSA Build and Release Engineering practice standards 
  for details about the system header.

$Revision: 1.1.1.1 $
$Date: 2005/08/23 08:23:04 $
$Author: benwang $
$RCSfile: system.h,v $
$Source: /cvsroot/rac5_ip/osa/ipmi_core/src/sdf/ipmi/inc/system.h,v $
$State: Exp $
$Locker:  $
$Name:  $

=========================================================================== */
/// :__START__:  

#if !defined(SDF_SYSTEM_H)
#define SDF_SYSTEM_H          /* unlike include fences for software
                                 components, we prefix any/all system 
																 supplied include fence symbolics with 
																 the reserved "SDF_" prefix.  The "SDF_" 
                                 4-letter name is set aside for system 
                                 component use.  No software component 
															   in any gear box can be named "sdf*".  
                                 Therefore, no software component can 
                                 ever introduce a similar include fence 
                                 symbolic name! */

# ifdef     OSTD_ENTRY        /* defined only in one "entry" point *.c file */
#  ifndef   OSTD_OMIT_KEY     /* defined if key embedding de-activated */
#   ifndef lint               /* stop unused global warnings from lint */   
//   
//   Product name (part of product key) embedding done by this header
//
#    if defined(OSTD_PRODUCT_SMC_BASIC)
      static char ostd_product_name[] = "ofw_smc_basic";
#    elif defined(OSTD_PRODUCT_SMC_DELUXE)
      static char ostd_product_name[] = "ofw_smc_deluxe";
#    else 
      static char ostd_product_name[] = "system";
#    endif
//
#   endif
#  endif
# endif 
#endif  /* SDF_SYSTEM_H */
