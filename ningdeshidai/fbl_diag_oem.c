/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  09.00.00   2020-06-18  visjdn  FBL-1819         Update to FblBm_Main version 5.00.00
 *  09.00.01   2021-03-04  vistmo  ESCAN00108085    Logical block can be verified with signature/checksum of other block
 *                                 FBL-2935         Added/adapted MemMap sections
 *  09.00.02   2021-05-11  vistbe  ESCAN00107662    No changes
 *                                 FBL-3334         Update to MISRA 2012
 *  09.01.00   2021-11-10  visjdn  FBL-3077         Support of embedded checksum/signature
 *  09.02.00   2022-05-02  vishor  FBL-5067         Support of processed and input verification
 **********************************************************************************************************************/

#define FBL_DIAG_OEM_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Common Bootloader includes */
#include "fbl_inc.h"
/* Core Diagnostic interface and global symbols */
#include "fbl_diag_core.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* Diagnostic OEM module version check */
#if ((FBLDIAG_14229_UDS2_VERSION != 0x0902u) || \
     (FBLDIAG_14229_UDS2_RELEASE_VERSION != 0x00u))
# error "Error in fbl_diag_oem.c: Source and header file are inconsistent!"
#endif
#if(( FBLDIAG_14229_UDS2_VERSION != _FBLDIAG_OEM_VERSION ) || \
    ( FBLDIAG_14229_UDS2_RELEASE_VERSION != _FBLDIAG_OEM_RELEASE_VERSION ))
# error "Error in fbl_diag_oem.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  TYPE DEFINITIONS
 **********************************************************************************************************************/

#if defined( FBL_DIAG_ENABLE_FLASHDRV_ROM )
/** Structure to initialize flash drivers from ROM */
typedef struct
{
   V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * sourceBuffer;  /**< Flash driver image source buffer */
   V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * destBuffer;    /**< Flash driver destination buffer */
   vuint32 copySize;                                     /**< Size of data to be copied */
   vuint8 decryptValue;                                  /**< Value to decrypt XOR-encrypted driver */
} tFlashDriverInitData;
#endif /* FBL_DIAG_ENABLE_FLASHDRV_ROM */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( FBL_DIAG_FLASH_CODE_BASE_ADDR )
/* Allow to override the flashCode base address for certain platforms.
 * Example: For word-addressed platforms the download address of the flash driver is going to be virtual, i.e. twice
 *          the physical address. For those platforms the base address can be overwritten in a user configuration file. */
#else
# define FBL_DIAG_FLASH_CODE_BASE_ADDR    ((tFblAddress)flashCode)
#endif

#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
   /* Report full diagnostic buffer size */
# define FBL_DIAG_RESPONSE_BUFFER_SIZE FBL_DIAG_BUFFER_LENGTH
#else
   /* Calculate buffer size so that a multiple of the diagnostic bufffer size + 2 is reported */
# define FBL_DIAG_RESPONSE_BUFFER_SIZE ((((FBL_DIAG_SEGMENT_SIZE - 1u) ^ 0xFFFFFFFFu) & (FBL_DIAG_BUFFER_LENGTH - 2u)) + 2u)
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */

#if( FBL_DIAG_RESPONSE_BUFFER_SIZE >= 0x1000000u )
# define FBL_DIAG_RESPONSE_BUFFER_LFI 4u
#elif( FBL_DIAG_RESPONSE_BUFFER_SIZE >= 0x10000u )
# define FBL_DIAG_RESPONSE_BUFFER_LFI 3u
#else
# define FBL_DIAG_RESPONSE_BUFFER_LFI 2u
#endif

/***********************************************************************************************************************
 *  State handling
 **********************************************************************************************************************/
/* Download sequence states */
#define FblDiagSetSecurityKeyAllowed()    SetFblDiagState( kFblDiagStateSecurityKeyAllowed )
#define FblDiagClrSecurityKeyAllowed()    ClrFblDiagState( kFblDiagStateSecurityKeyAllowed )
#define FblDiagSetSecurityUnlock()        SetFblDiagState( kFblDiagStateSecurityUnlock )
#define FblDiagClrSecurityUnlock()        ClrFblDiagState( kFblDiagStateSecurityUnlock )
#define FblDiagSetFingerprintValid()      SetFblDiagState( kFblDiagStateFingerprintValid )
#define FblDiagClrFingerprintValid()      ClrFblDiagState( kFblDiagStateFingerprintValid )
#define FblDiagSetEraseSucceeded()        SetFblDiagState( kFblDiagStateEraseSucceeded )
#define FblDiagClrEraseSucceeded()        ClrFblDiagState( kFblDiagStateEraseSucceeded )
#define FblDiagSetTransferDataAllowed()   SetFblDiagState( kFblDiagStateTransferDataAllowed )
#define FblDiagClrTransferDataAllowed()   ClrFblDiagState( kFblDiagStateTransferDataAllowed )
#define FblDiagSetTransferDataSucceeded() SetFblDiagState( kFblDiagStateTransferDataSucceeded )
#define FblDiagClrTransferDataSucceeded() ClrFblDiagState( kFblDiagStateTransferDataSucceeded )
#define FblDiagSetChecksumAllowed()       SetFblDiagState( kFblDiagStateChecksumAllowed )
#define FblDiagClrChecksumAllowed()       ClrFblDiagState( kFblDiagStateChecksumAllowed )
#define FblDiagSetFlashDriverPresent()    SetFblDiagState( kFblDiagStateFlashDriverPresent )
#define FblDiagClrFlashDriverPresent()    ClrFblDiagState( kFblDiagStateFlashDriverPresent )
#define FblDiagSetAppSecurityKeyAllowed() SetFblDiagState( kFblDiagStateAppSecurityKeyAllowed )
#define FblDiagClrAppSecurityKeyAllowed() ClrFblDiagState( kFblDiagStateAppSecurityKeyAllowed )
#define FblDiagSetAppSecurityUnlock()     SetFblDiagState( kFblDiagStateAppSecurityUnlock )
#define FblDiagClrAppSecurityUnlock()     ClrFblDiagState( kFblDiagStateAppSecurityUnlock )

/***********************************************************************************************************************
 *  Local constants
 **********************************************************************************************************************/
#define kDiagInitSequenceNum     ((vuint8) 0x01u)

#if defined( FBL_DIAG_ENABLE_FLASHDRV_ROM )
# if !defined( FLASH_DRIVER_INIT_DATA )
/** Initialization data for default flash driver. If additional drivers should be initialized, please overwrite macro. */
#  define FLASH_DRIVER_INIT_DATA {{flashDrvBlk0, flashCode, FLASHDRV_BLOCK0_LENGTH, FLASHDRV_DECRYPTVALUE}}
# endif
#endif /* FBL_DIAG_ENABLE_FLASHDRV_ROM */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define FBLDIAG_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Diagnostic service help functions */
static void FblDiagSessionControlParamInit(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static vuint8 FblDiagDownloadCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Erase help functions */
static tFblResult FblDiagEraseBlock( V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 *pBlockDescriptor );
static tFblResult FblDiagCheckErasePreconditions(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pbDiagData,
                                                 V_MEMRAM1 tFblAddress V_MEMRAM2 V_MEMRAM3 * pEraseAddress,
                                                 V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * pEraseLength);
static tFblResult FblDiagCheckProgAttemptCounter(V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 *pBlockDescriptor);
static tFblResult FblDiagPrepareFlashDriver( void );
/* Request download help functions */
static tFblResult FblDiagCheckRequestDownloadPreconditions(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pbDiagData,
                                                           V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo);
#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
static tFblResult FblDiagCheckForFlashDriverDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo);
static tFblResult FblDiagCheckFlashDriverDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo);
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */
static tFblResult FblDiagCheckFlashMemoryDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo);
static tFblResult FblDiagPrepareFirstDownloadSegment(V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * pBlockInfo,
                                                     V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo,
                                                     vuint8 tempBlockNr);
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
static tFblResult FblDiagInitDeltaDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo);
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

/* NRC handler functions */
static tFblResult FblDiagProcessSubfunctionNrc(vuint32 serviceErrorMask);
static tFblResult FblDiagProcessServiceNrc(vuint32 serviceErrorMask);
static tFblResult FblDiagProcessRoutineNrc(vuint32 serviceErrorMask);

/* Session handling */
static tFblResult FblDiagDefaultSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagExtendedSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagProgrammingSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagTesterPresentMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Reset */
static tFblResult FblDiagEcuResetMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* DID handling */
static tFblResult FblDiagDataByIdLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagReadDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagWriteDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Security access */
static tFblResult FblDiagSecAccessSeedMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagSecAccessKeyMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Communication control / DTC handling */
static tFblResult FblDiagCommCtrlMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagControlDTCMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Routine control */
static tFblResult FblDiagRCStartCsumMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#if defined( FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS )
static tFblResult FblDiagRCStartCheckProgPreCondMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#endif /* FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS */
#if defined( FBL_ENABLE_STAY_IN_BOOT )
static tFblResult FblDiagRCStartForceBootModeMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#endif /* FBL_ENABLE_STAY_IN_BOOT */
static tFblResult FblDiagRCStartEraseLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagRCStartEraseMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagRCStartCheckProgDepMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Data transfer */
static tFblResult FblDiagRequestDownloadLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagRequestDownloadMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagTransferDataLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagTransferDataMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagReqTransferExitMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/* Service pre-handler functions */
static tFblResult FblDiagDefaultPreHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

#define FBLDIAG_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  LOCAL CONSTANTS
 **********************************************************************************************************************/

#define FBLDIAG_START_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if defined( FBL_DIAG_ENABLE_FLASHDRV_ROM )
/** List of flash driver source and destination buffers to be initialized. */ /* PRQA S 3218 1 */ /* MD_FblDiag_3218 */
V_MEMROM0 static V_MEMROM1 tFlashDriverInitData V_MEMROM2 kFlashDriverInitData[] = FLASH_DRIVER_INIT_DATA;
#endif /* FBL_DIAG_ENABLE_FLASHDRV_ROM */

#define FBLDIAG_STOP_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
/* PRQA S 3218 TAG_FblDiag_3218_1 */ /* MD_FblDiag_3218 */
V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2                    calculateCRC[MAX_BLOCK];     /*Task:7278035*/
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2                    CrcErrorFlag = CRC_CORRECT;

#define FBLDIAG_START_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/** Current block sequence counter */
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      currentSequenceCnt;
#if defined( FBL_ENABLE_SEC_ACCESS_DELAY )
/** Invalid key counter */
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      secSendKeyInvalid;   /* PRQA S 3218 */ /* MD_FblDiag_3218 */
#endif

/* Segment handling variables */
/** Verification information to be transmitted to security module */
V_MEMRAM0 static V_MEMRAM1 SecM_VerifyParamType       V_MEMRAM2 verifyParam;
/** Download segment information. Used for gap fill and data verification. */
V_MEMRAM0 static V_MEMRAM1 FL_SegmentInfoType         V_MEMRAM2 downloadSegments[SWM_DATA_MAX_NOAR];  /* PRQA S 3218 */ /* MD_FblDiag_3218 */
#if defined( FBL_MEM_ENABLE_VERIFY_INPUT )
/** Verification information to be transmitted to security module - input verification */
V_MEMRAM0 static V_MEMRAM1 SecM_SignatureParamType    V_MEMRAM2 inputSigParam; /* PRQA S 3218 */ /* MD_FblDiag_3218 */
/** Length of already verified data */
V_MEMRAM0 static V_MEMRAM1 vuint32                    V_MEMRAM2 inputSigDataLength;  /* PRQA S 3218 */ /* MD_FblDiag_3218 */
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT */
#if defined( FBL_MEM_ENABLE_VERIFY_PROCESSED )
/** Verification information to be transmitted to security module - processed verification */
V_MEMRAM0 static V_MEMRAM1 SecM_SignatureParamType    V_MEMRAM2 procSigParam; /* PRQA S 3218 */ /* MD_FblDiag_3218 */
/** Length of already verified data */
V_MEMRAM0 static V_MEMRAM1 vuint32                    V_MEMRAM2 procSigDataLength;  /* PRQA S 3218 */ /* MD_FblDiag_3218 */
#endif /* FBL_MEM_ENABLE_VERIFY_PROCESSED */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Verification information to be transmitted to security module - pipelined verification */
V_MEMRAM0 static V_MEMRAM1 SecM_SignatureParamType    V_MEMRAM2 pipeSigParam; /* PRQA S 3218 */ /* MD_FblDiag_3218 */
/** Length of already verified data */
V_MEMRAM0 static V_MEMRAM1 vuint32                    V_MEMRAM2 pipeSigDataLength;  /* PRQA S 3218 */ /* MD_FblDiag_3218 */
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

/* Download handling variables */
/** Transfer block information */
V_MEMRAM0 static V_MEMRAM1 tBlockDescriptor     V_MEMRAM2      downloadBlockDescriptor;
/** Number of remaining transfer bytes */
V_MEMRAM0 static V_MEMRAM1 tFblLength           V_MEMRAM2      transferRemainder;
/** Block sequence counter */
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      expectedSequenceCnt;

V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2                    checkMemoryFlag;

#define FBLDIAG_STOP_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Diagnostic handler function call table
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Subfunction table configuration
 **********************************************************************************************************************/

