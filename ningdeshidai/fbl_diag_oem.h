/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic module external interface - OEM specific extension
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
 *  09.00.00   2020-06-18  visjdn  FBL-1819         No changes
 *  09.00.01   2021-03-04  vistmo  ESCAN00108085    No changes
 *                                 FBL-2935         Added/adapted MemMap sections
 *  09.00.02   2021-05-11  vistbe  ESCAN00107662    No changes
 *                                 FBL-3334         Update to MISRA 2012
 *  09.01.00   2021-11-10  visjdn  FBL-3077         Support of embedded checksum/signature
 *  09.02.00   2022-05-02  vishor  FBL-5067         Support of processed and input verification
 **********************************************************************************************************************/

#ifndef FBL_DIAG_OEM_H
#define FBL_DIAG_OEM_H

/* PRQA S 0857 EOF */ /* MD_MSR_1.1_857 */

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : FblDiag_14229_UDS2 CQComponent : Implementation */
#define FBLDIAG_14229_UDS2_VERSION           0x0902u
#define FBLDIAG_14229_UDS2_RELEASE_VERSION   0x00u

#define FBL_VERSION                          _VECTOR_SIP_VERSION
#define FBL_RELEASE_VERSION                  _VECTOR_SIP_RELEASE_VERSION

/* Reference interface version */
/** Major interface version identifies incompatible changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_MAJOR     0x04u
/** Minor interface version identifies backward compatible changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_MINOR     0x01u
/** Release interface version identifies cosmetic changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_RELEASE   0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Diagnostic configuration switches
 **********************************************************************************************************************/
#define FBL_ENABLE_TRANSFER_VERIFICATION_CHECK
#define FBL_ENABLE_EXCEEDED_RETRIES_REJECT

/* Select set of features from diagnostic core */
#define FBL_DIAG_ENABLE_CORE_GETBLOCKFROMADDR
#define FBL_DIAG_ENABLE_OEM_INITPOWERON
#define FBL_DIAG_ENABLE_CORE_PREWRITE
#define FBL_DIAG_ENABLE_CORE_POSTWRITE
#define FBL_DIAG_ENABLE_CORE_READ_SECACCESSDELAY_FLAG
#define FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING
#define FBL_DIAG_ENABLE_CORE_STAYINBOOT
#define FBL_DIAG_ENABLE_CORE_WRITE_SECACCESSDELAY_FLAG

#define FBL_DIAG_ENABLE_SERVICE_PREHANDLER
#define FBL_DIAG_ENABLE_SERVICE_POSTHANDLER

#if defined( FBL_ENABLE_STAY_IN_BOOT )
# if !defined( FBL_DIAG_STAY_IN_BOOT_ARRAY )
/** Default value of stay in boot message */
#  define FBL_DIAG_STAY_IN_BOOT_ARRAY {  kDiagSidRoutineControl, kDiagSubStartRoutine, kDiagRoutineIdStayInBootHigh, kDiagRoutineIdStayInBootLow }
# endif  /* FBL_DIAG_STAY_IN_BOOT_ARRAY */
#endif /* FBL_ENABLE_STAY_IN_BOOT */

/***********************************************************************************************************************
 *  State handling
 **********************************************************************************************************************/

/* Download sequence */
#define kFblDiagStateSecurityKeyAllowed         ( kFblDiagLastCoreStateIdx + 1u )
#define kFblDiagStateSecurityUnlock             ( kFblDiagLastCoreStateIdx + 2u )
#define kFblDiagStateFingerprintValid           ( kFblDiagLastCoreStateIdx + 3u )
#define kFblDiagStateEraseSucceeded             ( kFblDiagLastCoreStateIdx + 4u )
#define kFblDiagStateTransferDataAllowed        ( kFblDiagLastCoreStateIdx + 5u )
#define kFblDiagStateTransferDataSucceeded      ( kFblDiagLastCoreStateIdx + 6u )
#define kFblDiagStateChecksumAllowed            ( kFblDiagLastCoreStateIdx + 7u )
#define kFblDiagStateFlashDriverPresent         ( kFblDiagLastCoreStateIdx + 8u )
#define kFblDiagStateAppSecurityKeyAllowed      ( kFblDiagLastCoreStateIdx + 9u )
#define kFblDiagStateAppSecurityUnlock          ( kFblDiagLastCoreStateIdx + 10u)

