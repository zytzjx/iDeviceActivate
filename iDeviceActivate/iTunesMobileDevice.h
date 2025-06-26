/********************************************************************
	created:	2015/09/23
	created:	23:9:2015   18:46
	filename: 	iTunesMobileDevice.h
	file base:	iTunesMobileDevice
	file ext:	h
	author:		Jeffery Zhang
	
	purpose:	header.
	https://github.com/phonegap/ios-deploy/blob/master/src/ios-deploy/errors.h
	enum MOBILE_DEVICE_ERROR_CODE
	{
	kAMDSuccess                                   = 0x00000000,
	kAMDUndefinedError                            = 0xE8000001,
	kAMDBadHeaderError                            = 0xE8000002,
	kAMDNoResourcesError                          = 0xE8000003,
	kAMDReadError                                 = 0xE8000004,
	kAMDWriteError                                = 0xE8000005,
	kAMDUnknownPacketError                        = 0xE8000006,
	kAMDInvalidArgumentError                      = 0xE8000007,
	kAMDNotFoundError                             = 0xE8000008,
	kAMDIsDirectoryError                          = 0xE8000009,
	kAMDPermissionError                           = 0xE800000A,
	kAMDNotConnectedError                         = 0xE800000B,
	kAMDTimeOutError                              = 0xE800000C,
	kAMDOverrunError                              = 0xE800000D,
	kAMDEOFError                                  = 0xE800000E,
	kAMDUnsupportedError                          = 0xE800000F,
	kAMDFileExistsError                           = 0xE8000010,
	kAMDBusyError                                 = 0xE8000011,
	kAMDCryptoError                               = 0xE8000012,
	kAMDInvalidResponseError                      = 0xE8000013,
	kAMDMissingKeyError                           = 0xE8000014,
	kAMDMissingValueError                         = 0xE8000015,
	kAMDGetProhibitedError                        = 0xE8000016,
	kAMDSetProhibitedError                        = 0xE8000017,
	kAMDRemoveProhibitedError                     = 0xE8000018,
	kAMDImmutableValueError                       = 0xE8000019,
	kAMDPasswordProtectedError                    = 0xE800001A,
	kAMDMissingHostIDError                        = 0xE800001B,
	kAMDInvalidHostIDError                        = 0xE800001C,
	kAMDSessionActiveError                        = 0xE800001D,
	kAMDSessionInactiveError                      = 0xE800001E,
	kAMDMissingSessionIDError                     = 0xE800001F,
	kAMDInvalidSessionIDError                     = 0xE8000020,
	kAMDMissingServiceError                       = 0xE8000021,
	kAMDInvalidServiceError                       = 0xE8000022,
	kAMDInvalidCheckinError                       = 0xE8000023,
	kAMDCheckinTimeoutError                       = 0xE8000024,
	kAMDMissingPairRecordError                    = 0xE8000025,
	kAMDInvalidActivationRecordError              = 0xE8000026,
	kAMDMissingActivationRecordError              = 0xE8000027,
	kAMDWrongDroidError                           = 0xE8000028,
	kAMDSUVerificationError                       = 0xE8000029,
	kAMDSUPatchError                              = 0xE800002A,
	kAMDSUFirmwareError                           = 0xE800002B,
	kAMDProvisioningProfileNotValid               = 0xE800002C,
	kAMDSendMessageError                          = 0xE800002D,
	kAMDReceiveMessageError                       = 0xE800002E,
	kAMDMissingOptionsError                       = 0xE800002F,
	kAMDMissingImageTypeError                     = 0xE8000030,
	kAMDDigestFailedError                         = 0xE8000031,
	kAMDStartServiceError                         = 0xE8000032,
	kAMDInvalidDiskImageError                     = 0xE8000033,
	kAMDMissingDigestError                        = 0xE8000034,
	kAMDMuxError                                  = 0xE8000035,
	kAMDApplicationAlreadyInstalledError          = 0xE8000036,
	kAMDApplicationMoveFailedError                = 0xE8000037,
	kAMDApplicationSINFCaptureFailedError         = 0xE8000038,
	kAMDApplicationSandboxFailedError             = 0xE8000039,
	kAMDApplicationVerificationFailedError        = 0xE800003A,
	kAMDArchiveDestructionFailedError             = 0xE800003B,
	kAMDBundleVerificationFailedError             = 0xE800003C,
	kAMDCarrierBundleCopyFailedError              = 0xE800003D,
	kAMDCarrierBundleDirectoryCreationFailedError = 0xE800003E,
	kAMDCarrierBundleMissingSupportedSIMsError    = 0xE800003F,
	kAMDCommCenterNotificationFailedError         = 0xE8000040,
	kAMDContainerCreationFailedError              = 0xE8000041,
	kAMDContainerP0wnFailedError                  = 0xE8000042,
	kAMDContainerRemovalFailedError               = 0xE8000043,
	kAMDEmbeddedProfileInstallFailedError         = 0xE8000044,
	kAMDErrorError                                = 0xE8000045,
	kAMDExecutableTwiddleFailedError              = 0xE8000046,
	kAMDExistenceCheckFailedError                 = 0xE8000047,
	kAMDInstallMapUpdateFailedError               = 0xE8000048,
	kAMDManifestCaptureFailedError                = 0xE8000049,
	kAMDMapGenerationFailedError                  = 0xE800004A,
	kAMDMissingBundleExecutableError              = 0xE800004B,
	kAMDMissingBundleIdentifierError              = 0xE800004C,
	kAMDMissingBundlePathError                    = 0xE800004D,
	kAMDMissingContainerError                     = 0xE800004E,
	kAMDNotificationFailedError                   = 0xE800004F,
	kAMDPackageExtractionFailedError              = 0xE8000050,
	kAMDPackageInspectionFailedError              = 0xE8000051,
	kAMDPackageMoveFailedError                    = 0xE8000052,
	kAMDPathConversionFailedError                 = 0xE8000053,
	kAMDRestoreContainerFailedError               = 0xE8000054,
	kAMDSeatbeltProfileRemovalFailedError         = 0xE8000055,
	kAMDStageCreationFailedError                  = 0xE8000056,
	kAMDSymlinkFailedError                        = 0xE8000057,
	kAMDiTunesArtworkCaptureFailedError           = 0xE8000058,
	kAMDiTunesMetadataCaptureFailedError          = 0xE8000059,
	kAMDAlreadyArchivedError                      = 0xE800005A,
	kAMDServiceLimitError                         = 0xE800005B,
	kAMDInvalidPairRecordError                    = 0xE800005C,
	kAMDServiceProhibitedError                    = 0xE800005D,
	kAMDCheckinSetupFailedError                   = 0xE800005E,
	kAMDCheckinConnectionFailedError              = 0xE800005F,
	kAMDCheckinReceiveFailedError                 = 0xE8000060,
	kAMDCheckinResponseFailedError                = 0xE8000061,
	kAMDCheckinSendFailedError                    = 0xE8000062,
	kAMDMuxCreateListenerError                    = 0xE8000063,
	kAMDMuxGetListenerError                       = 0xE8000064,
	kAMDMuxConnectError                           = 0xE8000065,
	kAMDUnknownCommandError                       = 0xE8000066,
	kAMDAPIInternalError                          = 0xE8000067,
	kAMDSavePairRecordFailedError                 = 0xE8000068,
	kAMDCheckinOutOfMemoryError                   = 0xE8000069,
	kAMDDeviceTooNewError                         = 0xE800006A,
	kAMDDeviceRefNoGood                           = 0xE800006B,
	kAMDCannotTranslateError                      = 0xE800006C,
	kAMDMobileImageMounterMissingImageSignature   = 0xE800006D,
	kAMDMobileImageMounterResponseCreationFailed  = 0xE800006E,
	kAMDMobileImageMounterMissingImageType        = 0xE800006F,
	kAMDMobileImageMounterMissingImagePath        = 0xE8000070,
	kAMDMobileImageMounterImageMapLoadFailed      = 0xE8000071,
	kAMDMobileImageMounterAlreadyMounted          = 0xE8000072,
	kAMDMobileImageMounterImageMoveFailed         = 0xE8000073,
	kAMDMobileImageMounterMountPathMissing        = 0xE8000074,
	kAMDMobileImageMounterMountPathNotEmpty       = 0xE8000075,
	kAMDMobileImageMounterImageMountFailed        = 0xE8000076,
	kAMDMobileImageMounterTrustCacheLoadFailed    = 0xE8000077,
	kAMDMobileImageMounterDigestFailed            = 0xE8000078,
	kAMDMobileImageMounterDigestCreationFailed    = 0xE8000079,
	kAMDMobileImageMounterImageVerificationFailed = 0xE800007A,
	kAMDMobileImageMounterImageInfoCreationFailed = 0xE800007B,
	kAMDMobileImageMounterImageMapStoreFailed     = 0xE800007C,
	kAMDBonjourSetupError                         = 0xE800007D,
	kAMDDeviceOSVersionTooLow                     = 0xE800007E,
	kAMDNoWifiSyncSupportError                    = 0xE800007F,
	kAMDDeviceFamilyNotSupported                  = 0xE8000080,
	kAMDEscrowLockedError                         = 0xE8000081,
	kAMDPairingProhibitedError                    = 0xE8000082,
	kAMDProhibitedBySupervision                   = 0xE8000083,
	kAMDDeviceDisconnectedError                   = 0xE8000084,
	kAMDTooBigError                               = 0xE8000085,
	kAMDPackagePatchFailedError                   = 0xE8000086,
	kAMDIncorrectArchitectureError                = 0xE8000087,
	kAMDPluginCopyFailedError                     = 0xE8000088,
	kAMDBreadcrumbFailedError                     = 0xE8000089,
	kAMDBreadcrumbUnlockError                     = 0xE800008A,
	kAMDGeoJSONCaptureFailedError                 = 0xE800008B,
	kAMDNewsstandArtworkCaptureFailedError        = 0xE800008C,
	kAMDMissingCommandError                       = 0xE800008D,
	kAMDNotEntitledError                          = 0xE800008E,
	kAMDMissingPackagePathError                   = 0xE800008F,
	kAMDMissingContainerPathError                 = 0xE8000090,
	kAMDMissingApplicationIdentifierError         = 0xE8000091,
	kAMDMissingAttributeValueError                = 0xE8000092,
	kAMDLookupFailedError                         = 0xE8000093,
	kAMDDictCreationFailedError                   = 0xE8000094,
	kAMDUserDeniedPairingError                    = 0xE8000095,
	kAMDPairingDialogResponsePendingError         = 0xE8000096,
	kAMDInstallProhibitedError                    = 0xE8000097,
	kAMDUninstallProhibitedError                  = 0xE8000098,
	kAMDFMiPProtectedError                        = 0xE8000099,
	kAMDMCProtected                               = 0xE800009A,
	kAMDMCChallengeRequired                       = 0xE800009B,
	kAMDMissingBundleVersionError                 = 0xE800009C,
	kAMDAppBlacklistedError                       = 0xE800009D,
	};
	kAMDAlreadyArchived 										= 0xE800005A
	kAMDAPIInternalError 										= 0xE8000067
	kAMDApplicationAlreadyInstalled 							= 0xE8000036
	kAMDApplicationMoveFailed 									= 0xE8000037
	kAMDApplicationSINFCaptureFailed 							= 0xE8000038
	kAMDApplicationSandboxFailed 								= 0xE8000039
	kAMDApplicationVerificationFailed 							= 0xE800003A
	kAMDArchiveDestructionFailed 								= 0xE800003B
	kAMDBundleVerificationFailed 								= 0xE800003C
	kAMDCarrierBundleCopyFailed 								= 0xE800003D
	kAMDCarrierBundleDirectoryCreationFailed 					= 0xE800003E
	kAMDCarrierBundleMissingSupportedSIMs 						= 0xE800003F
	kAMDCommCenterNotificationFailed 							= 0xE8000040
	kAMDContainerCreationFailed 								= 0xE8000041
	kAMDContainerP0wnFailed 									= 0xE8000042
	kAMDContainerRemovalFailed 									= 0xE8000043
	kAMDEmbeddedProfileInstallFailed 							= 0xE8000044
	kAMDError 													= 0xE8000045
	kAMDExecutableTwiddleFailed 								= 0xE8000046
	kAMDExistenceCheckFailed 									= 0xE8000047
	kAMDInstallMapUpdateFailed 									= 0xE8000048
	kAMDManifestCaptureFailed 									= 0xE8000049
	kAMDMapGenerationFailed 									= 0xE800004A
	kAMDMissingBundleExecutable 								= 0xE800004B
	kAMDMissingBundleIdentifier 								= 0xE800004C
	kAMDMissingBundlePath 										= 0xE800004D
	kAMDMissingContainer 										= 0xE800004E
	kAMDNotificationFailed 										= 0xE800004F
	kAMDPackageExtractionFailed 								= 0xE8000050
	kAMDPackageInspectionFailed 								= 0xE8000051
	kAMDPackageMoveFailed 										= 0xE8000052
	kAMDPathConversionFailed 									= 0xE8000053
	kAMDRestoreContainerFailed 									= 0xE8000054
	kAMDSeatbeltProfileRemovalFailed 							= 0xE8000055
	kAMDStageCreationFailed 									= 0xE8000056
	kAMDSymlinkFailed 											= 0xE8000057
	kAMDUnknownCommand 											= 0xE8000066
	kAMDiTunesArtworkCaptureFailed 								= 0xE8000058
	kAMDiTunesMetadataCaptureFailed 							= 0xE8000059
	kAMDDeviceOSVersionTooLow 									= 0xE800007E
	kAMDDeviceFamilyNotSupported 								= 0xE8000080
	kAMDPackagePatchFailed 										= 0xE8000086
	kAMDIncorrectArchitecture 									= 0xE8000087
	kAMDPluginCopyFailed 										= 0xE8000088
	kAMDBreadcrumbFailed 										= 0xE8000089
	kAMDBreadcrumbUnlockFailed 									= 0xE800008A
	kAMDGeoJSONCaptureFailed 									= 0xE800008B
	kAMDNewsstandArtworkCaptureFailed 							= 0xE800008C
	kAMDMissingCommand 											= 0xE800008D
	kAMDNotEntitled 				 							= 0xE800008E
	kAMDMissingPackagePath 		 								= 0xE800008F
	kAMDMissingContainerPath 	 								= 0xE8000090
	kAMDMissingApplicationIdentifier 	 						= 0xE8000091
	kAMDMissingAttributeValue 		 							= 0xE8000092
	kAMDLookupFailed 		 									= 0xE8000093
	kAMDDictCreationFailed 		 								= 0xE8000094
	kAMDInstallProhibited 		 								= 0xE8000097
	kAMDUninstallProhibited 		 							= 0xE8000098
	kAMDMissingBundleVersion 		 							= 0xE800009C
	kAMDInvalidSymlink 			 				 				= 0xE80000BF
	kAMDAppBlacklisted 		 									= 0xE800009D
	kAMDAppexBundleIDNotPrefixed 		 						= 0xE800009E
	kAMDAppexBundleIllegalXPCServiceDict 	 					= 0xE800009F
	kAMDAppexBundleMissingNSExtensionDict 						= 0xE80000A0
	kAMDAppexBundleIllegalPackageTypeValue 						= 0xE80000A1
	kAMDAppexBundleMissingClassOrStoryboard 					= 0xE80000A2
	kAMDAppexBundleIllegalExtensionContextClass 				= 0xE80000A3
	kAMDAppexBundleIllegalExtensionContextHostClass 			= 0xE80000A4
	kAMDAppexBundleIllegalExtensionViewControllerHostClass 		= 0xE80000A5
	kAMDAppexBundleMissingExtensionPointIdentifierString 		= 0xE80000A6
	kAMDAppexBundleIllegalExtensionPointIdentifierString 		= 0xE80000A7
	kAMDAppexBundleIllegalExtensionAttributesDict 				= 0xE80000A8
	kAMDAppexBundleIllegalExtensionPointNameString 				= 0xE80000A9
	kAMDAppexBundleIllegalExtensionPointVersionString 			= 0xE80000AA
	kAMDMissingBundleNameString 			 					= 0xE80000AB
	kAMDMissingBundleDisplayNameString 			 				= 0xE80000AC
	kAMDIllegalBundleShortVersionString 						= 0xE80000AD
	kAMDIllegalXPCServiceRunLoopType 		 					= 0xE80000AE
	kAMDIllegalXPCServiceServiceType 		 					= 0xE80000AF
	kAMDDuplicateIdentifier 				 					= 0xE80000B0
	kAMDAppexBundleUnknownExtensionPointIdentifier 				= 0xE80000B1
	kAMDMultipleFileProviderAppexBundles 						= 0xE80000B2
	kAMDMismatchedApplicationIdentifierEntitlement 				= 0xE80000BE
	kAMDInvalidATVStubApplication 								= 0xE80000C2
	kAMDBundleiTunesMetadataVersionMismatch 					= 0xE80000C3
	kAMDInvalidiTunesMetadataPlist 				 				= 0xE80000C4
	kAMDMismatchedBundleIDSigningIdentifier 					= 0xE80000C5
	kAMDMultipleWatchKitAppexBundles 			 				= 0xE80000C6
	kAMDInvalidWatchKitApp 			 							= 0xE80000C7
	kAMDDeviceNotSupportedByThinning 			 				= 0xE80000C8
	kAMDInvalidSupportedDevices 								= 0xE80000C9
	kAMDAppexBundleIDSuffixContainsPeriod 						= 0xE80000CA
	kAMDAppexBundleIDConflictWithOtherIdentifier 				= 0xE80000CB
	kAMDBundleIDConflictWithOtherIdentifier 					= 0xE80000CC
	kAMDMultipleWatchKitOneApps 								= 0xE80000CD
	kAMDMultipleWatchKitTwoApps 	 							= 0xE80000CE
	kAMDInvalidStubExecutable 		 							= 0xE80000CF
	kAMDMultipleAppExtensionsFoundInWKApp 			 			= 0xE80000D0
	kAMDNonWKAppExtensionFoundInWKApp 	 						= 0xE80000D1
	kAMDWatchKitAppHasFrameworks 	 							= 0xE80000D2
	kAMDWatchKitOneAppHasPlugins 				 				= 0xE80000D3
	kAMDWatchKitTwoAppMissingExtension 	 						= 0xE80000D4
	kAMDInvalidCompanionAppBundleIdentifier 					= 0xE80000D5
	kAMDWatchKitAppInvalidInfoPlistKey 							= 0xE80000D6
	kAMDWatchKitAppIllegalInfoPlistKey 							= 0xE80000D7
	kAMDMismatchedWatchKitAppBundleIDs 							= 0xE80000D8
	kAMDWatchKitAppBundleIDNotPrefixed 							= 0xE80000D9
	kAMDWatchKitAppInvalidUIDeviceFamily 						= 0xE80000DA
	kAMDWatchKitAppExtensionAttributesInvalidAppBundleID 		= 0xE80000C1
	kAMDAppQuotaExceeded 										= 0xE80000DB
	kAMDIntentsAppexMissingIntentsSupported 					= 0xE80000DC
	kAMDIntentsAppexIntentsRestrictedFormat 					= 0xE80000DD
	kAMDIntentsAppexIntentsRestrictedNotSubset 					= 0xE80000DE
	kAMDIntentsAppexIntentsSupportedDuplicated 					= 0xE80000DF
	kAMDWatchKitAppExtensionInDisallowedLocation 				= 0xE80000E0
	kAMDWatchKitAppFrameworkInDisallowedLocation 				= 0xE80000E1
	kAMDInvalidSinf 											= 0xE80000E3
	kAMDMultipleMessagePayloadProviderExtensionsFound 			= 0xE80000E4
	kAMDMessagesStubAppMissingAppExtension 						= 0xE80000E5


*********************************************************************/
#pragma once
#include <CoreFoundation\CFString.h>
#include <CoreFoundation\CFPropertyList.h>

