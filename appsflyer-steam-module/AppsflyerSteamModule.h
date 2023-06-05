#pragma once
#include "steam/steam_api.h"
#include "RequestData.h"
#include <curl/curl.h>
#include <iostream>
#include <string>

class CAppsflyerSteamModule {
public:
	//This method receives your api key and app id,
	// This method receives your api key and app id,
	// and initializes the AppsFlyer Connector 
	void init(const char* devkey, const char* appID);
	// sends �first open/session� request to AppsFlyer.
	void start(bool skipFirst = false);
	/*  These methods are called upon a un/successful steam http request callback.
	Those are placeholders that you can fill with the desired actions upon success/failure
	(within AppsflyerSteamModule.cpp file) */
	void onCallbackSuccess(long responseCode, uint64 context);
	void onCallbackFailure(long responseCode, uint64 context);
	// This method receives an event name and json object and sends an in-app event to AppsFlyer.
	void logEvent(std::string event_name, json event_parameters);
	// get AppsFlyer's unique device ID. 
	std::string getAppsFlyerUID();
	// returns true whether the game was installed before the given date
	bool isInstallOlderThanDate(std::string datestring);
private:
	const char* devkey;
	const char* appID;
	friend CAppsflyerSteamModule* AppsflyerSteamModule();
	CAppsflyerSteamModule();
	void onHTTPCallBack(CURLcode res, long responseCode, uint64 context);
};

const uint64 FIRST_OPEN_REQUEST = 100;
const uint64 SESSION_REQUEST = 101;
const uint64 INAPP_EVENT_REQUEST = 102;

CAppsflyerSteamModule * AppsflyerSteamModule();