#define kFblDiagLastOemStateIdx                 kFblDiagStateFlashDriverPresent

/* Download sequence states */
#define FblDiagGetSecurityKeyAllowed()          GetFblDiagState( kFblDiagStateSecurityKeyAllowed )
#define FblDiagGetSecurityUnlock()              GetFblDiagState( kFblDiagStateSecurityUnlock )
#define FblDiagGetFingerprintValid()            GetFblDiagState( kFblDiagStateFingerprintValid )
#define FblDiagGetEraseSucceeded()              GetFblDiagState( kFblDiagStateEraseSucceeded )
#define FblDiagGetTransferDataAllowed()         GetFblDiagState( kFblDiagStateTransferDataAllowed )
#define FblDiagGetTransferDataSucceeded()       GetFblDiagState( kFblDiagStateTransferDataSucceeded )
#define FblDiagGetChecksumAllowed()             GetFblDiagState( kFblDiagStateChecksumAllowed )
#define FblDiagGetFlashDriverPresent()          GetFblDiagState( kFblDiagStateFlashDriverPresent )
#define FblDiagGetAppSecurityKeyAllowed()       GetFblDiagState( kFblDiagStateAppSecurityKeyAllowed )
#define FblDiagGetAppSecurityUnlock()           GetFblDiagState( kFblDiagStateAppSecurityUnlock )

/***********************************************************************************************************************
 *  Service handling
 **********************************************************************************************************************/

#define kFblDiagResponseResetPostHandler           FblDiagDefaultPostHandler
#define kFblDiagResponseSdsPostHandler             FblDiagDefaultPostHandler

/* ControlDTCSetting */
#define kDiagSubDtcOn                                    0x01u    /**< Subservice ID - DTC on */
#define kDiagSubDtcOff                                   0x02u    /**< Subservice ID - DTC off */

/* Security access */
/* Subservice IDs and lengths of security access can be overwritten
 * - e.g. by a user configuration file. */
#if !defined( kFblDiagSubRequestSeed )
# define kDiagSubRequestAppSeed                          0x01u    /**< Subservice ID - Request seed */
# define kFblDiagSubRequestSeed                          0x09u    /**< Subservice ID - Request seed */
#endif
#if !defined( kFblDiagSubSendKey )
# define kDiagSubSendAppKey                              0x02u    /**< Subservice ID - Send key */
# define kFblDiagSubSendKey                              0x0Au    /**< Subservice ID - Send key */
#endif

#if !defined( kFblDiagSecurityAccessSessionMask )
# define kFblDiagSecurityAccessSessionMask kFblDiagOptionSessionProgramming
#endif

#define kSecSeedLength                                   SEC_SEED_LENGTH   /**< Length of seed provided by ECU */
#define kSecKeyLength                                    SEC_KEY_LENGTH    /**< Length of key provided by ECU */