typedef HANDLE AFCHANDLE;
//typedef unsigned long long afc_file_ref;
typedef __int64 afc_file_ref;
typedef afc_file_ref	AFCFILEHANDLE;

//struct AMDeviceNotificationCallbackInformation {
//	HANDLE	deviceHandle;
//	int		msgType;
//} ;

typedef HANDLE AMDeviceRef;
typedef HANDLE AMRecoveryModeDeviceRef;
typedef HANDLE AMDFUModeDeviceRef;
typedef HANDLE AMRestoreModeDeviceRef;
typedef HANDLE AMRestorableDeviceRef;  //now all device called AMRestorableDevice
typedef CFTypeRef MISProvisionRef;

typedef unsigned char *AMDeviceSubscriptionRef;

enum ADNCI_MSG {
	ADNCI_MSG_CONNECTED    = 1,
	ADNCI_MSG_DISCONNECTED = 2,
	ADNCI_MSG_UNSUBSCRIBED = 3
};

typedef void (__cdecl *AMRestoreDeviceNotificationCallback)(HANDLE, void* ctx);//am_recovery_device *

typedef void(*notify_callback)(CFStringRef notification, void *data);

#pragma pack(push,1)

typedef struct am_device_notification_callback_info {
	HANDLE deviceHandle;				/* 0    device */ 
	unsigned int msgType;					/* 4    one of ADNCI_MSG_* */
	struct am_device_notification* subscription; 
}AMDeviceNotificationCallbackInformation, *AMDeviceNotificationCallbackInformationRef;

