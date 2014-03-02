#include <string>
#include <sstream>
#include <pthread.h>

#include "client/SyncClient.h"
#include "client/DMTClientConfig.h"
#include "base/adapter/PlatformAdapter.h"
#include "spds/DefaultConfigFactory.h"
#include <syncml/core/Constants.h>

#include "SDL.h"
#include "PDL.h"

#include "SysLog.h"
#include "Utils.h"
#include "WebOsCalendarSyncSource.h"
#include "WebOsContactsSyncSource.h"

//maybe use that: 
//PDL_GetUniqueID get's unique ID for the device. Necessary for device identification?
//PDL_GetDeviceName might be nice to see the name on the server.

//this app won't get a service! Just run the app from menu => sync. 
//send a message, if sync was successful or if errors occurred...
//maybe present some statistics in a mojo-scene after the sync finished (how to keep the mojo scene up to date?)

pthread_mutex_t jsCallMutex = PTHREAD_MUTEX_INITIALIZER;
#define BUFFERLEN 1024
char buffer[BUFFERLEN];
bool syncing = false;
std::string user;
std::string password;
std::string server;
bool doCalendar;
bool doContacts;
std::string calendarDataStore;
std::string calendarMethod;
std::string contactsDataStore;
std::string contactsMethod;

void doSync()
{
	using namespace Funambol;

	std::stringstream log;
	log << "Syncing: " << user << ":" << password << "@" << server << ". Calendar: " << doCalendar << ", Contacts: " << doContacts << ".";
	LOG.info(log.str().c_str());

	//create config object.
	StringMap env;
	env.put("HOME_FOLDER","/media/internal/.webOsSyncML");
	env.put("CONFIG_FOLDER","/media/internal/.webOsSyncML/config");
	PlatformAdapter::init("mobo/webOsSyncML",env);
	DMTClientConfig config;
	
	SyncSource* ssArray[3];
	int ssIndex = 0;
	ssArray[0] = ssArray[1] = ssArray[2] = NULL;

	//fill some config values
	if(!config.read()) 
	{
		PDL_GetDeviceName(buffer,BUFFERLEN);
		std::string id = "";//"webOsSyncML"; id.append(buffer);
		PDL_GetUniqueID(buffer,BUFFERLEN);
		id.append(buffer);
		//config.getDeviceConfig().setDevType("smartphone");
		config.getDeviceConfig().setDevID(id.c_str());
		config.getAccessConfig().setClientAuthType(AUTH_TYPE_MD5);
	}

	//fill access values:
	config.getAccessConfig().setSyncURL(server.c_str());
	config.getAccessConfig().setUsername(user.c_str());
	config.getAccessConfig().setPassword(password.c_str());

	WebOsCalendarSyncSource* calendar;
	WebOsContactsSyncSource* contacts;
	if(doCalendar)
	{
		SyncSourceConfig* calConfig = config.getSyncSourceConfig("calendar");
		if(!calConfig)
		{
			calConfig = DefaultConfigFactory::getSyncSourceConfig("calendar");
		}

		calConfig->setSyncModes("slow,two-way,refresh-from-server,refresh-from-client,one-way-from-server,one-way-from-client");
		calConfig->setSync(calendarMethod.c_str());
		calConfig->setURI(calendarDataStore.c_str());
		calConfig->setSupportedTypes("text/calendar");
		calConfig->setType("text/calendar");
		config.setSyncSourceConfig(*calConfig);
		calendar = new WebOsCalendarSyncSource(TEXT("calendar"),calConfig);
		ssArray[ssIndex] = calendar;
		ssIndex++;
	}

	if(doContacts)
	{
		SyncSourceConfig* conConfig = config.getSyncSourceConfig("contact");
		if(!conConfig)
		{
			conConfig = DefaultConfigFactory::getSyncSourceConfig("contact");
		}

		conConfig->setSyncModes("slow,two-way,refresh-from-server,refresh-from-client,one-way-from-server,one-way-from-client");
		conConfig->setSync(contactsMethod.c_str());
		conConfig->setSupportedTypes("text/x-vcard");
		conConfig->setType("text/x-vcard");
		conConfig->setURI(contactsDataStore.c_str());
		config.setSyncSourceConfig(*conConfig);
		contacts = new WebOsContactsSyncSource(TEXT("contact"),conConfig);
		ssArray[ssIndex] = contacts;
		ssIndex++;
	}

	//start sync process:
	SyncClient client;
	client.sync(config, ssArray);

	//save config options, includes configuration for next sync process:
	config.getAccessConfig().setPassword("");
	config.save();

	StringBuffer report;
	client.getSyncReport()->toString(report,true);
	LOG.info("Report: %s.",report.c_str());

	const char* params[2];
	if(doCalendar)
	{
		LOG.info("Cal_Error: %s (%d)",calendar->getReport()->getLastErrorMsg(),calendar->getReport()->getLastErrorCode());
		if(calendar->getReport()->getLastErrorCode() != 0)
			params[0] = "fail";
		else
			params[0] = "ok";
	}
	else
		params[0] = "ok";
	if(doContacts && contacts->getReport()->getLastErrorCode() != 0)
		params[1] = "fail";
	else
		params[1] = "ok";
 	PDL_CallJS("finished",params,2);

	for(int i = 0; i < ssIndex; i++)
	{
		delete ssArray[i];
		ssArray[i] = NULL;
	}
}