/* OEM specific RoutineControl, RoutineIdentifier */
#define kDiagRoutineIdCheckProgPreCond                   0x0203u
#define kDiagRoutineIdCheckProgPreCondHigh               GET_ID_HIGH(kDiagRoutineIdCheckProgPreCond)
#define kDiagRoutineIdCheckProgPreCondLow                GET_ID_LOW(kDiagRoutineIdCheckProgPreCond)
#define kDiagRoutineIdEraseMemory                        0xFF00u
#define kDiagRoutineIdEraseMemoryHigh                    GET_ID_HIGH(kDiagRoutineIdEraseMemory)
#define kDiagRoutineIdEraseMemoryLow                     GET_ID_LOW(kDiagRoutineIdEraseMemory)
#define kDiagRoutineIdChecksum                           0x0202u
#define kDiagRoutineIdChecksumHigh                       GET_ID_HIGH(kDiagRoutineIdChecksum)
#define kDiagRoutineIdChecksumLow                        GET_ID_LOW(kDiagRoutineIdChecksum)
#define kDiagRoutineIdCheckProgDep                       0xFF01u
#define kDiagRoutineIdCheckProgDepHigh                   GET_ID_HIGH(kDiagRoutineIdCheckProgDep)
#define kDiagRoutineIdCheckProgDepLow                    GET_ID_LOW(kDiagRoutineIdCheckProgDep)
#if defined ( FBL_ENABLE_STAY_IN_BOOT )
# define kDiagRoutineIdStayInBoot                        0xF518u
# define kDiagRoutineIdStayInBootHigh                    GET_ID_HIGH(kDiagRoutineIdStayInBoot)
# define kDiagRoutineIdStayInBootLow                     GET_ID_LOW(kDiagRoutineIdStayInBoot)
#endif

/* RoutineControl response codes */
#define kDiagCheckVerificationOk                         0x00u    /**< Routine Control - Check Memory routine finished successfully */
#define kDiagCheckVerificationFailed                     0x01u    /**< Routine Control - Check Memory routine failed */
#define kDiagEraseMemoryOk                               0x00u    /**< Routine Control - Erase Memory routine finished successfully */
#define kDiagEraseMemoryFailed                           0x01u    /**< Routine Control - Erase Memory routine failed */
#define kDiagCheckCompatibilityOk                        0x00u    /**< Routine Control - Check Programming Dependencies OK */
#define kDiagCheckCompatibilityFailed                    0x01u    /**< Routine Control - Check Programming Dependencies Failed */
#define kDiagCheckCompatibilitySwHw                      0x02u    /**< Routine Control - Check Programming Dependencies HW/SW incompatible */
#define kDiagCheckCompatibilitySwSw                      0x03u    /**< Routine Control - Check Programming Dependencies SW/SW incompatible */
#define kDiagCheckCompatibilityBlockMissing              0x04u    /**< Routine Control - Check Programming Dependencies Mandatory Block Missing */

/* Defines for additional length codes for optional request parameters */
#if (SEC_SECURITY_CLASS == SEC_CLASS_DDD)
# define kSecCRCLength                                   SEC_VERIFY_CLASS_DDD_VERIFY_SIZE
# define kSecSigLength                                   0x00u
#else
# if defined( kSecCRCLength)
# else
#  define kSecCRCLength                                  0x00u
# endif
# if (SEC_SECURITY_CLASS == SEC_CLASS_C )
#  define kSecSigLength                                  SEC_VERIFY_CLASS_C_VERIFY_SIZE
# endif
# if (SEC_SECURITY_CLASS == SEC_CLASS_CCC )
#  define kSecSigLength                                  SEC_VERIFY_CLASS_CCC_VERIFY_SIZE
# endif
#endif

/* Data identifier for WriteDataByIdentifier service */
#if !defined( kDiagWriteFingerprint )
# define kDiagWriteFingerprint                           0xF184u
#endif
#define kDiagWriteFingerprintHigh                        GET_ID_HIGH(kDiagWriteFingerprint)
#define kDiagWriteFingerprintLow                         GET_ID_LOW(kDiagWriteFingerprint)

/* Special return code for ApplFblWriteDataByIdentifier */
#define kDiagReturnValidationOk                          0x5Cu
#define kDiagReturnValidationFailed                      (kFblFailed)

