/*
 * SysLog.h
 *
 *  Created on: 13.02.2011
 *      Author: Moses
 */

#ifndef SYSLOGLOGGER_H_
#define SYSLOGLOGGER_H_

#include <base/Log.h>

BEGIN_NAMESPACE

/**
 * extended API, can only be used if it is certain that
 * Log::instance() returns a POSIXLog
 */
class SysLogLogger : public Log {
private:
	FILE* mFile;

 public:
	SysLogLogger();
    virtual ~SysLogLogger();

    void setLogPath(const char*  configLogPath);
    void setLogName(const char*  /*configLogName*/) {}
    void error(const char*  msg, ...);
    void info(const char*  msg, ...);
    void developer(const char*  msg, ...);
    void debug(const char*  msg, ...);
    void reset(const char* /*title = NULL*/) {}
    size_t getLogSize() { return 0; }
};

END_NAMESPACE

#endif /* SYSLOG_H_ */
