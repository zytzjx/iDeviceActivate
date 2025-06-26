/********************************************************************
created:	2015/09/28
created:	28:9:2015   10:04
filename: 	logit.cpp
file base:	logit
file ext:	cpp
author:		Jeffery Zhang

purpose:	Print Logs
*********************************************************************/
#include "stdafx.h"
#include "iDeviceUtil.h"

extern PARAMETERS_T g_args;

// 可选：全局日志互斥，保证多线程安全
static std::mutex log_mutex;
void logIt(const char* fmt, ...)
{
    std::lock_guard<std::mutex> lock(log_mutex);

    va_list args;
    va_start(args, fmt);
    int size = _vscprintf(fmt, args) + 1; // Get the size of the formatted string
    std::string buffer(size, L'\0'); // Create a buffer with necessary size
    vsprintf_s(&buffer[0], size, fmt, args); // Format the string
    va_end(args);

    std::string formattedString = "[Label_" + std::to_string(g_args.label) + "]: " + buffer;
    OutputDebugStringA(formattedString.c_str());
}

void Oops(const char* File, unsigned long Line, DWORD dwError)
{
    char szBuf[2048] = { 0 };
    snprintf(szBuf, sizeof(szBuf), "File: %s, Line %lu, Error %lu\n", File, Line, dwError);
    logIt("%s", szBuf);
}

void LogitErrorInfo(const char* str, int errorcode)
{
    std::string s;
    if (errorcode <= 0)
    {
        std::string serr = GetAMDErrorText(errorcode);
        char buf[1100];
        snprintf(buf, sizeof(buf), "[label_%d]: %s%s", g_args.label, str, serr.c_str());
        s = buf;
    }
    else
    {
        LPVOID lpMsgBuf = nullptr;
        FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorcode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&lpMsgBuf,
            0, NULL);

        if (lpMsgBuf) {
            char buf[1100];
            snprintf(buf, sizeof(buf), "[label_%d]: %s%s", g_args.label, str, (char*)lpMsgBuf);
            s = buf;
            LocalFree(lpMsgBuf);
        }
    }
    OutputDebugStringA(s.c_str());
}