/* Request parameter lengths excluding the service ID */
#define kDiagRqlDiagnosticSessionControlParameter           0u
#define kDiagRqlSecurityAccessSeedParameter                 0u
#define kDiagRqlSecurityAccessKeyParameter                  kSecKeyLength
#if !defined( kDiagRqlWriteDataByIdentifierFingerPrintParameter )
# define kDiagRqlWriteDataByIdentifierFingerPrintParameter  10u
#endif
#if !defined( kDiagRqlRoutineControlCheckRoutineParameter )
# if defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
#  define kDiagRqlRoutineControlCheckRoutineParameter       0u
# else
#  define kDiagRqlRoutineControlCheckRoutineParameter       (kSecCRCLength + kSecSigLength)
# endif /* FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA */
#endif /* !kDiagRqlRoutineControlCheckRoutineParameter */
#define kDiagRqlRoutineControlAddrAndLenFormatIdParameter   1u
#if defined( FBL_DIAG_ENABLE_CONTROLDTC_OPTIONRECORD )
# define kDiagRqlControlDTCSettingParameter                 3u
#else
# define kDiagRqlControlDTCSettingParameter                 0u
#endif /* FBL_DIAG_ENABLE_CONTROLDTC_OPTIONRECORD */

/* Request lengths excluding the service ID */
#define kDiagRqlServiceWithSubfunction             1u
#define kDiagRqlDiagnosticSessionControl           (1u + kDiagRqlDiagnosticSessionControlParameter)
#define kDiagRqlEcuReset                           1u
#define kDiagRqlReadDataByIdentifier               2u
#define kDiagRqlSecurityAccessSeed                 (1u + kDiagRqlSecurityAccessSeedParameter)
#define kDiagRqlSecurityAccessKey                  (1u + kDiagRqlSecurityAccessKeyParameter)
#define kDiagRqlCommunicationControl               2u
#define kDiagRqlWriteDataByIdentifier              2u
#define kDiagRqlWriteDataByIdentifierFingerPrint   (kDiagRqlWriteDataByIdentifier + kDiagRqlWriteDataByIdentifierFingerPrintParameter)
#define kDiagRqlRoutineControl                     3u
//#define kDiagRqlRoutineControlCheckRoutine         (kDiagRqlRoutineControl + kDiagRqlRoutineControlCheckRoutineParameter)
#define kDiagRqlRoutineControlCheckRoutine         (kDiagRqlRoutineControl)
#define kDiagRqlRoutineControlEraseRoutine         (kDiagRqlRoutineControl + 4u + 4u)
#define kDiagRqlRoutineControlProgPreCond          kDiagRqlRoutineControl
#define kDiagRqlRoutineControlCheckProgDep         kDiagRqlRoutineControl
#define kDiagRqlRoutineControlForceBoot            kDiagRqlRoutineControl
#define kDiagRqlRequestDownload                    2u
#define kDiagRqlTransferData                       1u
#define kDiagRqlRequestTransferExit                4u
#define kDiagRqlTesterPresent                      1u
#define kDiagRqlControlDTCSetting                  (1u + kDiagRqlControlDTCSettingParameter)

/* Response parameter lengths excluding the service ID */
#define kDiagRslEcuResetParameter                           0u
#define kDiagRslDiagnosticSessionControlParameter           4u
#define kDiagRslSecurityAccessSeedParameter                 kSecSeedLength
#define kDiagRslSecurityAccessKeyParameter                  0u
#define kDiagRslRoutineControlEraseRoutineParameter         0u
#define kDiagRslRoutineControlCheckRoutineParameter         1u
#define kDiagRslRoutineControlCheckPreCondParameter         3u
#define kDiagRslTransferDataParameter                       0u
#define kDiagRslRequestTransferExitParameter                4u

