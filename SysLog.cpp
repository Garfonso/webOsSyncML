/*
 * SysLog.cpp
 *
 *  Created on: 13.02.2011
 *      Author: Moses
 */

#include "SysLog.h"

#include <syslog.h>

Funambol::SysLogLogger::SysLogLogger()
{
	openlog("mobo.info.webossyncml",0,LOG_USER);
	mFile = fopen("/media/internal/.webOsSyncML/output.txt","w");
}

Funambol::SysLogLogger::~SysLogLogger()
{
	closelog();
}

void Funambol::SysLogLogger::setLogPath(const char*  configLogPath)
{
	closelog();
	openlog(configLogPath,0,LOG_USER);
}

void Funambol::SysLogLogger::error(const char*  msg, ...)
{
    va_list argList;
    va_start (argList, msg);
    vsyslog(LOG_ERR,msg,argList);
    vprintf(msg,argList);
    printf("\n");
    if(mFile)
    {
    	vfprintf(mFile,msg,argList);
    	fprintf(mFile,"\n");
    }
    va_end(argList);
}

void Funambol::SysLogLogger::info(const char*  msg, ...)
{
    va_list argList;
    va_start (argList, msg);
    vsyslog(LOG_WARNING,msg,argList);
    vprintf(msg,argList);
    printf("\n");
    if(mFile)
    {
    	vfprintf(mFile,msg,argList);
    	fprintf(mFile,"\n");
    }
    va_end(argList);
}

void Funambol::SysLogLogger::debug(const char*  msg, ...)
{
    va_list argList;
    va_start (argList, msg);
    vsyslog(LOG_NOTICE,msg,argList);
    vprintf(msg,argList);
    printf("\n");
    if(mFile)
    {
    	vfprintf(mFile,msg,argList);
    	fprintf(mFile,"\n");
    }
    va_end(argList);
}

void Funambol::SysLogLogger::developer(const char*  msg, ...)
{
    va_list argList;
    va_start (argList, msg);
    vsyslog(LOG_ERR,msg,argList);
    vprintf(msg,argList);
    printf("\n");
    if(mFile)
    {
    	vfprintf(mFile,msg,argList);
    	fprintf(mFile,"\n");
    }
    va_end(argList);
}
