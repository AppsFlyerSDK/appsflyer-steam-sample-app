#pragma once
#include "steam/steam_api.h"
#include "RequestData.h"

class CAppsflyerSteamModule {
public:
	//This method receives your api key and app id,
	// This method receives your api key and app id,
	// and initializes the AppsFlyer Connector 
	void init(const char* devkey, const char* appID);
	// sends “first open/session” request to AppsFlyer.
	void start(bool skipFirst = false);
	/*  These methods are called upon a un/successful steam http request callback. 
		Those are placeholders that you can fill with the desired actions upon success/failure 
		(within AppsflyerSteamModule.cpp file) */
	void onCallbackSuccess(HTTPRequestCompleted_t* pCallback);
	void onCallbackFailure(HTTPRequestCompleted_t* pCallback);
	//This method receives an event name and json object and sends an in-app event to AppsFlyer.
	void logEvent(std::string event_name, json event_values);
	// returns true whether the game was installed before the given date
	bool isInstallOlderThanDate(std::string datestring);
private:
	const char* devkey;
	const char* appID;
	friend CAppsflyerSteamModule* AppsflyerSteamModule();
	CAppsflyerSteamModule();
	CCallResult<CAppsflyerSteamModule, HTTPRequestCompleted_t> m_SteamAPICallCompleted;
	CCallResult<CAppsflyerSteamModule, HTTPRequestCompleted_t> m_SteamEventAPICallCompleted;
	void send_http_post(HTTPRequestHandle handle, uint64 context);
	void OnHTTPCallBack(HTTPRequestCompleted_t * pCallback, bool bIOFailure);
};

const uint64 FIRST_OPEN_REQUEST = 100;
const uint64 SESSION_REQUEST = 101;
const uint64 INAPP_EVENT_REQUEST = 102;

CAppsflyerSteamModule * AppsflyerSteamModule();