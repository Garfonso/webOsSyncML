function SyncSceneAssistant() {
	/* this is the creator function for your scene assistant object. It will be passed all the 
	   additional parameters (after the scene name) that were passed to pushScene. The reference
	   to the scene controller (this.controller) has not be established yet, so any initialization
	   that needs the scene controller should be done in the setup function below. */
	  
	this.LogMessage = "";
	this.LogElement = null;
}

SyncSceneAssistant.prototype.setup = function() {
	/* this function is for setup tasks that have to happen when the scene is first created */
		
	this.LogElement = this.controller.get("logOutput");
	
	eventCallbacks.eventsUpdatedElement = this.controller.get("eventsUpdated");
	eventCallbacks.eventsUpdateFailedElement = this.controller.get("eventsUpdateFailed");
	eventCallbacks.eventsAddedElement = this.controller.get("eventsAdded");
	eventCallbacks.eventsAddFailedElement = this.controller.get("eventsAddFailed");
	eventCallbacks.eventsDeletedElement = this.controller.get("eventsDeleted");
	eventCallbacks.eventsDeleteFailedElement = this.controller.get("eventsDeleteFailed");
	eventCallbacks.log = this.log.bind(this);
	eventCallbacks.continueWithContacts = this.continueWithContacts.bind(this);
	eventCallbacks.controller = this.controller;

	/* use Mojo.View.render to render view templates and add them to the scene, if needed */
	
	/* setup widgets here */
	this.controller.setupWidget("btnStart", { type : Mojo.Widget.activityButton }, { label: $L("Start sync")});
	
	/* add event handlers to listen to events from widgets */
	Mojo.Event.listen(this.controller.get("btnStart"),Mojo.Event.tap,this.startSync.bind(this));	
	
	try {
		cPlugin.setup(this.controller.get("webOsSyncMLPlugin"));
		cPlugin.thePluginObject.updateStatus = SyncSceneAssistant.prototype.log.bind(this);
		cPlugin.thePluginObject.finished = SyncSceneAssistant.prototype.finished.bind(this);
	}
	catch(e)
	{
		this.log("Error" + e + " - " + JSON.stringify(e));
	}
	
	this.checkAccount();
};

SyncSceneAssistant.prototype.checkAccount = function() 
{
	this.log("Check account");
	if(account.webOsAccountId !== undefined)
	{
		this.log("Have account Id: " + account.webOsAccountId);
		try {
			this.controller.serviceRequest('palm://com.palm.accounts/crud', {
				method: 'listAccounts',
				parameters: {},
				onSuccess: function(r){
					var i;
					var found = false;
					var accounts = r.list;
					for(i = 0; i < accounts.length; i++)
					{
						if(accounts[i].accountId === account.webOsAccountId)
						{
							found = true;
						}
					}
					
					if(found) 
					{
						this.log("Account is there.");
						eventCallbacks.checkCalendar();
					}
					else
					{
						this.log("Account not there, try to create it.");
						account.webOsAccountId = undefined;
						this.checkAccount();
					}
				}.bind(this),
				onFailur: function(error){ this.log("Something went very wrong: " + error + " - " + JSON.stringify(error));}.bind(this)
			});
		}
		catch(e)
		{
			this.log("Exception during get account..?? Try to recreate it.");
			account.webOsAccountId = undefined;
			eventCallbacks.checkAccount();
		}
	}
	else
	{	
		this.log("Need to create account.");
		var myDataTypes = [];
		if(account.syncCalendar)
		{
			myDataTypes.push("CALENDAR");
		}
		if(account.syncContacts)
		{
			myDataTypes.push("CONTACTS");
		}
		Mojo.Log.info(Mojo.appPath+"/icon32.png");
		
		this.controller.serviceRequest('palm://com.palm.accounts/crud', { 
			method: 'createAccount', 
			parameters: { 
				displayName: account.name,
				dataTypes: myDataTypes, 
				domain: account.name, 
				icons: {"32x32" : Mojo.appPath+"icon32.png", "48x48": Mojo.appPath+"icon48.png"}, 
				isDataReadOnly: false, 
				username: account.username 
			}, 
			onSuccess: function(accountId)
			{
				Mojo.Log.info("Created Account: " + Object.toJSON(accountId));
				account.webOsAccountId = accountId.accountId;
				saveConfig();
				eventCallbacks.checkCalendar();	
			}.bind(this), 
			onFailure: function(error)
			{
				Mojo.Controller.errorDialog("Could not create account. Can't sync. :(\n" + Object.toJSON(error));
			} 
		});  
	}
};

SyncSceneAssistant.prototype.continueWithContacts = function()
{
};

SyncSceneAssistant.prototype.startSync = function()
{
	try
	{
		var doCal = 0;
		var doCon = 0;
		if(account.syncCalendar)
		{
			doCal = 1;
		}
		if(account.syncContacts)
		{
			doCon = 1;
		}
		var result = cPlugin.thePluginObject.startSync(account.username,account.password,account.url,doCal,
		                                        doCon,account.syncCalendarPath,account.syncCalendarMethod,account.syncContactsPath,account.syncContactsMethod);

		this.controller.get("btnStart").mojo.activate();

		if ( result === null )
		{
			this.log("result is null");
		}
		else
		{
			this.log("result: " + result);
		}
	}
	catch(e)
	{
		this.log("exception: "+e);
	}
};

SyncSceneAssistant.prototype.finished = function(calOk,conOk)
{
	if(account.syncCalendar)
	{
		if(calOk === "ok")
		{
			this.log("Calendar sync worked.");
			eventCallbacks.finishSync(true);
		}
		else
		{
			this.log("Calendar sync had errors.");
			//account.syncCalendarMethod = "slow";
		}
	}
	if(account.syncContacts)
	{
		if(conOk === "ok")
		{
			this.log("Contacts sync worked.");
			//TODO: call doneWithChanges!
		
			if (account.syncContactsMethod === "slow" || account.syncContactsMethod.indexOf("refresh") !== -1) {
				account.syncContactsMethod = "two-way";
			}
		}
		else
		{
			this.log("Contacts sync had errors.");
			//account.syncContactsMethod = "slow";
		}
	}
	
	this.controller.get("btnStart").mojo.deactivate();
};

SyncSceneAssistant.prototype.log = function(message)
{
	this.LogMessage = "<p>" + message + "</p>" + this.LogMessage;
	this.LogElement.innerHTML = this.LogMessage;
	Mojo.Log.info(message);
};

SyncSceneAssistant.prototype.activate = function(event) {
	/* put in event handlers here that should only be in effect when this scene is active. For
	   example, key handlers that are observing the document */
};

SyncSceneAssistant.prototype.deactivate = function(event) {
	/* remove any event handlers you added in activate and do any other cleanup that should happen before
	   this scene is popped or another scene is pushed on top */
	saveConfig();
};

SyncSceneAssistant.prototype.cleanup = function(event) {
	/* this function should do any cleanup needed before the scene is destroyed as 
	   a result of being popped off the scene stack */
};
