#include <vector>
#include <sstream>

#include "Utils.h"
#include "WebOsCalendarSyncSource.h"

#include <vocl/VConverter.h>
#include <PDL.h>

USE_FUNAMBOL_NAMESPACE

//static const std::string delimiter = "-----";
//static const std::string importanceHigh = "high importance"; // = 2
//static const std::string importanceLow = "low importance";   // = 0
//static const std::string privateString = "is private"; // => Sensitivity == 2

//int WebOsCalendarSyncSource::getImportance(const std::string& note)
//{
//	if(note.rfind(importanceHigh) != std::string::npos)
//	{
//		return 2;
//	}
//	else if(note.rfind(importanceLow) != std::string::npos)
//	{
//		return 0;
//	}
//	else
//		return 1;
//}
//
//int WebOsCalendarSyncSource::getPrivate(const std::string& note)
//{
//	if(note.rfind(privateString) != std::string::npos)
//		return 2;
//	return 0;
//}

	//do initialization here. After that nextItem needs to work. getSyncMode will deliver the used sync mode.
//	virtual int beginSync();
	//commit changes here. 
//	virtual int endSync(); 	
	//called by the sync engine with the status returned by the server for a certain item that the client sent to the server.
	//It contains also the proper command associated to the item.
	//Parameters:
	//	key 	the local key of the item
	//	status 	the SyncML status returned by the server
	//	command the SyncML command associated to the item 
//	virtual void setItemStatus 	(const WCHAR * key,int status,const char * command);
	//Indicates that all the server status of the current package of the client items has been processed by the engine.
	//This signal can be useful to update the modification arrays
//	virtual void serverStatusPackageEnded();
	//Indicates that all the client status of the current package of the server items that has been processed by the client and are going to be sent to the server.
	//This signal can be useful to update the modification arrays
//	virtual void clientStatusPackageEnded();



//NEED to be implemented:
//Removes all the item of the sync source.
//It is called by the engine in the case of a refresh from server to clean all the client items before receiving the server ones. It is called after the beginSync() method.
//Returns:
//    0 if the remote succeded. (remove??)
int WebOsCalendarSyncSource::removeAllItems()
{
	int ret = STC_OK;
//	LOG.debug("Currently have: %d events.",mEvents.size());
	//int oldOffset = mItemOffset;
	//do {
	//	oldOffset = mItemOffset;
//		getMoreItems();
	//} while(oldOffset != mItemOffset);  //receive all items!
//	LOG.debug("Trying to delete all items. %d",mEvents.size());
//	while(!mEvents.empty())
//	{
//		std::string id = mEvents.front().first;
//		mEvents.pop_front();
//
//		LOG.info("Calling deleteEvent %s.",id.c_str());
//		std::vector<std::string> result = BlockingServiceCall("deleteEvent",id);
//		if(result.empty())
//		{
//			LOG.error("delete event %s did not work.",id.c_str());
//			ret = STC_COMMAND_FAILED;
//		}
//		else
//		{
//			LOG.info("Deletion OK, received: %s.",result[0].c_str());
//		}
//
//		mItemOffset = 0;
//		if(mEvents.empty())
//			getMoreItems();
//	}
//	LOG.debug("Finished deletion of events.");
//	mItemOffset = 0;

	LOG.debug("Trying to delete all elements.");
	std::vector<WString> result = BlockingServiceCall("deleteAllEvents",TEXT(""));
	if(result.empty())
	{
		LOG.error("Delete all events failed.");
		ret = STC_COMMAND_FAILED;
	}
	else
	{
		LOG.debug("Delete all events ok. %s",result[0].c_str());
		return 0;
	}
}

