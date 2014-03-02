var eventCallbacks = function(){
	//will return public interface:
	return {	
	    //variables:
		changedEvents: {
			received: false,
			callDeletedItems: false,
			changed: [],
			changedIds: [],
			delted: [],
			deletedIds: [],
			changesToken: ""
		},

		eventUpdated: 0,
		eventUpdateFailed: 0,
		eventAdded: 0,
		eventAddFailed: 0,
		eventDeleted: 0,
		eventDeleteFailed: 0,
		
		createEvent: function(event){
			setTimeout(this.createEventReal.bind(this,event),100);
		},
		
		/**
		 * Creates event, called from c++ part. Parameter is the iCal item data string.
		 * @param {Object} event
		 */
		createEventReal: function(event){
			try {
				var e = iCal.parseICalToEvent(event);
				this.controller.serviceRequest('palm://com.palm.calendar/crud', {
					method: 'createEvent',
					parameters: {
						calendarId: account.webOsCalendarId,
						trackChange: false, //because we are currently syncing..
						event: e
					},
					onSuccess: function(r){
						this.eventAdded++;
						this.eventsAddedElement.innerHTML = this.eventAdded;
						e.eventId = r.eventId;
						cPlugin.sendSingle(r.eventId, false);
					}.bind(this),
					onFailure: function(error){
						this.eventAddFailed++;
						this.eventsAddFailedElement.innerHTML = this.eventAddFailed;
						try {
							this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + "). at - " + event + "  :(");
							this.log("rrule: " + e.rrule);
							this.log("rruleTZ: " + e.rruleTZ);
						} 
						catch (exception) {
						}
						cPlugin.forceReceive();
					}.bind(this)
				});
			}
			catch(exception)
			{
				this.log("Exception in createEvent: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		
		updateEvent: function(event, eventId){
			setTimeout(this.updateEventReal.bind(this,event,eventId),100);
		},
		
		
		/**
		 * Updates an event. Parameters are the iCal item data string and the eventId.
		 * Called from c++.
		 * @param {Object} event
		 * @param {Object} eventId
		 */
		updateEventReal: function(event, eventId){
			try {
				var e = iCal.parseICalToEvent(event);
				e.eventId = eventId;
				e.trackChange = false; //defaults to false anyway, but I wanted to be sure.
				this.controller.serviceRequest('palm://com.palm.calendar/crud', {
					method: 'updateEvent',
					parameters: e, //this is a bit strange, but the doc on the HP seems a bit faulty here. Hm.
					onSuccess: function(r){
						this.eventUpdated++;
						this.eventsUpdatedElement.innerHTML = this.eventUpdated;
						cPlugin.sendSingle(r.returnValue, false);
					}.bind(this),
					onFailure: function(error){
						this.eventUpdateFailed++;
						this.eventsUpdateFailedElement.innerHTML = this.eventUpdateFailed;
						this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + "). :(" + event + JSON.stringify(e));
						cPlugin.forceReceive();
					}.bind(this)
				});
			}
			catch(exception)
			{
				this.log("Exception in UpdateEvent: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		/**
		 * Deteles event with eventId. Called from c++.
		 * @param {Object} eventid
		 */
		deleteEvent: function(eventid){
			try
			{
                this.controller.serviceRequest('palm://com.palm.calendar/crud', {
                    method: 'deleteEvent',
                    parameters: {
                        calendarId: account.webOsCalendarId,
                        trackChange: false, //because we are currently syncing..
                        eventId: eventid
                    },
                    onSuccess: function(r){
                        this.eventDeleted++;
                        this.eventsDeletedElement.innerHTML = this.eventDeleted;
                        cPlugin.sendSingle(r.returnValue, false);
                    }.bind(this),
                    onFailure: function(error){
                        this.eventDeleteFailed++;
                        this.eventsDeleteFailedElement.innerHTML = this.eventDeleteFailed;
                        this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + " for eventId: " + eventid + "). :(");
                        cPlugin.forceReceive();
                    }.bind(this)
                });
			}
			catch(exception)
			{
				this.log("Exception in DeleteEvent: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		/**
		 * Lists events. Returns 10 events at a time. The itemOffset
		 * can be used to get the next 10 events. Be careful: if events are deleted,
		 * they must be substracted from offset, i.e. if all events are deleted, this is
		 * called with offset 0 every time.
		 * @param {Object} itemOffset
		 */
		listEvents: function(itemOffset){
			try {
				this.log("List events from " + itemOffset);
				this.controller.serviceRequest('palm://com.palm.calendar/crud', {
					method: 'listEvents',
					parameters: {
						calendarId: account.webOsCalendarId,
						limit: 10,
						offset: itemOffset
					},
					onSuccess: function(r){
						this.log("Received " + r.events.length + " events.");
						cPlugin.sendLoop(r.events, true);
					}.bind(this),
					onFailure: function(error){
						this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + "). :(");
						cPlugin.receiveResultLoop();
					}.bind(this)
				});
			}
			catch(exception)
			{
				this.log("Exception in ListEvents: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		/**
		 * This is a shortcut to delete all events. This "simply" deletes
		 * the calendar, which in turn lets webOs delete all events. This is
		 * much faster than deleting all events manually.
		 * Creates a new calendar afterwards.
		 * 
		 * TODO: change this to delete all events and not the whole calendar.
		 *       Deleting the whole calendar will remove the calendar from being the 
		 *       default one, if user set it this way.. we don't want that. :(
		 */
		deleteAllEvents: function(){
			try {
				this.log("Deleting all events: ");
                var delAllDelete = function(eventid){
                    this.controller.serviceRequest('palm://com.palm.calendar/crud', {
                        method: 'deleteEvent',
                        parameters: {
                            calendarId: account.webOsCalendarId,
                            trackChange: false, //because we are currently syncing..
                            eventId: eventid
                        },
                        onSuccess: function(r){
                            this.eventDeleted++;
                            this.eventsDeletedElement.innerHTML = this.eventDeleted;
                        }.bind(this),
                        onFailure: function(error){
                            this.eventDeleteFailed++;
                            this.eventsDeleteFailedElement.innerHTML = this.eventDeleteFailed;
                            this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + " for eventId: " + eventid + "). :(");
                        }.bind(this)
                    });
                }.bind(this);
                var delAllGetEvents = function(){
                    this.controller.serviceRequest('palm://com.palm.calendar/crud', {
                        method: 'listEvents',
                        parameters: {
                            calendarId: account.webOsCalendarId,
                            limit: 10,
                            offset: 0
                        },
                        onSuccess: function(r){
                            if (r.events.length == 0) {
								cPlugin.forceReceive("ok");
                                return;
                            }
                            this.log("Received " + r.events.length + " events.");
                            for (var i = 0; i < r.events.length; i++) {
								if(r.events[i].subject=='ALLDAYTEST')
								{
									this.log(JSON.stringify(r.events[i]));
									this.log(new Date(r.events[i].startTimestamp));
									this.log(new Date(r.events[i].endTimestamp));
								}
								
                                delAllDelete(r.events[i].eventId);
                            }
                            delAllGetEvents();
                        }.bind(this),
                        onFailure: function(error){
                            this.log("Callback not successfull: " + error.errorText + "(" + error.errorCode + "). :(");
                        }.bind(this)
                    });
                }.bind(this);
				
				delAllGetEvents();
			}
			catch(exception)
			{
				this.log("Exception in deleteAllEvents: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		getDeletedEvent: function(){
			try {
				if (!this.changedEvents.received) {
					cPlugin.forceReceive();
				}
				else {
					if (this.changedEvents.deletedIds.length > 0) {
						cPlugin.forceReceive(this.changedEvents.deletedIds[0].id);
						this.changedEvents.deletedIds.splice(0, 1);
					}
					else {
						cPlugin.forceReceive("finished");
					}
				}
			}
			catch(exception)
			{
				this.log("Exception in getDeletedEvent: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		getEventChanges: function(){
			try {
				if (this.changedEvents.received) {
					cPlugin.forceReceive();
					return;
				}
				else {
					this.log("Get changes.");
					this.controller.serviceRequest('palm://com.palm.calendar/crud', {
						method: 'getChanges',
						parameters: {
							calendarId: account.webOsCalendarId,
							accountId: account.webOsAccountId
						},
						onSuccess: function(r){
							this.log("Got changes: " + JSON.stringify(r));
							this.changedEvents.changesToken = r.token; //this needs to be passed to "doneWithChanges".
							if (r.events.length === 0) {
								cPlugin.forceReceive("0"); //nothing changed or whatsover? Wakeup c++.
							}
							else {
								//have only one calendar, so take 0 allways:
								this.changedEvents.changedIds = r.events[0].changed;
								this.changedEvents.changed = [];
								if (this.changedEvents.changedIds.length === 0) {
									cPlugin.forceReceive("0");
								}
								else {
									this.getEvent(0, this.changedEvents.changedIds, this.changedEvents.changed);
								}
								this.changedEvents.deletedIds = r.events[0].deleted;
								this.changedEvents.deleted = [];
								this.changedEvents.received = true;
							}
						}.bind(this),
						onFailure: function(r){
							this.log("Get changes failed: " + r + " - " + JSON.stringify(r));
							//account.webOsCalendarId = undefined; 
							cPlugin.forceReceive();
						}.bind(this)
					});
				}
			}
			catch(exception)
			{
				this.log("Exception in getEventChanges: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		getEvent: function(index, ids, array){
			try {
				this.controller.serviceRequest('palm://com.palm.calendar/crud', {
					method: 'getEvent',
					parameters: {
						eventId: ids[index].id,
						accountId: account.webOsAccountId,
						tzFormat: "id"
					},
					onSuccess: function(r){
						try {
							array.push(r);
							if (index + 1 < ids.length) {
								this.getEvent(index + 1, ids, array);
							}
							else {
								cPlugin.sendLoop(array, true);
								ids = [];
								array = [];
							}
						} 
						catch (e) {
							this.log("Something went wrong in success of getEvent: " + e + " - " + JSON.stringify(e));
						}
					}.bind(this),
					onFailure: function(r){
						this.log("Get event failed: " + r + " - " + JSON.stringify(r));
						cPlugin.receiveResultLoop();
					}.bind(this)
				});
			}
			catch(exception)
			{
				this.log("Exception in getEvent: " + exception + " - " + JSON.stringify(exception));
				cPlugin.forceReceive();
			}
		},
		
		checkCalendar: function(){
			this.log("Check calendar.");
			if (account.syncCalendar) {
				if (account.webOsCalendarId !== undefined) {
					this.log("Have Calendar Id: " + account.webOsCalendarId);
					this.controller.serviceRequest('palm://com.palm.calendar/crud', {
						method: 'getCalendar',
						parameters: {
							calendarId: account.webOsCalendarId
						},
						onSuccess: function(){
							this.log("Calendar exists.");
							this.continueWithContacts();
						}.bind(this),
						onFailure: function(){
							this.log("Calenader needs to be created.");
							account.webOsCalendarId = undefined;
							this.checkCalendar();
						}.bind(this)
					});
				}
				else {
					this.log("Need to create calendar account.");
					this.controller.serviceRequest('palm://com.palm.calendar/crud', {
						method: 'createCalendar',
						parameters: {
							accountId: account.webOsAccountId,
							calendar: {
								name: account.name,
								externalId: account.webOsExternalId,
								trackChange: true
							}
						},
						onSuccess: function(calendarId){
							account.webOsCalendarId = calendarId.calendarId;
							saveConfig();
							this.startTrackingChanges();
							this.continueWithContacts();
							//wake up c++, in case this was called from c++. Should not matter otherwise, I hope.
							cPlugin.forceReceive("ok " + account.webOsCalendarId);
						}.bind(this),
						onFailure: function(error){
							Mojo.Controller.errorDialog("Could not create calendar account. Can't sync. :(" + Object.toJSON(error));
						}.bind(this)
					});
				}
			}
			else {
				this.continueWithContacts();
			}
		},
		
		startTrackingChanges: function(){
			this.log("Try to start tracking!");
			this.controller.serviceRequest('palm://com.palm.calendar/crud', {
				method: 'startTracking',
				parameters: {
					accountId: account.webOsAccountId
				},
				onSuccess: function(r){
					Mojo.Log.error("startTrackingChanges was successfull! :)");
					this.log("Start tracking changes was successfull.");
				}.bind(this),
				onFailure: function(r){
					Mojo.Log.error("CAN'T TRACK CHANGES" + JSON.stringify(r));
					this.log("CAN'T TRACK CHANGES " + JSON.stringify(r));
				}.bind(this)
			});
		},
		
		finishSync: function(successful){
			try {
				if (successful) {
					if (account.syncCalendarMethod === "slow" || account.syncCalendarMethod.indexOf("refresh") !== -1) {
						account.syncCalendarMethod = "two-way";
						
						//reset changes:
						this.log("Was slow sync, call getChanges first.");
						this.controller.serviceRequest('palm://com.palm.calendar/crud', {
							method: 'getChanges',
							parameters: {
								calendarId: account.webOsCalendarId,
								accountId: account.webOsAccountId
							},
							onSuccess: function(r){
								this.log("Got changes: " + JSON.stringify(r));
								this.controller.serviceRequest('palm://com.palm.calendar/crud', {
									method: 'doneWithChanges',
									parameters: {
										token: r.token,
										accountId: account.webOsAccountId
									},
									onSuccess: function(r){
										this.log("Have reseted changes..." + JSON.stringify(r));
										Mojo.Log.info("Could reset changes. " + JSON.stringify(r));
									}.bind(this),
									onFailur: function(r){
										this.log("Have NOT reseted changes " + JSON.stringify(r));
										Mojo.Log.info("Could not reset changes. " + JSON.stringify(r));
									}.bind(this)
								});
							}.bind(this),
							onFailur: function(e){
								this.log("Could NOT get changes " + JSON.stringify(e));
								Mojo.Log.error("Could not getChanges: " + e + " - " + JSON.stringify(e));
							}.bind(this)
						});
					}
					else {
						this.log("Was fast sync, will call doneWithChanges");
						this.controller.serviceRequest('palm://com.palm.calendar/crud', {
							method: 'doneWithChanges',
							parameters: {
								token: this.changedEvents.changesToken,
								accountId: account.webOsAccountId
							},
							onSuccess: function(r){
								this.log("DoneWithChanges success: " + JSON.stringify(r));
								Mojo.Log.info("Could reset changes. " + JSON.stringify(r));
							}.bind(this),
							onFailur: function(r){
								this.log("DoneWithChanges fail: " + JSON.stringify(r));
								Mojo.Log.info("Could not reset changes. " + JSON.stringify(r));
							}.bind(this)
						});
					}
				}
				
				if (this.changedEvents.received) {
					this.changedEvents.changed = [];
					this.changedEvents.changedIds = [];
					this.changedEvents.delted = [];
					this.changedEvents.deletedIds = [];
					this.changedEvents.received = false;
					this.changedEvents.changesToken = "";
				}
				
				this.eventUpdated = 0;
				this.eventUpdateFailed = 0;
				this.eventAdded = 0;
				this.eventAddFailed = 0;
				this.eventDeleted = 0;
				this.eventDeleteFailed = 0;
			}
			catch(exception)
			{
				this.log("Exception in finishSync: " + exception + " - " + JSON.stringify(exception));
			}
		}

	}; //end of public interface
}(); //selfinvoke function

