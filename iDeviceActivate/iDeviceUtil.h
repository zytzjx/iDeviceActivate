#pragma once
#include <map>
#include <list>
#include "DeviceList.h"
using namespace std;

#define ERROR_CLEANDEVICE	1
#define ERROR_NOIMEI		2
#define ERROR_INFOLESS		3
#define ERROR_UNACTIVATED	4
#define ERROR_INFOLESS_CTPOSTPONEMENTSTATUS		5
#define ERROR_INFOLESS_IMEI						6
#define ERROR_INFOLESS_ACTIVATIONSTATE						7
#define ERROR_INFOLESS_PRODUCTTYPE			8

#define SAFE_DELETE(p) do{ if(NULL!=p){delete p; p = NULL;}} while(0)
#define SAFE_FREE(p) do{ if(NULL!=p){free(p); p = NULL;}} while(0)


typedef struct _tagPARAMETERS_T
{
	_tagPARAMETERS_T() {
		deviceinfo.clear();
		bVerbose = FALSE;
		hEevntFound = NULL;
	};
	~_tagPARAMETERS_T()
	{
		if (hEevntFound != NULL)
		{
			CloseHandle(hEevntFound);
			hEevntFound = NULL;
		}
	}
	BOOL bVerbose;
	int label; // for log print
	std::string device_udid;
	int error_code;
	std::string usb_device_serialnumber;
	map<std::string, std::string> deviceinfo;
	CDeviceList deivces;
	CDeviceList deivces_dfu;
	CDeviceList deivces_recovery;
	CDeviceList deivces_restore;

	HANDLE	hEevntFound;
	std::wstring  m_sPathMDSupport;
	//STRING  m_sPathAASupport;
	BOOL bFMIChecked = FALSE; // if FMIP checked
public:
	void SetQueryUdid(std::string s)
	{
		device_udid = s;
		if (hEevntFound == NULL)
		{
			hEevntFound = CreateEvent(NULL, TRUE, FALSE, NULL);
		}
	}
	DWORD WaitEventFound(int nTimeOut)
	{
		if (hEevntFound == NULL)
		{
			return ERROR_INVALID_HANDLE;
		}
		return WaitForSingleObject(hEevntFound, nTimeOut);
	}
}PARAMETERS_T, * PPARAMETERS_T;

extern PARAMETERS_T g_args;
//Log print
void logIt(const char* fmt, ...);
//void fd_CFShow(CFTypeRef obj);

void LogitErrorInfo(char* str, int errorcode);

std::string GetAMDErrorText(int errorCode);

#define ENTER_FUNC_BEGIN	logIt("%s ++\n",__FUNCTION__)
#define ENTER_FUNC_END		logIt("%s --\n",__FUNCTION__)
#define ENTER_FUNC_ENDRET(ret)		logIt("%s -- return = %d\n",__FUNCTION__, ret)

void Oops(const char* File, unsigned long Line, DWORD dwError);

#define OOPS()		Oops(__FILE__, __LINE__, GetLastError())
#define OOPSERR(d)	Oops(__FILE__, __LINE__, d)