#define FBLDIAG_START_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/** Sub-function defintion for Default Session (01) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableDsc_DefaultSession[] = { kDiagSubDefaultSession };
/** Sub-function defintion for Programming Session (02) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableDsc_ProgSession[] = { kDiagSubProgrammingSession };
/** Sub-function defintion for Extended Session (03) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableDsc_ExtendedSession[] = { kDiagSubExtendedDiagSession };

/** Sub-function definition for Diagnostic Session Control service (10) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableSessionControl[] =
{
   /* Default Session (01) */
   {
      kFblDiagSubtableDsc_DefaultSession,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlDiagnosticSessionControl,
      (tFblDiagLengthCheck)0u,
      FblDiagDefaultSessionMainHandler
   },
   /* Programming Session (02) */
   {
      kFblDiagSubtableDsc_ProgSession,
      (kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlDiagnosticSessionControl,
      (tFblDiagLengthCheck)0u,
      FblDiagProgrammingSessionMainHandler
   },
   /* Extended Session (03) */
   {
      kFblDiagSubtableDsc_ExtendedSession,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest),
      kDiagRqlDiagnosticSessionControl,
      (tFblDiagLengthCheck)0u,
      FblDiagExtendedSessionMainHandler
   }
};

/** Sub-function definition for Hard Reset sub-function (01) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableEcuReset_HardReset[] = { kDiagSubHardReset };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableEcuReset_SoftReset[] = { kDiagSubSoftReset };
/** Sub-function definition for ECU Reset service (11) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableEcuReset[] =
{
   /* Hard Reset (01) */
   {
      kFblDiagSubtableEcuReset_HardReset,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlEcuReset,
      (tFblDiagLengthCheck)0u,
      FblDiagEcuResetMainHandler
   },
   /* software Reset (03) */
   {
      kFblDiagSubtableEcuReset_SoftReset,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlEcuReset,
      (tFblDiagLengthCheck)0u,
      FblDiagEcuResetMainHandler
   }
};

/** Sub-function definition for Request Seed sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableAppSecAccess_Seed[] = { kDiagSubRequestAppSeed };
/** Sub-function definition for Send Key sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableAppSecAccess_Key[] = { kDiagSubSendAppKey };
/** Sub-function definition for Request Seed sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableSecAccess_Seed[] = { kFblDiagSubRequestSeed };
/** Sub-function definition for Send Key sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableSecAccess_Key[] = { kFblDiagSubSendKey };

/** Sub-function definition for Security Access service (27) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableSecAccess[] =
{
   /* Request Seed (configurable) */
   {
      kFblDiagSubtableAppSecAccess_Seed,
      (kFblDiagOptionSessionExtended),
      kDiagRqlSecurityAccessSeed,
      (tFblDiagLengthCheck)0u,
      FblDiagSecAccessSeedMainHandler
   },
   /* Send Key (configurable) */
   {
      kFblDiagSubtableAppSecAccess_Key,
      (kFblDiagOptionSessionExtended),
      kDiagRqlSecurityAccessKey,
      (tFblDiagLengthCheck)0u,
      FblDiagSecAccessKeyMainHandler
   },
   /* Request Seed (configurable) */
   {
      kFblDiagSubtableSecAccess_Seed,
      (kFblDiagOptionSessionProgramming),
      kDiagRqlSecurityAccessSeed,
      (tFblDiagLengthCheck)0u,
      FblDiagSecAccessSeedMainHandler
   },
   /* Send Key (configurable) */
   {
      kFblDiagSubtableSecAccess_Key,
      (kFblDiagOptionSessionProgramming),
      kDiagRqlSecurityAccessKey,
      (tFblDiagLengthCheck)0u,
      FblDiagSecAccessKeyMainHandler
   }
};

/** Sub-function definition for EnableRxAndEnableTx sub-function (00) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableCommControl_EnRxEnTx[] = { kDiagSubEnableRxAndTx };
/** Sub-function definition for Disable sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableCommControl_Disable[] = { FBL_DIAG_COMMUNICATION_CONTROL_TYPE };
/** Sub-function definition for Disable sub-function (configurable) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableCommControl_DisableRxTx[] = { kDiagSubDisableRxAndTx };

/** Sub-function defintion for Communication Control service (28) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableCommControl[] =
{
   /* Enable Rx and Tx (00) */
   {
      kFblDiagSubtableCommControl_EnRxEnTx,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest | kFblDiagOptionSessionProgramming),
      kDiagRqlCommunicationControl,
      (tFblDiagLengthCheck)0u,
      FblDiagCommCtrlMainHandler
   },
   /* Disable communication (configurable) */
   {
      kFblDiagSubtableCommControl_Disable,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest | kFblDiagOptionSessionProgramming),
      kDiagRqlCommunicationControl,
      (tFblDiagLengthCheck)0u,
      FblDiagCommCtrlMainHandler
   },
   /* Disable communication (configurable) */
   {
      kFblDiagSubtableCommControl_DisableRxTx,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest | kFblDiagOptionSessionProgramming),
      kDiagRqlCommunicationControl,
      (tFblDiagLengthCheck)0u,
      FblDiagCommCtrlMainHandler
   }
};

/** Sub-function / RID definition for checksum verification (010202) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableRC_StartChecksum[] = { kDiagSubStartRoutine, kDiagRoutineIdChecksumHigh, kDiagRoutineIdChecksumLow };
#if defined( FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS )
/** Sub-function / RID definition for programming pre-condition check (010203) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableRC_StartProgPreCond[] = { kDiagSubStartRoutine, kDiagRoutineIdCheckProgPreCondHigh, kDiagRoutineIdCheckProgPreCondLow };
#endif /* FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS */
#if defined( FBL_ENABLE_STAY_IN_BOOT )
/** Sub-function / RID definition for force boot mode request (01F518) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableRC_StartForceBoot[] = { kDiagSubStartRoutine, kDiagRoutineIdStayInBootHigh, kDiagRoutineIdStayInBootLow };
#endif /* FBL_ENABLE_STAY_IN_BOOT */
/** Sub-function / RID definition for erase memory request (01FF00) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableRC_StartErase[] = { kDiagSubStartRoutine, kDiagRoutineIdEraseMemoryHigh, kDiagRoutineIdEraseMemoryLow };
/** Sub-function / RID definition for check programming dependencies request (01FF01) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableRC_StartCheckProgDep[] = { kDiagSubStartRoutine, kDiagRoutineIdCheckProgDepHigh, kDiagRoutineIdCheckProgDepLow };

/** Sub-function / RID defintion for Routine Control service (31) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableRoutine[] =
{
   /* Checksum verification (010202) */
   {
      kFblDiagSubtableRC_StartChecksum,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlRoutineControlCheckRoutine,
      (tFblDiagLengthCheck)0u,
      FblDiagRCStartCsumMainHandler
   },
#if defined( FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS )
   /* Check programming pre-conditions (010203) */
   {
      kFblDiagSubtableRC_StartProgPreCond,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest),
      kDiagRqlRoutineControlProgPreCond,
      (tFblDiagLengthCheck)0u,
      FblDiagRCStartCheckProgPreCondMainHandler
   },
#endif /* FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS */
#if defined( FBL_ENABLE_STAY_IN_BOOT )
   /* Force boot mode request (01F518) */
   {
      kFblDiagSubtableRC_StartForceBoot,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming),
      kDiagRqlRoutineControlForceBoot,
      (tFblDiagLengthCheck)0u,
      FblDiagRCStartForceBootModeMainHandler
   },
#endif /* FBL_ENABLE_STAY_IN_BOOT */
   /* Erase memory request (01FF00) */
   {
      kFblDiagSubtableRC_StartErase,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlRoutineControlEraseRoutine,
      FblDiagRCStartEraseLengthCheck,
      FblDiagRCStartEraseMainHandler
   },
   /* Check programming dependencies (01FF01) */
   {
      kFblDiagSubtableRC_StartCheckProgDep,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlRoutineControlCheckProgDep,
      (tFblDiagLengthCheck)0u,
      FblDiagRCStartCsumMainHandler
   }
};

/** Sub-function definition for Zero sub-function (00) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableTesterPresent_ZeroSubfunction[] = { kDiagSubTesterPresent };

/** Sub-function definition for Tester Present service (3E) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableTesterPresent[] =
{
   /* Zero sub-function (00) */
   {
      kFblDiagSubtableTesterPresent_ZeroSubfunction,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlTesterPresent,
      (tFblDiagLengthCheck)0u,
      FblDiagTesterPresentMainHandler
   }
};

/** Sub-function definition for Control DTC - On sub-function (01) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableControlDtc_OnSubfunction[] = { kDiagSubDtcOn };
/** Sub-function definition for Control DTC - Off sub-function (02) */
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kFblDiagSubtableControlDtc_OffSubfunction[] = { kDiagSubDtcOff };

/** Sub-function definition for Control DTC service (85) */
V_MEMROM0 static V_MEMROM1 tFblDiagServiceSubTable V_MEMROM2 kFblDiagSubtableControlDtc[] =
{
   /* On sub-function (01) */
   {
      kFblDiagSubtableControlDtc_OnSubfunction,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest),
      kDiagRqlControlDTCSetting,
      (tFblDiagLengthCheck)0u,
      FblDiagControlDTCMainHandler
   },
   /* Off sub-function (02) */
   {
      kFblDiagSubtableControlDtc_OffSubfunction,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest),
      kDiagRqlControlDTCSetting,
      (tFblDiagLengthCheck)0u,
      FblDiagControlDTCMainHandler
   }
};

/***********************************************************************************************************************
 *  Main service table configuration
 **********************************************************************************************************************/

/** Table of supported diagnostic servcies */
V_MEMROM0 V_MEMROM1 tFblDiagServiceTable V_MEMROM2 kFblDiagServiceTable[] =
{
   /* Diagnostic Session Control (10) */
   {
      kDiagSidDiagnosticSessionControl,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionFunctionalRequest),
      kDiagRqlDiagnosticSessionControl,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableSessionControl),
      ARRAY_SIZE(kFblDiagSubtableDsc_DefaultSession),
      kFblDiagSubtableSessionControl,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   },
   /* ECU Reset (11) */
   {
      kDiagSidEcuReset,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionFunctionalRequest),
      kDiagRqlEcuReset,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableEcuReset),
      ARRAY_SIZE(kFblDiagSubtableEcuReset_HardReset),
      kFblDiagSubtableEcuReset,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   },
   /* Read Data by Identifier (22) */
   {
      kDiagSidReadDataByIdentifier,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionFunctionalRequest),
      kDiagRqlReadDataByIdentifier,
      FblDiagDataByIdLengthCheck,
      0u,
      0u,
      (tFblDiagServiceSubTable*)V_NULL,
      FblDiagDefaultPreHandler,
      FblDiagReadDataByIdMainHandler,
      FblDiagProcessServiceNrc,
      FblDiagDefaultPostHandler
   },
   /* Security Access (27) */
   {
      kDiagSidSecurityAccess,
      (kFblDiagSecurityAccessSessionMask | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming),
      kDiagRqlServiceWithSubfunction,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableSecAccess),
      ARRAY_SIZE(kFblDiagSubtableSecAccess_Seed),
      kFblDiagSubtableSecAccess,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   },
   /* Communication Control (28) */
   {
      kDiagSidCommunicationControl,
      (kFblDiagOptionSessionExtended | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionFunctionalRequest | kFblDiagOptionSessionProgramming),
      kDiagRqlServiceWithSubfunction,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableCommControl),
      ARRAY_SIZE(kFblDiagSubtableCommControl_EnRxEnTx),
      kFblDiagSubtableCommControl,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   },
   /* Write Data by Identifier (2E) */
   {
      kDiagSidWriteDataByIdentifier,
      (kFblDiagOptionSessionProgramming),
      kDiagRqlWriteDataByIdentifier,
      FblDiagDataByIdLengthCheck,
      0u,
      0u,
      (tFblDiagServiceSubTable*)V_NULL,
      FblDiagDefaultPreHandler,
      FblDiagWriteDataByIdMainHandler,
      FblDiagProcessServiceNrc,
      FblDiagDefaultPostHandler
   },
   /* Routine Control (31) */
   {
      kDiagSidRoutineControl,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionFunctionalRequest),
      kDiagRqlRoutineControl,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableRoutine),
      ARRAY_SIZE(kFblDiagSubtableRC_StartChecksum),
      kFblDiagSubtableRoutine,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessRoutineNrc,
      FblDiagDefaultPostHandler
   },
   /* Request Download (34) */
   {
      kDiagSidRequestDownload,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlRequestDownload,
      FblDiagRequestDownloadLengthCheck,
      0u,
      0u,
      (tFblDiagServiceSubTable*)V_NULL,
      FblDiagDefaultPreHandler,
      FblDiagRequestDownloadMainHandler,
      FblDiagProcessServiceNrc,
      FblDiagDefaultPostHandler
   },
   /* Transfer Data (36) */
   {
      kDiagSidTransferData,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlTransferData,
      FblDiagTransferDataLengthCheck,
      0u,
      0u,
      (tFblDiagServiceSubTable*)V_NULL,
      FblDiagDefaultPreHandler,
      FblDiagTransferDataMainHandler,
      FblDiagProcessServiceNrc,
      FblDiagDefaultPostHandler
   },
   /* Request Transfer Exit (37) */
   {
      kDiagSidRequestTransferExit,
      (kFblDiagOptionSessionProgramming | kFblDiagOptionSecuredService),
      kDiagRqlRequestTransferExit,
      (tFblDiagLengthCheck)0u,
      0u,
      0u,
      (tFblDiagServiceSubTable*)V_NULL,
      FblDiagDefaultPreHandler,
      FblDiagReqTransferExitMainHandler,
      FblDiagProcessServiceNrc,
      FblDiagDefaultPostHandler
   },
   /* Tester Present (3E) */
   {
      kDiagSidTesterPresent,
      (kFblDiagOptionSessionDefault | kFblDiagOptionSessionExtended | kFblDiagOptionSessionProgramming | kFblDiagOptionServiceIsSubfunction | kFblDiagOptionFunctionalRequest),
      kDiagRqlTesterPresent,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableTesterPresent),
      ARRAY_SIZE(kFblDiagSubtableTesterPresent_ZeroSubfunction),
      kFblDiagSubtableTesterPresent,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   },
   /* Control DTC (85) */
   {
      kDiagSidControlDTCSetting,
      (kFblDiagOptionSessionExtended | kFblDiagOptionFunctionalRequest | kFblDiagOptionServiceIsSubfunction),
      kDiagRqlServiceWithSubfunction,
      (tFblDiagLengthCheck)0u,
      ARRAY_SIZE(kFblDiagSubtableControlDtc),
      ARRAY_SIZE(kFblDiagSubtableControlDtc_OnSubfunction),
      kFblDiagSubtableControlDtc,
      FblDiagDefaultPreHandler,
      (tFblDiagMainHandler)0u,
      FblDiagProcessSubfunctionNrc,
      FblDiagDefaultPostHandler
   }
};