// warum endet der sync damit:
// Sync loop complete, ending and commiting sources
// The source calendar got and error 10004: An error occurred on one or more items.
//??

// warum speichter das doof ding immer noch in klartext?


PDL_bool startSync(PDL_JSParameters *params)
{
    double val;
    if (PDL_GetNumJSParams(params) != 9)
    {
        PDL_JSException(params, "Not all params specified!");
        return PDL_TRUE;
    }

    if(pthread_mutex_trylock(&jsCallMutex) == 0)
    {
    	user = PDL_GetJSParamString(params,0);
    	password = PDL_GetJSParamString(params,1);
    	server = PDL_GetJSParamString(params,2);
    	doCalendar = PDL_GetJSParamInt(params,3) != 0;
    	doContacts = PDL_GetJSParamInt(params,4) != 0;
    	calendarDataStore = PDL_GetJSParamString(params,5);
    	calendarMethod = PDL_GetJSParamString(params,6);
    	contactsDataStore = PDL_GetJSParamString(params,7);
    	contactsMethod = PDL_GetJSParamString(params,8);
    	syncing = true;
    	pthread_mutex_unlock(&jsCallMutex);

        PDL_JSReply(params, "Will start sync now. May take a while...");
    }
    else
    	PDL_JSReply(params, "Sync already in progress. Please be patient.");

    return PDL_TRUE;
}

int main(int argc, char** argv)
{
//	openlog("mobo.info.webossyncml",0,LOG_USER);
//	syslog(LOG_INFO, "Im running!");
	Funambol::Log::setLogger(new Funambol::SysLogLogger());
	Funambol::LOG.setLogPath("info.mobo.webossyncml");
	Funambol::LOG.setLevel(LOG_LEVEL_DEBUG);
	Funambol::LOG.info("Im running!!");
    int result = SDL_Init(SDL_INIT_VIDEO);
    if ( result != 0 )
    {
        Funambol::LOG.info("Could not init SDL: %s\n", SDL_GetError());
        printf("Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }
	PDL_Init(0);

    // register the js callback
	PDL_Err err = PDL_RegisterJSHandler("startSync", startSync);
	Funambol::LOG.info("Register: %s (%d)",PDL_GetError(),err);
	printf("Register: %s (%d)\n",PDL_GetError(),err);
    err = PDL_RegisterJSHandler("receiveResult", receiveResult);
    err = PDL_RegisterJSHandler("receiveResultLoop",receiveResultLoop);
    err = PDL_RegisterJSHandler("dateToUTCTimestamp",dateToUTCTimestamp);
    PDL_JSRegistrationComplete();

    Funambol::LOG.info("Registered methods. Ready to receive..");

    // Event descriptor
    SDL_Event Event;

    Uint32 fireTime = SDL_GetTicks() + 1000;

    if(PDL_IsPlugin() == PDL_FALSE)
    {
    	if(argc >= 2)
    		user = argv[1];
    	if(argc >= 3)
    		password = argv[2];
    	if(argc >= 4)
    		server = argv[3];
    	doCalendar = true;
    	doContacts = true;
    	calendarDataStore = "./calendar";
    	contactsDataStore = "./contacts";

    	if(argc >= 6)
    	{
    		syncing = true;
    		LOG.info("Running in debug mode with %s:%s@%s, %s, %s.",user.c_str(),password.c_str(),server.c_str());
    	}
    }

    do
    {
    	/*if ((SDL_GetTicks() > fireTime) )
    	{
    		// fire off the js
    		const char *params[2];
    		params[0] = "foo";
    		params[1] = "bar";
    		PDL_Err mjErr = PDL_CallJS("updateStatus", params, 2);
    		if ( mjErr != PDL_NOERROR )
    		{
    			Funambol::LOG.info("error in JS-Callback! %s",PDL_GetError());
    		}
    		fireTime = SDL_GetTicks() + 1000;
    	}*/

    	if(syncing)
    	{
    		pthread_mutex_lock(&jsCallMutex);
    		doSync();
    		syncing = false;
    		pthread_mutex_unlock(&jsCallMutex);
    	}

    	sleep(1);

        // Process the events
    	SDL_PollEvent(&Event); //suggested in the forum: SDL_WaitEvent. Problem: Hangs and doesn't start sync?
    } while (Event.type != SDL_QUIT);

	PDL_Quit();
    SDL_Quit();
	Funambol::LOG.info("Fertig.");

	return 0;
}