/* Response lengths excluding the service ID */
#define kDiagRslDiagnosticSessionControl           (1u + kDiagRslDiagnosticSessionControlParameter)
#define kDiagRslEcuReset                           (1u + kDiagRslEcuResetParameter)
#define kDiagRslSecurityAccessSeed                 (1u + kDiagRslSecurityAccessSeedParameter)
#define kDiagRslSecurityAccessKey                  (1u + kDiagRslSecurityAccessKeyParameter)
#define kDiagRslCommunicationControl               1u
#define kDiagRslWriteDataByIdentifier              2u
#define kDiagRslRoutineControlEraseRoutine         (3u + kDiagRslRoutineControlEraseRoutineParameter)
#define kDiagRslRoutineControlCheckRoutine         (3u + kDiagRslRoutineControlCheckRoutineParameter)
#define kDiagRslRoutineControlCheckPreCond         (3u + kDiagRslRoutineControlCheckPreCondParameter)
#if defined ( FBL_ENABLE_STAY_IN_BOOT )
# define kDiagRslRoutineControlStayInBoot          3u
#endif
#define kDiagRslRequestDownload                    1u /* + maxNumberOfBlockLength */
#define kDiagRslTransferData                       (1u + kDiagRslTransferDataParameter)
#define kDiagRslRequestTransferExit                (0u + kDiagRslRequestTransferExitParameter)
#define kDiagRslTesterPresent                      1u
#define kDiagRslControlDTCSetting                  1u

/* Diagnostic service format definitions */
#define kDiagFmtServiceId                    0u                               /**< Position of service id */
#define kDiagFmtpbDiagData                   1u                               /**< Access parameter to remove service ID */
#define kDiagFmtSubparam                     (kDiagFmtServiceId + 1u)         /**< Position of DID */
#define kDiagFmtDataIdSize                   2u                               /**< Size of data identifier  (2 bytes) */
#define kDiagFmtDataIdHigh                   (kDiagFmtServiceId + 1u)         /**< Position of data identifier (MSB) */
#define kDiagFmtDataIdLow                    (kDiagFmtDataIdHigh + 1u)        /**< Position of data identifier (LSB) */
#define kDiagFmtSubFctDataIdHigh             (kDiagFmtSubparam + 1u)          /**< Position of data identifier (MSB) */
#define kDiagFmtSubFctDataIdLow              (kDiagFmtSubFctDataIdHigh + 1u)  /**< Position of data identifier (LSB) */
#define kDiagFmtDataRecord                   (kDiagFmtDataIdLow + 1u)         /**< Position of record data identifier */
#define kDiagFmtSequenceCnt                  (kDiagFmtServiceId + 1u)         /**< Position of sequence counter */
#define kDiagFmtDataOffset                   (kDiagFmtSequenceCnt + 1u)       /**< Offset to download data in TransferData frame */
#define kDiagFmtFormatOffset                 2u
#define kDiagFmtNegResponse                  (kDiagFmtSubparam + 1u)          /**< Position of negative response code */
#define kDiagFmtAddrOffset                   3u
#define kDiagFmtRoutineIdHigh                (kDiagFmtSubparam + 1u)          /**< RoutineControl service formats */
#define kDiagFmtRoutineIdLow                 (kDiagFmtRoutineIdHigh + 1u)
#define kDiagFmtRoutineStatus                (kDiagFmtRoutineIdLow + 1u)
#define kDiagFmtRoutineAlfi                  (kDiagFmtRoutineIdLow + 1u)
#define kDiagFmtStartAddress                 (kDiagFmtRoutineAlfi + 1u)
#define kDiagFmtStopAddress                  (kDiagFmtStartAddress + 4u)
#define kDiagFmtChecksumHigh                 (kDiagFmtStopAddress + 4u)
#define kDiagFmtChecksumLow                  (kDiagFmtChecksumHigh + 1u)
#define kDiagFmtSeedKeyStart                 (kDiagFmtSubparam + 1u)          /**< Start index of seed/key value */

/* Diagnostic service index to diagnostic parameters.
 * This index shall only be used with the pointer to diag-buffer
 * that was passed to the function as a parameter.
 * NOTE:  You MUST NOT use the kDiagLocFmtXxx with the global DiagBuffer!!! */
