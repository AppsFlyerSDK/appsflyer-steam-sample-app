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

RequestData CAppsflyerSteamModule::CreateRequestData()
{
	AppsflyerModule afc(devkey, appID, collectSteamUid);
	RequestData req;

	//steam app build id
	int bid = SteamApps()->GetAppBuildId();
	std::string app_version = std::to_string(bid);

	//create timestamp
	std::time_t t = std::time(0);
	std::ostringstream oss;
	oss << t;
	std::string timestamp = oss.str();

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

	if (collectSteamUid) {
		//adding steam uid to the request
		DeviceIDs steam_id;
		steam_id.type = "steamid";
		steam_id.value = GetSteamUID().c_str();
		req.device_ids.insert(req.device_ids.end(), steam_id);
	}

	if (!cuid.empty()) {
		req.customer_user_id = cuid;
	}

	return req;
}

void CAppsflyerSteamModule::Init(const char* dkey, const char* appid, bool collectSteam) {
	devkey = dkey;
	appID = appid;
	collectSteamUid = collectSteam;
	isStopped = true;
}

std::string CAppsflyerSteamModule::GetSteamUID() {
	CSteamID usrID = SteamUser()->GetSteamID();
	const auto steamIDInt = SteamUser()->GetSteamID().ConvertToUint64();
	std::ostringstream os;
	os << steamIDInt;
	std::string steamID = os.str();
	return steamID;
}

void CAppsflyerSteamModule::Start(bool skipFirst) {
	isStopped = false;
	AppsflyerModule afc(devkey, appID, collectSteamUid);
	RequestData req = CreateRequestData();
	auto [res, rescode, context] = afc.af_firstOpen_init(req);
	AppsflyerSteamModule()->OnHTTPCallBack(res, rescode, context);
}

void CAppsflyerSteamModule::Stop()
{
	isStopped = true;
}

void CAppsflyerSteamModule::SetCustomerUserId(std::string customerUserID)
{
	if (!isStopped) {
		// Cannot set CustomerUserID while the SDK has started.
		return;
	}
	// Customer User ID has been set
	cuid = customerUserID;
}

void CAppsflyerSteamModule::LogEvent(std::string event_name, json event_parameters, json event_custom_parameters) {
	if (isStopped) {
		return;
	}
	AppsflyerModule afc(devkey, appID, collectSteamUid);

	RequestData req = CreateRequestData();

	req.event_name = event_name;
	req.event_parameters = event_parameters;
	req.event_custom_parameters = event_custom_parameters;

	auto [res, rescode, context] = afc.af_inappEvent(req);
	AppsflyerSteamModule()->OnHTTPCallBack(res, rescode, context);
}

void CAppsflyerSteamModule::OnHTTPCallBack(CURLcode res, long responseCode, uint64 context)
{
	if (res != CURLE_OK) {
		// response failed
		OnCallbackFailure(responseCode, context);
	}
	else {
		OnCallbackSuccess(responseCode, context);
		AppsflyerModule afc(devkey, appID, collectSteamUid);

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

void CAppsflyerSteamModule::OnCallbackSuccess(long responseCode, uint64 context) {
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

void CAppsflyerSteamModule::OnCallbackFailure(long responseCode, uint64 context) {
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

bool CAppsflyerSteamModule::IsInstallOlderThanDate(std::string datestring)
{
	AppsflyerModule afc(devkey, appID, collectSteamUid);
	return afc.isInstallOlderThanDate(datestring);
}

std::string CAppsflyerSteamModule::GetAppsFlyerUID()
{
    AppsflyerModule afc(devkey, appID, collectSteamUid);
	return afc.get_AF_id();
}

std::string CAppsflyerSteamModule::to_utf8(std::wstring& wide_string)
{
	static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
	return utf8_conv.to_bytes(wide_string);
}
