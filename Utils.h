/*
 * Utils.h
 *
 *  Created on: 18.02.2011
 *      Author: Moses
 */

#include <base/util/WString.h>

#include <vector>
#include <PDL.h>

#ifndef UTILS_H_
#define UTILS_H_

/**
 * Calls the JS Part of our hybrid app. This call will block until a result is received
 * and return this result as vector of strings. This comes in very handy, if we need a
 * result from the JS Part of the application!
 * @param  method name of the JS method to be called.
 * @param  param1/2 first and second parameter to the JS method, optional
 */
std::vector<Funambol::WString>& BlockingServiceCall(const char* method, const Funambol::WString& param1 = TEXT(""), const Funambol::WString& param2 = TEXT(""));
/**
 * Helper for BlockingServiceCall. This will be called from JS to "release"
 * the main C++ thread. There are two variants: The first one is used to
 * recieve short things, like "ok" or event ids.
 * The second one can receieve an array of strings which in turn might me cut
 * into several short strings, because the interface cuts off the strings after 255 chars
 * and doesn't allow to transfer too many of them. So the second version is called with
 * string chuncks and some information how to assemble them.
 *
 * Calling one of this functions without parameters is interpreted as failure notice. The C++
 * Part will wake up and (try to) handle the error. Be sure to call at least one of this functions
 * at least one time or life with a locked up C++ main thread!
 */
PDL_bool receiveResult(PDL_JSParameters *params);
PDL_bool receiveResultLoop(PDL_JSParameters *params);

/**
 * Funambol help function.
 */
bool isErrorCode(int code);

/**
 * Used for string conversions. Might be helpful if we
 * get the strange idea to use multibyte strings someday...
 */
const char* fromWString(const Funambol::WString& string);
Funambol::WString toWString(const char* input);

/**
 * Calculates a UTC timestamp for TZname, Year, Month,
 * Day, Hours, Minutes, Seconds (last three are optional)
 * and returns it to JS.
 */
PDL_bool dateToUTCTimestamp(PDL_JSParameters *params);

#endif /* UTILS_H_ */