#define kDiagLocFmtSubparam                              0u                                  /**< Position of DID */
#define kDiagLocFmtSubfunction                           0u                                  /**< Position of Subfunction */
#define kDiagLocFmtDataIdHigh                            0u                                  /**< Position of data identifier (MSB) */
#define kDiagLocFmtDataIdLow                             (kDiagLocFmtDataIdHigh + 1u)        /**< Position of data identifier (LSB) */
#define kDiagLocFmtSubFctDataIdHigh                      (kDiagLocFmtSubfunction + 1u)       /**< Position of data identifier (MSB) for a subfunction */
#define kDiagLocFmtSubFctDataIdLow                       (kDiagLocFmtSubFctDataIdHigh + 1u)  /**< Position of data identifier (LSB) for a subfunction */
#define kDiagLocFmtDataRecord                            (kDiagLocFmtDataIdLow + 1u)         /**< Position of record data identifier */
#define kDiagLocFmtSequenceCnt                           0u                                  /**< Position of sequence counter */
#define kDiagLocFmtDataOffset                            (kDiagLocFmtSequenceCnt + 1u)       /**< Offset to download data in TransferData frame */
#define kDiagLocFmtFormatOffset                          1u
#define kDiagLocFmtNegResponse                           (kDiagLocFmtSubparam + 1u)          /**< Position of negative response code */
#define kDiagLocFmtAddrOffset                            2u
#define kDiagLocFmtRoutineIdHigh                         (kDiagLocFmtSubparam + 1u)          /**< RoutineControl service formats */
#define kDiagLocFmtRoutineIdLow                          (kDiagLocFmtRoutineIdHigh + 1u)
#define kDiagLocFmtRoutineStatus                         (kDiagLocFmtRoutineIdLow + 1u)
#define kDiagLocFmtRoutineAlfi                           (kDiagLocFmtRoutineIdLow + 1u)
#define kDiagLocFmtStartAddress                          (kDiagLocFmtRoutineAlfi + 1u)
#define kDiagLocFmtStopAddress                           (kDiagLocFmtStartAddress + 4u)
#define kDiagLocFmtChecksumHigh                          (kDiagLocFmtStopAddress + 4u)
#define kDiagLocFmtChecksumLow                           (kDiagLocFmtChecksumHigh + 1u)
#define kDiagLocFmtSeedKeyStart                          (kDiagLocFmtSubparam + 1u)          /**< Start index of seed/key value */

/***********************************************************************************************************************
 *  Error handling
 **********************************************************************************************************************/
#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Error codes for detailed error status reporting */
# define FBL_ERR_NONE                        0x00u

# define FBL_ERR_FLASH_ERASE_ERROR           0x10u
# define FBL_ERR_FLASH_PROG_ERROR            0x11u
# define FBL_ERR_FLASH_PROG_CONDITIONS       0x12u

# define FBL_ERR_APPL_OVERLAPPING            0x20u
# define FBL_ERR_APPL_NOT_ALIGNED            0x21u
# define FBL_ERR_TOO_MANY_SEGMENTS_IN_MODULE 0x22u
# define FBL_ERR_NO_MEMORY_REGION_FOUND      0x23u
# define FBL_ERR_SEED_GENERATION_FAILED      0x24u
# define FBL_ERR_GAP_FILL                    0x25u
# define FBL_ERR_VERIFICATION                0x26u

# define FBL_ERR_FLASHCODE_INIT_FAILED       0x30u
# define FBL_ERR_FLASHCODE_EXCEEDS_MEMORY    0x31u
# define FBL_ERR_FLASHCODE_NOT_ACCEPTED      0x32u

