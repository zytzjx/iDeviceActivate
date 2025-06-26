/********************************************************************
	created:	2015/09/23
	created:	23:9:2015   18:10
	filename: 	J:\Works\iDeviceUtilCore\iDeviceUtilCore\iTunesMobileDevice.cpp
	file path:	J:\Works\iDeviceUtilCore\iDeviceUtilCore
	file base:	iTunesMobileDevice
	file ext:	cpp
	author:		Jeffery Zhang
	
	purpose:	iTuneMobileDevice.dll interface
*********************************************************************/
#include "stdafx.h"
#include "iDeviceUtil.h"
#include "iTunesMobileDevice.h"

_iTunesMobileDevice_t g_iTunesMobileDevice;


void eventHandler(void* _device, int type, void *arg)
{// type 0 is connected, type 1 is disconnected
#define DEV_CONNECTED		0
#define DEV_DISCONNECTED	1
#define DEV_STATE_DFUMODE		1
#define DEV_STATE_RECOVERY		2
#define DEV_STATE_RESTORABLE	3
#define DEV_STATE_NORMAL		4
	if (type==DEV_CONNECTED)
	{
		// connect
		int device_state=g_iTunesMobileDevice.AMRestorableDeviceGetState(_device);
		switch(device_state)
		{
		case DEV_STATE_DFUMODE:// Dfu mode
			break;
		case DEV_STATE_RECOVERY:
			{
				char buffer[MAX_PATH]={0};
				//CFStringRef srnm=g_iTunesMobileDevice.AMRestorableDeviceCopySerialNumber(_device);
				int64_t ecid = g_iTunesMobileDevice.AMRestorableDeviceGetECID(_device);
				sprintf_s(buffer, "%016I64X", ecid);

				AMRecoveryModeDeviceRef device = g_iTunesMobileDevice.AMRestorableDeviceCopyRecoveryModeDevice(_device);
				if (strlen(buffer)>0)//if (srnm!=NULL)
				{
					//CFStringGetCString(srnm, buffer, MAX_PATH, kCFStringEncodingUTF8);
					logIt(buffer);
					if (device!=NULL)
					{
						g_args.deivces_recovery.add_device(buffer, device);
					}
				}
			}
			break;
		case DEV_STATE_RESTORABLE:
			{
				char buffer[MAX_PATH]={0};
				CFStringRef srnm=g_iTunesMobileDevice.AMRestorableDeviceCopySerialNumber(_device);
				AMRestoreModeDeviceRef device=g_iTunesMobileDevice.AMRestorableDeviceCopyRestoreModeDevice(_device);
				if (srnm!=NULL)
				{
					CFStringGetCString(srnm, buffer, MAX_PATH, kCFStringEncodingUTF8);
					logIt(buffer);
					if (device!=NULL)
					{
						g_args.deivces_restore.add_device(buffer, device);
					}
				}
			}
			break;
		case DEV_STATE_NORMAL:
			{
				char _udid[MAX_PATH];
				ZeroMemory(_udid,sizeof(_udid));
				void* device=g_iTunesMobileDevice.AMRestorableDeviceCopyAMDevice(_device);
				if (device!=NULL)
				{
					CFStringRef udid=g_iTunesMobileDevice.AMDeviceCopyDeviceIdentifier(device);
					if(udid!=NULL)
					{
						CFStringGetCString(udid, _udid, 128, kCFStringEncodingUTF8);
						g_args.deivces.add_device(_udid, device);
						CF_RELEASE_CLEAR(udid);
						if (g_args.hEevntFound != NULL && g_args.device_udid.compare(_udid) == 0)
						{
							SetEvent(g_args.hEevntFound);
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else if (type==DEV_DISCONNECTED)
	{
		// disconnect
		int device_state=g_iTunesMobileDevice.AMRestorableDeviceGetState(_device);
		switch(device_state)
		{
		case DEV_STATE_DFUMODE:// Dfu mode
			break;
		case DEV_STATE_RECOVERY:
			{
				char buffer[MAX_PATH]={0};
				CFStringRef srnm=g_iTunesMobileDevice.AMRestorableDeviceCopySerialNumber(_device);
				if (srnm!=NULL)
				{
					CFStringGetCString(srnm, buffer, MAX_PATH, kCFStringEncodingUTF8);
					g_args.deivces_recovery.remove_device(buffer);
				}
			}
			break;
		case DEV_STATE_RESTORABLE:
			{
				char buffer[MAX_PATH]={0};
				CFStringRef srnm=g_iTunesMobileDevice.AMRestorableDeviceCopySerialNumber(_device);
				if (srnm!=NULL)
				{
					CFStringGetCString(srnm, buffer, MAX_PATH, kCFStringEncodingUTF8);
					g_args.deivces_restore.remove_device(buffer);
				}
			}
			break;
		case DEV_STATE_NORMAL:
			{
				char _udid[MAX_PATH];
				ZeroMemory(_udid,sizeof(_udid));
				HANDLE device=g_iTunesMobileDevice.AMRestorableDeviceCopyAMDevice(_device);
				if (device!=NULL)
				{
					CFStringRef udid=g_iTunesMobileDevice.AMDeviceCopyDeviceIdentifier(device);
					if(udid!=NULL)
					{
						CFStringGetCString(udid, _udid, 128, kCFStringEncodingUTF8);
						g_args.deivces.remove_device(_udid);
						CF_RELEASE_CLEAR(udid);
					}
				}
			}
			break;
		default:
			break;
		}
	}
	else
	{
		// ??
	}
}


void deviceNotificationCallback(AMDeviceNotificationCallbackInformationRef info, void *user)
{
	CFStringRef udid;
	PARAMETERS_T *pThis = (PARAMETERS_T  *)user;
	char c_value[512];
	switch (info->msgType)
	{
	case ADNCI_MSG_CONNECTED:
		{
			udid=g_iTunesMobileDevice.AMDeviceCopyDeviceIdentifier(info->deviceHandle);
			CFStringGetCString(udid, c_value, 510, kCFStringEncodingUTF8);
			pThis->deivces.add_device(c_value, info->deviceHandle);
			if (pThis->device_udid.empty()) {
				SetEvent(g_iTunesMobileDevice.hEventDevArrive);
			}
			else if(pThis->hEevntFound != NULL && pThis->device_udid.compare(c_value) == 0)
			{
				SetEvent(pThis->hEevntFound);
				SetEvent(g_iTunesMobileDevice.hEventDevArrive);
			}
			//pThis->deivces.PrintList();
		}
		break;
	case ADNCI_MSG_DISCONNECTED:
		{
			udid=g_iTunesMobileDevice.AMDeviceCopyDeviceIdentifier(info->deviceHandle);
			CFStringGetCString(udid, c_value, 510, kCFStringEncodingUTF8);
			pThis->deivces.remove_device(c_value);
		}
		break;
	case ADNCI_MSG_UNSUBSCRIBED:// 驱动被卸载时会走到这里 （该信号值未完全确定是代表Itunes环境被卸载）
		break;
	default:break;
	}
}
   
DWORD WINAPI PROCTHREAD(LPVOID pParam) {
	//Children Thread CFRunLoop Exit
	int* pnTimeOut = (int*)pParam;

	try
	{
		if (WaitForSingleObject(g_iTunesMobileDevice.hEventDevArrive, *pnTimeOut) != WAIT_OBJECT_0) {
			*pnTimeOut = 1;
		}
		CFRunLoopStop(g_iTunesMobileDevice.PCurloopRun);
		g_iTunesMobileDevice.PCurloopRun = NULL;
	}
	catch (...)
	{

	}
	return 0;
}
   

BOOL Register_iTunesMobileDevice_Callback(int nTimeout)
{
	BOOL ret=TRUE;
	ENTER_FUNC_BEGIN;
	int err=g_iTunesMobileDevice.AMDeviceNotificationSubscribe(&deviceNotificationCallback, 0, 1, 
		&g_args, (void **)&g_iTunesMobileDevice._notification);	
	if (err!=ERROR_SUCCESS)
	{
		ret=FALSE;
	}

	g_iTunesMobileDevice.PCurloopRun = CFRunLoopGetCurrent();
	CreateThread(NULL, 0, PROCTHREAD, &nTimeout, 0, NULL);
	CFRunLoopRun();
	if (nTimeout == 1) ret = FALSE;

	ENTER_FUNC_ENDRET(ret);
	return ret;
}

BOOL RestorableDevice_Register_CallBack()
{
	BOOL ret=TRUE;
	CFErrorRef err=NULL;
	ENTER_FUNC_BEGIN;

	if (g_iTunesMobileDevice.AMRestorableDeviceRegisterForNotificationsForDevices) {
		g_iTunesMobileDevice._AMRestorableDeviceRegisterForNotificationsId = g_iTunesMobileDevice.AMRestorableDeviceRegisterForNotificationsForDevices(eventHandler, (void *)&g_args, 79, &err);
		if (err == NULL)
			return ret;
		else {
			CFIndex i = CFErrorGetCode(err);
			g_args.error_code = i;
			//CFStringRef desc=(err);
			ret = FALSE;
		}
	}


	g_iTunesMobileDevice._AMRestorableDeviceRegisterForNotificationsId=g_iTunesMobileDevice.AMRestorableDeviceRegisterForNotifications(eventHandler, (void *)&g_args, &err);
	if (err!=NULL)
	{
		CFIndex i=CFErrorGetCode(err);
		g_args.error_code=i;
		ret=FALSE;
	}
	ENTER_FUNC_ENDRET(g_iTunesMobileDevice._AMRestorableDeviceRegisterForNotificationsId);
	return ret;
}

void uninit_iTunesMobileDevice()
{
	if (g_iTunesMobileDevice._notification!=NULL)
	{
		g_iTunesMobileDevice.AMDeviceNotificationUnsubscribe(g_iTunesMobileDevice._notification);
	}
	if (g_iTunesMobileDevice._AMRestorableDeviceRegisterForNotificationsId>0)
	{
		g_iTunesMobileDevice.AMRestorableDeviceUnregisterForNotifications(g_iTunesMobileDevice._AMRestorableDeviceRegisterForNotificationsId);
	}
	if (g_iTunesMobileDevice.PCurloopRun != NULL) {
		CFRunLoopStop(g_iTunesMobileDevice.PCurloopRun);
	}
}

void PrintMobileDeviceDLLVersion() {
	logIt("Bundle Version:");
	CFShow(g_iTunesMobileDevice.AMDGetBundleVersion());
}

BOOL init_iTunesMobileDevice(AMModeNotifications notification, int nTimeout)
{
	BOOL ret=FALSE;
	//SetCurrentDirectory(g_args.m_sPathAASupport.c_str());
	SetCurrentDirectory(g_args.m_sPathMDSupport.c_str());

	HMODULE h = LoadLibrary(_T("CoreFoundation.dll"));
	if (h != NULL)
	{
		g_iTunesMobileDevice.my_kCFTypeDictionaryKeyCallBacks = *(CFDictionaryKeyCallBacks*)GetProcAddress(h, "kCFTypeDictionaryKeyCallBacks");
		g_iTunesMobileDevice.my_kCFTypeDictionaryValueCallBacks = *(CFDictionaryValueCallBacks*)GetProcAddress(h, "kCFTypeDictionaryValueCallBacks");
		g_iTunesMobileDevice.my_kCFTypeArrayCallBacks = (CFArrayCallBacks*)GetProcAddress(h, "kCFTypeArrayCallBacks");
		g_iTunesMobileDevice.my_kCFStreamPropertyDataWritten = *(CFStringRef*)GetProcAddress(h, "kCFStreamPropertyDataWritten");
		g_iTunesMobileDevice.my_kCFBooleanTrue = *(CFBooleanRef*)GetProcAddress(h, "kCFBooleanTrue");
		g_iTunesMobileDevice.my_kCFBooleanFalse = *(CFBooleanRef*)GetProcAddress(h, "kCFBooleanFalse");

		//FreeLibrary(h);
	}
	else{
		OOPSERR(GetLastError());
	}


	g_iTunesMobileDevice.hDll = LoadLibrary(_T("MobileDevice.dll"));
	if (g_iTunesMobileDevice.hDll == NULL) {
		OOPSERR(GetLastError());
		g_iTunesMobileDevice.hDll = LoadLibrary(_T("iTunesMobileDevice.dll"));
	}
	if (g_iTunesMobileDevice.hDll!=NULL)
	{
		SetDllDirectory(NULL);
#define resolve_entry(f)	g_iTunesMobileDevice.f = (f)GetProcAddress(g_iTunesMobileDevice.hDll, #f);if(g_iTunesMobileDevice.f==NULL){logIt("can not load api %s",#f);}
		resolve_entry(AMRestoreRegisterForDeviceNotifications);
		resolve_entry(AMDeviceNotificationSubscribe);
		resolve_entry(AMDeviceNotificationUnsubscribe);
		resolve_entry(AMDCreateDeviceList);
		resolve_entry(AMDCopyArrayOfDevicesMatchingQuery);
		resolve_entry(AMDeviceConnect);
		resolve_entry(AMDeviceEnterRecovery);
		resolve_entry(AMRecoveryModeDeviceReboot);
		resolve_entry(AMRecoveryModeDeviceSetAutoBoot);
		resolve_entry(AMRecoveryModeDeviceGetECID);
		resolve_entry(AMRecoveryModeDeviceCopySerialNumber);
		resolve_entry(AMRecoveryModeDeviceCopyIMEI);
		resolve_entry(AMRecoveryModeDeviceGetLocationID);
		resolve_entry(AMRecoveryModeDeviceGetProductID);
		resolve_entry(AMRecoveryModeDeviceGetProductType);
		resolve_entry(AMRecoveryModeGetSoftwareBuildVersion);
		resolve_entry(AMRecoveryModeDeviceGetBoardID);
		resolve_entry(AMRecoveryModeDeviceGetChipID);
										
										   
		resolve_entry(AMDeviceGetWirelessBuddyFlags);
		resolve_entry(AMDeviceSetWirelessBuddyFlags);
		resolve_entry(AMDeviceGetInterfaceType);
		resolve_entry(AMDeviceUSBDeviceID);
		resolve_entry(AMDeviceUSBLocationID);
		resolve_entry(AMDeviceUSBProductID);
		resolve_entry(AMDeviceDeleteHostPairingRecordForUDID);



		resolve_entry(AMRecoveryModeDeviceSendCommandToDevice);
		resolve_entry(AMRecoveryModeDeviceSendBlindCommandToDevice);


		resolve_entry(AMDFUModeDeviceGetECID);
		resolve_entry(AMRestorableDeviceGetECID);
		resolve_entry(AMDeviceDisconnect);
		resolve_entry(AMDeviceValidatePairing);
		resolve_entry(AMDevicePair);
		resolve_entry(AMDeviceUnpair);
		resolve_entry(AMDeviceExtendedPairWithOptions);
		resolve_entry(AMDeviceIsPaired);
		resolve_entry(AMDeviceIsValid);
		resolve_entry(AMDeviceStartSession);
		resolve_entry(AMDeviceStopSession);
		resolve_entry(AMDServiceConnectionGetSecureIOContext);
		resolve_entry(AMDServiceConnectionGetSocket);
		resolve_entry(AFCConnectionSetSecureContext);
		resolve_entry(AFCConnectionCreate);

		resolve_entry(AMDeviceSecureStartService);
		resolve_entry(AMDServiceConnectionReceive);
		resolve_entry(AMDServiceConnectionSend);
		resolve_entry(AMDServiceConnectionReceiveMessage);
		resolve_entry(AMDServiceConnectionSendMessage);
		resolve_entry(AMDeviceCopyDeviceIdentifier);
		resolve_entry(AMRestorableDeviceCopySerialNumber);
		resolve_entry(AMRestorableDeviceCopySerialDevicePath);
		resolve_entry(AMRestorableDeviceRegisterForNotifications);
		resolve_entry(AMRestorableDeviceRegisterForNotificationsForDevices);
		resolve_entry(AMRestorableDeviceUnregisterForNotifications);
		resolve_entry(AMRestorableDeviceGetState);
		resolve_entry(AMRestorableDeviceCopyDefaultRestoreOptions);
		resolve_entry(AMRestoreCreateDefaultOptions);
		resolve_entry(AMRestorableDeviceRestore);
		resolve_entry(AMRestorePerformRecoveryModeRestore);
		resolve_entry(AMRestorableDeviceCopyAMDevice);
		resolve_entry(AMRestorableDeviceCopyRestoreModeDevice);
		resolve_entry(AMRestorableDeviceCopyRecoveryModeDevice);
		resolve_entry(AMRestoreModeDeviceReboot);
		resolve_entry(AMDeviceCopyValue);
		resolve_entry(AMDeviceSetValue);
		resolve_entry(AMDeviceRemoveValue);
		
		resolve_entry(AMDeviceStartService);
		resolve_entry(AMDeviceDeactivate);
		resolve_entry(AMDeviceActivate);
		resolve_entry(AMDeviceCreateActivationSessionInfo);//(HANDLE device, int *errcode);
		resolve_entry(AMDeviceActivateWithOptions);//(HANDLE device, CFTypeRef pl_p, CFTypeRef pl_header);
		resolve_entry(AMDeviceCreateActivationInfoWithOptions);//(HANDLE device, CFTypeRef pdata, int zero, int *errcode);
		resolve_entry(AMDeviceCreateActivationInfo);//(HANDLE device, CFTypeRef pdata, int *errcode);

		
		resolve_entry(AFCConnectionOpen);
		resolve_entry(AFCConnectionClose);
		resolve_entry(AFCDeviceInfoOpen);
		resolve_entry(AFCKeyValueRead);
		resolve_entry(AFCKeyValueClose);
		resolve_entry(AFCFileInfoOpen);

		resolve_entry(AFCFileRefTell);
		resolve_entry(AFCFlushData);


		/* mode 2 = read, mode 3 = write */
		resolve_entry(AFCFileRefOpen);
		resolve_entry(AFCFileRefSeek);
		resolve_entry(AFCFileRefRead);
		resolve_entry(AFCFileRefSetFileSize);
		resolve_entry(AFCFileRefWrite);
		resolve_entry(AFCFileRefClose);

		resolve_entry(AFCDirectoryOpen);
		resolve_entry(AFCDirectoryClose);
		resolve_entry(AFCDirectoryCreate);
		resolve_entry(AFCDirectoryRead );
		resolve_entry(AFCRemovePath );
		resolve_entry(AFCRenamePath );
		resolve_entry(AFCLinkPath );



		resolve_entry(AMDeviceSecureTransferPath);
		resolve_entry(AMDeviceSecureInstallApplication);
		resolve_entry(AMDeviceSecureUninstallApplication);
		resolve_entry(AMDeviceLookupApplications);
		resolve_entry(AMDeviceLookupApplicationArchives);
		resolve_entry(AMDeviceArchiveApplication);

		resolve_entry(AMDeviceInstallApplication);


		//provisioning profile
		resolve_entry(AMDeviceCopyProvisioningProfiles);
		resolve_entry(AMDeviceInstallProvisioningProfile);
		resolve_entry(AMDeviceRemoveProvisioningProfile);


		resolve_entry(AMDeviceStartHouseArrestService);
		resolve_entry(AMDeviceCreateHouseArrestService);

		resolve_entry(AMDeviceGetConnectionID );
		resolve_entry(USBMuxConnectByPort );
		resolve_entry(AMRestoreSetLogLevel );
		resolve_entry(AMRestoreEnableFileLogging );
		resolve_entry(AMDSetLogLevel );
		//resolve_entry(AMDAddLogFileDescriptor );

		resolve_entry(AMDCopyErrorText);

		resolve_entry(AMDPostNotification );
		resolve_entry(AMDObserveNotification );
		resolve_entry(AMDListenForNotifications );
		resolve_entry(AMDShutdownNotificationProxy );

		resolve_entry(AMDSecurePostNotification);
		resolve_entry(AMDSecureObserveNotification);
		resolve_entry(AMDSecureListenForNotifications);
		resolve_entry(AMDSecureShutdownNotificationProxy);
		resolve_entry(AMDeviceIsAtLeastVersionOnPlatform);

		//MISProfile
		resolve_entry(MISProfileCreateWithFile);
		resolve_entry(MISProfileCreateWithData);
		resolve_entry(MISProfileWriteToFile);
		resolve_entry(MISProfileGetValue);
		resolve_entry(MISProvisioningProfileGetProvisionedDevices);
		resolve_entry(MISProvisioningProfileGetCreationDate);
		resolve_entry(MISProvisioningProfileGetDeveloperCertificates);
		resolve_entry(MISProvisioningProfileGetExpirationDate);
		resolve_entry(MISProvisioningProfileGetName);
		resolve_entry(MISProvisioningProfileGetUUID);
		resolve_entry(MISProvisioningProfileGetVersion);
		resolve_entry(MISProvisioningProfileIncludesDevice);
		resolve_entry(MISProvisioningProfileProvisionsAllDevices);
		resolve_entry(MISProvisioningProfileValidateSignature);
		resolve_entry(MISProvisioningProfileCheckValidity);

		//new itunes
		resolve_entry(GetMobileDeviceSupportPath);
		resolve_entry(AMDGetPairingRecordDirectoryPath);
		resolve_entry(AMDGetBundleVersion);
		//_FinalizeMobileDevice 100A3E90 561
		//_InitializeMobileDevice 100A3EA0 562
		resolve_entry(_InitializeMobileDevice);
		resolve_entry(AMDeviceCreateFromProperties);


		resolve_entry(AMSInitialize);
		resolve_entry(AMSCleanup);
		resolve_entry(AMSConnectToCrashReportCopyTarget);
		resolve_entry(AMSDisconnectFromCrashReportCopyTarget);
		resolve_entry(AMSCopyCrashReportsFromTarget);
		resolve_entry(AMSCopyCrashReportPath);
		resolve_entry(AMSCancelCrashReportCopy);
#undef	resolve_entry
		if (notification == kAMDWithoutCallBack) {
			//g_iTunesMobileDevice._InitializeMobileDevice();
			ret = true;
		} //Must callback.
		else if (notification == kAMNormalNotification)
		{
			ret=Register_iTunesMobileDevice_Callback(nTimeout);
		}
		else
		{
			ret = RestorableDevice_Register_CallBack();
        } 
	}		
	else{
		OOPSERR(GetLastError());
	}
	return ret;
}

void SetVerbose(BOOL b)
{
	extern PARAMETERS_T g_args;
	g_args.bVerbose = b;
	if (b)
	{
		g_iTunesMobileDevice.AMDSetLogLevel(INT_MAX);
		//g_iTunesMobileDevice.AMDAddLogFileDescriptor(_fileno(stderr));
		g_iTunesMobileDevice.AMRestoreSetLogLevel(INT_MAX);
		g_iTunesMobileDevice.AMRestoreEnableFileLogging("CON");
	} else {
		g_iTunesMobileDevice.AMDSetLogLevel(0);
		g_iTunesMobileDevice.AMRestoreSetLogLevel(0);
	}

}

char* iTunesErrorToStr(int errorcode)
{
	char *result = NULL;

	if (errorcode <= 0)
	{
		if (errorcode != 0)
		{
			switch (errorcode)
			{
			case 0xE8000001:
				result = "kAMDUndefinedError";
				break;
			case 0xE8000002:
				result = "kAMDBadHeaderError";
				break;
			case 0xE8000003:
				result = "kAMDNoResourcesError";
				break;
			case 0xE80000C0:
				result = "kAMDNoSpaceError";
				break;
			case 0xE8000004:
				result = "kAMDReadError";
				break;
			case 0xE8000005:
				result = "kAMDWriteError";
				break;
			case 0xE8000006:
				result = "kAMDUnknownPacketError";
				break;
			case 0xE8000007:
				result = "kAMDInvalidArgumentError";
				break;
			case 0xE8000008:
				result = "kAMDNotFoundError";
				break;
			case 0xE8000009:
				result = "kAMDIsDirectoryError";
				break;
			case 0xE800000A:
				result = "kAMDPermissionError";
				break;
			case 0xE800000B:
				result = "kAMDNotConnectedError";
				break;
			case 0xE800000C:
				result = "kAMDTimeOutError";
				break;
			case 0xE800000D:
				result = "kAMDOverrunError";
				break;
			case 0xE800000E:
				result = "kAMDEOFError";
				break;
			case 0xE800000F:
				result = "kAMDUnsupportedError";
				break;
			case 0xE8000010:
				result = "kAMDFileExistsError";
				break;
			case 0xE8000011:
				result = "kAMDBusyError";
				break;
			case 0xE8000012:
				result = "kAMDCryptoError";
				break;
			case 0xE8000085:
				result = "kAMDTooBigError";
				break;
			case 0xE8000013:
				result = "kAMDInvalidResponseError";
				break;
			case 0xE8000014:
				result = "kAMDMissingKeyError";
				break;
			case 0xE8000015:
				result = "kAMDMissingValueError";
				break;
			case 0xE8000016:
				result = "kAMDGetProhibitedError";
				break;
			case 0xE8000017:
				result = "kAMDSetProhibitedError";
				break;
			case 0xE8000018:
				result = "kAMDRemoveProhibitedError";
				break;
			case 0xE8000019:
				result = "kAMDImmutableValueError";
				break;
			case 0xE800001A:
				result = "kAMDPasswordProtectedError";
				break;
			case 0xE80000EE:
				result = "kAMDPasswordRequiredError";
				break;
			case 0xE8000095:
				result = "kAMDUserDeniedPairingError";
				break;
			case 0xE8000096:
				result = "kAMDPairingDialogResponsePendingError";
				break;
			case 0xE800001B:
				result = "kAMDMissingHostIDError";
				break;
			case 0xE800001C:
				result = "kAMDInvalidHostIDError";
				break;
			case 0xE800001D:
				result = "kAMDSessionActiveError";
				break;
			case 0xE800001E:
				result = "kAMDSessionInactiveError";
				break;
			case 0xE800001F:
				result = "kAMDMissingSessionIDError";
				break;
			case 0xE8000020:
				result = "kAMDInvalidSessionIDError";
				break;
			case 0xE8000021:
				result = "kAMDMissingServiceError";
				break;
			case 0xE8000022:
				result = "kAMDInvalidServiceError";
				break;
			case 0xE800005B:
				result = "kAMDServiceLimitError";
				break;
			case 0xE800005E:
				result = "kAMDCheckinSetupFailedError";
				break;
			case 0xE8000023:
				result = "kAMDInvalidCheckinError";
				break;
			case 0xE8000024:
				result = "kAMDCheckinTimeoutError";
				break;
			case 0xE800005F:
				result = "kAMDCheckinConnectionFailedError";
				break;
			case 0xE8000060:
				result = "kAMDCheckinReceiveFailedError";
				break;
			case 0xE8000061:
				result = "kAMDCheckinResponseFailedError";
				break;
			case 0xE8000069:
				result = "kAMDCheckinOutOfMemoryError";
				break;
			case 0xE8000062:
				result = "kAMDCheckinSendFailedError";
				break;
			case 0xE8000025:
				result = "kAMDMissingPairRecordError";
				break;
			case 0xE800005C:
				result = "kAMDInvalidPairRecordError";
				break;
			case 0xE8000068:
				result = "kAMDSavePairRecordFailedError";
				break;
			case 0xE8000026:
				result = "kAMDInvalidActivationRecordError";
				break;
			case 0xE8000027:
				result = "kAMDMissingActivationRecordError";
				break;
			case 0xE800005D:
				result = "kAMDServiceProhibitedError";
				break;
			case 0xE8000081:
				result = "kAMDEscrowLockedError";
				break;
			case 0xE8000082:
				result = "kAMDPairingProhibitedError";
				break;
			case 0xE8000083:
				result = "kAMDProhibitedBySupervision";
				break;
			case 0xE8000028:
				result = "kAMDWrongDroidError";
				break;
			case 0xE8000029:
				result = "kAMDSUVerificationError";
				break;
			case 0xE800002A:
				result = "kAMDSUPatchError";
				break;
			case 0xE800002B:
				result = "kAMDSUFirmwareError";
				break;
			case 0xE800002C:
				result = "kAMDProvisioningProfileNotValid";
				break;
			case 0xE800002D:
				result = "kAMDSendMessageError";
				break;
			case 0xE800002E:
				result = "kAMDReceiveMessageError";
				break;
			case 0xE800002F:
				result = "kAMDMissingOptionsError";
				break;
			case 0xE8000030:
				result = "kAMDMissingImageTypeError";
				break;
			case 0xE8000031:
				result = "kAMDDigestFailedError";
				break;
			case 0xE8000032:
				result = "kAMDStartServiceError";
				break;
			case 0xE8000033:
				result = "kAMDInvalidDiskImageError";
				break;
			case 0xE8000034:
				result = "kAMDMissingDigestError";
				break;
			case 0xE8000035:
				result = "kAMDMuxError";
				break;
			case 0xE8000036:
				result = "kAMDApplicationAlreadyInstalledError";
				break;
			case 0xE8000037:
				result = "kAMDApplicationMoveFailedError";
				break;
			case 0xE8000038:
				result = "kAMDApplicationSINFCaptureFailedError";
				break;
			case 0xE8000039:
				result = "kAMDApplicationSandboxFailedError";
				break;
			case 0xE800003A:
				result = "kAMDApplicationVerificationFailedError";
				break;
			case 0xE800003B:
				result = "kAMDArchiveDestructionFailedError";
				break;
			case 0xE800003C:
				result = "kAMDBundleVerificationFailedError";
				break;
			case 0xE800003D:
				result = "kAMDCarrierBundleCopyFailedError";
				break;
			case 0xE800003E:
				result = "kAMDCarrierBundleDirectoryCreationFailedError";
				break;
			case 0xE800003F:
				result = "kAMDCarrierBundleMissingSupportedSIMsError";
				break;
			case 0xE8000040:
				result = "kAMDCommCenterNotificationFailedError";
				break;
			case 0xE8000041:
				result = "kAMDContainerCreationFailedError";
				break;
			case 0xE8000042:
				result = "kAMDContainerP0wnFailedError";
				break;
			case 0xE8000043:
				result = "kAMDContainerRemovalFailedError";
				break;
			case 0xE8000044:
				result = "kAMDEmbeddedProfileInstallFailedError";
				break;
			case 0xE8000045:
				result = "kAMDErrorError";
				break;
			case 0xE8000046:
				result = "kAMDExecutableTwiddleFailedError";
				break;
			case 0xE8000047:
				result = "kAMDExistenceCheckFailedError";
				break;
			case 0xE8000048:
				result = "kAMDInstallMapUpdateFailedError";
				break;
			case 0xE8000049:
				result = "kAMDManifestCaptureFailedError";
				break;
			case 0xE800004A:
				result = "kAMDMapGenerationFailedError";
				break;
			case 0xE800004B:
				result = "kAMDMissingBundleExecutableError";
				break;
			case 0xE800004C:
				result = "kAMDMissingBundleIdentifierError";
				break;
			case 0xE800004D:
				result = "kAMDMissingBundlePathError";
				break;
			case 0xE800004E:
				result = "kAMDMissingContainerError";
				break;
			case 0xE800004F:
				result = "kAMDNotificationFailedError";
				break;
			case 0xE8000050:
				result = "kAMDPackageExtractionFailedError";
				break;
			case 0xE8000051:
				result = "kAMDPackageInspectionFailedError";
				break;
			case 0xE8000052:
				result = "kAMDPackageMoveFailedError";
				break;
			case 0xE8000053:
				result = "kAMDPathConversionFailedError";
				break;
			case 0xE8000054:
				result = "kAMDRestoreContainerFailedError";
				break;
			case 0xE8000055:
				result = "kAMDSeatbeltProfileRemovalFailedError";
				break;
			case 0xE8000056:
				result = "kAMDStageCreationFailedError";
				break;
			case 0xE8000057:
				result = "kAMDSymlinkFailedError";
				break;
			case 0xE8000058:
				result = "kAMDiTunesArtworkCaptureFailedError";
				break;
			case 0xE8000059:
				result = "kAMDiTunesMetadataCaptureFailedError";
				break;
			case 0xE800005A:
				result = "kAMDAlreadyArchivedError";
				break;
			case 0xE8000066:
				result = "kAMDUnknownCommandError";
				break;
			case 0xE8000067:
				result = "kAMDAPIInternalError";
				break;
			case 0xE8000063:
				result = "kAMDMuxCreateListenerError";
				break;
			case 0xE8000064:
				result = "kAMDMuxGetListenerError";
				break;
			case 0xE8000065:
				result = "kAMDMuxConnectError";
				break;
			case 0xE800006A:
				result = "kAMDDeviceTooNewError";
				break;
			case 0xE800006B:
				result = "kAMDDeviceRefNoGood";
				break;
			case 0xE8000084:
				result = "kAMDDeviceDisconnectedError";
				break;
			case 0xE800006C:
				result = "kAMDCannotTranslateError";
				break;
			case 0xE800006D:
				result = "kAMDMobileImageMounterMissingImageSignature";
				break;
			case 0xE800006E:
				result = "kAMDMobileImageMounterResponseCreationFailed";
				break;
			case 0xE800006F:
				result = "kAMDMobileImageMounterMissingImageType";
				break;
			case 0xE8000070:
				result = "kAMDMobileImageMounterMissingImagePath";
				break;
			case 0xE8000071:
				result = "kAMDMobileImageMounterImageMapLoadFailed";
				break;
			case 0xE8000072:
				result = "kAMDMobileImageMounterAlreadyMounted";
				break;
			case 0xE8000073:
				result = "kAMDMobileImageMounterImageMoveFailed";
				break;
			case 0xE8000074:
				result = "kAMDMobileImageMounterMountPathMissing";
				break;
			case 0xE8000075:
				result = "kAMDMobileImageMounterMountPathNotEmpty";
				break;
			case 0xE8000076:
				result = "kAMDMobileImageMounterImageMountFailed";
				break;
			case 0xE8000077:
				result = "kAMDMobileImageMounterTrustCacheLoadFailed";
				break;
			case 0xE8000078:
				result = "kAMDMobileImageMounterDigestFailed";
				break;
			case 0xE8000079:
				result = "kAMDMobileImageMounterDigestCreationFailed";
				break;
			case 0xE800007A:
				result = "kAMDMobileImageMounterImageVerificationFailed";
				break;
			case 0xE800007B:
				result = "kAMDMobileImageMounterImageInfoCreationFailed";
				break;
			case 0xE800007C:
				result = "kAMDMobileImageMounterImageMapStoreFailed";
				break;
			case 0xE80000E2:
				result = "kAMDMobileImageMounterDeviceLocked";
				break;
			case 0xE800007D:
				result = "kAMDBonjourSetupError";
				break;
			case 0xE800007F:
				result = "kAMDNoWifiSyncSupportError";
				break;
			case 0xE800007E:
				result = "kAMDDeviceOSVersionTooLow";
				break;
			case 0xE8000080:
				result = "kAMDDeviceFamilyNotSupported";
				break;
			case 0xE8000086:
				result = "kAMDPackagePatchFailedError";
				break;
			case 0xE8000087:
				result = "kAMDIncorrectArchitectureError";
				break;
			case 0xE8000088:
				result = "kAMDPluginCopyFailedError";
				break;
			case 0xE8000089:
				result = "kAMDBreadcrumbFailedError";
				break;
			case 0xE800008A:
				result = "kAMDBreadcrumbUnlockError";
				break;
			case 0xE800008B:
				result = "kAMDGeoJSONCaptureFailedError";
				break;
			case 0xE800008C:
				result = "kAMDNewsstandArtworkCaptureFailedError";
				break;
			case 0xE800008D:
				result = "kAMDMissingCommandError";
				break;
			case 0xE800008E:
				result = "kAMDNotEntitledError";
				break;
			case 0xE800008F:
				result = "kAMDMissingPackagePathError";
				break;
			case 0xE8000090:
				result = "kAMDMissingContainerPathError";
				break;
			case 0xE8000091:
				result = "kAMDMissingApplicationIdentifierError";
				break;
			case 0xE8000092:
				result = "kAMDMissingAttributeValueError";
				break;
			case 0xE8000093:
				result = "kAMDLookupFailedError";
				break;
			case 0xE8000094:
				result = "kAMDDictCreationFailedError";
				break;
			case 0xE8000097:
				result = "kAMDInstallProhibitedError";
				break;
			case 0xE8000098:
				result = "kAMDUninstallProhibitedError";
				break;
			case 0xE800009C:
				result = "kAMDMissingBundleVersionError";
				break;
			case 0xE80000BF:
				result = "kAMDInvalidSymlinkError";
				break;
			case 0xE8000099:
				result = "kAMDFMiPProtectedError";
				break;
			case 0xE800009A:
				result = "kAMDMCProtected";
				break;
			case 0xE800009B:
				result = "kAMDMCChallengeRequired";
				break;
			case 0xE800009D:
				result = "kAMDAppBlacklistedError";
				break;
			case 0xE80000B3:
				result = "kMobileHouseArrestMissingCommand";
				break;
			case 0xE80000B4:
				result = "kMobileHouseArrestUnknownCommand";
				break;
			case 0xE80000B5:
				result = "kMobileHouseArrestMissingIdentifier";
				break;
			case 0xE80000B6:
				result = "kMobileHouseArrestDictionaryFailed";
				break;
			case 0xE80000B7:
				result = "kMobileHouseArrestInstallationLookupFailed";
				break;
			case 0xE80000B8:
				result = "kMobileHouseArrestApplicationLookupFailed";
				break;
			case 0xE80000B9:
				result = "kMobileHouseArrestMissingContainer";
				break;
			case 0xE80000BB:
				result = "kMobileHouseArrestPathConversionFailed";
				break;
			case 0xE80000BC:
				result = "kMobileHouseArrestPathMissing";
				break;
			case 0xE80000BD:
				result = "kMobileHouseArrestInvalidPath";
				break;
			case 0xE80000BE:
				result = "kAMDMismatchedApplicationIdentifierEntitlementError";
				break;
			case 0xE80000C3:
				result = "kAMDBundleiTunesMetadataVersionMismatchError";
				break;
			case 0xE80000C4:
				result = "kAMDInvalidiTunesMetadataPlistError";
				break;
			case 0xE80000C5:
				result = "kAMDMismatchedBundleIDSigningIdentifierError";
				break;
			case 0xE80000C8:
				result = "kAMDDeviceNotSupportedByThinningError";
				break;
			case 0xE80000CB:
				result = "kAMDAppexBundleIDConflictWithOtherIdentifierError";
				break;
			case 0xE80000CC:
				result = "kAMDBundleIDConflictWithOtherIdentifierError";
				break;
			case 0xE80000E3:
				result = "kAMDInvalidSINFError";
				break;
			case 0xE80000ED:
				result = "kAMDRequireCUPairingError";
				break;
			case 0xE80000E8:
				result = "kAMDRequireCUPairingCodeError";
				break;
			case 0xE80000E9:
				result = "kAMDRequireCUPairingBackoffError";
				break;
			case 0xE80000EA:
				result = "kAMDCUPairingError";
				break;
			case 0xE80000EB:
				result = "kAMDCUPairingContinueError";
				break;
			case 0xE80000EC:
				result = "kAMDCUPairingResetError";
				break;
			case 0xE800009E:
				result = "This app contains an app extension with an illegal bundle identifier. App extension bundle identifiers must have a prefix consisting of their containing application's bundle identifier followed by a '.'.";
				break;
			case 0xE800009F:
				result = "If an app extension defines the XPCService key in its Info.plist, it must have a dictionary value.";
				break;
			case 0xE80000A0:
				result = "App extensions must define the NSExtension key with a dictionary value in their Info.plist.";
				break;
			case 0xE80000A1:
				result = "If an app extension defines the CFBundlePackageType key in its Info.plist, it must have the value \"XPC!\".";
				break;
			case 0xE80000A2:
				result = "App extensions must define either NSExtensionMainStoryboard or NSExtensionPrincipalClass keys in the NSExtension dictionary in their Info.plist.";
				break;
			case 0xE80000A3:
				result = "If an app extension defines the NSExtensionContextClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
				break;
			case 0xE80000A4:
				result = "If an app extension defines the NSExtensionContextHostClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
				break;
			case 0xE80000A5:
				result = "If an app extension defines the NSExtensionViewControllerHostClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
				break;
			case 0xE80000A6:
				result = "This app contains an app extension that does not define the NSExtensionPointIdentifier key in its Info.plist. This key must have a reverse-DNS format string value.";
				break;
			case 0xE80000A7:
				result = "This app contains an app extension that does not define the NSExtensionPointIdentifier key in its Info.plist with a valid reverse-DNS format string value.";
				break;
			case 0xE80000A8:
				result = "If an app extension defines the NSExtensionAttributes key in the NSExtension dictionary in its Info.plist, it must have a dictionary value.";
				break;
			case 0xE80000A9:
				result = "If an app extension defines the NSExtensionPointName key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
				break;
			case 0xE80000AA:
				result = "If an app extension defines the NSExtensionPointVersion key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
				break;
			case 0xE80000AB:
				result = "This app or a bundle it contains does not define the CFBundleName key in its Info.plist with a string value containing one or more characters.";
				break;
			case 0xE80000AC:
				result = "This app or a bundle it contains does not define the CFBundleDisplayName key in its Info.plist with a string value containing one or more characters.";
				break;
			case 0xE80000AD:
				result = "This app or a bundle it contains defines the CFBundleShortVersionStringKey key in its Info.plist with a non-string value or a zero-length string value.";
				break;
			case 0xE80000AE:
				result = "This app or a bundle it contains defines the RunLoopType key in the XPCService dictionary in its Info.plist with a non-string value or a zero-length string value.";
				break;
			case 0xE80000AF:
				result = "This app or a bundle it contains defines the ServiceType key in the XPCService dictionary in its Info.plist with a non-string value or a zero-length string value.";
				break;
			case 0xE80000B0:
				result = "This application or a bundle it contains has the same bundle identifier as this application or another bundle that it contains. Bundle identifiers must be unique.";
				break;
			case 0xE80000B1:
				result = "This app contains an app extension that specifies an extension point identifier that is not supported on this version of iOS for the value of the NSExtensionPointIdentifier key in its Info.plist.";
				break;
			case 0xE80000B2:
				result = "This app contains multiple app extensions that are file providers. Apps are only allowed to contain at most a single file provider app extension.";
				break;
			case 0xE80000C2:
				result = "This app is not a valid AppleTV Stub App";
				break;
			case 0xE80000DB:
				result = "The device is out of storage for apps. Please remove some apps from the device and try again.";
				break;
			case 0xE80000C6:
				result = "This app contains multiple WatchKit app extensions. Only a single WatchKit extension is allowed.";
				break;
			case 0xE80000C7:
				result = "A WatchKit app within this app is not a valid bundle.";
				break;
			case 0xE80000C9:
				result = "The UISupportedDevices key in this app's Info.plist does not specify a valid set of supported devices.";
				break;
			case 0xE80000CA:
				result = "This app contains an app extension with an illegal bundle identifier. App extension bundle identifiers must have a prefix consisting of their containing application's bundle identifier followed by a '.', with no further '.' characters after the prefix.";
				break;
			case 0xE80000CD:
				result = "This app contains multiple WatchKit 1.0 apps. Only a single WatchKit 1.0 app is allowed.";
				break;
			case 0xE80000CE:
				result = "This app contains multiple WatchKit 2.0 apps. Only a single WatchKit 2.0 app is allowed.";
				break;
			case 0xE80000CF:
				result = "The WatchKit app has an invalid stub executable.";
				break;
			case 0xE80000D0:
				result = "The WatchKit app has multiple app extensions. Only a single WatchKit extension is allowed in a WatchKit app, and only if this is a WatchKit 2.0 app.";
				break;
			case 0xE80000D1:
				result = "The WatchKit 2.0 app contains non-WatchKit app extensions. Only WatchKit app extensions are allowed in WatchKit apps.";
				break;
			case 0xE80000D2:
				result = "The WatchKit app has one or more embedded frameworks. Frameworks are only allowed in WatchKit app extensions in WatchKit 2.0 apps.";
				break;
			case 0xE80000D3:
				result = "This app contains a WatchKit 1.0 app with app extensions. This is not allowed.";
				break;
			case 0xE80000D4:
				result = "This app contains a WatchKit 2.0 app without an app extension. WatchKit 2.0 apps must contain a WatchKit app extension.";
				break;
			case 0xE80000D5:
				result = "The WatchKit app's Info.plist must have a WKCompanionAppBundleIdentifier key set to the bundle identifier of the companion app.";
				break;
			case 0xE80000D6:
				result = "The WatchKit app's Info.plist contains a non-string key.";
				break;
			case 0xE80000D7:
				result = "The WatchKit app's Info.plist contains a key that is not in the whitelist of allowed keys for a WatchKit app.";
				break;
			case 0xE80000D8:
				result = "The WatchKit 1.0 and a WatchKit 2.0 apps within this app must have have the same bundle identifier.";
				break;
			case 0xE80000D9:
				result = "This app contains a WatchKit app with an invalid bundle identifier. The bundle identifier of a WatchKit app must have a prefix consisting of the companion app's bundle identifier, followed by a '.'.";
				break;
			case 0xE80000DA:
				result = "This app contains a WatchKit app where the UIDeviceFamily key in its Info.plist does not specify the value 4 to indicate that it's compatible with the Apple Watch device type.";
				break;
			case 0xE80000C1:
				result = "The WatchKit app extension must have, in its Info.plist's NSExtension dictionary's NSExtensionAttributes dictionary, the key WKAppBundleIdentifier with a value equal to the associated WatchKit app's bundle identifier.";
				break;
			case 0xE80000DC:
				result = "This app or an app that it contains has a Siri Intents app extension that is missing the IntentsSupported array in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist.";
				break;
			case 0xE80000DD:
				result = "This app or an app that it contains has a Siri Intents app extension that does not correctly define the IntentsRestrictedWhileLocked key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist. The key's value must be an array of strings.";
				break;
			case 0xE80000DE:
				result = "This app or an app that it contains has a Siri Intents app extension that declares values in its IntentsRestrictedWhileLocked key's array value that are not in its IntentsSupported key's array value (in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist).";
				break;
			case 0xE80000DF:
				result = "This app or an app that it contains declares multiple Siri Intents app extensions that declare one or more of the same values in the IntentsSupported array in the NSExtensionAttributes dictionary in the NSExtension dictionary in their Info.plist. IntentsSupported must be distinct among a given Siri Intents extension type within an app.";
				break;
			case 0xE80000E7:
				result = "This app contains a WatchKit app with one or more Siri Intents app extensions that declare IntentsSupported that are not declared in any of the companion app's Siri Intents app extensions. WatchKit Siri Intents extensions' IntentsSupported values must be a subset of the companion app's Siri Intents extensions' IntentsSupported values.";
				break;
			case 0xE80000E0:
				result = "The WatchKit 2.0 app, which expects to be compatible with watchOS versions earlier than 3.0, contains a non-WatchKit extension in a location that's not compatible with watchOS versions earlier than 3.0.";
				break;
			case 0xE80000E1:
				result = "The WatchKit 2.0 app, which expects to be compatible with watchOS versions earlier than 3.0, contains a framework in a location that's not compatible with watchOS versions earlier than 3.0.";
				break;
			case 0xE80000E4:
				result = "Multiple iMessage app extensions were found in this app. Only one is allowed.";
				break;
			case 0xE80000E5:
				result = "This iMessage application is missing its required iMessage app extension.";
				break;
			case 0xE80000E6:
				result = "This iMessage application contains an app extension type other than an iMessage app extension. iMessage applications may only contain one iMessage app extension and may not contain other types of app extensions.";
				break;
			case 0xE80000EF:
				result = "This app's Info.plist has too many items in the array for the key INAlternativeAppNames. A maximum of three is allowed.";
				break;
			case 0xE80000F0:
				result = "The version of WatchKit used by this app is no longer supported on this version of watchOS.";
				break;
			default:
				break;
			}
		}
		else
		{
			result = "kAMDSuccess";
		}
	}
	return result;
}

std::string GetAMDErrorText(int errorCode)
{
	std::string error;
	CFStringRef errorPtr = (CFStringRef)g_iTunesMobileDevice.AMDCopyErrorText(errorCode);
	if (errorPtr != NULL)
	{
		if (CFGetTypeID(errorPtr) == CFStringGetTypeID())
		{
			CFIndex length = CFStringGetLength(errorPtr);
			CFIndex maxSize =
				CFStringGetMaximumSizeForEncoding(length,
				kCFStringEncodingUTF8);
			char *buffer = (char *)malloc(maxSize);
			if (NULL != buffer)
			{
				if (CFStringGetCString(errorPtr, buffer, maxSize, kCFStringEncodingUTF8))
				{
					error.append(buffer);
				}
				SAFE_DELETE(buffer);
			}

		}
		CF_RELEASE_CLEAR(errorPtr);
	}
	if (!error.empty())
	{
		return error;
	}

	char *result = iTunesErrorToStr(errorCode);
	error.append(result);
	return error;


	switch (errorCode)
	{
	case -402653183:
		result = "kAMDUndefinedError";
		break;
	case -402653182:
		result = "kAMDBadHeaderError";
		break;
	case -402653181:
		result = "kAMDNoResourcesError";
		break;
	case -402652992:
		result = "kAMDNoSpaceError";
		break;
	case -402653180:
		result = "kAMDReadError";
		break;
	case -402653179:
		result = "kAMDWriteError";
		break;
	case -402653178:
		result = "kAMDUnknownPacketError";
		break;
	case -402653177:
		result = "kAMDInvalidArgumentError";
		break;
	case -402653176:
		result = "kAMDNotFoundError";
		break;
	case -402653175:
		result = "kAMDIsDirectoryError";
		break;
	case -402653174:
		result = "kAMDPermissionError";
		break;
	case -402653173:
		result = "kAMDNotConnectedError";
		break;
	case -402653172:
		result = "kAMDTimeOutError";
		break;
	case -402653171:
		result = "kAMDOverrunError";
		break;
	case -402653170:
		result = "kAMDEOFError";
		break;
	case -402653169:
		result = "kAMDUnsupportedError";
		break;
	case -402653168:
		result = "kAMDFileExistsError";
		break;
	case -402653167:
		result = "kAMDBusyError";
		break;
	case -402653166:
		result = "kAMDCryptoError";
		break;
	case -402653051:
		result = "kAMDTooBigError";
		break;
	case -402653165:
		result = "kAMDInvalidResponseError";
		break;
	case -402653164:
		result = "kAMDMissingKeyError";
		break;
	case -402653163:
		result = "kAMDMissingValueError";
		break;
	case -402653162:
		result = "kAMDGetProhibitedError";
		break;
	case -402653161:
		result = "kAMDSetProhibitedError";
		break;
	case -402653160:
		result = "kAMDRemoveProhibitedError";
		break;
	case -402653159:
		result = "kAMDImmutableValueError";
		break;
	case -402653158:
		result = "kAMDPasswordProtectedError";
		break;
	case -402653035:
		result = "kAMDUserDeniedPairingError";
		break;
	case -402653034:
		result = "kAMDPairingDialogResponsePendingError";
		break;
	case -402653157:
		result = "kAMDMissingHostIDError";
		break;
	case -402653156:
		result = "kAMDInvalidHostIDError";
		break;
	case -402653155:
		result = "kAMDSessionActiveError";
		break;
	case -402653154:
		result = "kAMDSessionInactiveError";
		break;
	case -402653153:
		result = "kAMDMissingSessionIDError";
		break;
	case -402653152:
		result = "kAMDInvalidSessionIDError";
		break;
	case -402653151:
		result = "kAMDMissingServiceError";
		break;
	case -402653150:
		result = "kAMDInvalidServiceError";
		break;
	case -402653093:
		result = "kAMDServiceLimitError";
		break;
	case -402653090:
		result = "kAMDCheckinSetupFailedError";
		break;
	case -402653149:
		result = "kAMDInvalidCheckinError";
		break;
	case -402653148:
		result = "kAMDCheckinTimeoutError";
		break;
	case -402653089:
		result = "kAMDCheckinConnectionFailedError";
		break;
	case -402653088:
		result = "kAMDCheckinReceiveFailedError";
		break;
	case -402653087:
		result = "kAMDCheckinResponseFailedError";
		break;
	case -402653079:
		result = "kAMDCheckinOutOfMemoryError";
		break;
	case -402653086:
		result = "kAMDCheckinSendFailedError";
		break;
	case -402653147:
		result = "kAMDMissingPairRecordError";
		break;
	case -402653092:
		result = "kAMDInvalidPairRecordError";
		break;
	case -402653080:
		result = "kAMDSavePairRecordFailedError";
		break;
	case -402653146:
		result = "kAMDInvalidActivationRecordError";
		break;
	case -402653145:
		result = "kAMDMissingActivationRecordError";
		break;
	case -402653091:
		result = "kAMDServiceProhibitedError";
		break;
	case -402653055:
		result = "kAMDEscrowLockedError";
		break;
	case -402653054:
		result = "kAMDPairingProhibitedError";
		break;
	case -402653053:
		result = "kAMDProhibitedBySupervision";
		break;
	case -402653144:
		result = "kAMDWrongDroidError";
		break;
	case -402653143:
		result = "kAMDSUVerificationError";
		break;
	case -402653142:
		result = "kAMDSUPatchError";
		break;
	case -402653141:
		result = "kAMDSUFirmwareError";
		break;
	case -402653140:
		result = "kAMDProvisioningProfileNotValid";
		break;
	case -402653139:
		result = "kAMDSendMessageError";
		break;
	case -402653138:
		result = "kAMDReceiveMessageError";
		break;
	case -402653137:
		result = "kAMDMissingOptionsError";
		break;
	case -402653136:
		result = "kAMDMissingImageTypeError";
		break;
	case -402653135:
		result = "kAMDDigestFailedError";
		break;
	case -402653134:
		result = "kAMDStartServiceError";
		break;
	case -402653133:
		result = "kAMDInvalidDiskImageError";
		break;
	case -402653132:
		result = "kAMDMissingDigestError";
		break;
	case -402653131:
		result = "kAMDMuxError";
		break;
	case -402653130:
		result = "kAMDApplicationAlreadyInstalledError";
		break;
	case -402653129:
		result = "kAMDApplicationMoveFailedError";
		break;
	case -402653128:
		result = "kAMDApplicationSINFCaptureFailedError";
		break;
	case -402653127:
		result = "kAMDApplicationSandboxFailedError";
		break;
	case -402653126:
		result = "kAMDApplicationVerificationFailedError";
		break;
	case -402653125:
		result = "kAMDArchiveDestructionFailedError";
		break;
	case -402653124:
		result = "kAMDBundleVerificationFailedError";
		break;
	case -402653123:
		result = "kAMDCarrierBundleCopyFailedError";
		break;
	case -402653122:
		result = "kAMDCarrierBundleDirectoryCreationFailedError";
		break;
	case -402653121:
		result = "kAMDCarrierBundleMissingSupportedSIMsError";
		break;
	case -402653120:
		result = "kAMDCommCenterNotificationFailedError";
		break;
	case -402653119:
		result = "kAMDContainerCreationFailedError";
		break;
	case -402653118:
		result = "kAMDContainerP0wnFailedError";
		break;
	case -402653117:
		result = "kAMDContainerRemovalFailedError";
		break;
	case -402653116:
		result = "kAMDEmbeddedProfileInstallFailedError";
		break;
	case -402653115:
		result = "kAMDErrorError";
		break;
	case -402653114:
		result = "kAMDExecutableTwiddleFailedError";
		break;
	case -402653113:
		result = "kAMDExistenceCheckFailedError";
		break;
	case -402653112:
		result = "kAMDInstallMapUpdateFailedError";
		break;
	case -402653111:
		result = "kAMDManifestCaptureFailedError";
		break;
	case -402653110:
		result = "kAMDMapGenerationFailedError";
		break;
	case -402653109:
		result = "kAMDMissingBundleExecutableError";
		break;
	case -402653108:
		result = "kAMDMissingBundleIdentifierError";
		break;
	case -402653107:
		result = "kAMDMissingBundlePathError";
		break;
	case -402653106:
		result = "kAMDMissingContainerError";
		break;
	case -402653105:
		result = "kAMDNotificationFailedError";
		break;
	case -402653104:
		result = "kAMDPackageExtractionFailedError";
		break;
	case -402653103:
		result = "kAMDPackageInspectionFailedError";
		break;
	case -402653102:
		result = "kAMDPackageMoveFailedError";
		break;
	case -402653101:
		result = "kAMDPathConversionFailedError";
		break;
	case -402653100:
		result = "kAMDRestoreContainerFailedError";
		break;
	case -402653099:
		result = "kAMDSeatbeltProfileRemovalFailedError";
		break;
	case -402653098:
		result = "kAMDStageCreationFailedError";
		break;
	case -402653097:
		result = "kAMDSymlinkFailedError";
		break;
	case -402653096:
		result = "kAMDiTunesArtworkCaptureFailedError";
		break;
	case -402653095:
		result = "kAMDiTunesMetadataCaptureFailedError";
		break;
	case -402653094:
		result = "kAMDAlreadyArchivedError";
		break;
	case -402653082:
		result = "kAMDUnknownCommandError";
		break;
	case -402653081:
		result = "kAMDAPIInternalError";
		break;
	case -402653085:
		result = "kAMDMuxCreateListenerError";
		break;
	case -402653084:
		result = "kAMDMuxGetListenerError";
		break;
	case -402653083:
		result = "kAMDMuxConnectError";
		break;
	case -402653078:
		result = "kAMDDeviceTooNewError";
		break;
	case -402653077:
		result = "kAMDDeviceRefNoGood";
		break;
	case -402653052:
		result = "kAMDDeviceDisconnectedError";
		break;
	case -402653076:
		result = "kAMDCannotTranslateError";
		break;
	case -402653075:
		result = "kAMDMobileImageMounterMissingImageSignature";
		break;
	case -402653074:
		result = "kAMDMobileImageMounterResponseCreationFailed";
		break;
	case -402653073:
		result = "kAMDMobileImageMounterMissingImageType";
		break;
	case -402653072:
		result = "kAMDMobileImageMounterMissingImagePath";
		break;
	case -402653071:
		result = "kAMDMobileImageMounterImageMapLoadFailed";
		break;
	case -402653070:
		result = "kAMDMobileImageMounterAlreadyMounted";
		break;
	case -402653069:
		result = "kAMDMobileImageMounterImageMoveFailed";
		break;
	case -402653068:
		result = "kAMDMobileImageMounterMountPathMissing";
		break;
	case -402653067:
		result = "kAMDMobileImageMounterMountPathNotEmpty";
		break;
	case -402653066:
		result = "kAMDMobileImageMounterImageMountFailed";
		break;
	case -402653065:
		result = "kAMDMobileImageMounterTrustCacheLoadFailed";
		break;
	case -402653064:
		result = "kAMDMobileImageMounterDigestFailed";
		break;
	case -402653063:
		result = "kAMDMobileImageMounterDigestCreationFailed";
		break;
	case -402653062:
		result = "kAMDMobileImageMounterImageVerificationFailed";
		break;
	case -402653061:
		result = "kAMDMobileImageMounterImageInfoCreationFailed";
		break;
	case -402653060:
		result = "kAMDMobileImageMounterImageMapStoreFailed";
		break;
	case -402652958:
		result = "kAMDMobileImageMounterDeviceLocked";
		break;
	case -402653059:
		result = "kAMDBonjourSetupError";
		break;
	case -402653057:
		result = "kAMDNoWifiSyncSupportError";
		break;
	case -402653058:
		result = "kAMDDeviceOSVersionTooLow";
		break;
	case -402653056:
		result = "kAMDDeviceFamilyNotSupported";
		break;
	case -402653050:
		result = "kAMDPackagePatchFailedError";
		break;
	case -402653049:
		result = "kAMDIncorrectArchitectureError";
		break;
	case -402653048:
		result = "kAMDPluginCopyFailedError";
		break;
	case -402653047:
		result = "kAMDBreadcrumbFailedError";
		break;
	case -402653046:
		result = "kAMDBreadcrumbUnlockError";
		break;
	case -402653045:
		result = "kAMDGeoJSONCaptureFailedError";
		break;
	case -402653044:
		result = "kAMDNewsstandArtworkCaptureFailedError";
		break;
	case -402653043:
		result = "kAMDMissingCommandError";
		break;
	case -402653042:
		result = "kAMDNotEntitledError";
		break;
	case -402653041:
		result = "kAMDMissingPackagePathError";
		break;
	case -402653040:
		result = "kAMDMissingContainerPathError";
		break;
	case -402653039:
		result = "kAMDMissingApplicationIdentifierError";
		break;
	case -402653038:
		result = "kAMDMissingAttributeValueError";
		break;
	case -402653037:
		result = "kAMDLookupFailedError";
		break;
	case -402653036:
		result = "kAMDDictCreationFailedError";
		break;
	case -402653033:
		result = "kAMDInstallProhibitedError";
		break;
	case -402653032:
		result = "kAMDUninstallProhibitedError";
		break;
	case -402653028:
		result = "kAMDMissingBundleVersionError";
		break;
	case -402652993:
		result = "kAMDInvalidSymlinkError";
		break;
	case -402653031:
		result = "kAMDFMiPProtectedError";
		break;
	case -402653030:
		result = "kAMDMCProtected";
		break;
	case -402653029:
		result = "kAMDMCChallengeRequired";
		break;
	case -402653027:
		result = "kAMDAppBlacklistedError";
		break;
	case -402653005:
		result = "kMobileHouseArrestMissingCommand";
		break;
	case -402653004:
		result = "kMobileHouseArrestUnknownCommand";
		break;
	case -402653003:
		result = "kMobileHouseArrestMissingIdentifier";
		break;
	case -402653002:
		result = "kMobileHouseArrestDictionaryFailed";
		break;
	case -402653001:
		result = "kMobileHouseArrestInstallationLookupFailed";
		break;
	case -402653000:
		result = "kMobileHouseArrestApplicationLookupFailed";
		break;
	case -402652999:
		result = "kMobileHouseArrestMissingContainer";
		break;
	case -402652997:
		result = "kMobileHouseArrestPathConversionFailed";
		break;
	case -402652996:
		result = "kMobileHouseArrestPathMissing";
		break;
	case -402652995:
		result = "kMobileHouseArrestInvalidPath";
		break;
	case -402652994:
		result = "kAMDMismatchedApplicationIdentifierEntitlementError";
		break;
	case -402652989:
		result = "kAMDBundleiTunesMetadataVersionMismatchError";
		break;
	case -402652988:
		result = "kAMDInvalidiTunesMetadataPlistError";
		break;
	case -402652987:
		result = "kAMDMismatchedBundleIDSigningIdentifierError";
		break;
	case -402652984:
		result = "kAMDDeviceNotSupportedByThinningError";
		break;
	case -402652981:
		result = "kAMDAppexBundleIDConflictWithOtherIdentifierError";
		break;
	case -402652980:
		result = "kAMDBundleIDConflictWithOtherIdentifierError";
		break;
	case -402652957:
		result = "kAMDInvalidSINFError";
		break;
	case -402653026:
		result = "This app contains an app extension with an illegal bundle identifier. App extension bundle identifiers must have a prefix consisting of their containing application's bundle identifier followed by a '.'.";
		break;
	case -402653025:
		result = "If an app extension defines the XPCService key in its Info.plist, it must have a dictionary value.";
		break;
	case -402653024:
		result = "App extensions must define the NSExtension key with a dictionary value in their Info.plist.";
		break;
	case -402653023:
		result = "If an app extension defines the CFBundlePackageType key in its Info.plist, it must have the value \"XPC!\".";
		break;
	case -402653022:
		result = "App extensions must define either NSExtensionMainStoryboard or NSExtensionPrincipalClass keys in the NSExtension dictionary in their Info.plist.";
		break;
	case -402653021:
		result = "If an app extension defines the NSExtensionContextClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
		break;
	case -402653020:
		result = "If an app extension defines the NSExtensionContextHostClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
		break;
	case -402653019:
		result = "If an app extension defines the NSExtensionViewControllerHostClass key in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
		break;
	case -402653018:
		result = "This app contains an app extension that does not define the NSExtensionPointIdentifier key in its Info.plist. This key must have a reverse-DNS format string value.";
		break;
	case -402653017:
		result = "This app contains an app extension that does not define the NSExtensionPointIdentifier key in its Info.plist with a valid reverse-DNS format string value.";
		break;
	case -402653016:
		result = "If an app extension defines the NSExtensionAttributes key in the NSExtension dictionary in its Info.plist, it must have a dictionary value.";
		break;
	case -402653015:
		result = "If an app extension defines the NSExtensionPointName key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
		break;
	case -402653014:
		result = "If an app extension defines the NSExtensionPointVersion key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist, it must have a string value containing one or more characters.";
		break;
	case -402653013:
		result = "This app or a bundle it contains does not define the CFBundleName key in its Info.plist with a string value containing one or more characters.";
		break;
	case -402653012:
		result = "This app or a bundle it contains does not define the CFBundleDisplayName key in its Info.plist with a string value containing one or more characters.";
		break;
	case -402653011:
		result = "This app or a bundle it contains defines the CFBundleShortVersionStringKey key in its Info.plist with a non-string value or a zero-length string value.";
		break;
	case -402653010:
		result = "This app or a bundle it contains defines the RunLoopType key in the XPCService dictionary in its Info.plist with a non-string value or a zero-length string value.";
		break;
	case -402653009:
		result = "This app or a bundle it contains defines the ServiceType key in the XPCService dictionary in its Info.plist with a non-string value or a zero-length string value.";
		break;
	case -402653008:
		result = "This application or a bundle it contains has the same bundle identifier as this application or another bundle that it contains. Bundle identifiers must be unique.";
		break;
	case -402653007:
		result = "This app contains an app extension that specifies an extension point identifier that is not supported on this version of iOS for the value of the NSExtensionPointIdentifier key in its Info.plist.";
		break;
	case -402653006:
		result = "This app contains multiple app extensions that are file providers. Apps are only allowed to contain at most a single file provider app extension.";
		break;
	case -402652990:
		result = "This app is not a valid AppleTV Stub App";
		break;
	case -402652965:
		result = "The device is out of storage for apps. Please remove some apps from the device and try again.";
		break;
	case -402652986:
		result = "This app contains multiple WatchKit app extensions. Only a single WatchKit extension is allowed.";
		break;
	case -402652985:
		result = "A WatchKit app within this app is not a valid bundle.";
		break;
	case -402652983:
		result = "The UISupportedDevices key in this app's Info.plist does not specify a valid set of supported devices.";
		break;
	case -402652982:
		result = "This app contains an app extension with an illegal bundle identifier. App extension bundle identifiers must have a prefix consisting of their containing application's bundle identifier followed by a '.', with no further '.' characters after the prefix.";
		break;
	case -402652979:
		result = "This app contains multiple WatchKit 1.0 apps. Only a single WatchKit 1.0 app is allowed.";
		break;
	case -402652978:
		result = "This app contains multiple WatchKit 2.0 apps. Only a single WatchKit 2.0 app is allowed.";
		break;
	case -402652977:
		result = "The WatchKit app has an invalid stub executable.";
		break;
	case -402652976:
		result = "The WatchKit app has multiple app extensions. Only a single WatchKit extension is allowed in a WatchKit app, and only if this is a WatchKit 2.0 app.";
		break;
	case -402652975:
		result = "The WatchKit 2.0 app contains non-WatchKit app extensions. Only WatchKit app extensions are allowed in WatchKit apps.";
		break;
	case -402652974:
		result = "The WatchKit app has one or more embedded frameworks. Frameworks are only allowed in WatchKit app extensions in WatchKit 2.0 apps.";
		break;
	case -402652973:
		result = "This app contains a WatchKit 1.0 app with app extensions. This is not allowed.";
		break;
	case -402652972:
		result = "This app contains a WatchKit 2.0 app without an app extension. WatchKit 2.0 apps must contain a WatchKit app extension.";
		break;
	case -402652971:
		result = "The WatchKit app's Info.plist must have a WKCompanionAppBundleIdentifier key set to the bundle identifier of the companion app.";
		break;
	case -402652970:
		result = "The WatchKit app's Info.plist contains a non-string key.";
		break;
	case -402652969:
		result = "The WatchKit app's Info.plist contains a key that is not in the whitelist of allowed keys for a WatchKit app.";
		break;
	case -402652968:
		result = "The WatchKit 1.0 and a WatchKit 2.0 apps within this app must have have the same bundle identifier.";
		break;
	case -402652967:
		result = "This app contains a WatchKit app with an invalid bundle identifier. The bundle identifier of a WatchKit app must have a prefix consisting of the companion app's bundle identifier, followed by a '.'.";
		break;
	case -402652966:
		result = "This app contains a WatchKit app where the UIDeviceFamily key in its Info.plist does not specify the value 4 to indicate that it's compatible with the Apple Watch device type.";
		break;
	case -402652991:
		result = "The WatchKit app extension must have, in its Info.plist's NSExtension dictionary's NSExtensionAttributes dictionary, the key WKAppBundleIdentifier with a value equal to the associated WatchKit app's bundle identifier.";
		break;
	case -402652964:
		result = "This app or an app that it contains has a Siri Intents app extension that is missing the IntentsSupported array in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist.";
		break;
	case -402652963:
		result = "This app or an app that it contains has a Siri Intents app extension that does not correctly define the IntentsRestrictedWhileLocked key in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist. The key's value must be an array of strings.";
		break;
	case -402652962:
		result = "This app or an app that it contains has a Siri Intents app extension that declares values in its IntentsRestrictedWhileLocked key's array value that are not in its IntentsSupported key's array value (in the NSExtensionAttributes dictionary in the NSExtension dictionary in its Info.plist).";
		break;
	case -402652961:
		result = "This app or an app that it contains declares multiple Siri Intents app extensions that declare one or more of the same values in the IntentsSupported array in the NSExtensionAttributes dictionary in the NSExtension dictionary in their Info.plist. IntentsSupported must be distinct among a given Siri Intents extension type within an app.";
		break;
	case -402652960:
		result = "The WatchKit 2.0 app, which expects to be compatible with watchOS versions earlier than 3.0, contains a non-WatchKit extension in a location that's not compatible with watchOS versions earlier than 3.0.";
		break;
	case -402652959:
		result = "The WatchKit 2.0 app, which expects to be compatible with watchOS versions earlier than 3.0, contains a framework in a location that's not compatible with watchOS versions earlier than 3.0.";
		break;
	case -402652956:
		result = "Multiple iMessage app extensions were found in this app. Only one is allowed.";
		break;
	case -402652955:
		result = "This iMessage application is missing its required iMessage app extension.";
		break;
	case -402652954:
		result = "This iMessage application contains an app extension type other than an iMessage app extension. iMessage applications may only contain one iMessage app extension and may not contain other types of app extensions.";
		break;
	default:
		result = "kAMDSuccess";
	}
	error.append(result);

	return error;
}