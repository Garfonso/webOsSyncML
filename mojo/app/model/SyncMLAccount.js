function SyncMLAccount(){
    this.username = undefined;
    this.password = undefined;
    this.url = undefined;
    this.name = undefined;
    
    this.syncContacts = false;
    this.syncContactsPath = undefined;
    this.syncContactsMethod = "slow";
    this.syncCalendar = false;
    this.syncCalendarPath = undefined;
    this.syncCalendarMethod = "slow";
    
    this.webOsAccountId = undefined;
    this.webOsCalendarId = undefined;
    this.webOsContactsId = undefined;
    
    this.version = 1;
}

var account = new SyncMLAccount();

function saveConfig(){
    var cookie1 = new Mojo.Model.Cookie("defaultAccount");
    cookie1.put(account);
}

function readFromConfig(){
    //TODO: maybe change that to allow more accounts?
    var cookie1 = new Mojo.Model.Cookie("defaultAccount");
    var acc = cookie1.get();
    if (acc !== undefined) {
        if (acc.version === account.version) {
            account = acc;
        }
        else {
            Mojo.Controller.errorDialog("Versions don't match " + acc.version + " != " + account.version);
        }
    }
}