typedef void (__cdecl *AMDeviceNotificationCallback)(AMDeviceNotificationCallbackInformationRef, void* ctx);
typedef void (__cdecl *AMRecoveryModeDeviceConnectionCallback)(AMRecoveryModeDeviceRef device, void* ctx);
typedef void (__cdecl *AMDFUModeDeviceConnectionCallback)(AMDFUModeDeviceRef device, void* ctx);

typedef struct am_device_notification {
	unsigned int unknown0;                      /* 0 */
	unsigned int unknown1;                      /* 4 */
	unsigned int unknown2;                      /* 8 */
	AMDeviceNotificationCallback callback;      /* 12 */ 
	unsigned int cookie;                        /* 16 */
}AMDeviceNotification, *AMDeviceNotificationRef;

#pragma pack(pop)



#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFError.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFNumber.h>
#include <CoreFoundation/CFURL.h>

/******************************************************************************/
//CFSafeRelease
#define CF_RELEASE_CLEAR(cf)  \
	do {                      \
		if ( (cf) != NULL ) { \
		CFRelease((cf));  \
		(cf) = NULL;        \
		}                     \
	} while ( 0 )

#define CF_RELEASE(cf)  \
	do {                      \
		if ( (cf) != NULL ) { \
		CFRelease((cf));  \
						}                     \
			} while ( 0 )
