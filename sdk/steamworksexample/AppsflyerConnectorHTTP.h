#pragma once
#include "steam/steam_api.h"
#include "RequestData.h"

class CAppsflyerConnectorHTTP {
public:
	//This method receives your api key and app id,
	//and initializes the AppsFlyer Connector (and sends “first open/session” request to AppsFlyer).
	void start(const char* devkey, const char* appID); 
	/*  These methods are called upon a un/successful steam http request callback. 
		Those are placeholders that you can fill with the desired actions upon success/failure 
		(within AppsflyerConnectorHTTP.cpp file) */
	void onCallbackSuccess(HTTPRequestCompleted_t* pCallback);
	void onCallbackFailure(HTTPRequestCompleted_t* pCallback);
	//This method receives an event name and json object and sends an in-app event to AppsFlyer.
	void logEvent(std::string event_name, json event_values);
private:
	const char* devkey;
	const char* appID;
	friend CAppsflyerConnectorHTTP* AppsflyerConnectorHTTP();
	CAppsflyerConnectorHTTP();
	CCallResult<CAppsflyerConnectorHTTP, HTTPRequestCompleted_t> m_SteamAPICallCompleted;
	void send_http_post(HTTPRequestHandle handle);
	void OnHTTPCallBack(HTTPRequestCompleted_t * pCallback, bool bIOFailure);
};

const uint64 FIRST_OPEN_REQUEST = 100;
const uint64 SESSION_REQUEST = 101;
const uint64 INAPP_EVENT_REQUEST = 102;

CAppsflyerConnectorHTTP * AppsflyerConnectorHTTP();