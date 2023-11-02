/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
 *
 *  \note          Please note, that this file contains a collection of callback functions to be used with the
 *                 Flash Bootloader. These functions may influence the behavior of the bootloader in principle.
 *                 Therefore, great care must be taken to verify the correctness of the implementation.
 *                 The contents of the originally delivered files are only examples resp. implementation proposals.
 *                 With regard to the fact that these functions are meant for demonstration purposes only, Vector
 *                 Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent
 *                 admissible by law or statute.
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2018-03-13  visach  ESCAN00098699    Support VAG 80126 2.5
 *  05.00.01   2018-03-13  visach  -                ALM versions inconsistent
 *  05.01.00   2018-09-19  visach  ESCAN00100791    Support Rueckflashschutz
 *  05.02.00   2019-06-26  vistbe  ESCAN00103512    Added support for stream output
 *  05.03.00   2021-04-01  vishor  FBL-2523         Support of the new FblMio
 *  05.04.00   2021-11-10  visjdn  FBL-3077         Support of embedded checksum/signature
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2018-09-14  visrie  ESCAN00100739    Added support for FblLib_NvPattern
 *                                                  Removed unused code
 *                         viscb   ESCAN00101418    Callout function for memory condition check
 *                         visach  ESCAN00101686    No changes
 *  02.01.00   2019-02-26  visrie  ESCAN00102257    Added additional callout functions
 *  02.01.01   2019-09-23  visrie  FBL-452          No changes
 *  02.02.00   2019-12-05  visrie  FBL-458          Added FblMio callout functions
 *  02.03.00   2020-05-07  visrie  FBL-1414         Added callout functions for One-Step-Updater
 *  02.03.01   2020-06-02  visjdn  ESCAN00106533    No changes
 *  02.03.02   2020-09-14  visrcn  FBL-2257         No changes
 *  02.03.03   2020-11-23  visrcn  FBL-2564         No changes
 *  02.03.04   2021-03-19  visjdn  FBL-2916         No changes
 *  02.04.00   2021-05-25  vishor  FBL-3165         MISRA corrections and justifications
 *  02.05.00   2021-06-15  visrie  FBL-3591         No changes
 *  02.06.00   2021-07-15  visjdn  FBL-3381         Re-mapping of EcuM_SetWakeupEvent
 *  02.07.00   2021-07-23  visrie  FBL-3822         No changes
 *  02.08.00   2021-09-14  visjdn  FBL-2859         No changes
 *  02.09.00   2021-10-12  vistmo  FBL-3901         Add authentication callbacks
 *  02.10.00   2021-10-15  visrie  FBL-3945         Remove content of ApplFblStartApplication
 *  02.10.01   2021-11-12  vishor  ESCAN00110133    Compiler error: Unknown symbols
 *                                                   ApplFblSave/RestoreStatusSector/Dynamic
 *  02.10.02   2022-06-27  visrie  FBL-4694         No changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY (KbFbl_apOem.h)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  06.00.00   2019-03-08  visrie  ESCAN00102262    No changes
 *                                 ESCAN00102274    No changes
 *                         visach  ESCAN00102407    No changes
 *  06.00.01   2019-03-11  visrie  ESCAN00102428    No changes
 *                         visach  ESCAN00102447    No changes
 *  06.00.02   2019-04-25  vistbe  ESCAN00102870    No changes
 *  06.01.00   2019-05-16  visach  ESCAN00103160    No changes
 *  06.02.00   2019-06-16  vistbe  ESCAN00103514    Added support for stream output
 *  06.02.01   2019-09-23  visrie  FBL-452          No changes
 *  06.02.02   2019-09-27  visrcn  ESCAN00102945    No changes
 *  07.00.00   2020-04-27  visrie  FBL-1412         No changes
 *  07.01.00   2020-05-07  visrie  FBL-1414         No changes
 *  07.01.01   2020-06-23  visjdn  ESCAN00105424    No changes
 *                                 ESCAN00103341    No changes
 *                                 ESCAN00106311    No changes
 *  07.01.02   2020-08-05  visjdn  ESCAN00106957    No changes
 *  07.01.03   2020-10-16  visrcn  ESCAN00107250    No changes
 *                                 ESCAN00107269    No changes
 *  07.01.04   2021-01-13  vistmo  ESCAN00107087    No changes
 *  07.01.05   2021-02-15  vishor  ESCAN00107905    No changes
 *  07.02.00   2021-04-01  vishor  FBL-2523         No changes
 *  07.02.01   2021-07-21  visrie  ESCAN00108794    No changes
 *                                 ESCAN00108042    No changes
 *                                 ESCAN00109818    No changes
 *  07.03.00   2021-11-10  visjdn  FBL-3077         No changes
 *  07.03.01   2022-02-01  vistmo  ESCAN00110600    No changes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apHW.h, Brs)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2018-02-26  Csz                      Initial version
 *  01.00.01   2018-03-08  Csz     ESCAN00102984    No changes
 *  01.00.02   2019-04-16  Csz     ESCAN00102985    No changes
 *  02.00.00   2020-04-08  Rie     FBL-1016         No changes
 *  02.01.00   2020-07-13  JDn     ESCAN00106756    No changes
 **********************************************************************************************************************/