/******************************************************************************/

typedef enum {
	kAMNormalNotification = 0,
	kAMRestoreNotification = 1,
	kAMRecoveryNotification = 2,
	kAMDFUNotification = 3,
	kAMNoNotification = 4,
	kAMDWithoutCallBack=5
}AMModeNotifications;

typedef unsigned int(__cdecl *AMRestoreRegisterForDeviceNotifications)(AMDFUModeDeviceConnectionCallback DFUConnectCallback,
	AMRecoveryModeDeviceConnectionCallback recoveryConnectCallback,
	AMDFUModeDeviceConnectionCallback DFUDisconnectCallback,
	AMRecoveryModeDeviceConnectionCallback recoveryDisconnectCallback,
	unsigned int alwaysZero,
	void *userInfo);
//unsigned int AMRestoreRegisterForDeviceNotifications(AMRestoreDeviceNotificationCallback dfu_connect_callback, 
//	AMRestoreDeviceNotificationCallback recovery_connect_callback, 
//	AMRestoreDeviceNotificationCallback dfu_disconnect_callback, 
//	AMRestoreDeviceNotificationCallback recovery_disconnect_callback, 
//	unsigned int alwaysZero, 
//	void *ctx);
typedef int(__cdecl *AMDeviceNotificationSubscribe)(HANDLE, int, int, void *, void**);
typedef int(__cdecl *AMDeviceNotificationUnsubscribe)(HANDLE);
typedef CFArrayRef(__cdecl *AMDCreateDeviceList)();
typedef int(__cdecl * AMDCopyArrayOfDevicesMatchingQuery)(HANDLE, CFDictionaryRef, CFArrayRef*);
typedef int(__cdecl *AMDeviceConnect)(HANDLE);
typedef unsigned int(__cdecl *AMDeviceGetConnectionID)(HANDLE);
typedef int(__cdecl *AMDeviceDisconnect)(HANDLE);
typedef int(__cdecl *AMDeviceValidatePairing)(HANDLE);
typedef int(__cdecl *AMDevicePair)(HANDLE);
typedef int(__cdecl *AMDeviceUnpair)(HANDLE);
typedef int(__cdecl *AMDeviceExtendedPairWithOptions)(HANDLE, CFDictionaryRef, void*);
typedef BOOL(__cdecl *AMDeviceIsPaired)(HANDLE);//add jeffery
typedef BOOL(__cdecl *AMDeviceIsValid)(HANDLE);//add jeffery
typedef int(__cdecl *AMDeviceStartSession)(HANDLE);
typedef int(__cdecl *AMDeviceStopSession)(HANDLE);
typedef HANDLE(__cdecl *AMDServiceConnectionGetSecureIOContext)(HANDLE);//return is SSL *ssl_enabled=AMDServiceConnectionGetSecureIOContext
typedef HANDLE(__cdecl *AMDServiceConnectionGetSocket)(HANDLE);
typedef int(__cdecl *AFCConnectionSetSecureContext)(HANDLE, HANDLE ssl);
typedef HANDLE(__cdecl *AFCConnectionCreate)(int kloc, HANDLE, int, int, int);
typedef int(__cdecl *AMDeviceDeleteHostPairingRecordForUDID)(CFStringRef);

