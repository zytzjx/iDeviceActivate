// iDeviceActivate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MobileActivation.h"
#include "iDeviceUtil.h"
#include "iTunesMobileDevice.h"
#include <cli/CLI11.hpp>

#pragma comment(lib, "libxml2s.lib")

extern "C" FILE** __cdecl __iob_func(void)
{
    static FILE* iob[] = { stdin, stdout, stderr };
    return iob;
}

PARAMETERS_T g_args;
#define INTERVALTIME	1000
#define RETRYWAITDEV	3


TCHAR* get_apple_support_path(TCHAR* module)
{
	TCHAR* ret = NULL;
	if (module != NULL)
	{
		HKEY hKey;
		TCHAR buf[1024];
		_stprintf_s(buf, _T("SOFTWARE\\Apple Inc.\\%s"), module);
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, buf, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			DWORD size = MAX_PATH;
			ZeroMemory(buf, sizeof(buf));
			if (RegQueryValueEx(hKey, _T("InstallDir"), NULL, NULL, (LPBYTE)buf, &size) == ERROR_SUCCESS)
			{
				ret = _tcsdup(buf);
			}
			RegCloseKey(hKey);
		}
	}
	return ret;
}
void set_path()
{
	//char *p = _dupenv_s("PATH");
	TCHAR* p = nullptr, * psthome = nullptr;
	size_t len;
	errno_t err = _tdupenv_s(&p, &len, _T("PATH"));
	if (err) return;
	//free(tmp);
	TCHAR* pAppleMobileDeviceSupport = nullptr;

	int nLen = _tcslen(p) + 2 * MAX_PATH;
	TCHAR* pp = (TCHAR*)malloc(nLen *sizeof(TCHAR));
	if (pp != NULL)
	{
		_tcscpy_s(pp, nLen, _T("PATH="));
		_tcscat_s(pp, nLen, p);
		//strcat(pp, ";C:\\Program Files (x86)\\Common Files\\Apple\\Apple Application Support;C:\\Program Files (x86)\\Common Files\\Apple\\Mobile Device Support");
		//char* pAppleApplicationSupport = get_apple_support_path("Apple Application Support");
		//logIt("Apple Application Support=%s\n", pAppleApplicationSupport);
#if defined(_M_IX86) || defined(__i386__) || defined(_X86_)
		err = _tdupenv_s(&psthome, &len, _T("APSTHOME"));
		if (err == 0) {
			TCHAR itunsdll[MAX_PATH] = { 0 };
			PathCombine(itunsdll, psthome, _T("iTunesDLL"));
			if (PathIsDirectory(itunsdll)) {
				pAppleMobileDeviceSupport = _tcsdup(itunsdll);
			}
		}
		SAFE_FREE(psthome);
		logIt("APSTHOMEITUNES=%ws\n", pAppleMobileDeviceSupport);
#endif
		if (pAppleMobileDeviceSupport == nullptr) {
			pAppleMobileDeviceSupport = get_apple_support_path(_T("Apple Mobile Device Support"));
		}
		logIt("Apple Mobile Device Support=%ws\n", pAppleMobileDeviceSupport);

		if (pAppleMobileDeviceSupport != NULL)
		{
			g_args.m_sPathMDSupport.assign(pAppleMobileDeviceSupport);
			_tcscat_s(pp, nLen, _T(";"));
			_tcscat_s(pp, nLen, pAppleMobileDeviceSupport);
			SAFE_FREE(pAppleMobileDeviceSupport);
		}
		_tputenv(pp);
		SAFE_FREE(pp);
	}
	SAFE_FREE(p);
}

LONG WINAPI my_UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	logIt("iDeviceActivate. exceptionCode: %x, exceptionAddress: %x (%x)\n", 
		ExceptionInfo->ExceptionRecord->ExceptionCode, 
		ExceptionInfo->ExceptionRecord->ExceptionAddress, 
		GetModuleHandle(_T("MobileDevice.dll")));
	return EXCEPTION_EXECUTE_HANDLER;
}

BOOL waitDeivceTimeout(std::string udid, int nTimeout)
{
	int nStartTime = GetTickCount();
	int nEndTime = GetTickCount();
	BOOL bTimeout = TRUE;
	if (udid.empty()) return FALSE;
	do
	{
		if (g_args.deivces.check_device(udid))
		{
			bTimeout = FALSE;
			break;
		}
		else
		{
			Sleep(INTERVALTIME);
			nEndTime = GetTickCount();
		}
	} while (nEndTime - nStartTime < nTimeout);

	return bTimeout;

}

BOOL waitDeivceTimeout(CDeviceList& devs, std::string srnm, int nTimeout)
{
	int nStartTime = GetTickCount();
	int nEndTime = GetTickCount();
	BOOL bTimeout = TRUE;
	do
	{
		if (devs.check_device(srnm))
		{
			bTimeout = FALSE;
			break;
		}
		else
		{
			Sleep(INTERVALTIME);
			nEndTime = GetTickCount();
		}
	} while (nEndTime - nStartTime < nTimeout);

	return bTimeout;

}