#ifndef FBL_AP_H
#define FBL_AP_H

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi CQComponent : Implementation */
#define FBLKBAPI_VERSION                           0x0210u
#define FBLKBAPI_RELEASE_VERSION                   0x02u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_Brs CQComponent : Implementation */
#define FBLKBAPI_BRS_VERSION          0x0201u
#define FBLKBAPI_BRS_RELEASE_VERSION  0x00u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_UDS1 CQComponent : Implementation */
#define FBLKBAPI_UDS1_VERSION                    0x0703u
#define FBLKBAPI_UDS1_RELEASE_VERSION            0x01u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_Frame_UDS1 CQComponent : Implementation */
#define FBLKBAPI_FRAME_UDS1_VERSION               0x0504u
#define FBLKBAPI_FRAME_UDS1_RELEASE_VERSION       0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define FBL_MAX_DID_COUNT   8u

# if defined( FBL_ENABLE_DATA_PROCESSING )
#  if !defined( GetOemProcessingModeSupported )
/* Accept compression and encryption */
#   define GetOemProcessingModeSupported(m) (GetOemCompressionMode((m)) || GetOemEncryptionMode((m)))
#  endif /* GetOemProcessingModeSupported */

#  if !defined( GetOemCompressionMode )
#   if defined( FBL_ENABLE_COMPRESSION_MODE )
/* Accept compression routine "1" */
#    define GetOemCompressionMode(m) ((FblDiagGetCompressionMode(m) & kDiagSubDefaultCompression) == kDiagSubDefaultCompression)
#   else
/* No compression routine supported */
#    define GetOemCompressionMode(m) (0 != 0)
#   endif /* FBL_ENABLE_COMPRESSION_MODE */
#  endif /* GetOemCompressionMode */

#  if !defined( GetOemEncryptionMode )
#   if defined( FBL_ENABLE_ENCRYPTION_MODE )
/* Accept encryption routine "1" */
#    define GetOemEncryptionMode(m) ((FblDiagGetEncryptionMode(m) & kDiagSubDefaultEncryption) == kDiagSubDefaultEncryption)
#   else
/* No encryption routine supported */
#    define GetOemEncryptionMode(m) (0 != 0)
#   endif /* FBL_ENABLE_ENCRYPTION_MODE */
#  endif /* GetOemEncryptionMode */
# endif /* FBL_ENABLE_DATA_PROCESSING */
# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT)
#  if !defined( GetOemStreamModeSupported )
#   define GetOemStreamModeSupported(m) ((FblDiagGetCompressionMode(m) & kDiagSubDefaultDelta) == kDiagSubDefaultDelta)
#  endif /* GetOemStreamModeSupported */
# endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_CW_ENABLE_MULTIPLE_NODES )
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 fblEcuHandle;
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

