
#include "spds/SyncSource.h"

#ifndef WEBOSCONTACTSSYNCMLSOURCE
#define WEBOSCONTACTSSYNCMLSOURCE

USE_FUNAMBOL_NAMESPACE

class WebOsContactsSyncSource : public SyncSource
{
private:

public:
	//Optional methods:
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


	WebOsContactsSyncSource(const WCHAR *name,
			AbstractSyncSourceConfig *sc) :
				SyncSource(name,sc) {}
	virtual ~WebOsContactsSyncSource() {}

	//NEED to be implemented:
	//Removes all the item of the sync source.
	//It is called by the engine in the case of a refresh from server to clean all the client items before receiving the server ones. It is called after the beginSync() method.
	//Returns:
	//    0 if the remote succeded. (remove??)
	virtual int removeAllItems();
	//called to get first/next item during slow sync.
	virtual SyncItem* getFirstItem();
	virtual SyncItem* getNextItem();
	//get first/next new item
	virtual SyncItem* getFirstNewItem();
	virtual SyncItem* getNextNewItem();
	//get first/next updated item
	virtual SyncItem* getFirstUpdatedItem();
	virtual SyncItem* getNextUpdatedItem();
	//get first/next deleted item
	virtual SyncItem* getFirstDeletedItem();
	virtual SyncItem* getNextDeletedItem();
	//add/update/delete item from server. Returns SyncML Status code
	//add needs to set the correct local key here.
	virtual int addItem(SyncItem &item);
	virtual int updateItem(SyncItem &item);
	virtual int deleteItem(SyncItem &item);
};

#endif