void WebOsCalendarSyncSource::getMoreItems()
{
//	if(!mEvents.empty())
//		return;

	//this will need to call listItems on the calendar service and fill a structure with all event ids.
	//user mItemOffset to get a bunch of items every time and not all of them at once.
	std::stringstream parameter;
	parameter << mItemOffset;

	std::vector<WString>& result = BlockingServiceCall("listEvents",toWString(parameter.str().c_str()));
	for(int i = 0; i < result.size(); i+=2)
	{
		std::pair<WString,WString> event;
		event.second = result[i];
		event.first = result[i+1];
		//LOG.debug("Got event %s from %s",event.second.c_str(),result[i].c_str());
		mEvents.push_back(event);
		mItemOffset++;
	}

	LOG.info("Received %d events. Now have %d events. Offset now is: %d.",result.size(),mEvents.size(),mItemOffset);
}

//called to get first/next item during slow sync.
SyncItem* WebOsCalendarSyncSource::getFirstItem()
{
	LOG.info("=========== GET FIRST ITEM CALLED ==================");

	return getNextItem();
//	//Get some items form JS.
//	getMoreItems();
//	if(mEvents.empty())
//	{
//		//no Items received => there are no items!?
//		LOG.info("No items.");
//		return NULL;
//	}
//	else
//	{
//		//take first event and convert into item of correct type.
//		std::pair<std::string,std::string>& e = mEvents.front();
//		SyncItem* item = new SyncItem();
//		item->setDataType(this->getConfig().getType());
//		item->setData(e.second.c_str(),e.second.length());
//		item->setKey(e.first.c_str());
//		mEvents.pop_front();
//		return item;
//	}
//	return NULL;
}

SyncItem* WebOsCalendarSyncSource::getNextItem()
{
	LOG.info("========== GET NEXT ITEM CALLED ===========");
	if(mEvents.empty())
	{
		//no items anymore, need to get more. If that doesn't work, there are no more items.
		getMoreItems();
		if(mEvents.empty())
		{
			LOG.info("======= NO MORE ELEMENTS ==========");
			return NULL;
		}
	}

	if(!mEvents.empty())
	{
		//take first event and convert into item of correct type.
		std::pair<WString,WString>& e = mEvents.front();
		SyncItem* item = new SyncItem();
		item->setDataType(TEXT("text/calendar"));
		item->setData(e.second.c_str(),e.second.length());
		item->setKey(e.first.c_str());
		LOG.debug("Received %s from JS.",e.second.c_str());
		mEvents.pop_front();
		return item;
	}
	else
		return NULL;
}

void WebOsCalendarSyncSource::getChanges()
{
	std::vector<WString>& result = BlockingServiceCall("getEventChanges",TEXT(""));
	if(result.empty())
	{
		LOG.error("GetChanges failed.");
	}
	else
	{
		if(result.front() != "0")
		{
			for(size_t i = 0; i < result.size(); i+=2)
			{
				std::pair<WString,WString> event;
				event.second = result[i];
				event.first = result[i+1];
				mEvents.push_back(event);
			}
		}
		mGetChangesCalled = true;
		LOG.debug("GetChanges successful.. now have %d changed objects.",mEvents.size());
	}
}