//third param if CFDictionaryref, if null , system add ["CloseOnInvalidate", kCFBooleanFalse]
typedef int(__cdecl *AMDeviceSecureStartService)(HANDLE, CFStringRef service_name, CFDictionaryRef, void*);
typedef int(__cdecl *AMDServiceConnectionInvalidate)(void*);
typedef int(__cdecl *AMDeviceStartService)(HANDLE, CFStringRef service_name, HANDLE*);//jeffery add
typedef int(__cdecl *AMDServiceConnectionReceive)(HANDLE, char *buf, int size);
typedef int(__cdecl *AMDServiceConnectionReceiveMessage)(HANDLE, char *buf, CFPropertyListFormat *format);
typedef int(__cdecl *AMDServiceConnectionSend)(HANDLE, char *buf, int size);//it is my guess
typedef int(__cdecl *AMDServiceConnectionSendMessage)(HANDLE, char *buf, int size);
typedef CFStringRef(__cdecl* AMDeviceCopyDeviceIdentifier)(HANDLE);
typedef int(__cdecl *AMRestorableDeviceRegisterForNotifications)(void* callback, void* args, CFErrorRef* err);
typedef int(__cdecl *AMRestorableDeviceRegisterForNotificationsForDevices)(void* callback, void* args, int unkown79, CFErrorRef* err);
typedef int(__cdecl *AMRestorableDeviceUnregisterForNotifications)(int);
typedef int(__cdecl *AMRestorableDeviceGetState)(HANDLE);
typedef CFMutableDictionaryRef(__cdecl *AMRestorableDeviceCopyDefaultRestoreOptions)();
typedef CFMutableDictionaryRef(__cdecl *AMRestoreCreateDefaultOptions)(int);
typedef void* (__cdecl *AMRestorableDeviceRestore)(HANDLE, CFMutableDictionaryRef options, void* callback, void* args);
typedef void* (__cdecl *AMRestorePerformRecoveryModeRestore)(HANDLE, CFMutableDictionaryRef options, void* callback, void* args);
typedef void* (__cdecl *AMRestorableDeviceCopyAMDevice)(HANDLE);
typedef int(__cdecl *AMDeviceEnterRecovery)(HANDLE);
typedef int(__cdecl *AMRecoveryModeDeviceReboot)(AMRecoveryModeDeviceRef);
typedef int(__cdecl *AMRecoveryModeDeviceSetAutoBoot)(AMRecoveryModeDeviceRef, int);
typedef LONGLONG(__cdecl *AMRecoveryModeDeviceGetECID)(AMRecoveryModeDeviceRef);
typedef CFStringRef(__cdecl* AMRecoveryModeDeviceCopySerialNumber)(AMRecoveryModeDeviceRef);
typedef CFStringRef(__cdecl* AMRecoveryModeDeviceCopyIMEI)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeDeviceGetLocationID)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeDeviceGetProductID)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeDeviceGetProductType)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeGetSoftwareBuildVersion)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeDeviceGetBoardID)(AMRecoveryModeDeviceRef);
typedef int(__cdecl* AMRecoveryModeDeviceGetChipID)(AMRecoveryModeDeviceRef);																														  

typedef int(__cdecl* AMDeviceGetWirelessBuddyFlags)(HANDLE device, int *flags);
typedef int(__cdecl* AMDeviceSetWirelessBuddyFlags)(HANDLE device, int flags);
typedef int(__cdecl* AMDeviceGetInterfaceType)(HANDLE device);//0=Unknown, 1 = USB, 2 = WIFI
typedef int(__cdecl* AMDeviceUSBDeviceID)(HANDLE device);
typedef int(__cdecl* AMDeviceUSBLocationID)(HANDLE device);
typedef int(__cdecl* AMDeviceUSBProductID)(HANDLE device);
																	 

typedef int(_cdecl * AMRecoveryModeDeviceSendCommandToDevice)(AMRecoveryModeDeviceRef device, CFStringRef command);
typedef int(_cdecl * AMRecoveryModeDeviceSendBlindCommandToDevice)(AMRecoveryModeDeviceRef device, CFStringRef command);

typedef LONGLONG(__cdecl *AMDFUModeDeviceGetECID)(AMDFUModeDeviceRef);
typedef LONGLONG(__cdecl *AMRestorableDeviceGetECID)(AMRestorableDeviceRef);
typedef CFStringRef(__cdecl *AMRestorableDeviceCopySerialNumber)(HANDLE);
typedef CFStringRef(__cdecl *AMRestorableDeviceCopySerialDevicePath)(HANDLE);
typedef int(__cdecl *AMRestoreModeDeviceReboot)(HANDLE);
typedef void* (__cdecl *AMRestorableDeviceCopyRestoreModeDevice)(HANDLE);
typedef void* (__cdecl *AMRestorableDeviceCopyRecoveryModeDevice)(HANDLE);
typedef CFStringRef(__cdecl *AMDeviceCopyValue)(HANDLE, CFStringRef, CFStringRef);
typedef int(__cdecl *AMDeviceSetValue)(HANDLE, CFStringRef, CFStringRef, CFTypeRef);
typedef int(__cdecl *AMDeviceRemoveValue)(HANDLE, CFStringRef, CFStringRef);
typedef int(__cdecl *AMDeviceDeactivate)(HANDLE device);//jeffery add
typedef int(__cdecl *AMDeviceActivate)(HANDLE device, CFMutableDictionaryRef);//jeffery add
typedef CFTypeRef(__cdecl *AMDeviceCreateActivationSessionInfo)(HANDLE device, int *errcode);
typedef int(__cdecl *AMDeviceActivateWithOptions)(HANDLE device, CFTypeRef pl_p, CFTypeRef pl_header);
typedef CFTypeRef(__cdecl *AMDeviceCreateActivationInfoWithOptions)(HANDLE device, CFTypeRef pdata, int zero, int *errcode);
typedef CFTypeRef(__cdecl *AMDeviceCreateActivationInfo)(HANDLE device, CFTypeRef pdata, int *errcode);								 

//AFC function// add jeffery
typedef int(__cdecl *AFCConnectionOpen)(HANDLE handle, unsigned int io_timeout, AFCHANDLE *conn);
typedef int(__cdecl *AFCConnectionClose)(AFCHANDLE conn);
typedef int(__cdecl *AFCDeviceInfoOpen)(HANDLE conn, HANDLE *info);
typedef int(__cdecl *AFCKeyValueRead)(AFCHANDLE dict, char **key, char **val);
typedef int(__cdecl *AFCKeyValueClose)(AFCHANDLE dict);
typedef int(__cdecl *AFCFileInfoOpen)(AFCHANDLE conn, const char *path, HANDLE *info);

typedef int(__cdecl *AFCFileRefTell)(AFCHANDLE conn, AFCFILEHANDLE fileHandle, int *len);
typedef int(__cdecl *AFCFlushData)(AFCHANDLE conn, AFCFILEHANDLE fileHandle);


/* mode 2 = read, mode 3 = write */
typedef int(__cdecl *AFCFileRefOpen)(HANDLE devHandle, const char *path, unsigned long long mode, AFCFILEHANDLE *fileHandle);
typedef int(__cdecl *AFCFileRefSeek)(HANDLE devHandle, AFCFILEHANDLE fileHandle, unsigned int offset1, unsigned int offset2);
typedef int(__cdecl *AFCFileRefRead)(HANDLE devHandle, AFCFILEHANDLE fileHandle, void *buf, unsigned int *len);
typedef int(__cdecl *AFCFileRefSetFileSize)(HANDLE devHandle, AFCFILEHANDLE fileHandle, unsigned long long offset);
typedef int(__cdecl *AFCFileRefWrite)(HANDLE devHandle, AFCFILEHANDLE fileHandle, const void *buf, unsigned int len);
typedef int(__cdecl *AFCFileRefClose)(HANDLE devHandle, AFCFILEHANDLE fileHandle);

