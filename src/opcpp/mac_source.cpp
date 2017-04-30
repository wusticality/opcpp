///****************************************************************
/// Copyright © 2008 opGames LLC - All Rights Reserved
///
/// Authors: Kevin Depue & Lucas Ellis
///
/// File: WindowsSource.cpp
/// Date: 08/07/2007
///
/// Description:
///
/// Linux Platform Specific Code
///****************************************************************

#include <mach-o/dyld.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include "opcpp/opcpp.h"

/*=== logging ===*/

void errors::opLog::DebugLog(const opString& s) {
    // do nothing, we have no IDE..
}

///==========================================
/// Windows opCpp.exe Path
///==========================================

void opPlatform::Breakpoint() {
    // nothing to do here..
}

// Parse the executable location.
bool opPlatform::ParseExeLocation(const opString& cmdarg) {
    opString resolved;
    unsigned int length = 10000;

    resolved.Resize(length);

    int ret = _NSGetExecutablePath(&resolved[0], &length);

    // If we have a problem parsing the exe location, error.
    if (ret == 0) {
        resolved.Resize(length);
    } else {
        Log("An error occurred when determining the executable path in Linux.  "
            "Please report to www.opcpp.com.");

        return false;
    }

    path resolvedpath = resolved.GetString();
    resolved = resolvedpath.string();

    opCppPath = resolved;
    opCppDirectory = resolved.RLeft('/');
    opCppExecutableName = resolved.RRight('/');

    return true;
}

/*=== timing ===*/

double timing::opTimer::GetTimeSeconds() {
    timeval curr_time;
    double time;

    gettimeofday(&curr_time, 0);

    time = curr_time.tv_sec;
    time += (double)curr_time.tv_usec * 1e-6;

    return time;
}

void timing::opTimer::InitTimeSeconds() {
    timeval curr_time;
    double time;

    gettimeofday(&curr_time, 0);

    time = curr_time.tv_sec;
    time += (double)curr_time.tv_usec * 1e-6;

    timerstart = time;
}