/** Size of service table */
V_MEMROM0 V_MEMROM1 vuintx V_MEMROM2 kSizeOfServiceTable = ARRAY_SIZE(kFblDiagServiceTable);

#define FBLDIAG_STOP_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  Service support functions
 **********************************************************************************************************************/
 #if(TRUE == ENABLE_INTERNAL_CRC_CHECK)
vuint32 Crc_Table32[256] = 
{
    0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
    0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
    0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
    0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
    0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
    0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
    0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
    0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
    0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
    0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
    0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
    0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
    0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
    0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
    0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
    0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,
    0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
    0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
    0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
    0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
    0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
    0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
    0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
    0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
    0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
    0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
    0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
    0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
    0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
    0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
    0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
    0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,
    0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
    0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
    0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
    0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
    0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
    0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
    0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
    0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
    0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
    0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
    0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
    0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
    0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
    0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
    0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
    0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
    0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
    0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
    0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
    0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
    0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
    0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
    0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
    0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
    0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
    0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
    0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
    0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
    0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
    0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
    0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
    0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D
};

uint8 BlockNum = 0u;
BlockInfo BlockInfoArray[MAX_BLOCK] = {0u};
uint32 CRCByCalcuAllBlock = 0u;

void OemInitPowerOn(void)
{
    uint8 i = 0u;
    
    BlockNum = 0u;
    CRCByCalcuAllBlock = 0u;

    for(i = 0u;i<MAX_BLOCK;i++)
    {
        BlockInfoArray[i].StartAddress = 0u;
        BlockInfoArray[i].Length = 0u;
        BlockInfoArray[i].Crc32 = 0;
        BlockInfoArray[i].Crc32Result = 0;
    }
}

vuint32 CalcCrc32(vuint8 *Crc_DataPtr, vuint32 Crc_Length)
{
	static vuint32 TableIndex;
	static vuint32 Crc_Value;
	static vuint32 Crc_DataPtrAddr;
    Crc_Value = 0xFFFFFFFFu;
    
	for (Crc_DataPtrAddr = 0; Crc_DataPtrAddr < Crc_Length; Crc_DataPtrAddr++)
	{
	    
        TableIndex = (vuint32)((Crc_Value ^ Crc_DataPtr[Crc_DataPtrAddr]) & 0xFF);
		Crc_Value = (Crc_Value >> 8u) ^ Crc_Table32[TableIndex];

		if (Crc_DataPtrAddr & 0xff == 0x00u)
		{
			FblLookForWatchdogVoid();
		}
	}
    return (~Crc_Value) ;
}


void GetFileBlockInfo(uint32 address,uint32 length)
{
    /*BlockNum 0 is flash driver*/
    BlockInfoArray[BlockNum].StartAddress = address;
    BlockInfoArray[BlockNum].Length = length;
}

void GetFileBlockCrc(vuint8 *pbDiagData)
{
    vuint8 CrcFormat = 4u;
    BlockInfoArray[BlockNum].Crc32 = (tFblAddress)FblMemGetInteger(CrcFormat, pbDiagData);    
    //BlockInfoArray[BlockNum].Crc32 = ((vuint32)pbDiagData[0]<<24u | (vuint32)pbDiagData[1]<<16u | (vuint32)pbDiagData[2]<<8u| (vuint32)pbDiagData[3]);
    BlockNum++;
}

boolean CalcCrc32ForAllBlock(void)
{
    uint32 CRC = 0u;
    uint8 i =0u;
    boolean Result = TRUE; 
    for(i = 0u;i<=BlockNum;i++)
    {
            if(BlockInfoArray[i].Crc32Result== TRUE)
            {
                /*The initial value is FALSE,and no second comparison will be made after comparison,because block0 is stored in Ram for flash*/
                Result = TRUE; 
            }
            else
            {
                calculateCRC[i] = CalcCrc32(BlockInfoArray[i].StartAddress,BlockInfoArray[i].Length);
                if(BlockInfoArray[i].Crc32 == calculateCRC[i])
                {
                     Result = TRUE;
                     BlockInfoArray[i].Crc32Result = TRUE;
                }
                else
                {
                     Result = FALSE;
                     BlockInfoArray[i].Crc32Result = FALSE;   
                     break;

                }
            }
    }
    return Result;
}

#if 0

vuint32 CalcCrc32ForAllBlock(void)
{
    uint32 CRC = 0u;
    CRC = CalcCrc32(BlockInfoArray[0].StartAddress,BlockInfoArray[0].Length);
    return CRC;
}
#endif

boolean CheckFileCRCIsOK(void)
{
    boolean Result = FALSE;    
    vuint8 RecvInternalCRCDataArray[4] = {0u};

    #if 0
    if(APP_START_ADDRESS == BlockInfoArray[0u].StartAddress)               /*App Data Start From 0x00092000;Ignore The FlashDriver CRC Check*/
    {
        Result = CalcCrc32ForAllBlock();
    }
    else
    {
        Result = TRUE;
    }
    #endif

    Result = CalcCrc32ForAllBlock();
    
    BlockNum = 1u;                                                          /*Clear BlockNum When The File Transfer Done(Recv 31 01 02 02)*/
    return Result;
}
#endif

#define FBLDIAG_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  FblDiagDownloadCheck
 **********************************************************************************************************************/
/*! \brief       Verify the downloaded data.
 *  \details     Uses the configured security module to verify if the download has been done correctly and
 *               the data is allowed to be downloaded. Also calculates CRC total if enabled.
 *  \pre         Logical block or flash driver has to be downloaded completely.
 *  \param[in]   pbDiagData Pointer to data in the diagBuffer (without SID).
 *  \param[in]   diagReqDataLen Length of data (without SID).
 *  \return      Verification result supplied by the security module.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static vuint8 FblDiagDownloadCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblMemBlockVerifyData verifyData;
   SecM_StatusType verifyErrorCode;
   vuint8 result;
   tFblMemRamData pVerificationData;
   vuintx checksumLength;
#if defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
   vuint8 checksumBuffer[kSecCRCLength + kSecSigLength];
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
# endif
#endif /* FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA */

#if !defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
   checksumLength = (vuintx)diagReqDataLen - 3u;
   pVerificationData = &pbDiagData[kDiagLocFmtRoutineStatus];
#else
   checksumLength = kSecCRCLength + kSecSigLength;

   if (ApplFblGetVerificationData((V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 *)&verifyParam,
      (V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *)checksumBuffer,
      checksumLength) == kFblOk)
   {
      result = kDiagCheckVerificationOk;
      pVerificationData = checksumBuffer;
   }
   else
   {
      result = kDiagCheckVerificationFailed;
   }

   if (result == kDiagCheckVerificationOk)
#endif
   {
# if defined( FBL_MEM_ENABLE_VERIFY_INPUT )
      verifyData.verifyDataInput.length = (tFblLength)checksumLength;
      verifyData.verifyDataInput.data = pVerificationData;
# endif /* FBL_MEM_ENABLE_VERIFY_INPUT */
# if defined( FBL_MEM_ENABLE_VERIFY_PROCESSED )
      verifyData.verifyDataProcessed.length = (tFblLength)checksumLength;
      verifyData.verifyDataProcessed.data = pVerificationData;
# endif /* FBL_MEM_ENABLE_VERIFY_PROCESSED */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      verifyData.verifyDataPipe.length = (tFblLength)checksumLength;
      verifyData.verifyDataPipe.data = pVerificationData;
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

# if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
      verifyData.verifyDataOutput.length = (tFblLength)checksumLength;
      verifyData.verifyDataOutput.data = pVerificationData;
# endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */
   }

    /* Call verification routine */
#if(TRUE == ENABLE_INTERNAL_CRC_CHECK)
      if ((CrcErrorFlag != CRC_CORRECT)||(FALSE == CheckFileCRCIsOK()) || (FblMemBlockVerify(&verifyData, &verifyErrorCode) != kFblMemStatus_Ok))                 /*Task:7278035*/
#else
      if(CrcErrorFlag != CRC_CORRECT)