# define FBL_ERR_LBT_MISSING_INVALID_FORMAT  0x40u
# define FBL_ERR_LBT_ADDR_MISMATCH           0x41u
# define FBL_ERR_LBT_BLOCK_INDEX_EXCEEDED    0x42u
# define FBL_ERR_LBT_NOT_REPROGRAMMABLE      0x43u
# define FBL_ERR_LBT_BLOCK_INDEX_ILLEGAL     0x44u

# define FBL_ERR_DATA_OVERFLOW               0x50u

/* Macros for error status reporting */
/* PRQA S 3453 12 */ /* MD_MSR_FctLikeMacro */
# define FblErrStatSetSId(id)                (errStatLastServiceId = (id))
# define FblErrStatSetState(state)           (errStatFblStates = (state))
# define FblErrStatSetFlashDrvError(error)   (errStatFlashDrvErrorCode = (error))
# define FblErrStatSetFlashDrvVersion()      \
{                                            \
   errStatFlashDrvVersion[0] = flashCode[0]; \
   errStatFlashDrvVersion[1] = flashCode[1]; \
   errStatFlashDrvVersion[2] = flashCode[3]; \
}  /* PRQA S 3458 */ /* MD_MSR_FctLikeMacro */
# define FblErrStatSetError(error)           (errStatErrorCode = (error))
# define FblErrStatSetBlockNr(blockNumber)   (errStatDescriptor.blockNr = (blockNumber))
# define FblErrStatSetAddress(address)       (errStatDescriptor.blockStartAddress = (address))
#else
# define FblErrStatSetSId(id)
# define FblErrStatSetState(state)
# define FblErrStatSetFlashDrvError(error)
# define FblErrStatSetFlashDrvVersion()
# define FblErrStatSetError(error)
# define FblErrStatSetBlockNr(blockNr)
# define FblErrStatSetAddress(address)
#endif

/***********************************************************************************************************************
 *  Data processing
 **********************************************************************************************************************/
#if !defined( kDiagSubDefaultEncryption )
# define kDiagSubDefaultEncryption        0x01u
#endif

#if !defined(kDiagSubDefaultCompression)
# define kDiagSubDefaultCompression       0x01u
#endif

#if !defined( kDiagSubDefaultDelta )
# define kDiagSubDefaultDelta             0x02u
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
extern V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2                    checkMemoryFlag;
extern V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2                    calculateCRC[11];     /*Task:7278035*/


#define FBLDIAG_START_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Variables for error status reporting */
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatErrorCode;
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatFblStates;
V_MEMRAM0 extern V_MEMRAM1      vuint8             V_MEMRAM2      errStatLastServiceId;
V_MEMRAM0 extern V_MEMRAM1      vuint8             V_MEMRAM2      errStatFlashDrvVersion[3];
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatFlashDrvErrorCode;
V_MEMRAM0 extern V_MEMRAM1      tBlockDescriptor   V_MEMRAM2      errStatDescriptor;
#endif

#define FBLDIAG_STOP_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
extern void OemInitPowerOn(void);

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

/* Check for unsupported configurations. These checks cannot be done in the generation tool. */
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
# define FBL_MEM_VERIFICATION_SET
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# define FBL_MEM_VERIFICATION_SET
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_MEM_ENABLE_VERIFY_PROCESSED )
# define FBL_MEM_VERIFICATION_SET
#endif /* FBL_MEM_ENABLE_VERIFY_PROCESSED */

#if defined( FBL_MEM_ENABLE_VERIFY_INPUT )
# define FBL_MEM_VERIFICATION_SET
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT */

#if !defined( FBL_MEM_VERIFICATION_SET )
# error "At least one verification method must be activated."
#endif /* !FBL_MEM_VERIFICATION_SET */

#if defined( FBL_MEM_ENABLE_VERIFY_INPUT ) && \
    defined( FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA )
# error "Input verification is not available for embedded verification data"
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT && FBL_DIAG_ENABLE_EMBEDDED_VERIFY_DATA */

#endif /* FBL_DIAG_OEM_H */
