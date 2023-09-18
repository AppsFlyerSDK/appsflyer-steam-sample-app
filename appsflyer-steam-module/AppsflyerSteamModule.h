#pragma once
#include "steam/steam_api.h"
#include "RequestData.h"
#include <curl/curl.h>


class CAppsflyerSteamModule {
public:
	//This method receives your api key and app id,
	// This method receives your api key and app id,
	// and initializes the AppsFlyer Connector 
	void Init(const char* devkey, const char* appID, bool collectSteamUid = true);
	// sends “first open/session” request to AppsFlyer.
	void Start(bool skipFirst = false);
	// stop the AppsFlyer SDK
	void Stop();
	void SetCustomerUserId(std::string cuid);
	/*  These methods are called upon a un/successful steam http request callback.
	Those are placeholders that you can fill with the desired actions upon success/failure
	(within AppsflyerSteamModule.cpp file) */
	void OnCallbackSuccess(long responseCode, uint64 context);
	void OnCallbackFailure(long responseCode, uint64 context);
	// This method receives an event name and json object and sends an in-app event to AppsFlyer.
	void LogEvent(std::string event_name, json event_parameters);
	// returns true whether the game was installed before the given date
	bool IsInstallOlderThanDate(std::string datestring);
	std::string GetAppsFlyerUID();
private:
	const char* devkey;
	const char* appID;
	bool isStopped;
	bool collectSteamUid;
	std::string cuid;
	std::string GetSteamUID();
	friend CAppsflyerSteamModule* AppsflyerSteamModule();
	CAppsflyerSteamModule();
	RequestData CreateRequestData();
	void OnHTTPCallBack(CURLcode res, long responseCode, uint64 context);
};

const uint64 FIRST_OPEN_REQUEST = 100;
const uint64 SESSION_REQUEST = 101;
const uint64 INAPP_EVENT_REQUEST = 102;

CAppsflyerSteamModule * AppsflyerSteamModule();