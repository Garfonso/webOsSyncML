/*
 * Utils.cpp
 *
 *  Created on: 18.02.2011
 *      Author: Moses
 */

#include <vector>
#include <ctime>
#include <sstream>

#include <pthread.h>
#include <spds/SyncStatus.h>
#include <base/Log.h>
#include <base/util/WString.h>
#include <PDL.h>

#include "Utils.h"

pthread_cond_t callbackReady  = PTHREAD_COND_INITIALIZER;
pthread_mutex_t callbackMutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<Funambol::WString> globalResults;
Funambol::WString partlyResult;

Funambol::WString toWString(const char* input)
{
#ifdef USE_WCHAR
	wchar_t buffer[260];
	mbstowcs(buffer,input,260);
	return Funambol::WString(buffer);
#else
	return Funambol::WString(input);
#endif
}

const char* fromWString(const Funambol::WString& string)
{
#ifdef USE_WCHAR
	wcstombs(cBuffer,string.c_str(),4096);
#else
	return string.c_str();
#endif
}

PDL_bool receiveResult(PDL_JSParameters *params)
{
	pthread_mutex_lock(&callbackMutex);
	int numParams = PDL_GetNumJSParams(params);
	if(numParams == 0)
	{
		Funambol::LOG.info("JSCallback failed.");
	}
	else
	{
		for(size_t i = 0; i < numParams; i++)
		{
			Funambol::WString result = toWString(PDL_GetJSParamString(params,i));
			globalResults.push_back(result); //for events this will add event and eventId as two different entries into vector.
		}
	}
	pthread_cond_signal(&callbackReady);
	pthread_mutex_unlock(&callbackMutex);
	return PDL_TRUE;
}

PDL_bool receiveResultLoop(PDL_JSParameters *params)
{
	pthread_mutex_lock(&callbackMutex);
	int numParams = PDL_GetNumJSParams(params);
	if(numParams == 0)
	{
		Funambol::LOG.info("JSCallback Loop failed.");
		pthread_cond_signal(&callbackReady);
	}
	else if(numParams < 6)
	{
		Funambol::LOG.info("Empty Loop.");
		pthread_cond_signal(&callbackReady);
	}
	else
	{
		Funambol::WString chunk = toWString(PDL_GetJSParamString(params,0));
		int chunkIndex = PDL_GetJSParamInt(params,1);
		int numChunks = PDL_GetJSParamInt(params,2);
		int entryIndex = PDL_GetJSParamInt(params,3);
		int numEntries = PDL_GetJSParamInt(params,4);
		bool lastEventID = PDL_GetJSParamInt(params,5) != 0;

		if(chunkIndex != numChunks)
		{
			partlyResult += chunk;
		}
		else //chunkIndex == numChunks => last chunk!
		{
			if(lastEventID) //last chunk == eventId
			{
				//add parts until now as first result
				//and event id as second result.
				globalResults.push_back(partlyResult);
				globalResults.push_back(chunk);
			}
			else
			{
				partlyResult += chunk;
				globalResults.push_back(partlyResult);
			}
			partlyResult = ""; //clear result string for next entry.

			//was this the last entry in the list?
			if(entryIndex == numEntries)
			{
				//wake up processing thread, loop finished!
				pthread_cond_signal(&callbackReady);
			}
		}
	}

	pthread_mutex_unlock(&callbackMutex);
	return PDL_TRUE;
}

std::vector<Funambol::WString>& BlockingServiceCall(const char* method, const Funambol::WString& parameters,const Funambol::WString& param2)
{
	pthread_mutex_lock(&callbackMutex);
	globalResults.clear();
	partlyResult = "";
	const char* params[2];
	params[0] = fromWString(parameters);
	params[1] = fromWString(param2);
	PDL_Err error = PDL_CallJS(method,params,2);
	if(error == PDL_NOERROR)
	{
		Funambol::LOG.debug("Set of callback, now blocking till result received.");
		pthread_cond_wait(&callbackReady,&callbackMutex);
		Funambol::LOG.debug("Callback succeeded.");
	}
	else
	{
		Funambol::LOG.error("Could not start callback. Error: %s (%d).",PDL_GetError(),error);
	}
	pthread_mutex_unlock(&callbackMutex);
	return globalResults;
}

bool isErrorCode(int code)
{

    if ((code >= STC_OK && code < STC_MULTIPLE_CHOICES &&
         code != STC_CHUNKED_ITEM_ACCEPTED &&
         code != STC_PARTIAL_CONTENT       &&
         code != STC_RESET_CONTENT         &&
         code != STC_NO_CONTENT            ) ||
         code == STC_ALREADY_EXISTS) {
        return false;
    } else {
        return true;
    }
}

PDL_bool dateToUTCTimestamp(PDL_JSParameters *params)
{
	int numParams = PDL_GetNumJSParams(params);
	if(numParams < 4)
	{
		//problem: not enough params!
		PDL_JSException(params,"Not enough params, need TZName, Year, Month, Day, at least.");
		return PDL_TRUE;
	}

	const char* oldTZ = getenv("TZ");

	struct std::tm t;
	setenv("TZ",PDL_GetJSParamString(params,0),true);
	tzset();
	t.tm_year = PDL_GetJSParamInt(params,1)-1900;
	t.tm_mon  = PDL_GetJSParamInt(params,2); //-1 already in JS.
	t.tm_mday = PDL_GetJSParamInt(params,3);
	if(numParams >= 5)
		t.tm_hour = PDL_GetJSParamInt(params,4);
	else
		t.tm_hour = 0;
	if(numParams >= 6)
		t.tm_min = PDL_GetJSParamInt(params,5);
	else
		t.tm_min = 0;
	if(numParams >= 7)
		t.tm_sec = PDL_GetJSParamInt(params,6);
	else
		t.tm_sec = 0;

	time_t time = std::mktime(&t); //this should produce a timestamp in UTC from the set timezone!
	Funambol::LOG.debug("Converted %02d.%02d.%04d - %02d:%02d:%02d to %d in timezone %s.",t.tm_mday,t.tm_mon+1,t.tm_year+1900,t.tm_hour,t.tm_min,t.tm_sec,time,getenv("TZ"));

	std::stringstream res;
	res << time;
	PDL_JSReply(params,res.str().c_str());

	setenv("TZ",oldTZ,1);

	return PDL_TRUE;
}

//std::pair<Funambol::WString,Funambol::WString> splitOfID(Funambol::WString event)
//{
//	size_t start = event.rfind("\neventId:");
//	if(start == Funambol::WString::npos)
//	{
//		Funambol::LOG.error("String did not contain eventid.. :(");
//		exit(-1);
//	}
//	start += 9;
//	Funambol::WString id = event.substr(start);
//	Funambol::WString value = event.substr(0,start);
//	return std::pair<Funambol::WString,Funambol::WString>(id,value);
//}
