/*	CFLogUtilities.h
	Copyright (c) 2004-2012, Apple Inc. All rights reserved.
*/

/*
        APPLE SPI:  NOT TO BE USED OUTSIDE APPLE!
*/

#if !defined(__COREFOUNDATION_CFLOGUTILITIES__)
#define __COREFOUNDATION_CFLOGUTILITIES__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFString.h>

CF_EXTERN_C_BEGIN

#ifndef CF_OPEN_SOURCE
enum {	// Options for the flags argument for CFLogTest
    kCFTestLogGlobal = 1 << 0   // Causes log to go to "global" location; note, this should be set consistently for all calls within a process
};

/* Log a message to the standard self-test log file using the standard Apple "self test" format. 
*/
CF_EXPORT void CFLogTest(CFOptionFlags flags, CFStringRef format, ...);
#endif

enum {	// Legal level values for CFLog()
    kCFLogLevelEmergency = 0,
    kCFLogLevelAlert = 1,
    kCFLogLevelCritical = 2,
    kCFLogLevelError = 3,
    kCFLogLevelWarning = 4,
    kCFLogLevelNotice = 5,
    kCFLogLevelInfo = 6,
    kCFLogLevelDebug = 7,
};

CF_EXPORT void CFLog(int32_t level, CFStringRef format, ...);
/*	Passing in a level value which is outside the range of 0-7 will cause the the call to do nothing.
	CFLog() logs the message using the asl.h API, and uses the level parameter as the log level.
	Note that the asl subsystem ignores some log levels by default.
	CFLog() is not fast, and is not going to be guaranteed to be fast.
	Even "no-op" CFLogs are not necessarily fast.
	If you care about performance, you shouldn't be logging.
*/

CF_EXTERN_C_END

#endif /* ! __COREFOUNDATION_CFLOGUTILITIES__ */

