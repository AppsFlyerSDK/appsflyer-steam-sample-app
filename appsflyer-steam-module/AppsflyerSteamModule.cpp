#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "steam/steam_api.h"
#include "AppsflyerSteamModule.h"
#include "AppsflyerModule.cpp"

using namespace std;


CAppsflyerSteamModule* AppsflyerSteamModule()
{
	static CAppsflyerSteamModule inv;
	return &inv;
}

CAppsflyerSteamModule::CAppsflyerSteamModule() {
	SteamAPI_RunCallbacks();
}

void CAppsflyerSteamModule::init(const char* dkey, const char* appid) {
	devkey = dkey;
	appID = appid;
}

void CAppsflyerSteamModule::start(bool skipFirst) {
	AppsflyerModule afc(devkey, appID);
	CSteamID usrID = SteamUser()->GetSteamID();
	const auto steamIDInt = SteamUser()->GetSteamID().ConvertToUint64();
	std::ostringstream os;
	os << steamIDInt;
	std::string steamID = os.str();

	//steam app build id
	int bid = SteamApps()->GetAppBuildId();
	std::string app_version = std::to_string(bid);

	//create timestamp
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	std::string timestamp = oss.str();

	RequestData req;
	req.timestamp = timestamp;
	req.device_os_version = "1.0.0";
	req.app_version = app_version;
	req.device_model = afc.get_OS(); //TODO: check how to retreive device model - in the meantime send 'steam'
	req.limit_ad_tracking = "false";
	req.request_id = afc.uuid_gen().c_str();

	//adding AF id to the request
	DeviceIDs af_id;
	af_id.type = "custom";
	af_id.value = afc.get_AF_id().c_str();
	req.device_ids.insert(req.device_ids.end(), af_id);

	//adding steam uid to the request - TODO: add to request json after approved by the server
	DeviceIDs steam_id;
	steam_id.type = "steamid";
	steam_id.value = steamID.c_str();
	req.device_ids.insert(req.device_ids.end(), steam_id);
	auto [res, rescode, context] = afc.af_firstOpen_init(req);
	AppsflyerSteamModule()->onHTTPCallBack(res, rescode, context);
}

void CAppsflyerSteamModule::logEvent(std::string event_name, json event_parameters) {
	AppsflyerModule afc(devkey, appID);

	CSteamID usrID = SteamUser()->GetSteamID();
	const auto steamIDInt = SteamUser()->GetSteamID().ConvertToUint64();
	std::ostringstream os;
	os << steamIDInt;
	std::string steamID = os.str();

	//steam app build id
	int bid = SteamApps()->GetAppBuildId();
	std::string app_version = std::to_string(bid);

	//create timestamp
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	std::string timestamp = oss.str();

	RequestData req;
	req.timestamp = timestamp;
	req.device_os_version = "1.0.0";
	req.app_version = app_version;
	req.device_model = afc.get_OS(); //TODO: check how to retreive device model - in the meantime send 'steam'
	req.limit_ad_tracking = "false";
	req.request_id = afc.uuid_gen().c_str();

	//adding AF id to the request
	DeviceIDs af_id;
	af_id.type = "custom";
	af_id.value = afc.get_AF_id().c_str();
	req.device_ids.insert(req.device_ids.end(), af_id);

	//adding steam uid to the request - TODO: add to request json after approved by the server
	DeviceIDs steam_id;
	steam_id.type = "steamid";
	steam_id.value = steamID.c_str();
	req.device_ids.insert(req.device_ids.end(), steam_id);

	req.event_name = event_name;
	req.event_parameters = event_parameters;
	auto [res, rescode, context] = afc.af_inappEvent(req);
	AppsflyerSteamModule()->onHTTPCallBack(res, rescode, context);
}

void CAppsflyerSteamModule::onHTTPCallBack(CURLcode res, long responseCode, uint64 context)
{
	if (res != CURLE_OK) {
		// response failed
		onCallbackFailure(responseCode, context);
	}
	else {
		onCallbackSuccess(responseCode, context);
		AppsflyerModule afc(devkey, appID);

		switch (context)
		{
		case FIRST_OPEN_REQUEST:
		case SESSION_REQUEST:
			if (responseCode == k_EHTTPStatusCode202Accepted)
			{
				afc.increase_AF_counter();
			}
			break;
		case INAPP_EVENT_REQUEST:
			break;
		default:
			break;
		}
	}
}

void CAppsflyerSteamModule::onCallbackSuccess(long responseCode, uint64 context) {
	// Handle Success
	switch (context)
	{
	case FIRST_OPEN_REQUEST:
	case SESSION_REQUEST:
		// ** handle success for these callback **
		break;
	case INAPP_EVENT_REQUEST:
		// ** handle success for this callback **
		break;
	default:
		break;
	}
}

void CAppsflyerSteamModule::onCallbackFailure(long responseCode, uint64 context) {
	// Handle Failure
	switch (context)
	{
	case FIRST_OPEN_REQUEST:
	case SESSION_REQUEST:
		// ** handle failure for these callback **
		break;
	case INAPP_EVENT_REQUEST:
		// ** handle failure for this callback **
		break;
	default:
		break;
	}
}

bool CAppsflyerSteamModule::isInstallOlderThanDate(std::string datestring)
{
	AppsflyerModule afc(devkey, appID);
	return afc.isInstallOlderThanDate(datestring);
}