#endif
     
   {
      result = kDiagCheckVerificationFailed;
   }
   else
   {
      result = kDiagCheckVerificationOk;
   }

   if (result == kDiagCheckVerificationOk)
   {
      if (FblDiagGetTransferTypeFlash())
      {
         (void)FblRealTimeSupport();
#if defined( SEC_ENABLE_CRC_TOTAL )
         (void)ApplFblWriteCRCTotal(&downloadBlockDescriptor,
                                    (vuint32)verifyParam.blockStartAddress,
                                    (vuint32)verifyParam.blockLength,
                                    (vuint32)verifyParam.crcTotal
                                   );
#endif /* SEC_ENABLE_CRC_TOTAL */

         /* Call validation function to inform application */
         if (ApplFblValidateBlock(downloadBlockDescriptor) != kFblOk)
         {
            result = kDiagCheckVerificationFailed;
         }
         else
         {
            result = ApplFblUpdateBlockMac(&downloadBlockDescriptor, &verifyParam.segmentList);
            checkMemoryFlag = 0x01;
         }

         if (result == kFblOk)
         {
            /* Increment programming success counter */
            (void)ApplFblIncProgCounts(downloadBlockDescriptor);
         }

      }
#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
      else /* Download of flash driver */
      {
         /* Send response pending for flash driver initialization */
         DiagExRCRResponsePending(kForceSendRpIfNotInProgress);

         /* Mark flash driver as present */
         FblDiagSetFlashDriverPresent();

         if (FblDiagPrepareFlashDriver() == kFblOk)
         {
            /* Expect download of application next */
            FblDiagSetTransferTypeFlash();
         }
         else
         {
            /* Flash driver initialization failed */
            result = kDiagCheckVerificationFailed;
         }
      }
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagEraseBlock
 **********************************************************************************************************************/
/*! \brief       Erase selected block
 *  \details     Prepare data structure for FblLib_Mem and erase block
 *  \pre         Flash driver initialized, erase pre-conditions have been checked
 *  \param[in]   pBlockDescriptor Pointer to block descriptor structure
 *  \return      Erase result
 **********************************************************************************************************************/
static tFblResult FblDiagEraseBlock( V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 *pBlockDescriptor )
{
   tFblResult result;
   tFblMemBlockInfo blockInfo;

   /* Initialize download block descriptor continued */
   result = FblLbtGetBlockDescriptorByNr(pBlockDescriptor->blockNr, pBlockDescriptor);

   if (result == kFblOk)
   {
      /* Initialize block structure for FblLib_Mem */
      blockInfo.targetAddress = pBlockDescriptor->blockStartAddress;
      blockInfo.targetLength = pBlockDescriptor->blockLength;
      blockInfo.logicalAddress = pBlockDescriptor->blockStartAddress;
      blockInfo.logicalLength = pBlockDescriptor->blockLength;

      /* Start erase by FblLib_Mem */
      if (FblMemBlockEraseIndication(&blockInfo) == kFblMemStatus_Ok)
      {
         /* Erase succeeded */
         FblDiagSetEraseSucceeded();
         /* After erase, the first segment of a logical block will be downloaded */
         /* Adjust the size of the logical block according to presence pattern size. */
         (void)ApplFblAdjustLbtBlockData(pBlockDescriptor);

         /* Initialize segment counter */
         FblDiagSegmentInit();
      }
      else
      {
         /* Erase failed */
         result = kFblFailed;
      }
   }

   if (result != kFblOk)
   {
      /* Clear state */
      FblDiagClrEraseSucceeded();
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagCheckErasePreconditions
 **********************************************************************************************************************/
/*! \brief         Verify that all erase preconditions are fulfilled
 *  \pre           Fingerprint is available, memory driver initialized.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[out]    pEraseAddress Start address of erase area
 *  \param[out]    pEraseLength Length of erase area
 *  \return        kFblOk: Preconditions fulfilled; kFblFailed: Preconditions not fulfilled
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagCheckErasePreconditions(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pbDiagData,
                                                 V_MEMRAM1 tFblAddress V_MEMRAM2 V_MEMRAM3 * pEraseAddress,
                                                 V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * pEraseLength)
{
   tFblResult result;

   vuint8 addrFormat;
   vuint8 lengthFormat;

   /* Initialize variables */
   *pEraseAddress = 0u;
   *pEraseLength = 0u;

   if(0u == kDiagRslRoutineControlEraseRoutineParameter)
   {
       addrFormat = 4u;
       lengthFormat = 4u;
   }
   else
   {
          /* Get address and length format from request */
       addrFormat = (vuint8)(pbDiagData[kDiagLocFmtRoutineAlfi] & 0x0Fu);
       lengthFormat = (vuint8)((pbDiagData[kDiagLocFmtRoutineAlfi] & 0xF0u) >> 4u);
   }
   /* Check address and length format */
   if ((addrFormat == 0u) || (lengthFormat == 0u) || (addrFormat > 4u) || (lengthFormat > 4u))
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   /* Check status flags */
   else if (!FblDiagGetFingerprintValid())
   {
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
   else if (FblDiagPrepareFlashDriver() != kFblOk)
   {
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
   else
   {
      /* Get memory address and length */
      *pEraseAddress = FblMemGetInteger(addrFormat, &pbDiagData[kDiagLocFmtStartAddress-1]);
      *pEraseLength = FblMemGetInteger(lengthFormat, &pbDiagData[kDiagLocFmtStartAddress-1 + addrFormat]);

      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagPrepareFlashDriver
 **********************************************************************************************************************/
/*! \brief         Prepare flash driver for usage
 *  \return        Preparation result
 **********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
static tFblResult FblDiagPrepareFlashDriver( void )
{
   tFblResult result;
   tFlashErrorCode flashErrorCode;
#if defined( FBL_DIAG_ENABLE_FLASHDRV_ROM )
   vuintx flashDriverIndex;
   vuint32 byteIndex;
   V_MEMROM1 tFlashDriverInitData V_MEMROM2 V_MEMROM3 * flashDriverInitData;
#endif /* FBL_DIAG_ENABLE_FLASHDRV_ROM */

   /* Initialize variable */
   result = kFblFailed;

   if (!FblDiagGetMemDriverInitialized())
   {
#if defined( FBL_DIAG_ENABLE_FLASHDRV_ROM )
      /* Use flash driver from image in case no driver has been downloaded */
      if (!FblDiagGetFlashDriverPresent())
      {
         /* Copy data of all configured flash drivers */
         for (flashDriverIndex = 0u; flashDriverIndex < ARRAY_SIZE(kFlashDriverInitData); flashDriverIndex++)
         {
            flashDriverInitData = &kFlashDriverInitData[flashDriverIndex];

            /* Check if target buffer is big enough for data */
            if (flashDriverInitData->copySize > (vuint32)FLASH_SIZE)
            {
               /* Data doesn't fit into buffer. Assumes that all buffers are configured to the same size
                * if more than one buffer is used.
               */
               FblErrStatSetError(FBL_ERR_FLASHCODE_EXCEEDS_MEMORY);
               break;
            }

            for (byteIndex = 0u; byteIndex < flashDriverInitData->copySize; byteIndex++)
            {
               flashDriverInitData->destBuffer[byteIndex] = (flashDriverInitData->decryptValue ^ flashDriverInitData->sourceBuffer[byteIndex]);
               if ((byteIndex & 0xFFu) == 0x00u)
               {
                  FblLookForWatchdogVoid();
               }
            }
         }

         /* Check if all flash drivers have been copied */
         if (flashDriverIndex == ARRAY_SIZE(kFlashDriverInitData))
         {
            FblDiagSetFlashDriverPresent();
         }
      }
#endif /* FBL_DIAG_ENABLE_FLASHDRV_ROM */

      if (FblDiagGetFlashDriverPresent())
      {
         /* Initialize version */
         FblErrStatSetFlashDrvVersion();  /* PRQA S 3109 */ /* MD_FblDiag_3109 */

         /* Turn on programming voltage (if necessary) */
         ApplFblSetVfp();

         /* Initialze memory drivers */
         FblCwSetOfflineMode();
         /* Check version and initialize flash driver */
         flashErrorCode = MemDriver_InitSync(V_NULL);
         FblCwSetOnlineMode();

         if (flashErrorCode == IO_E_OK)
         {
            FblDiagSetMemDriverInitialized();
            result = kFblOk;
         }
         else
         {
            FblErrStatSetError(FBL_ERR_FLASHCODE_INIT_FAILED);
            FblErrStatSetFlashDrvError(flashErrorCode);
         }
      }
   }
   else
   {
      /* Flash driver already initialized */
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagCheckProgAttemptCounter
 **********************************************************************************************************************/
/*! \brief         Check if programming attempt counter allows another programming cycle
 *  \pre           Response pending processing has been started, block has been selected
 *  \param[in]     pBlockDescriptor Pointer to current download block descriptor
 *  \return        kFblOk: Programming allowed; kFblFailed: Programming not allowed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagCheckProgAttemptCounter(V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 *pBlockDescriptor)
{
   tFblResult result;
   vuint16 progAttempts;

   /* Initialize variables */
   result = kFblOk;

   /* Initialize variables */
   if (ApplFblGetProgAttempts(*pBlockDescriptor, &progAttempts) != kFblOk)
   {
      /* Programming attempt counter couldn't be read. Use 0 as default value. */
      progAttempts = 0u;
   }

   /* Check if logical block does not restrict the number of programming attempts */
   if (ApplFblGetPromMaxProgAttempts(pBlockDescriptor->blockNr) != 0u)
   {
      /* Check logical block programming attempts */
      if (progAttempts >= ApplFblGetPromMaxProgAttempts(pBlockDescriptor->blockNr))
      {
         /* Logical block cannot be programmed anymore */
         FblErrStatSetError(FBL_ERR_FLASH_PROG_CONDITIONS);
         DiagNRCConditionsNotCorrect();
         result = kFblFailed;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagCheckRequestDownloadPreconditions
 **********************************************************************************************************************/
/*! \brief         Verify that all request download preconditions are fulfilled
 *  \pre           Fingerprint is available
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[out]    pSegmentInfo FblLib_Mem segment information for this download
 *  \return        kFblOk: Preconditions fulfilled; kFblFailed: Preconditions not fulfilled
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagCheckRequestDownloadPreconditions(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pbDiagData,
                                                           V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo)
{
   tFblResult result;

   vuint8  lengthFormat;
   vuint8  addrFormat;

   /* Initialize variables */
   pSegmentInfo->logicalAddress = 0u;
   pSegmentInfo->logicalLength = 0u;
   pSegmentInfo->targetAddress = 0u;
   pSegmentInfo->targetLength = 0u;

   /* Get length and address format from message */
   lengthFormat = (vuint8)((pbDiagData[kDiagLocFmtFormatOffset] & 0xF0u) >> 4u);
   addrFormat = (vuint8) (pbDiagData[kDiagLocFmtFormatOffset] & 0x0Fu);
   /* Read compression and encryption method */
   pSegmentInfo->dataFormat = (tFblMemDfi)pbDiagData[kDiagLocFmtSubparam];

   /* Check address and length format */
   if ((addrFormat == 0u) || (lengthFormat == 0u) || (addrFormat > 4u) || (lengthFormat > 4u))
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   else if ((FblDiagGetTransferDataAllowed()) || (!FblDiagGetFingerprintValid()))
   {
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
#if defined( FBL_ENABLE_DATA_PROCESSING )
# if defined( FBL_ENABLE_ENCRYPTION_MODE )
# else
   /* No encrypted data supported */
   else if (FblDiagGetEncryptionMode(pSegmentInfo->dataFormat) != kDiagSubNoEncryption)
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
# endif /* FBL_ENABLE_ENCRYPTION_MODE */
# if defined( FBL_ENABLE_COMPRESSION_MODE )
# else
   /* No compressed data supported */
   else if ((FblDiagGetCompressionMode(pSegmentInfo->dataFormat) != kDiagSubNoCompression)
#  if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      && (!GetOemStreamModeSupported(pSegmentInfo->dataFormat))
#  endif
      )
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
# endif /* FBL_ENABLE_COMPRESSION_MODE */
#else
   else if ((pSegmentInfo->dataFormat != kDiagSubNoDataProcessing)
# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      && (!GetOemStreamModeSupported(pSegmentInfo->dataFormat))
# endif
      )
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
#endif /* FBL_ENABLE_DATA_PROCESSING */
   else
   {
      /* Get memoryAddress */
      pSegmentInfo->logicalAddress = FblMemGetInteger(addrFormat, &(pbDiagData[kDiagLocFmtAddrOffset]));
      /* Get memorySize */
      pSegmentInfo->logicalLength = FblMemGetInteger(lengthFormat, &(pbDiagData[kDiagLocFmtAddrOffset + addrFormat]));

      result = kFblOk;
   }

   return result;
}

#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
/***********************************************************************************************************************
 *  FblDiagCheckForFlashDriverDownload
 **********************************************************************************************************************/
/*! \brief         Check if a flash driver or application should be downloaded
 *  \pre           Download precondition check succeeded
 *  \param[in,out] pSegmentInfo FblLib_Mem segment information for this download
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
static tFblResult FblDiagCheckForFlashDriverDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

# if defined( FLASHCODE_RELOCATABLE ) || \
     defined( FLASH_DRIVER_RELOCATABLE )
   /* For relocatable drivers: If erase succeeded state is set, assume a data download
      is performed. Otherwise, a driver download is assumed. This enables downloads to
      logical blocks starting with address 0x00 in combination with relocatable flash
      drivers.
   */
   if ((pSegmentInfo->logicalAddress == 0u) && (!FblDiagGetEraseSucceeded()))
# else
   if (pSegmentInfo->logicalAddress == FBL_DIAG_FLASH_CODE_BASE_ADDR) /* PRQA S 0306 */ /* MD_FblDiagOem_030x */
# endif /* FLASHCODE_RELOCATABLE || FLASH_DRIVER_RELOCATABLE */
   {
      /* Download of flash driver requested */
      FblDiagClrFlashDriverPresent();
      /* Deinit flash driver in case it is already initialized */
      if (FblDiagGetMemDriverInitialized())
      {
         DiagExRCRResponsePending(kForceSendRpIfNotInProgress);
         FblDiagClrMemDriverInitialized();
         (void)MemDriver_DeinitSync(V_NULL);
      }
      /* Download of flash driver requested */
      FblDiagClrTransferTypeFlash();
      pSegmentInfo->type = kFblMemType_RAM;

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      if (GetOemStreamModeSupported(pSegmentInfo->dataFormat))
      {
         /* Delta download not supported for flash driver download */
         DiagNRCRequestOutOfRange();
         result = kFblFailed;
      }
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
   }
   else
   {
      /* Download of data requested */
      FblDiagSetTransferTypeFlash();

      /* Set target memory type for download */
      pSegmentInfo->type = kFblMemType_ROM;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagCheckFlashDriverDownload
 **********************************************************************************************************************/
/*! \brief         Check if the download request matches the flash driver buffer
 *  \param[in,out] pSegmentInfo FblLib_Mem segment information for this download
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
static tFblResult FblDiagCheckFlashDriverDownload( V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo)
{
   tFblResult result;

   /* Initialize target address. This address is used to store the downloaded data */
#if defined( FLASHCODE_RELOCATABLE ) || \
    defined( FLASH_DRIVER_RELOCATABLE )
   pSegmentInfo->targetAddress = FBL_DIAG_FLASH_CODE_BASE_ADDR; /* PRQA S 0306 */ /* MD_FblDiagOem_030x */
#else
   pSegmentInfo->targetAddress = pSegmentInfo->logicalAddress;
#endif /* FLASHCODE_RELOCATABLE || FLASH_DRIVER_RELOCATABLE */

   /* Store download length */
   pSegmentInfo->targetLength = pSegmentInfo->logicalLength;

   /* Check if flash driver fits into flash driver buffer */ /* PRQA S 0306 1 */ /* MD_FblDiagOem_030x */
   if (FblLbtCheckRangeContained(pSegmentInfo->targetAddress, pSegmentInfo->targetLength, FBL_DIAG_FLASH_CODE_BASE_ADDR, FLASH_SIZE) == kFblOk)
   {
      /* Initialize segment counter */
      FblDiagSegmentInit();
      /* Copy data and address of first segment to initialize downloadHeader */
      (void)FblDiagSegmentNext();

      result = kFblOk;
   }
   else
   {
      FblErrStatSetError(FBL_ERR_FLASHCODE_EXCEEDS_MEMORY);
      DiagNRCRequestOutOfRange();

      result = kFblFailed;
   }

   /* Verification routine of first logical block used for flash driver */
   downloadBlockDescriptor.blockNr = 0u;

   return result;
}
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */

/***********************************************************************************************************************
 *  FblDiagCheckFlashMemoryDownload
 **********************************************************************************************************************/
/*! \brief         Check if the download request matches the erased logical block
 *  \param[in,out] pSegmentInfo FblLib_Mem segment information for this download
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
static tFblResult FblDiagCheckFlashMemoryDownload( V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo)
{
   FL_SegmentInfoType const * segment;

   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Memory driver has to be available */
   if (!FblDiagGetMemDriverInitialized())
   {
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      if (GetOemStreamModeSupported(pSegmentInfo->dataFormat))
      {
         if (FblDiagPrepareFlashDriver() != kFblOk)
         {
            DiagNRCConditionsNotCorrect();
            result = kFblFailed;
         }
      }
      else
#endif
      {
         DiagNRCUploadDownloadNotAccepted();
         result = kFblFailed;
      }
   }

   if (result == kFblOk)
   {
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      if (GetOemStreamModeSupported(pSegmentInfo->dataFormat))
      {
         result = FblDiagInitDeltaDownload(pSegmentInfo);
      }
      else
#endif
      {
         /* Memory has to be erased in case of first segment in logical block */
         if ((segmentCount == 0u) && (!FblDiagGetEraseSucceeded()))
         {
            DiagNRCUploadDownloadNotAccepted();
            result = kFblFailed;
         }
         /* Verify addresses (address must be in the area described by downloadBlockDescriptor)
          * this check requires EraseRoutine, because this initializes downloadBlockDescriptor */
         else
         {
            if (  (FblLbtCheckRangeContained(pSegmentInfo->logicalAddress,
                                             pSegmentInfo->logicalLength,
                                             downloadBlockDescriptor.blockStartAddress,
                                             downloadBlockDescriptor.blockLength) != kFblOk)
               || (transferRemainder == 0u)
               )
            {
               FblErrStatSetError(FBL_ERR_LBT_ADDR_MISMATCH);
               DiagNRCRequestOutOfRange();
               result = kFblFailed;
            }
         }
      }
   }

   if (result == kFblOk)
   {
      if (FblDiagSegmentNext() == kSwmOutOfRange)
      {
         FblErrStatSetError(FBL_ERR_TOO_MANY_SEGMENTS_IN_MODULE);
         DiagNRCRequestOutOfRange();
         result = kFblFailed;
      }
      else
      {
         /* Store address/length information of RequestDownload for flash operation */
         pSegmentInfo->targetAddress = pSegmentInfo->logicalAddress;
         pSegmentInfo->targetLength = pSegmentInfo->logicalLength;

         /* Check if requested segment overlaps with previously downloaded data */
         if (segmentCount > 1u)
         {
            /* Download second segment */
            segment = &verifyParam.segmentList.segmentInfo[segmentCount - 2u];

            if (   (pSegmentInfo->targetAddress <= segment->transferredAddress)
                || ((pSegmentInfo->targetAddress - segment->transferredAddress) < segment->length)
               )
            {
               /* New segment is not in order or overlaps previous one */
               DiagNRCRequestOutOfRange();
               result = kFblFailed;
            }
         }
      }
   }

   return result;
}

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
/***********************************************************************************************************************
*  FblDiagInitDeltaDownload
**********************************************************************************************************************/
/*! \brief        Perform initialization steps only required for delta download
*  \param[in,out] pSegmentInfo FblLib_Mem segment information for this download
*  \return        kFblOk/kFblFailed
**********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagInitDeltaDownload(V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Initialize blockDescriptor - is done in Erase for a regular download */
   if (FblLbtGetBlockDescriptorByAddressLength(pSegmentInfo->logicalAddress, pSegmentInfo->logicalLength, &downloadBlockDescriptor) != kFblOk)
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   else
   {
      (void)ApplFblAdjustLbtBlockData(&downloadBlockDescriptor);

      if (ApplFblInvalidateBlock(downloadBlockDescriptor) != kFblOk)
      {
         DiagNRCConditionsNotCorrect();
         result = kFblFailed;
      }
      else
      {
         /* Explicitly erase mask/pattern area to allow re-write of pattern after delta download */
         if (ApplFblErasePatternArea(&downloadBlockDescriptor) != kFblOk)
         {
            /* NRC is set by user function */
            assertFblUser(DiagGetError() != kDiagErrorNone, kFblOemAssertNoNrcSet); /* PRQA S 2214 */ /* MD_FblDiag_2214 */
            result = kFblFailed;
         }
         else
         {
            FblDiagSegmentInit();
         }
      }
   }

   return result;

}
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
/***********************************************************************************************************************
 *  FblDiagPrepareFirstDownloadSegment
 **********************************************************************************************************************/
/*! \brief         Add block structure to FblLib_Mem data
 *  \param[out]    pBlockInfo Logical block information data provided to FblLib_Mem
 *  \param[in]     pSegmentInfo Segment information of requested download
 *  \param[in]     tempBlockNr Logical index of verification routine for this download
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
/* PRQA S 3673 3 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagPrepareFirstDownloadSegment(V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * pBlockInfo,
                                                     V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentInfo,
                                                     vuint8 tempBlockNr)
{
   tFblResult result;
   tFblDiagNrc libMemResult;
   tBlockDescriptor localBlockDescriptor;
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
# if !defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
   /* Parameters not used: avoid compiler warning */
   (void)pSegmentInfo;  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
# endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */


   /* Initialize variables */
   result = FblLbtGetBlockDescriptorByNr(tempBlockNr, &localBlockDescriptor);

   /* Info: Flash driver uses verification routines of first logical block */
   if (result == kFblOk)
   {
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
      pBlockInfo->verifyRoutineOutput.function = (tFblMemVerifyFctOutput)localBlockDescriptor.verifyOutput;       /* PRQA S 0313 */ /* MD_FblDiagOem_0313 */
      pBlockInfo->verifyRoutineOutput.param = (V_MEMRAM1 tFblMemVerifyParamOutput V_MEMRAM2 V_MEMRAM3 *)&verifyParam;
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */
#if defined( FBL_MEM_ENABLE_VERIFY_INPUT )
      /* Configure input verification */
      pBlockInfo->verifyRoutineInput.function = (tFblMemVerifyFctInput)localBlockDescriptor.verifyInput;          /* PRQA S 0313 */ /* MD_FblDiagOem_0313 */
      pBlockInfo->verifyRoutineInput.param = (V_MEMRAM1 tFblMemVerifyParamInput V_MEMRAM2 V_MEMRAM3 *)&inputSigParam;
      /* Force use of internal workspace */
      inputSigParam.currentHash.sigResultBuffer = (SecM_ResultBufferType)SEC_DEFAULT_WORKSPACE; /* PRQA S 0306 */ /* MD_FblDiagOem_030x */
      inputSigParam.currentHash.length = 0u;
      inputSigParam.currentDataLength = &inputSigDataLength;
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT */
#if defined( FBL_MEM_ENABLE_VERIFY_PROCESSED )
      /* Configure processed verification */
      pBlockInfo->verifyRoutineProcessed.function = (tFblMemVerifyFctInput)localBlockDescriptor.verifyProcessed;  /* PRQA S 0313 */ /* MD_FblDiagOem_0313 */
      pBlockInfo->verifyRoutineProcessed.param = (V_MEMRAM1 tFblMemVerifyParamInput V_MEMRAM2 V_MEMRAM3 *)&procSigParam;
      /* Force use of internal workspace */
      procSigParam.currentHash.sigResultBuffer = (SecM_ResultBufferType)SEC_DEFAULT_WORKSPACE; /* PRQA S 0306 */ /* MD_FblDiagOem_030x */
      procSigParam.currentHash.length = 0u;
      procSigParam.currentDataLength = &procSigDataLength;
#endif /* FBL_MEM_ENABLE_VERIFY_PROCESSED */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      /* Configure pipelined verification */
      pBlockInfo->verifyRoutinePipe.function = (tFblMemVerifyFctInput)localBlockDescriptor.verifyPipelined;       /* PRQA S 0313 */ /* MD_FblDiagOem_0313 */
      pBlockInfo->verifyRoutinePipe.param = (V_MEMRAM1 tFblMemVerifyParamInput V_MEMRAM2 V_MEMRAM3 *)&pipeSigParam;
      /* Force use of internal workspace */
      pipeSigParam.currentHash.sigResultBuffer = (SecM_ResultBufferType)SEC_DEFAULT_WORKSPACE; /* PRQA S 0306 */ /* MD_FblDiagOem_030x */
      pipeSigParam.currentHash.length = 0u;
      pipeSigParam.currentDataLength = &pipeSigDataLength;
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
      pBlockInfo->segmentList = &verifyParam.segmentList;
      pBlockInfo->maxSegments = SWM_DATA_MAX_NOAR;
      verifyParam.segmentList.segmentInfo = downloadSegments;

      /* Prepare block data structure */
      if( FblDiagGetTransferTypeFlash())
      {
         pBlockInfo->targetAddress = downloadBlockDescriptor.blockStartAddress;
         pBlockInfo->targetLength = downloadBlockDescriptor.blockLength;
         pBlockInfo->logicalAddress = downloadBlockDescriptor.blockStartAddress;
         pBlockInfo->logicalLength = downloadBlockDescriptor.blockLength;

         pBlockInfo->readFct = (tFblMemVerifyReadFct)FblReadProm;
      }
#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
      else
      {
         pBlockInfo->targetAddress = pSegmentInfo->targetAddress;
         pBlockInfo->targetLength = pSegmentInfo->targetLength;
         pBlockInfo->logicalAddress = pSegmentInfo->logicalAddress;
         pBlockInfo->logicalLength = pSegmentInfo->logicalLength;

         pBlockInfo->readFct = (tFblMemVerifyReadFct)FblReadRam;
      }
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */

      /* Add block to FblLib_Mem state machine */
      libMemResult = FblMemRemapStatus(FblMemBlockStartIndication(pBlockInfo));
      /* Check if FblLib_Mem reported an error */
      if (libMemResult != kDiagErrorNone)
      {
         FblDiagSetError(libMemResult);
         result = kFblFailed;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagSessionControlParamInit
 **********************************************************************************************************************/
/*! \brief         Initialization of session control request response.
 *  \param[in,out] pbDiagData Pointer to data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 **********************************************************************************************************************/
static void FblDiagSessionControlParamInit(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   /* Reset internal states */
   FblDiagClrPreconditionsChecked();
   FblDiagClrSecurityKeyAllowed();
   FblDiagClrSecurityUnlock();
   FblDiagClrAppSecurityUnlock();
   FblDiagClrFingerprintValid();
   FblDiagClrEraseSucceeded();
   FblDiagClrTransferDataAllowed();
   FblDiagClrTransferDataSucceeded();
   FblDiagClrChecksumAllowed();

   /* Reset variables */
   transferRemainder = 0u;
   checkMemoryFlag = 0u;
   /* Initialize flash driver download handling */
   FblDiagClrTransferTypeFlash();

   /* Initialize segment counter */
   FblDiagSegmentInit();

   /* Initialize security module */
   (void)ApplFblSecurityInit();

   /* Prepare diagnostic response */
   pbDiagData[kDiagLocFmtSubparam + 1u] = (vuint8)((kDiagSessionTimingP2 >> 8u) & 0xFFu);
   pbDiagData[kDiagLocFmtSubparam + 2u] = (vuint8)(kDiagSessionTimingP2 & 0xFFu);
   pbDiagData[kDiagLocFmtSubparam + 3u] = (vuint8)((kDiagSessionTimingP2Star >> 8u) & 0xFFu);
   pbDiagData[kDiagLocFmtSubparam + 4u] = (vuint8)(kDiagSessionTimingP2Star & 0xFFu);
}

/***********************************************************************************************************************
 *  FblDiagOemInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Initialize module variables
 **********************************************************************************************************************/
void FblDiagOemInitPowerOn(void)
{
   expectedSequenceCnt = 0;
}

/***********************************************************************************************************************
 *  FblDiagProcessServiceNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done against the service table can be mapped to NRCs here. This function can be
 *               used as default NRC handler for services without sub-table entry.
 *  \param[in]   serviceErrorMask Error bit mask of table check
 *  \return      kFblOk/kFblFailed
 **********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
static tFblResult FblDiagProcessServiceNrc(vuint32 serviceErrorMask)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Check if service should be supported functionally and drop support if not */
   if ((serviceErrorMask & kFblDiagServiceNoFuncSupport) != 0u)
   {
      DiagSetNoResponse();
   }
   else
   {
      /* Service supported in active session? */
      if ((serviceErrorMask & kFblDiagServiceSessionFailure) == kFblDiagServiceSessionFailure)
      {
         DiagNRCServiceNotSupportedInActiveSession();
      }
      /* Minimum length check on service level */
      else if ((serviceErrorMask & kFblDiagServiceMinLenFailure) == kFblDiagServiceMinLenFailure)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      /* Check for security access */
      else if ((serviceErrorMask & kFblDiagServiceSecAccessFailure) == kFblDiagServiceSecAccessFailure)
      {
         DiagNRCSecurityAccessDenied();
      }
      /* Pending length checks */
      else if ((serviceErrorMask & kFblDiagServiceLenFailure) == kFblDiagServiceLenFailure)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      else if ((serviceErrorMask & kFblDiagServicePrehandlerFailure) == kFblDiagServicePrehandlerFailure)
      {
         /* Set default NRC if no NRC has been set in callout function. */
         if (DiagGetError() == kDiagErrorNone)
         {
            DiagNRCConditionsNotCorrect();
         }
      }
      /* Remaining, uncovered errors */
      else
      {
         DiagNRCGeneralReject();
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagProcessSubfunctionNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done against the service table can be mapped to NRCs here. This function can be
 *               used as default NRC handler for services with sub-functions.
 *  \param[in]   serviceErrorMask Error bit mask of table check
 *  \return      kFblOk/kFblFailed
 **********************************************************************************************************************/
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
static tFblResult FblDiagProcessSubfunctionNrc(vuint32 serviceErrorMask)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Check if service should be supported functionally and drop support if not */
   if ((serviceErrorMask & (kFblDiagServiceNoFuncSupport | kFblDiagSubNoFuncSupport)) != 0u)
   {
      DiagSetNoResponse();
   }
   else
   {
      /* Service supported in active session? */
      if ((serviceErrorMask & kFblDiagServiceSessionFailure) == kFblDiagServiceSessionFailure)
      {
         DiagNRCServiceNotSupportedInActiveSession();
      }
      /* Minimum length check on service level */
      else if ((serviceErrorMask & kFblDiagServiceMinLenFailure) == kFblDiagServiceMinLenFailure)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      /* Check if sub-function is supported */
      else if ((serviceErrorMask & kFblDiagSubfunctionNotFound) == kFblDiagSubfunctionNotFound)
      {
#if defined( FBL_ENABLE_USERSUBFUNCTION )
         /* Subfunction not found in check
          * Call user handler to manage its own subfunction */
         FblDiagSetRcrRpAllowed();
         ApplDiagUserSubFunction(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);
#else
         DiagNRCSubFunctionNotSupported();
#endif /* FBL_ENABLE_USERSUBFUNCTION */
      }
      /* Check if sub-parameter is supported */
      else if ((serviceErrorMask & kFblDiagSubparamNotFound) == kFblDiagSubparamNotFound)
      {
         DiagNRCRequestOutOfRange();
      }
      /* Check for security access (service level and sub-function level) */
      else if ((serviceErrorMask & (kFblDiagServiceSecAccessFailure | kFblDiagSubfuncSecAccessFailure | kFblDiagSubparamSecAccessFailure)) != 0u)
      {
         DiagNRCSecurityAccessDenied();
      }
      /* Pending length checks */
      else if ((serviceErrorMask & (kFblDiagServiceLenFailure | kFblDiagSubMinLenFailure | kFblDiagSubLenFailure)) != 0u)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      /* Subfunction session check */
      else if ((serviceErrorMask & (kFblDiagSubfuncSessionFailure | kFblDiagSubparamSessionFailure)) != 0u)
      {
         DiagNRCSubfunctionNotSupportedInActiveSession();
      }
      else if ((serviceErrorMask & kFblDiagServicePrehandlerFailure) == kFblDiagServicePrehandlerFailure)
      {
         /* Set default NRC if no NRC has been set in callout function. */
         if (DiagGetError() == kDiagErrorNone)
         {
            DiagNRCConditionsNotCorrect();
         }
      }
      /* Remaining, uncovered errors */
      else
      {
         DiagNRCGeneralReject();
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagProcessRoutineNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done against the service table can be mapped to NRCs here. This function can
 *               be used as a NRC hanler for the routine control service.
 *  \param[in]   serviceErrorMask Error bit mask of table check
 *  \return      kFblOk/kFblFailed
 **********************************************************************************************************************/
/* PRQA S 6030, 6080 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF */
static tFblResult FblDiagProcessRoutineNrc(vuint32 serviceErrorMask)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Check if service should be supported functionally and drop support if not */
   if ((serviceErrorMask & (kFblDiagServiceNoFuncSupport | kFblDiagSubNoFuncSupport)) != 0u)
   {
      DiagSetNoResponse();
   }
   else
   {
      /* Service supported in active session? */
      if ((serviceErrorMask & kFblDiagServiceSessionFailure) == kFblDiagServiceSessionFailure)
      {
         DiagNRCServiceNotSupportedInActiveSession();
      }
      /* Minimum length check on service level */
      else if ((serviceErrorMask & kFblDiagServiceMinLenFailure) == kFblDiagServiceMinLenFailure)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      /* Check if sub-parameter is supported */
      else if ((serviceErrorMask & kFblDiagSubparamNotFound) == kFblDiagSubparamNotFound)
      {
#if defined( FBL_ENABLE_USERROUTINE )
         /* Subfunction not found in check
          * Call user handler to manage its own subfunction */
         FblDiagSetRcrRpAllowed();
         ApplDiagUserRoutine(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);
#else
         DiagNRCRequestOutOfRange();
#endif /* FBL_ENABLE_USERSUBFUNCTION */
      }
      /* Check if sub-function is supported */
      else if ((serviceErrorMask & kFblDiagSubfunctionNotFound) == kFblDiagSubfunctionNotFound)
      {
#if defined( FBL_ENABLE_USERSUBFUNCTION )
         /* Subfunction not found in check
          * Call user handler to manage its own subfunction */
         FblDiagSetRcrRpAllowed();
         ApplDiagUserSubFunction(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);
#else
         DiagNRCSubFunctionNotSupported();
#endif /* FBL_ENABLE_USERSUBFUNCTION */
      }
      /* Check for security access (service level and sub-function level) */
      else if ((serviceErrorMask & (kFblDiagServiceSecAccessFailure | kFblDiagSubfuncSecAccessFailure | kFblDiagSubparamSecAccessFailure)) != 0u)
      {
         DiagNRCSecurityAccessDenied();
      }
      /* Pending length checks */
      else if ((serviceErrorMask & (kFblDiagServiceLenFailure | kFblDiagSubMinLenFailure | kFblDiagSubLenFailure)) != 0u)
      {
         DiagNRCIncorrectMessageLengthOrInvalidFormat();
      }
      /* Subfunction session check */
      else if ((serviceErrorMask & (kFblDiagSubfuncSessionFailure | kFblDiagSubparamSessionFailure)) != 0u)
      {
         DiagNRCSubfunctionNotSupportedInActiveSession();
      }
      else if ((serviceErrorMask & kFblDiagServicePrehandlerFailure) == kFblDiagServicePrehandlerFailure)
      {
         /* Set default NRC if no NRC has been set in callout function. */
         if (DiagGetError() == kDiagErrorNone)
         {
            DiagNRCConditionsNotCorrect();
         }
      }
      /* Remaining, uncovered errors */
      else
      {
         DiagNRCGeneralReject();
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  Diagnostic main handler service functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagDefaultSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Default session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: Always kFblOk.
 **********************************************************************************************************************/
static tFblResult FblDiagDefaultSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( FBL_ENABLE_RESPONSE_AFTER_RESET )
   vuint8 responseFlag;
#endif /* FBL_ENABLE_REPONSE_AFTER_RESET */

   if (FblDiagGetProgrammingSession())
   {
      /* Request bootloader reset */
      FblDiagSetWaitEcuReset();   /* PRQA S 3109 */ /* MD_FblDiag_3109 */

#if defined( FBL_ENABLE_RESPONSE_AFTER_RESET )
      /* Send response pending before reset */
      DiagExRCRResponsePending(kForceSendResponsePending);

      /* Set flag for response after reset */
      responseFlag = RESET_RESPONSE_SDS_REQUIRED;
      (void)ApplFblWriteResetResponseFlag(&responseFlag);   /* PRQA S 3425 */ /* MD_FblDiag_3425 */
      /* Save target address */
      if (FblCwSaveResponseAddress() != kFblOk) /* PRQA S 2742 */ /* MD_FblDiagOem_2742 */
      {
         /* If clause is necessary here to process return value or macro. However,
          * there is currently no strategy to handle this error. */
      }
#else
      /* Prepare response and reset states */
      FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
      /* Send response */
      DiagProcessingDone(kDiagRslDiagnosticSessionControl);
#endif /* FBL_ENABLE_RESPONSE_AFTER_RESET */
      FblDiagSetEcuResetFctFinished();
   }
   else
   {
      /* Change to default session without issuing a reset */
      FblDiagSetDefaultSession();   /* PRQA S 3109 */ /* MD_FblDiag_3109 */

      /* Prepare response and reset states */
      FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
      /* Send response */
      DiagProcessingDone(kDiagRslDiagnosticSessionControl);
   }

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagExtendedSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Extended session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: Always kFblOk.
 **********************************************************************************************************************/
static tFblResult FblDiagExtendedSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   /* Change to extended session */
   FblDiagSetExtendedSession();  /* PRQA S 3109 */ /* MD_FblDiag_3109 */

   /* Prepare response and reset states */
   FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
   /* Send response */
   DiagProcessingDone(kDiagRslDiagnosticSessionControl);

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagProgrammingSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Programming session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: kFblOk if reprogramming conditions are fulfilled, kFblFailed if not.
 **********************************************************************************************************************/
static tFblResult FblDiagProgrammingSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;
   IO_ErrorType memErrorCode;

   /* Check of programming preconditions done? */
#if defined( FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS )
   if ((!FblDiagGetPreconditionsChecked()) && (!FblMainGetStartFromAppl()))
#else
   if (ApplFblCheckProgConditions() != kFblOk)
#endif /* FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS */
   {
      /* Check of programming preconditions failed */
      FblErrStatSetError(FBL_ERR_FLASH_PROG_CONDITIONS);
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
   else
   {

      /* Check of programming preconditions succeeded - switch to programming session */
      result = kFblOk;

      /* Change to programming session */
      FblDiagSetProgrammingSession();  /* PRQA S 3109 */ /* MD_FblDiag_3109 */

      /* JKN specific: Program attempt counter increase when enter programming session(CR:7709991) */
      (void)ApplFblIncProgAttempts();

      /* Prepare response and reset states */
      FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
      /* Send response */
      DiagProcessingDone(kDiagRslDiagnosticSessionControl);
      
      CrcErrorFlag = CRC_CORRECT;                              /*Task:7278035*/  
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagEcuResetMainHandler
 **********************************************************************************************************************/
/*! \brief         Function manages the EcuReset service request
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        Result: Always kFblOk.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagEcuResetMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( FBL_ENABLE_RESPONSE_AFTER_RESET )
   vuint8 responseFlag;
#endif /* FBL_ENABLE_REPONSE_AFTER_RESET */

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)diagReqDataLen;
   (void)pbDiagData;
#endif

   /* Request bootloader reset */
   FblDiagSetWaitEcuReset();   /* PRQA S 3109 */ /* MD_FblDiag_3109 */

#if defined( FBL_ENABLE_RESPONSE_AFTER_RESET )
   /* Send response pending before reset */
   DiagExRCRResponsePending(kForceSendResponsePending);

   /* Set flag for response after reset */
   responseFlag = RESET_RESPONSE_ECURESET_REQUIRED;
   (void)ApplFblWriteResetResponseFlag(&responseFlag);   /* PRQA S 3425 */ /* MD_FblDiag_3425 */
   /* Save target address */
   if (FblCwSaveResponseAddress() != kFblOk) /* PRQA S 2742 */ /* MD_FblDiagOem_2742 */
   {
      /* If clause is necessary here to process return value or macro. However,
       * there is currently no strategy to handle this error. */
   }
#else
   /* Send response */
   DiagProcessingDone(kDiagRslEcuReset);
#endif /* FBL_ENABLE_RESPONSE_AFTER_RESET */
   FblDiagSetEcuResetFctFinished();

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagSecAccessSeedMainHandler
 **********************************************************************************************************************/
/*! \brief         Request Seed request main handler.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagSecAccessSeedMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   /* Initialize variables */
   result = kFblOk;

#if defined( FBL_ENABLE_SEC_ACCESS_DELAY )
   /* Check if security level is locked (delay counter is active) */
   if (GetSecurityAccessDelay() > 0u)
   {
      DiagNRCRequiredTimeDelayNotExpired();
      result = kFblFailed;
   }
#endif /* FBL_ENABLE_SEC_ACCESS_DELAY */

   if (result == kFblOk)
   {
      if ((FblDiagGetSecurityUnlock()) || (FblDiagGetAppSecurityUnlock()))
      {
         
         vuintx i;

         /* ECU is already unlocked, send zero seed */
         for (i = 0; i < kSecSeedLength; i++)
         {
            pbDiagData[kDiagLocFmtSeedKeyStart + i] = 0x00u;
         }
      
      }
      /* Check if security level is already active */
      else
      {
         /* Generate seed by application function */
         if (ApplFblSecuritySeed() == kFblOk)
         {
            /* Accept security key next */
            FblDiagSetSecurityKeyAllowed();
            FblDiagSetAppSecurityKeyAllowed();
         }
         else
         {
            /* Error occurred during seed computation */
            FblErrStatSetError(FBL_ERR_SEED_GENERATION_FAILED);
            DiagNRCConditionsNotCorrect();
            result = kFblFailed;
         }
      }

   }

   /* Send positive response if seed was computed correctly */
   if (result == kFblOk)
   {
      DiagProcessingDone(kDiagRslSecurityAccessSeed);
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagSecAccessKeyMainHandler
 **********************************************************************************************************************/
/*! \brief         Send Key request main handler.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagSecAccessKeyMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)diagReqDataLen;
   (void)pbDiagData;
#endif

   /* Check preconditions for key processing */
   if (    (!FblDiagGetSecurityKeyAllowed())
        || (!FblDiagGetAppSecurityKeyAllowed())
#if defined( FBL_ENABLE_SEC_ACCESS_DELAY )
        || (GetSecurityAccessDelay() > 0u)
#endif /* FBL_EANBLE_SECURITY_ACCESS_DELAY */
      )
   {
      DiagNRCRequestSequenceError();
      result = kFblFailed;
   }
   else
   {
      /* Send response pending */
      DiagExRCRResponsePending(kForceSendResponsePending);

      /* No second key allowed for this seed */
      FblDiagClrSecurityKeyAllowed();
      FblDiagClrAppSecurityKeyAllowed();

      if (pbDiagData[0] == kFblDiagSubSendKey)
      {
         /* Check security access key in application function */
         if (ApplFblSecurityKey() == kFblOk)
         {
            /* Security access successful */
            FblDiagSetSecurityUnlock();
            result = kFblOk;
   #if defined( FBL_ENABLE_SEC_ACCESS_DELAY )
            /* Reset invalid counter */
            secSendKeyInvalid = 0u;
            /* Reset flags in non-volatile memory */
            (void)ApplFblClrSecAccessDelayFlag();
            (void)ApplFblWriteSecAccessInvalidCount(&secSendKeyInvalid);
   #endif /* FBL_ENABLE_SEC_ACCESS_DELAY */
         }
         else
         {
            result = kFblFailed;
   #if defined( FBL_ENABLE_SEC_ACCESS_DELAY )
            /* Read invalid access counter */
            if (ApplFblReadSecAccessInvalidCount(&secSendKeyInvalid) != kFblOk)
            {
               /* If read failed set default value */
               secSendKeyInvalid = (kSecMaxInvalidKeys - 1u);
            }

            secSendKeyInvalid++;

            if (secSendKeyInvalid >= kSecMaxInvalidKeys)
            {
               /* Too many invalid security attempts, enable security access delay */
               SetSecurityAccessDelay();
               (void)ApplFblSetSecAccessDelayFlag();

               DiagNRCExceedNumberOfAttempts();
            }
            else
            {
               /* Write counter to non-volatile memory */
               (void)ApplFblWriteSecAccessInvalidCount(&secSendKeyInvalid);

               DiagNRCInvalidKey();
            }
   #else
            DiagNRCInvalidKey();
   #endif /* FBL_ENABLE_SEC_ACCESS_DELAY */
         }
      }
      else
      {
         if (ApplFblSecurityAppKey() == kFblOk)
         {
            /* Security access successful */
            FblDiagSetAppSecurityUnlock();
            result = kFblOk;
         }
         else
         {
            result = kFblFailed;
         }
      }
   }

   if (result == kFblOk)
   {
      DiagProcessingDone(kDiagRslSecurityAccessKey);
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagCommCtrlMainHandler
 **********************************************************************************************************************/
/*! \brief         Communication Control request main handler.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagCommCtrlMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   /* Check communication type (Bits 1-7 must be 0) */
   if ((pbDiagData[kDiagLocFmtSubparam + 0x01u] > kDiagSubNmAndNormalCommunication)||(pbDiagData[kDiagLocFmtSubparam + 0x01u])<kDiagSubNormalCommunication)
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   else
   {
      /* Simply transmit a positive response message with subfunction parameter */
      DiagProcessingDone(kDiagRslCommunicationControl);
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagDataByIdLengthCheck
 **********************************************************************************************************************/
/*! \brief         Read/WriteDataByIdentifier service dynamic length check.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Length of data (without SID).
 *  \return        kFblOk: Length check OK; kFblFailed: Length check failed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagDataByIdLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   /* Nothing to do here. Length check should be done in corresponding user callback function. */
   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagWriteDataByIdMainHandler
 **********************************************************************************************************************/
/*! \brief         WriteDataByIdentifier service, handling of fingerprint and identification data.
 *  \pre           Request only accepted after successful security access procedure.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Length of data (without SID).
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagWriteDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

   /* Initialize variables */
   result = kFblOk;

   /* Callback function for WDBI */
   if (ApplFblWriteDataByIdentifier(pbDiagData, diagReqDataLen) == kDiagReturnValidationOk)
   {
      /* if fingerprint is completely written, set valid */
      FblDiagSetFingerprintValid();
   }

   if (DiagGetError() != kDiagErrorNone)
   {
      /* NRC already set - do nothing */
      result = kFblFailed;
   }
   else
   {
      /* Send positive response */
      DiagProcessingDone(kDiagRslWriteDataByIdentifier);
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRCStartCsumMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Check Memory.
 *  \pre           Data to be checked has been downloaded to the corresponding memory.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCStartCsumMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblDiagNrc serviceNrc;
   vuint8 checkResult;
   tFblResult result;

   /* Initialize variables */
   serviceNrc = kDiagErrorNone;
   checkResult = kDiagCheckVerificationFailed;

   /* Check state flags */
   if (    (!FblDiagGetFingerprintValid())
        || (!FblDiagGetTransferDataSucceeded())
        || (!FblDiagGetChecksumAllowed())
      )
   {
      serviceNrc = kDiagNrcConditionsNotCorrect;
   }

   if (serviceNrc == kDiagErrorNone)
   {

      /* Invoke LibMem block end indication */
      serviceNrc = FblMemRemapStatus(FblMemBlockEndIndication());
   }

   if (serviceNrc == kDiagErrorNone)
   {
      /* Watchdog and response pending handling */
      (void)FblRealTimeSupport();

      /* Do verification */
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
      (void)SecM_InitVerification(V_NULL);
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */

      /* Avoid P2 timeout during invalidation and erasure */
      DiagExRCRResponsePending(kForceSendResponsePending);

      checkResult = FblDiagDownloadCheck(pbDiagData, diagReqDataLen);
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
      (void)SecM_DeinitVerification(V_NULL);
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */
   }
   else
   {
      /* Prepare NRC possibly set before */
      FblDiagSetError(serviceNrc);
   }

   if (DiagGetError() != kDiagErrorNone)
   {
      /* NRC already set - do nothing */
      result = kFblFailed;
   }
   else
   {
      /* No NRC set - submit positive response */
      if (result == kDiagCheckVerificationFailed)
      {
         DiagNRCConditionsNotCorrect();
      }
      else
      {
         if(0x01 == checkMemoryFlag)
         {
            checkResult = kFblFailed;
            checkResult = ApplFblCheckProgDependencies();
         }

         /* No NRC set - submit positive response */
         pbDiagData[kDiagLocFmtRoutineStatus] = checkResult;
         DiagProcessingDone(kDiagRslRoutineControlCheckRoutine);
      }
   }
   return result;
}

/***********************************************************************************************************************
 *  FblDiagRCStartEraseLengthCheck
 **********************************************************************************************************************/
/*! \brief         Routine Control - Erase Memory length check
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: Length of erase request is OK; kFblFailed: Length check failed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagRCStartEraseLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

   vuint8 addrFormat;
   vuint8 lengthFormat;

   /* Get length and address format from message */
   lengthFormat = (vuint8)((pbDiagData[kDiagLocFmtRoutineAlfi] & 0xF0u ) >> 4u);
   addrFormat   = (vuint8)(pbDiagData[kDiagLocFmtRoutineAlfi] & 0x0Fu);

   /* Check length of request against calculated length */
   if( diagReqDataLen != ((tCwDataLengthType)kDiagRqlRoutineControlEraseRoutine + lengthFormat + addrFormat))
   {
      result = kFblFailed;
   }
   else
   {
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRCStartEraseMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Erase Memory.
 *  \pre           Fingerprint is available, memory driver initialized.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 6010, 6030, 6050 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */
static tFblResult FblDiagRCStartEraseMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblAddress memoryAddress;
   tFblLength memorySize;
   tBlockDescriptor tempBlockDescriptor = {0};
   tFblResult result;

   vuint32 TransferTypeFlash = FblDiagGetTransferTypeFlash();

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   /* If an erase is triggered, clear checksum verification flag.
    * This protect against tentatives of UDS command injection.
    */
   FblDiagClrChecksumAllowed();
   /* Verify all erase preconditions are fulfilled */
   result = FblDiagCheckErasePreconditions(pbDiagData, &memoryAddress, &memorySize);

   if (result == kFblOk)
   {
      /* Check if address exists in logical block table */
      if (FblLbtGetBlockNrByAddressLength(memoryAddress, memorySize, &tempBlockDescriptor.blockNr) != kFblOk)
      {
         FblErrStatSetError(FBL_ERR_LBT_BLOCK_INDEX_EXCEEDED);
         DiagNRCRequestOutOfRange();
         result = kFblFailed;
      }
      else
      {
         /* Do some additional checks to verify if block can be erased */
         /* Initialize error status block number */
         FblErrStatSetBlockNr(tempBlockDescriptor.blockNr);
      }
   }

   if (result == kFblOk)
   {
      /* Send response pending in case of long NV accesses */
      DiagExRCRResponsePending(kForceSendResponsePending);

      result = FblDiagCheckProgAttemptCounter(&tempBlockDescriptor);
   }

   if (result == kFblOk)
   {
      (void)FblRealTimeSupport();

      /* Invalidate logical block to be erased */
      if (ApplFblInvalidateBlock(tempBlockDescriptor) != kFblOk)
      {
         /* Block could not be invalidated or programming attempt counter could not be stored */
         DiagNRCConditionsNotCorrect();
         result = kFblFailed;
      }
   }

   if (result == kFblOk)
   {
      result = FblDiagEraseBlock(&tempBlockDescriptor);
      if (result == kFblOk)
      {
         /* Initialize downloadBlockDescriptor only after successful erase */
         downloadBlockDescriptor = tempBlockDescriptor;
         /* Prepare positive response */
         pbDiagData[kDiagLocFmtRoutineStatus] = kDiagEraseMemoryOk;
      }
      else
      {
         /* Prepare positive response with error code*/
         pbDiagData[kDiagLocFmtRoutineStatus] = kDiagEraseMemoryFailed;
      }
   }

   /* Send positive response in case no NRC is set */
   if (DiagGetError() == kDiagErrorNone)
   {
        DiagProcessingDone(kDiagRslRoutineControlEraseRoutine);
   }

   if (TransferTypeFlash)
	{
		 FblMemSetErase();
	}

   return result;
}

#if defined( FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS )
/***********************************************************************************************************************
 *  FblDiagRCStartCheckProgPreCondMainHandler
 **********************************************************************************************************************/
/*! \brief         Check Programming Preconditions service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagRCStartCheckProgPreCondMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
# endif

   /* Clear status variables */
   FblDiagClrFingerprintValid();

   /* Check for preconditions - response should be prepared in callback function */
   if (ApplFblCheckProgConditions() == kFblOk)
   {
      FblDiagSetPreconditionsChecked();
   }
   else
   {
      FblDiagClrPreconditionsChecked();
   }

   /* DiagProcessingDone is called from callback function */
   return kFblOk;
}
#endif /* FBL_DIAG_ENABLE_CHECK_PROGRAMMING_PRECONDITIONS */

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/***********************************************************************************************************************
 *  FblDiagRCStartForceBootModeMainHandler
 **********************************************************************************************************************/
/*! \brief         Check for Force Boot Mode message
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagRCStartForceBootModeMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   /* Nothing to do here - just send positive response */
   DiagProcessingDone(kDiagRslRoutineControlStayInBoot);

   return kFblOk;
}
#endif /* FBL_EANBLE_STAY_IN_BOOT */

/***********************************************************************************************************************
 *  FblDiagRCStartCheckProgDepMainHandler
 **********************************************************************************************************************/
/*! \brief         Check program dependencies
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCStartCheckProgDepMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   if ((!FblDiagGetFingerprintValid()) || (!FblDiagGetMemDriverInitialized()))
   {
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
   else
   {
      /* Call function to check programming dependencies */
      pbDiagData[kDiagLocFmtRoutineStatus] = ApplFblCheckProgDependencies();

      /* Call validation function to inform application */
      if (ApplFblValidateBlock(downloadBlockDescriptor) != kFblOk)
      {
         result = kDiagCheckVerificationFailed;
      }

      DiagProcessingDone(kDiagRslRoutineControlCheckRoutine);
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRequestDownloadLengthCheck
 **********************************************************************************************************************/
/*! \brief         Request download length check
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: Length of check request is OK; kFblFailed: Length check failed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagRequestDownloadLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

   vuint8 addrFormat;
   vuint8 lengthFormat;

   /* Get length and address format from message */
   lengthFormat = (vuint8)((pbDiagData[kDiagLocFmtFormatOffset] & 0xF0u) >> 4u);
   addrFormat   = (vuint8)(pbDiagData[kDiagLocFmtFormatOffset] & 0x0Fu);

   /* Check length of request against calculated length */
   if(diagReqDataLen != ((tCwDataLengthType)kDiagRqlRequestDownload + lengthFormat + addrFormat))
   {
      result = kFblFailed;
   }
   else
   {
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRequestDownloadMainHandler
 **********************************************************************************************************************/
/*! \brief         Request download service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
static tFblResult FblDiagRequestDownloadMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblMemBlockInfo blockInfo;
   tFblMemSegmentInfo segmentInfoLocal;
   tFblResult result;
   tFblDiagNrc libMemResult;
   vuint8 * localPbDiagData;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   /* Initialize variables */
   segmentInfoLocal.logicalAddress = 0u;
   segmentInfoLocal.logicalLength = 0u;
   localPbDiagData = pbDiagData;

   /* Verify all programming preconditions are fulfilled. Get address and length (logical) from request */
   result = FblDiagCheckRequestDownloadPreconditions(localPbDiagData, &segmentInfoLocal);

#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
   /* Flash Driver download? */
   if (result == kFblOk)
   {
      result = FblDiagCheckForFlashDriverDownload(&segmentInfoLocal);
   }
#else
   FblDiagSetTransferTypeFlash();
   segmentInfoLocal.type = kFblMemType_ROM;
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */

   if (result == kFblOk)
   {
      /* Get length from request */
      transferRemainder = segmentInfoLocal.logicalLength;

      /* Initialize error status address */
      FblErrStatSetAddress(segmentInfoLocal.logicalAddress);

      /* Init expected sequence counter for TransferData */
      expectedSequenceCnt = kDiagInitSequenceNum;
      /* Init current sequence counter for TransferData */
      currentSequenceCnt = kDiagInitSequenceNum;
    
#if(TRUE == ENABLE_INTERNAL_CRC_CHECK)


      GetFileBlockInfo(segmentInfoLocal.logicalAddress,transferRemainder);     /*Store The Block Info For Calulate The All File CRC cluding the FLASH DRIVER*/

     
#endif      

      /* Prepare download of application or flash driver. Target addresses are valid after these calls */
      if (FblDiagGetTransferTypeFlash())
      {
         result = FblDiagCheckFlashMemoryDownload(&segmentInfoLocal);
      }
#if defined( FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD )
      else
      {
         result = FblDiagCheckFlashDriverDownload(&segmentInfoLocal);
      }
#endif /* FBL_DIAG_ENABLE_FLASHDRV_DOWNLOAD */
   }

   /* First download segment */
   if ((result == kFblOk) && (segmentCount <= 1u))
   {
      /* The downloadBlockDescriptor.blockNr is initialized at different locations before:
         - In FblDiagCheckFlashDriverDownload for flash driver download
         - In FblDiagRCStartEraseMainHandler for normal flash download
         - In FblDiagInitDeltaDownload for delta download */
      result = FblDiagPrepareFirstDownloadSegment(&blockInfo, &segmentInfoLocal, downloadBlockDescriptor.blockNr);
      localPbDiagData = FblDiagMemGetActiveBuffer(); /* PRQA S 2982 */ /* MD_FblDiagOem_298x */

#if defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
      if (result == kFblOk)
      {
         /* Initialize read function for each new block */
         verifyParam.readMemory = blockInfo.readFct;
      }
#endif /* FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA */
   }

   /* Add segment data to FblLib_Mem */
   if (result == kFblOk)
   {
#if ( defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) || defined( FBL_MEM_ENABLE_VERIFY_PROCESSED ) ) && \
      defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
      if (segmentCount == FBL_VERIFICATION_SEGMENT_INDEX)
      {
         /* Specific segment must contain signature/CRC of logical block if pipelined verification is used */
         segmentInfoLocal.logicalLength -= kSecCRCLength + kSecSigLength;
      }
#endif /* (FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_MEM_ENABLE_VERIFY_PROCESSED) && FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA */
     /* Indicate start of new segment */
      libMemResult = FblMemRemapStatus(FblMemSegmentStartIndication(&segmentInfoLocal));
      localPbDiagData = FblDiagMemGetActiveBuffer();
      if (libMemResult == kDiagErrorNone)
      {
         /* Init expected sequence counter for TransferData */
         expectedSequenceCnt = kDiagInitSequenceNum;
         /* Init current sequence counter for TransferData */
         currentSequenceCnt = kDiagInitSequenceNum;

         /* Now allow reception of TransferData */
         FblDiagSetTransferDataAllowed();
         FblDiagClrTransferDataSucceeded();
         FblDiagClrChecksumAllowed();

         /* Prepare response */
         localPbDiagData[kDiagLocFmtSubparam] = (FBL_DIAG_RESPONSE_BUFFER_LFI << 4u);
         FblMemSetInteger(FBL_DIAG_RESPONSE_BUFFER_LFI, FBL_DIAG_RESPONSE_BUFFER_SIZE, &localPbDiagData[kDiagLocFmtSubparam + 1u]);

         DiagProcessingDone(kDiagRslRequestDownload + FBL_DIAG_RESPONSE_BUFFER_LFI);
      }
      else
      {
         /* Set NRC provided by FblLib_Mem */
         FblDiagSetError(libMemResult);
         result = kFblFailed;
      }
   }

   return result;
}  /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  FblDiagTransferDataLengthCheck
 **********************************************************************************************************************/
/*! \brief         Transfer data length check
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: Length of transfer data service is OK; kFblFailed: Length check failed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagTransferDataLengthCheck(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pbDiagData;       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

   if ((diagReqDataLen <= ((tCwDataLengthType)kDiagRqlTransferData)) || (diagReqDataLen > (tCwDataLengthType)((tCwDataLengthType)FBL_DIAG_RESPONSE_BUFFER_SIZE - 1u)))
   {
      /* Requested transfer length is larger than indicated data length */
      FblDiagClrTransferDataAllowed();
      result = kFblFailed;
   }
   else
   {
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagTransferDataMainHandler
 **********************************************************************************************************************/
/*! \brief         TransferData service function;
 *  \pre           TransferData must be enabled by RequestDownload service
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagTransferDataMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;
   tFblDiagNrc libMemResult;
   tFblLength transferDataLength;
   vuint8 * localPbDiagData = pbDiagData; /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */

   if (!FblDiagGetTransferDataAllowed())
   {
      DiagNRCRequestSequenceError();
      result = kFblFailed;
   }
   /* Check if the requested sequence number is expected */
   else if (localPbDiagData[kDiagLocFmtSubparam] != expectedSequenceCnt)
   {
      /* Check if sequence number corresponds to a retransmission of the last message */
      if (localPbDiagData[kDiagLocFmtSubparam] == currentSequenceCnt)
      {
         /* Repetition of last transferData request
          * Simply send a positive response without loading data to memory */
         DiagProcessingDone(kDiagRslTransferData);
         result = kFblOk;
      }
      else /* Sequence number is not for a retry */
      {
         /* Send a WrongSequenceError */
         DiagNRCWrongBlockSequenceCounter();
         result = kFblFailed;
      }
   }
   else
   {
      /* Length without sequence counter byte */
      transferDataLength = (tFblLength)diagReqDataLen - 1u;

      /* Indicate data to FblLib_Mem */
      FblDiagClrEraseSucceeded();
      libMemResult = FblMemRemapStatus(FblMemDataIndication(DiagBuffer, kDiagFmtDataOffset, transferDataLength));

      /* Caution: Depending on configuration, DiagBuffer pointer may change. */
      localPbDiagData = FblDiagMemGetActiveBuffer(); /* PRQA S 2983 */ /* MD_FblDiagOem_298x */
      if (libMemResult == kDiagErrorNone)
      {
         /* Memorize current counter */
         currentSequenceCnt = expectedSequenceCnt;
         /* Sequence counter value of next transferData request
          * Note: We do not rely on an implicit 8-bit caused overflow at 256, which does not happen on certain platforms */
         expectedSequenceCnt = ((expectedSequenceCnt + 1u) & 0xFFu);

         DiagProcessingDone(kDiagRslTransferData);
         result = kFblOk;
      }
      else
      {
         FblDiagSetError(libMemResult);
         FblDiagClrTransferDataAllowed();
         result = kFblFailed;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagReqTransferExitMainHandler
 **********************************************************************************************************************/
/*! \brief         RequestTransferExit service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagReqTransferExitMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;
   tFblDiagNrc libMemResult;
   tFblLength  totalProgramLength;  /* Total number of programmed bytes */

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

#if(TRUE == ENABLE_INTERNAL_CRC_CHECK)
         GetFileBlockCrc(pbDiagData);     /*Store The Block CRC For Compare The All File CRC*/
#endif 

   /* Do sequence and parameter checks */
   if (!FblDiagGetTransferDataAllowed())
   {
      DiagNRCRequestSequenceError();
      result = kFblFailed;
   }
   else
   {
      FblDiagClrTransferDataAllowed();

      /* Finalize programming of current segment */
      libMemResult = FblMemRemapStatus((FblMemSegmentEndIndication(&totalProgramLength)));
      (void)FblDiagMemGetActiveBuffer();

      if (libMemResult == kDiagErrorNone)
      {
         /* RequestTransferExit was successful */
         FblDiagSetTransferDataSucceeded();
         FblDiagSetChecksumAllowed();
         DiagProcessingDone(kDiagRslRequestTransferExit);
         result =  kFblOk;
      }
      else
      {
         /* Set return value of FblLib_Mem as NRC */
         FblDiagSetError(libMemResult);
         result = kFblFailed;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagTesterPresentMainHandler
 **********************************************************************************************************************/
/*! \brief         FblTesterPresent service function.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagTesterPresentMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */ /* PRQA S 3112 2 */ /* MD_MSR_DummyStmt */
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   DiagProcessingDone(kDiagRslTesterPresent);

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagControlDTCMainHandler
 **********************************************************************************************************************/
/*! \brief         ControlDTCSetting service function.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_MSR_Rule8.13 */
static tFblResult FblDiagControlDTCMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if !defined( FBL_DIAG_ENABLE_CONTROLDTC_OPTIONRECORD )
   (void)pbDiagData;       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
# endif
   (void)diagReqDataLen;   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif

#if defined( FBL_DIAG_ENABLE_CONTROLDTC_OPTIONRECORD )
   /* Check DTCSettingControlOptionRecord */
   if (   ( pbDiagData[kDiagLocFmtRoutineIdHigh] != 0xFFu) \
       || ( pbDiagData[kDiagLocFmtRoutineIdLow]  != 0xFFu) \
       || ( pbDiagData[kDiagLocFmtRoutineStatus] != 0xFFu)
      )
   {
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   else
#endif /* FBL_DIAG_ENABLE_CONTROLDTC_OPTIONRECORD */
   {
      /* Simply transmit a positive response message with subfunction parameter */
      DiagProcessingDone(kDiagRslControlDTCSetting);

      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagReadDataByIdMainHandler
 **********************************************************************************************************************/
/*! \brief         ReadDataByIdentifier service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 *********************************************************************************************************************/
static tFblResult FblDiagReadDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   ApplFblReadDataByIdentifier(pbDiagData, diagReqDataLen);

   return kFblOk;
}

/***********************************************************************************************************************
 * Diagnostic pre handler service functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagDefaultPreHandler
 **********************************************************************************************************************/
/*! \brief         Pre-handler function to call ApplFblCheckConditions()
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 *********************************************************************************************************************/
static tFblResult FblDiagDefaultPreHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   return ApplFblCheckConditions(pbDiagData, diagReqDataLen);
}

#define FBLDIAG_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 * Diagnostic post handler service functions
 **********************************************************************************************************************/

/* Start section to execute code from RAM */
#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  FblDiagDefaultPostHandler
 **********************************************************************************************************************/
/*! \brief       Can be used to switch to next state AFTER successful transmission of the service, e.g. EcuReset.
 *  \param[in]   postParam Parameter indicating service response.
 **********************************************************************************************************************/
void FblDiagDefaultPostHandler( vuint8 postParam )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)postParam;  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif
   if(FblDiagGetDefaultSession())
   {
      FblCwResetResponseAddress();
   }
}
/* Stop section to execute code from RAM */
#define FBLDIAG_RAMCODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_FblDiagOem_030x:
      Reason:     Address conversion between preset value and function pointer is needed to detect if
                  the default function should be used or if a specific function is configured.
      Risk:       The size of integer required to hold the result of a pointer cast is implementation defined.
      Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware
                  specific basis.

   MD_FblDiagOem_0313:
      Reason: Signature/checksum verification functions are stored as void pointers as actual type is not known at
       configuration time.
      Risk: Function signature not compatible with expected type.
      Prevention: Integrator has to take care the configured functions match with the expected signature.

   MD_FblDiagOem_2742:
      Reason:     Configuration related constant data.
      Risk:       Statements in the 'if' or 'else' block are not executed.
      Prevention: Code inspection and test of the different configurations in the component test.

   MD_FblDiagOem_298x:
      Reason: Value assignment is done but the value is modified again before use or not used. The assignment is done to keep
       DiagData and the local pointer pbDiagData consistent.
      Risk: Adds a unused assignment which could result in a compiler warning.
      Prevention: Ignore compiler warning if observed.

*/