void ApplFblInit( void );
void ApplFblStartup( vuint8 initposition );
tFblResult ApplFblCheckProgConditions( void );
tFblResult ApplFblCheckConditions( vuint8 * pbDiagData, tTpDataType diagReqDataLen );
vuint8 ApplFblCheckProgDependencies( void );
void ApplTrcvrNormalMode( void );
void ApplTrcvrSleepMode( void );
void ApplFblSetVfp( void );
void ApplFblResetVfp( void );
void ApplFblReset( void );
#define FBLAP_RAMCODE_START_SEC_CODE_EXPORT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
vuint8 ApplFblSecuritySeedInit( void );
#define FBLAP_RAMCODE_STOP_SEC_CODE_EXPORT
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
vuint8 ApplFblSecuritySeed( void );
vuint8 ApplFblSecurityKey( void );
vuint8 ApplFblSecurityAppKey( void );
vuint8 ApplFblSecurityInit( void );
#if defined( FBL_ENABLE_DATA_PROCESSING )
tFblResult ApplFblInitDataProcessing( tProcParam * procParam );
tFblResult ApplFblDataProcessing( tProcParam * procParam );
tFblResult ApplFblDeinitDataProcessing( tProcParam * procParam );
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
tFblResult ApplFblInitStreamOutput( V_MEMRAM1 tFblMemStreamProcessing V_MEMRAM2 V_MEMRAM3 * streamParam );
tFblResult ApplFblStreamOutput( V_MEMRAM1 tFblMemStreamProcessing V_MEMRAM2 V_MEMRAM3 * streamParam );
tFblResult ApplFblFinalizeStreamOutput( V_MEMRAM1 tFblMemStreamProcessing V_MEMRAM2 V_MEMRAM3 * streamParam );
tFblResult ApplFblDeinitStreamOutput( V_MEMRAM1 tFblMemStreamProcessing V_MEMRAM2 V_MEMRAM3 * streamParam );
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_ENABLE_APPL_TASK ) || \
    defined( FBL_ENABLE_APPL_TIMER_TASK )
#if defined( FBL_ENABLE_APPL_TASK ) || \
    defined( FBL_ENABLE_APPL_TIMER_TASK )
void ApplFblTask( void );
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_TIMER_TASK */
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_TIMER_TASK */
#if defined( FBL_ENABLE_APPL_TASK ) ||\
    defined( FBL_ENABLE_APPL_STATE_TASK )
#if defined( FBL_ENABLE_APPL_TASK ) ||\
    defined( FBL_ENABLE_APPL_STATE_TASK )
void ApplFblStateTask( void );
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_STATE_TASK */
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_STATE_TASK */

#if defined( FBL_ENABLE_ASSERTION )
void ApplFblFatalError( FBL_DECL_ASSERT_EXTENDED_INFO(vuint8 errorCode) );
#endif

void ApplFblCanBusOff( void );


#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION) || \
    defined( FBL_CW_ENABLE_MULTIPLE_NODES )
void ApplFblCanParamInit( void );
#endif


#if defined( FBL_ENABLE_SLEEPMODE )
void ApplFblBusSleep( void );
#endif /* FBL_ENABLE_SLEEPMODE */



/* Additional functions depending on hardware platform requirements */

tFblResult ApplFblGetVerificationData(V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * verificationParam,
                                      V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * verificationData,
                                      vuintx verificationDataLength);



#if defined( FBL_MIO_ENABLE_HOOKS )
void ApplFblPreMemDriver( vuint8 device, vuint8 function );
#endif /* FBL_MIO_ENABLE_HOOKS */

#if defined( FBL_MIO_ENABLE_HOOKS )
void ApplFblPostMemDriver( vuint8 device, vuint8 function );
#endif /* FBL_MIO_ENABLE_HOOKS */

#endif /* __FBL_AP_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_AP.H
 **********************************************************************************************************************/