//Directory
typedef int(__cdecl *AFCDirectoryOpen)(HANDLE devHandle, const char *path, HANDLE *hDirectory);
typedef int(__cdecl *AFCDirectoryClose)(HANDLE devHandle, HANDLE hDirectory);
typedef int(__cdecl *AFCDirectoryCreate)(HANDLE devHandle, const char *dirname);
typedef int(__cdecl *AFCDirectoryRead)(HANDLE devHandle/*unsigned int unused*/, HANDLE hDirectory, char **dirent);
typedef int(__cdecl *AFCRemovePath)(HANDLE devHandle, const char *dirname);
typedef int(__cdecl *AFCRenamePath)(HANDLE devHandle, const char *from, const char *to);
typedef int(__cdecl *AFCLinkPath)(HANDLE devHandle, long long int linktype, const char *target, const char *linkname);

//install app
typedef int(__cdecl *AMDeviceSecureTransferPath)(int unknown0, HANDLE devHandle, CFURLRef url,
	CFDictionaryRef options, void *callback, int callback_arg); //unkwon0 is install_proxy service handle, default is 0
typedef int(__cdecl *AMDeviceSecureInstallApplication)(int unknown0, HANDLE devHandle, CFURLRef url,
	CFDictionaryRef options, void *callback, int callback_arg);
typedef int(__cdecl *AMDeviceSecureUninstallApplication)(int unknown0, HANDLE devHandle, CFStringRef bundle_id,
	int unknown1, void *callback, int callback_arg);
typedef int(__cdecl *AMDeviceLookupApplications)(HANDLE devHandle, CFDictionaryRef opts, CFDictionaryRef* apps);
typedef int(__cdecl *AMDeviceLookupApplicationArchives)(HANDLE devHandle, CFDictionaryRef opts, CFDictionaryRef* apps);
typedef int(__cdecl *AMDeviceArchiveApplication)(HANDLE devHandle, CFStringRef appID, void* funcCallW2Params, void* passedToCallback, void* unknown);

typedef void(*install_callback_t)(CFDictionaryRef, int);
typedef int(__cdecl *AMDeviceInstallApplication)(HANDLE devHandle, CFStringRef path, CFDictionaryRef options, install_callback_t callback, void *callback_arg);


//ProvisioningProfile
typedef CFArrayRef(__cdecl *AMDeviceCopyProvisioningProfiles)(HANDLE devHandle);
typedef int(__cdecl *AMDeviceInstallProvisioningProfile)(HANDLE devHandle, MISProvisionRef provision);
typedef int(__cdecl *AMDeviceRemoveProvisioningProfile)(HANDLE devHandle, CFStringRef bundid);
/*
typedef void (*mount_callback_t)(CFDictionaryRef, int);
int (__cdecl *AMDeviceMountImage)(HANDLE devHandle, CFStringRef image, CFDictionaryRef options, mount_callback_t callback, void *callback_arg);
typedef void (*transfer_callback_t)(CFDictionaryRef, int);
int (__cdecl *AMDeviceTransferApplication)(AFCFILEHANDLE afcFd, CFStringRef path, void *null, transfer_callback_t callback, void *callback_arg);
typedef void (*install_callback_t)(CFDictionaryRef, int);
int (__cdecl *AMDeviceInstallApplication)(HANDLE installFd, CFStringRef path, CFDictionaryRef options, install_callback_t callback, void *callback_arg);
typedef void (*uninstall_callback_t)(CFDictionaryRef, int);
int (__cdecl *AMDeviceUninstallApplication)(HANDLE installFd, CFStringRef bundleId, CFDictionaryRef options, uninstall_callback_t callback, void *callback_arg);

*/

typedef int(__cdecl *AMDeviceStartHouseArrestService)(HANDLE devHandle, CFStringRef identifier, CFDictionaryRef dict, HANDLE *svrhandle, unsigned int *what);
typedef int(__cdecl *AMDeviceCreateHouseArrestService)(HANDLE devHandle, CFStringRef identifier, CFDictionaryRef dict, HANDLE *svrhandle);

typedef int(__cdecl *USBMuxConnectByPort)(unsigned int muxConn, short netPort, int* sockHandle);

// Log functions (iSn0wra1n)
typedef void(__cdecl *AMRestoreSetLogLevel)(int level);
typedef int(__cdecl *AMRestoreEnableFileLogging)(const char *filename);
typedef void(__cdecl *AMDSetLogLevel)(int level);
/// typedef void(__cdecl *AMDAddLogFileDescriptor)(int fd); //apple has removed the api

typedef CFStringRef(__cdecl *AMDCopyErrorText)(int);

typedef int(__cdecl *AMDPostNotification)(HANDLE socket, CFStringRef  notification, CFStringRef userinfo);
typedef int(__cdecl *AMDObserveNotification)(HANDLE socket, CFStringRef notification);
typedef int(__cdecl *AMDListenForNotifications)(HANDLE service, notify_callback cb, void *data);
typedef int(__cdecl *AMDShutdownNotificationProxy)(HANDLE socket);

typedef int(__cdecl *AMDSecurePostNotification)(HANDLE socket, CFStringRef  notification, CFStringRef userinfo);
typedef int(__cdecl *AMDSecureObserveNotification)(HANDLE socket, CFStringRef notification);
typedef int(__cdecl *AMDSecureListenForNotifications)(HANDLE service, notify_callback cb, void *data);
typedef int(__cdecl *AMDSecureShutdownNotificationProxy)(HANDLE socket);
//bool AMDeviceIsAtLeastVersionOnPlatform(AMDeviceRef device, CFDictionaryRef vars)
typedef bool(__cdecl *AMDeviceIsAtLeastVersionOnPlatform)(AMDeviceRef device, CFDictionaryRef vars);

//MISProfile
typedef MISProvisionRef(__cdecl *MISProfileCreateWithFile)(CFAllocatorRef allocator, CFStringRef xFileName);
typedef MISProvisionRef(__cdecl *MISProfileCreateWithData)(CFAllocatorRef allocator, CFDataRef data);
typedef DWORD(__cdecl *MISProfileWriteToFile)(MISProvisionRef misobject, CFStringRef xFileName);
typedef CFTypeRef(__cdecl *MISProfileGetValue)(MISProvisionRef misobject, CFStringRef key);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetProvisionedDevices)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetCreationDate)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetDeveloperCertificates)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetExpirationDate)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetName)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetUUID)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileGetVersion)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileIncludesDevice)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileProvisionsAllDevices)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileValidateSignature)(MISProvisionRef misobject);
typedef	CFTypeRef(__cdecl *MISProvisioningProfileCheckValidity)(MISProvisionRef misobject);

//new iTunes
typedef CFStringRef(__cdecl *GetMobileDeviceSupportPath)();
typedef CFStringRef(__cdecl *AMDGetPairingRecordDirectoryPath)();
typedef CFStringRef(__cdecl *AMDGetBundleVersion)();
typedef int(__cdecl *_InitializeMobileDevice)();
typedef AMDeviceRef(__cdecl *AMDeviceCreateFromProperties)(CFDictionaryRef);