int _tmain(int argc, _TCHAR* argv[])
{
	// 设置控制台输出编码为UTF-8
	//_setmode(_fileno(stdout), _O_U8TEXT);
    // Add these includes at the top of the file to resolve the undefined identifiers.
	typedef enum {
		OP_NONE = 0, OP_ACTIVATE, OP_DEACTIVATE, OP_GETSTATE
	} op_t;
	op_t op = OP_NONE;
    CLI::App app{ "Apple Device activate" };
	std::string device_udid;
	int nTimeout = 10000; // default timeout 10 seconds
	// 定义命令参数（必须提供且只能是特定值）
	std::string command;
	app.add_option("command", command, "Operation to perform")
		->required()
		->check(CLI::IsMember({ "activate", "deactivate", "activatestatus" }));
	app.add_flag("-v,--verbose", g_args.bVerbose, "Enable verbose output")->default_val(false);
	app.add_flag("-f,--fmicheck", g_args.bFMIChecked, "only check FMI, do not activate device")->default_val(false);
	app.add_option("-l,--label", g_args.label, "label for lot")->check(CLI::PositiveNumber);
	app.add_option("-u,--udid", device_udid, "Use UDID instead of serial number");
	app.add_option("-t,--timeout", nTimeout, "Wait timeout for device in milliseconds")->default_val(10000)->check(CLI::PositiveNumber);
	//app.add_option_group("Device Options", "Options related to device handling");
	CLI11_PARSE(app, argc, argv);
	
	int ret = 21;
#ifdef NDEBUG
	SetUnhandledExceptionFilter(my_UnhandledExceptionFilter);
	_CrtSetReportMode(_CRT_ERROR | _CRT_ASSERT | _CRT_WARN, 0);
#endif // NODEBUG
	logIt("System codepage=%d", GetACP());
	set_path();

	g_args.SetQueryUdid(device_udid);
	if (!init_iTunesMobileDevice(kAMNormalNotification, 10000)) {
		fprintf(stderr, "Failed to initialize iTunes Mobile Device\n");
		return 1;
	}

	/*CMobileActivation ma;
	ma.response_new();
	ma.CreateHtmlResponseFromFile("E:\\Works\\iDeviceUtility\\data\\selectcarrier.html");
	ma.parse_html_response();
	*/


	if (g_args.bVerbose) {
		SetVerbose(TRUE);
	}
	else {
		SetVerbose(FALSE);
	}
	PrintMobileDeviceDLLVersion();
	if (op == OP_NONE && command.empty()) {
		std::cout << "No command specified. Use --help for usage information." << std::endl;
		return 1;
	}
	CMobileActivation mobileActivation;
	if (g_args.bVerbose) {
		mobileActivation.set_debug_level(10);
	}
	if (device_udid.empty())
	{
		int itry = RETRYWAITDEV;
		do
		{
			Sleep(INTERVALTIME);
			device_udid = g_args.deivces.getAnyOne();
		} while (itry-- > 0 && device_udid.empty());
		if (device_udid.empty())
		{
			std::cout << "No devices found!" << endl;
			//return ERROR_NOT_FOUND;
			ret = ERROR_NOT_FOUND;
			goto EXIT;
		}
	}
	BOOL bTimeout = waitDeivceTimeout(device_udid, nTimeout);

	if (bTimeout)
	{
		ret = ERROR_TIMEOUT;
		//return ret;
		goto EXIT;
	}
	if (command == "activate") {
		op = OP_ACTIVATE;
		std::cout << "Activating device: " << device_udid << std::endl;
	}
	else if (command == "deactivate") {
		op = OP_DEACTIVATE;
		std::cout << "Deactivating device: " << device_udid << std::endl;
	}
	else if (command == "activatestatus") {
		op = OP_GETSTATE;
		std::cout << "Checking status for: " << device_udid << std::endl;
	}
	HANDLE m_hDevice = NULL;
	if (!g_args.deivces.find_device(device_udid, m_hDevice))
	{
		logIt("Device %s not found in list", device_udid.c_str());
		ret = ERROR_NOT_FOUND;
		goto EXIT;
	}
	if (op == OP_ACTIVATE) {
		// Example usage
		if (mobileActivation.request_new() != E_SUCCESS) {
			fprintf(stderr, "Failed to create new request\n");
			return 1;
		}
	
		mobileActivation.SetDeviceHandle(m_hDevice, device_udid);
		if (mobileActivation.StartSession() != ERROR_SUCCESS)
		{
			logIt("Failed to start session for device %s", device_udid.c_str());
			ret = ERROR_SESSION_CREDENTIAL_CONFLICT;
			goto EXIT;
		}
		mobileActivation.GetVersionActivation();
		if (mobileActivation.IsActivated()) {
			ret = 1226;
			goto EXIT;
		}
		mobileActivation.Activate();
		mobileActivation.StopSession();
	}
	else if (op == OP_DEACTIVATE) {
		mobileActivation.SetDeviceHandle(m_hDevice, device_udid);
		if (mobileActivation.StartSession() != ERROR_SUCCESS)
		{
			logIt("Failed to start session for device %s", device_udid.c_str());
			ret = ERROR_SESSION_CREDENTIAL_CONFLICT;
			goto EXIT;
		}
		mobileActivation.Deactivate();
		mobileActivation.StopSession();
	}
	else if (op == OP_GETSTATE) {
		mobileActivation.SetDeviceHandle(m_hDevice, device_udid);
		if (mobileActivation.StartSession() != ERROR_SUCCESS)
		{
			logIt("Failed to start session for device %s", device_udid.c_str());
			ret = ERROR_SESSION_CREDENTIAL_CONFLICT;
			goto EXIT;
		}
		mobileActivation.IsActivated(TRUE);
		mobileActivation.StopSession();
	}
	else {
		logIt("no task need to do");
	}
EXIT:
	uninit_iTunesMobileDevice();
	return 0;
}