//get first/next new item
SyncItem* WebOsCalendarSyncSource::getFirstNewItem()
{
	LOG.info("=========== GET FIRST NEW ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}
	return getNextNewItem();
}

SyncItem* WebOsCalendarSyncSource::getNextNewItem()
{
	LOG.info("=========== GET NEXT NEW ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}
	return NULL;
}

//get first/next updated item
SyncItem* WebOsCalendarSyncSource::getFirstUpdatedItem()
{
	LOG.info("=========== GET FIRST UPDATED ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}
	return getNextUpdatedItem();
}

SyncItem* WebOsCalendarSyncSource::getNextUpdatedItem()
{
	LOG.info("=========== GET NEXT UPDATED ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}

	if(!mEvents.empty())
	{
		//take first event and convert into item of correct type.
		std::pair<WString,WString>& e = mEvents.front();
		SyncItem* item = new SyncItem();
		item->setDataType(TEXT("text/calendar"));
		item->setData(e.second.c_str(),e.second.length());
		item->setKey(e.first.c_str());
		LOG.debug("Received %s from JS.",e.second.c_str());
		mEvents.pop_front();
		return item;
	}
	else
		return NULL;


	return NULL;
}

//get first/next deleted item
SyncItem* WebOsCalendarSyncSource::getFirstDeletedItem()
{
	LOG.info("=========== GET FIRST DELETED ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}
	return getNextDeletedItem();
}

SyncItem* WebOsCalendarSyncSource::getNextDeletedItem()
{
	LOG.info("=========== GET NEXT DELETED ITEM CALLED ==================");
	if(!mGetChangesCalled)
	{
		getChanges();
	}

	std::vector<WString> result = BlockingServiceCall("getDeletedEvent",TEXT(""));
	if(result.empty())
	{
		LOG.error("GetDeleted failed.");
	}
	else
	{
		if(result.front() != "finished")
		{
			SyncItem* item = new SyncItem();
			item->setDataType(TEXT("text/calendar"));
			//item->setData(e.second.c_str(),e.second.length()); no data for deleted items..
			item->setKey(result.front().c_str());
			LOG.debug("Added %s as deleted.",result.front().c_str());
			return item;
		}
	}

	return NULL;
}

//add/update/delete item from server. Returns SyncML Status code
//add needs to set the correct local key here.
int WebOsCalendarSyncSource::addItem(SyncItem &item)
{
	int ret = STC_COMMAND_FAILED;
	LOG.info("addItem called with type %s and data %s.",item.getDataType(),item.getData());

	LOG.info("Calling createEvent.");
	std::vector<WString> result = BlockingServiceCall("createEvent",(WCHAR*)item.getData());
	if(result.empty())
	{
		LOG.error("Add event %s did not work.",item.getData());
		ret = STC_COMMAND_FAILED;
	}
	else
	{
		LOG.debug("Add event was successful, received: %s.",result[0].c_str());
		item.setKey(result[0].c_str());
		ret = STC_OK;
	}

	if (isErrorCode(ret))
	{
		report->setLastErrorCode(ERR_ITEM_ERROR);
	    report->setLastErrorMsg(ERRMSG_ITEM_ERROR);
	    report->setState(SOURCE_ERROR);
	    LOG.debug("Error adding item: %s", item.getKey());
	}

	return ret;
}

int WebOsCalendarSyncSource::updateItem(SyncItem &item)
{
	int ret = STC_COMMAND_FAILED;
	LOG.info("updateItem called with %s data. And Type %s.",item.getKey(),item.getDataType());

	LOG.info("Calling updateEvent %s.",item.getKey());
	std::vector<WString> result = BlockingServiceCall("updateEvent",(WCHAR*)item.getData(),item.getKey());
	if(result.empty())
	{
		LOG.error("update event %s did not work.",item.getData());
		ret = STC_COMMAND_FAILED;
	}
	else
	{
		LOG.debug("update event was successful, received: %s.",result[0].c_str());
		ret = STC_OK;
	}

	if (isErrorCode(ret))
	{
		report->setLastErrorCode(ERR_ITEM_ERROR);
	    report->setLastErrorMsg(ERRMSG_ITEM_ERROR);
	    report->setState(SOURCE_ERROR);
	    LOG.debug("Error updating item: %s", item.getKey());
	}

	return ret;
}

int WebOsCalendarSyncSource::deleteItem(SyncItem &item)
{
	int ret = STC_COMMAND_FAILED;
	LOG.info("deleteItem called with %s data. And Type %s.",item.getKey(),item.getDataType());

	LOG.info("Calling deleteEvent.");
	std::vector<WString> result = BlockingServiceCall("deleteEvent",item.getKey());
	if(result.empty())
	{
		LOG.error("delete event %s did not work.",item.getKey());
		ret = STC_COMMAND_FAILED;
	}
	else
	{
		LOG.error("delete event was successful, received: %s.",result[0].c_str());
		ret = STC_OK;
	}

	if (isErrorCode(ret))
	{
		report->setLastErrorCode(ERR_ITEM_ERROR);
	    report->setLastErrorMsg(ERRMSG_ITEM_ERROR);
	    report->setState(SOURCE_ERROR);
	    LOG.debug("Error deleting item: %", item.getKey());
	}

	return ret;
}


