
#include "WebOsContactsSyncSource.h"

USE_FUNAMBOL_NAMESPACE

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
int WebOsContactsSyncSource::removeAllItems()
{
	return 0;
}

//called to get first/next item during slow sync.
SyncItem* WebOsContactsSyncSource::getFirstItem()
{
	//create SyncItems:
	//key: WCHAR*
	//modificationTime: long
	//dataSize: long
	//dataType: WCHAR*
	//state: SyncState
	//targetParent: WCHAR*
	//sourceParent: WCHAR*
	//moreData: bool <- only for very big items.
	//TODO: find out how to use that right for contact/calendar/task/notes entries!
	return NULL;
}

SyncItem* WebOsContactsSyncSource::getNextItem()
{
	return NULL;
}

//get first/next new item
SyncItem* WebOsContactsSyncSource::getFirstNewItem()
{
	return NULL;
}

SyncItem* WebOsContactsSyncSource::getNextNewItem()
{
	return NULL;
}

//get first/next updated item
SyncItem* WebOsContactsSyncSource::getFirstUpdatedItem()
{
	return NULL;
}

SyncItem* WebOsContactsSyncSource::getNextUpdatedItem()
{
	return NULL;
}

//get first/next deleted item
SyncItem* WebOsContactsSyncSource::getFirstDeletedItem()
{
	return NULL;
}

SyncItem* WebOsContactsSyncSource::getNextDeletedItem()
{
	return NULL;
}

//add/update/delete item from server. Returns SyncML Status code
//add needs to set the correct local key here.
int WebOsContactsSyncSource::addItem(SyncItem &item)
{
	LOG.info("addItem called with %s data. And Type %s.\n",item.getData(),item.getDataType());
	printf("addItem called with %s data. And Type %s.\n",item.getData(),item.getDataType());
	//gut = 201.
	return 500;
}

/*
 * Item: vcard
 */

int WebOsContactsSyncSource::updateItem(SyncItem &item)
{
	printf("updateItem called with %s data. And Type %s.\n",item.getData(),item.getDataType());
	return 500;
}

int WebOsContactsSyncSource::deleteItem(SyncItem &item)
{
	printf("deleteItem called with %s data. And Type %s.\n",item.getData(),item.getDataType());
	return 500;
}