typedef enum AMSStatus
{
	// XXX: Define status codes
	AMSStatus_OK = 0,
	AMSStatus_Unsupported = 1,
	AMSStatus_ComponentsMissing = 2,
	AMSStatus_GenericError /* placeholder */
} AMSStatus;


/* Need to call this first before calling anything else. */
//extern AMSStatus AMSInitialize(CFStringRef path);
typedef int(__cdecl *AMSInitialize)(CFStringRef path);
//extern AMSStatus AMSCleanup(void);
typedef int(__cdecl *AMSCleanup)();

/* This progress callback is made while copying or submitting crash logs. The progress dictionary contains the keys
   defined below. */
typedef void(*AMSCrashReportCopyProgressCallback) (CFStringRef targetIdentifier, CFDictionaryRef progress, void *context);

/* Connect to a crash report copy target. If the device is not paired, this returns a kMobileSyncCRCNotPaired error */
//extern AMSStatus AMSConnectToCrashReportCopyTarget(CFStringRef targetIdentifier, int *errorCode);
typedef int(__cdecl *AMSConnectToCrashReportCopyTarget)(CFStringRef targetIdentifier, int *errorCode);

/* Disconnect from the crash report copy target. */
//extern AMSStatus AMSDisconnectFromCrashReportCopyTarget(CFStringRef targetIdentifier, int *errorCode);
typedef int(__cdecl *AMSDisconnectFromCrashReportCopyTarget)(CFStringRef targetIdentifier, int *errorCode);

/* Copies crash logs from the device to the desktop. Note that this does not submit the crash reports to Apple.
   It is ok to call this without first having called AMSGetNumberOfCrashReportsToCopy. */
//extern AMSStatus AMSCopyCrashReportsFromTarget(CFStringRef targetIdentifier, AMSCrashReportCopyProgressCallback callback, int *errorCode, void *context);
typedef int(__cdecl *AMSCopyCrashReportsFromTarget)(CFStringRef targetIdentifier, AMSCrashReportCopyProgressCallback callback, int *errorCode, void *context);

/* Returns the local path where crash reports are stored for a given target. Since the crash reports are stored
   at a path using the device's name, not just the target identifier, the device needs to be plugged in to get
   this path.
   It is possible that you could call this back and get a path that does not exist. The returned path is the
   directory that would be created if any crash logs are copied off the device. */
//AMSStatus AMSCopyCrashReportPath(CFStringRef targetIdentifier, CFStringRef *crashReportPath, int *errorCode);
typedef int(__cdecl *AMSCopyCrashReportPath)(CFStringRef targetIdentifier, CFStringRef *crashReportPath, int *errorCode);

/* Cancels any existing crash report copy or submission for the specified device */
//extern AMSStatus AMSCancelCrashReportCopy(CFStringRef targetIdentifier, int *errorCode);
typedef int(__cdecl *AMSCancelCrashReportCopy)(CFStringRef targetIdentifier, int *errorCode);



