# AppsFlyer Steam Connector

## **Getting started with AppsFlyer Steam Connector**

AppsFlyer empowers marketers and helps them make better decisions.

This is done by providing marketers with powerful tools that solve real pain points. These include cross-platform attribution, mobile and web analytics, deep linking, fraud detection, privacy management and preservation, and much more.

By integrating the connector with AppsFlyer, you get [attribution](https://support.appsflyer.com/hc/en-us/articles/207447053) out-of-the-box. 

With this sample app, we will be able to demonstrate basic integration which includes the first open/sessions and in-app events (i.e purchase events).

<hr/>

## **Connector Set-up:**



1. Add the files from the "appsflyer-connector" folder into your C++ project under Header Files -> AppsFlyer
2. Add [Steamworks SDK](https://partner.steamgames.com/doc/sdk) to your project
3. Use VCPKG to install the dependencies:
* vcpkg install nlohmann-json:x86-windows
* vcpkg install openssl:x86-windows

<hr/>

## **AppsflyerConnectorHTTP - Interface**

First, Import the connector: 
<pre><code>
#include "AppsflyerConnectorHTTP.h"
</code></pre>

#### void **start**(const char* **devkey**, const char* **appID**)

This method receives your api key and app id, and initializes the AppsFlyer Connector (and sends “first open/session” request to AppsFlyer).

##### <span style="text-decoration:underline;">Usage:</span>

<pre><code>
AppsflyerConnectorHTTP()->start("DEV_KEY", "STEAM_APP_ID");
</code></pre>

##### App-Details

* DEV_KEY - retrieve the Dev key from the marketer or the [AppsFlyer HQ](https://support.appsflyer.com/hc/en-us/articles/211719806-App-settings-#general-app-settings).
* STEAM_APP_ID - you may find your app id on the [SteamDB](https://steamdb.info/apps/).

<br/>

#### void **onCallbackSuccess**(HTTPRequestCompleted_t* **pCallback**) 
#### void **onCallbackFailure**(HTTPRequestCompleted_t* **pCallback**)

These methods are placeholders for the desired actions upon success/failure. \
It is possible to handle different types of events with the switch case of the context within each function. (“FIRST_OPEN_REQUEST”, ”SESSION_REQUEST”, ”INAPP_EVENT_REQUEST”)

<br/>

#### void **logEvent**(std::string **event_name**, json **event_values**)

This method receives an event name and json object and sends an in-app event to AppsFlyer.


##### <span style="text-decoration:underline;">Usage:</span>

<pre><code>
json event_values = { {"af_currency", "USD"}, {"af_price", 6.66}, {"af_revenue", 24.12} };
std::string event_name = "af_purchase";
AppsflyerConnectorHTTP()->logEvent(event_name, event_values);
</code></pre>

##### *In order to use json please make sure to use the following imports:

<pre><code>
#include &lt;nlohmann/json.hpp>
using json = nlohmann::json;
</code></pre>


<hr>

## Running the Sample App 

1. Install [Visual Studio ](https://visualstudio.microsoft.com/)
2. Open the solution "../appsflyer-steam-sample-app/sdk/steamworksexample"
3. Open Main.cpp file
4. On line 244, replace DEV_KEY and STEAM_APP_ID with your [app details](#App-Details)
5. Run the app by pressing on the play button on the top toolbar ("Local Windows Debugger"). Make sure the the mode is Debug and Win32.
![Visual Studio Toolbar Image](images/vs-run.PNG?raw=true "Visual Studio Toolbar Image")
6. After 24 hours, the dashboard will update and show organic/non-organic install and in-app events.