typedef struct _iTunesMobileDevice
{
	_iTunesMobileDevice() :my_kCFTypeArrayCallBacks(NULL)
	{
		ZeroMemory(this,sizeof(_iTunesMobileDevice));
		hEventDevArrive = CreateEvent(NULL, false, false, NULL);
	};
	~_iTunesMobileDevice()
	{
		if (hDll!=NULL)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
		if (hEventDevArrive != NULL) {
			CloseHandle(hEventDevArrive);
			hEventDevArrive = NULL;
		}
	}

	CFDictionaryKeyCallBacks my_kCFTypeDictionaryKeyCallBacks;
	CFDictionaryValueCallBacks my_kCFTypeDictionaryValueCallBacks;
	CFArrayCallBacks* my_kCFTypeArrayCallBacks;
	CFStringRef my_kCFStreamPropertyDataWritten;
	CFBooleanRef  my_kCFBooleanTrue;
	CFBooleanRef  my_kCFBooleanFalse;

	HMODULE hDll = NULL;
	int _AMRestorableDeviceRegisterForNotificationsId;
	AMDeviceNotificationRef _notification = NULL;
	CFRunLoopRef PCurloopRun = NULL;
	HANDLE hEventDevArrive = NULL;

#define declare_entry(f)        f f

	declare_entry(AMRestoreRegisterForDeviceNotifications);
	declare_entry(AMDeviceNotificationSubscribe);
	declare_entry(AMDeviceNotificationUnsubscribe);
															 
	declare_entry(AMDCreateDeviceList);
	declare_entry(AMDCopyArrayOfDevicesMatchingQuery);
	declare_entry(AMDeviceConnect);
	declare_entry(AMDeviceGetConnectionID);
	declare_entry(AMDeviceDisconnect);
	declare_entry(AMDeviceValidatePairing);
	declare_entry(AMDevicePair);
	declare_entry(AMDeviceUnpair);
	declare_entry(AMDeviceExtendedPairWithOptions);
	declare_entry(AMDeviceIsPaired);
	declare_entry(AMDeviceIsValid);
	declare_entry(AMDeviceStartSession);
	declare_entry(AMDeviceStopSession);
	declare_entry(AMDServiceConnectionGetSecureIOContext);
	declare_entry(AMDServiceConnectionGetSocket);
	declare_entry(AFCConnectionSetSecureContext);
	declare_entry(AFCConnectionCreate);
	declare_entry(AMDeviceDeleteHostPairingRecordForUDID);

	declare_entry(AMDeviceSecureStartService);
	declare_entry(AMDServiceConnectionInvalidate);
	declare_entry(AMDeviceStartService);
	declare_entry(AMDServiceConnectionReceive);
	declare_entry(AMDServiceConnectionReceiveMessage);
	declare_entry(AMDServiceConnectionSend);
	declare_entry(AMDServiceConnectionSendMessage);
	declare_entry(AMDeviceCopyDeviceIdentifier);
	declare_entry(AMRestorableDeviceRegisterForNotifications);
	declare_entry(AMRestorableDeviceRegisterForNotificationsForDevices);
	declare_entry(AMRestorableDeviceUnregisterForNotifications);
	declare_entry(AMRestorableDeviceGetState);
	declare_entry(AMRestorableDeviceCopyDefaultRestoreOptions);
	declare_entry(AMRestoreCreateDefaultOptions);
	declare_entry(AMRestorableDeviceRestore);
	declare_entry(AMRestorePerformRecoveryModeRestore);
	declare_entry(AMRestorableDeviceCopyAMDevice);
	declare_entry(AMDeviceEnterRecovery);
	declare_entry(AMRecoveryModeDeviceReboot);
	declare_entry(AMRecoveryModeDeviceSetAutoBoot);
	declare_entry(AMRecoveryModeDeviceGetECID);
	declare_entry(AMRecoveryModeDeviceCopySerialNumber);
	declare_entry(AMRecoveryModeDeviceCopyIMEI);
	declare_entry(AMRecoveryModeDeviceGetLocationID);
	declare_entry(AMRecoveryModeDeviceGetProductID);
	declare_entry(AMRecoveryModeDeviceGetProductType);
	declare_entry(AMRecoveryModeGetSoftwareBuildVersion);
	declare_entry(AMRecoveryModeDeviceGetBoardID);
	declare_entry(AMRecoveryModeDeviceGetChipID);
	declare_entry(AMDeviceGetWirelessBuddyFlags);
	declare_entry(AMDeviceSetWirelessBuddyFlags);
	declare_entry(AMDeviceGetInterfaceType);
	declare_entry(AMDeviceUSBDeviceID);//                                                            100A3A10 356
	declare_entry(AMDeviceUSBLocationID); //                                                        100A3A40 357
	declare_entry(AMDeviceUSBProductID);//

	declare_entry(AMRecoveryModeDeviceSendCommandToDevice);
	declare_entry(AMRecoveryModeDeviceSendBlindCommandToDevice);


	declare_entry(AMDFUModeDeviceGetECID);
	declare_entry(AMRestorableDeviceGetECID);
	declare_entry(AMRestorableDeviceCopySerialNumber);
	declare_entry(AMRestorableDeviceCopySerialDevicePath);
	declare_entry(AMRestoreModeDeviceReboot);
	declare_entry(AMRestorableDeviceCopyRestoreModeDevice);
	declare_entry(AMRestorableDeviceCopyRecoveryModeDevice);
	declare_entry(AMDeviceCopyValue);
	declare_entry(AMDeviceSetValue);
	declare_entry(AMDeviceRemoveValue);
	declare_entry(AMDeviceDeactivate);
	declare_entry(AMDeviceActivate);
	declare_entry(AMDeviceCreateActivationSessionInfo);//(HANDLE device, int *errcode);
	declare_entry(AMDeviceActivateWithOptions);//(HANDLE device, CFTypeRef pl_p, CFTypeRef pl_header);
	declare_entry(AMDeviceCreateActivationInfoWithOptions);//(HANDLE device, CFTypeRef pdata, int zero, int *errcode);
	declare_entry(AMDeviceCreateActivationInfo);//(HANDLE device, CFTypeRef pdata, int *errcode);
		 
																		  
																							  

	

	//AFC function// add jeffery
	declare_entry(AFCConnectionOpen);
	declare_entry(AFCConnectionClose);
	declare_entry(AFCDeviceInfoOpen);
	declare_entry(AFCKeyValueRead);
	declare_entry(AFCKeyValueClose);
	declare_entry(AFCFileInfoOpen);

	declare_entry(AFCFileRefTell);
	declare_entry(AFCFlushData);


	/* mode 2 = read, mode 3 = write */
	declare_entry(AFCFileRefOpen);
	declare_entry(AFCFileRefSeek);
	declare_entry(AFCFileRefRead);
	declare_entry(AFCFileRefSetFileSize);
	declare_entry(AFCFileRefWrite);
	declare_entry(AFCFileRefClose);

	//Directory
	declare_entry(AFCDirectoryOpen);
	declare_entry(AFCDirectoryClose);
	declare_entry(AFCDirectoryCreate);
	declare_entry(AFCDirectoryRead);
	declare_entry(AFCRemovePath);
	declare_entry(AFCRenamePath);
	declare_entry(AFCLinkPath);

	//install app
	declare_entry(AMDeviceSecureTransferPath);
	declare_entry(AMDeviceSecureInstallApplication);
	declare_entry(AMDeviceSecureUninstallApplication);
	declare_entry(AMDeviceLookupApplications);
	declare_entry(AMDeviceLookupApplicationArchives);
	declare_entry(AMDeviceArchiveApplication);

	declare_entry(AMDeviceInstallApplication);

	//provisioning profile
	declare_entry(AMDeviceCopyProvisioningProfiles);
	declare_entry(AMDeviceInstallProvisioningProfile);
	declare_entry(AMDeviceRemoveProvisioningProfile);


	declare_entry(AMDeviceStartHouseArrestService);
	declare_entry(AMDeviceCreateHouseArrestService);//

	declare_entry(USBMuxConnectByPort);

	// Log functions (iSn0wra1n)
	declare_entry(AMRestoreSetLogLevel);
	declare_entry(AMRestoreEnableFileLogging);
	declare_entry(AMDSetLogLevel);
	//declare_entry(AMDAddLogFileDescriptor);

	declare_entry(AMDCopyErrorText);

	declare_entry(AMDPostNotification);
	declare_entry(AMDObserveNotification);
	declare_entry(AMDListenForNotifications);
	declare_entry(AMDShutdownNotificationProxy); 

	declare_entry(AMDSecurePostNotification);
	declare_entry(AMDSecureObserveNotification);
	declare_entry(AMDSecureListenForNotifications);
	declare_entry(AMDSecureShutdownNotificationProxy);
	declare_entry(AMDeviceIsAtLeastVersionOnPlatform);

	//MISProfile
	declare_entry(MISProfileCreateWithFile);
	declare_entry(MISProfileCreateWithData);
	declare_entry(MISProfileWriteToFile);
	declare_entry(MISProfileGetValue);
	declare_entry(MISProvisioningProfileGetProvisionedDevices);
	declare_entry(MISProvisioningProfileGetCreationDate);
	declare_entry(MISProvisioningProfileGetDeveloperCertificates);
	declare_entry(MISProvisioningProfileGetExpirationDate);
	declare_entry(MISProvisioningProfileGetName);
	declare_entry(MISProvisioningProfileGetUUID);
	declare_entry(MISProvisioningProfileGetVersion);
	declare_entry(MISProvisioningProfileIncludesDevice);
	declare_entry(MISProvisioningProfileProvisionsAllDevices);
	declare_entry(MISProvisioningProfileValidateSignature);
	declare_entry(MISProvisioningProfileCheckValidity);

	//new iTunes
	declare_entry(GetMobileDeviceSupportPath);
	declare_entry(AMDGetPairingRecordDirectoryPath);
	declare_entry(AMDGetBundleVersion);
	declare_entry(_InitializeMobileDevice);
	declare_entry(AMDeviceCreateFromProperties);


	declare_entry(AMSInitialize);
	declare_entry(AMSCleanup);
	declare_entry(AMSConnectToCrashReportCopyTarget);
	declare_entry(AMSDisconnectFromCrashReportCopyTarget);
	declare_entry(AMSCopyCrashReportsFromTarget);
	declare_entry(AMSCopyCrashReportPath);
	declare_entry(AMSCancelCrashReportCopy);

#undef declare_entry

}_iTunesMobileDevice_t;


void uninit_iTunesMobileDevice();
BOOL init_iTunesMobileDevice(AMModeNotifications notification,  int nTimeout = 5000);
void SetVerbose(BOOL b);
void PrintMobileDeviceDLLVersion();
