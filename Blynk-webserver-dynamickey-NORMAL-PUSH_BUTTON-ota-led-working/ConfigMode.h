/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>





ESP8266WebServer server(WIFI_AP_CONFIG_PORT);
ESP8266HTTPUpdateServer httpUpdater;
DNSServer dnsServer;
const byte DNS_PORT = 53;

bool first_time=0;
String ap_mode="0";

String ap_ssid="A-LINK Server";
String ap_pass="12345678";
String switch_type="";

const char* config_form = R"html(
<!DOCTYPE HTML>
<html>
<head>
  <title>A-LINK SETUP</title>
  <style>
  @import url(
  https://fonts.googleapis.com/css?family=Roboto:400,
  300,
  600,
  400italic
);
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
  -webkit-box-sizing: border-box;
  -moz-box-sizing: border-box;
  -webkit-font-smoothing: antialiased;
  -moz-font-smoothing: antialiased;
  -o-font-smoothing: antialiased;
  font-smoothing: antialiased;
  text-rendering: optimizeLegibility;
}

body {
  font-family: "Roboto", Helvetica, Arial, sans-serif;
  font-weight: 100;
  font-size: 12px;
  line-height: 30px;
  color: #777;
  background: #4caf50;
}

.container {
  max-width: 400px;
  width: 100%;
  margin: 0 auto;
  position: relative;
}

#contact input[type="text"],
#contact input[type="password"],
#contact input[type="number"],
#contact button[type="submit"] {
  font: 400 12px/16px "Roboto", Helvetica, Arial, sans-serif;
}

#contact {
  background: #f9f9f9;
  padding: 25px;
  margin: 25px 0;
  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);
}

#contact h3 {
  display: block;
  font-size: 30px;
  font-weight: 500;
  margin-bottom: 10px;
  text-align: center;
  text-decoration: underline;
}

#contact h4 {
  margin: 25px 0 15px;
  display: block;
  font-size: 20px;
  font-weight: 400;
}

fieldset {
  border: medium none !important;
  margin: 0 0 10px;
  min-width: 100%;
  padding: 0;
  width: 100%;
}

#contact input[type="text"],
#contact input[type="password"],
#contact input[type="text"],
#contact input[type="number"] {
  width: 100%;
  border: 1px solid #ccc;
  background: #fff;
  margin: 0 0 5px;
  padding: 10px;
}

#contact input[type="text"]:hover,
#contact input[type="password"]:hover,
#contact input[type="number"]:hover:hover {
  -webkit-transition: border-color 0.3s ease-in-out;
  -moz-transition: border-color 0.3s ease-in-out;
  transition: border-color 0.3s ease-in-out;
  border: 1px solid #aaa;
}

#contact button[type="submit"] {
  cursor: pointer;
  width: 100%;
  border: none;
  background: #4caf50;
  color: #fff;
  margin: 0 0 5px;
  padding: 10px;
  font-size: 15px;
}

#contact button[type="submit"]:hover {
  background: #43a047;
  -webkit-transition: background 0.3s ease-in-out;
  -moz-transition: background 0.3s ease-in-out;
  transition: background-color 0.3s ease-in-out;
}

#contact button[type="submit"]:active {
  box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.5);
}

.copyright {
  text-align: center;
}

#contact input:focus,
#contact textarea:focus {
  outline: 0;
  border: 1px solid #aaa;
}

::-webkit-input-placeholder {
  color: #888;
}

:-moz-placeholder {
  color: #888;
}

::-moz-placeholder {
  color: #888;
}

:-ms-input-placeholder {
  color: #888;
}

#reset {
  cursor: pointer;
  width: 50%;
  border: 2px solid #43a047;
  background: #959595;
  color: #fff;
  margin: 0 0 5px;
  padding: 10px;
  font-size: 15px;
  display: inline;
  position: float;
  float: left;
}

#reset:hover {
  background: #43a047;
  -webkit-transition: background 0.3s ease-in-out;
  -moz-transition: background 0.3s ease-in-out;
  transition: background-color 0.3s ease-in-out;
}

#restart {
  cursor: pointer;
  width: 50%;
  border: 2px solid #43a047;
  background: #959595;
  color: #fff;
  margin: 0 0 5px;
  padding: 10px;
  font-size: 15px;
  display: inline;
  position: float;
  float: left;
}
#restart:hover {
  background: #43a047;
  -webkit-transition: background 0.3s ease-in-out;
  -moz-transition: background 0.3s ease-in-out;
  transition: background-color 0.3s ease-in-out;
}

#contact button[type="submit"]:active {
  box-shadow: inset 0 1px 3px rgba(0, 0, 0, 0.5);
}

#freset {
  background: #f9f9f9;
  padding: 10px;
  margin: 5px 0;
  min-height: 60px;
  box-shadow: 0 0 20px 0 rgba(0, 0, 0, 0.2), 0 5px 5px 0 rgba(0, 0, 0, 0.24);
}

h5 {
  margin-top: -15px;
}
h6 {
  margin-top: -18px;
  font-size: 12px;
}

</style>
<script>
function reset() {
  var r = confirm(
    "Do You want to Reset the device??!!\nAll Data will be deleted."
  );
  if (r == true) {
    document.getElementById("rst").submit();
  } else {
    alert("Cancel!!!");
  }
}
function reboot() {
  var r = confirm("Do You want to Reboot the device?");
  if (r == true) {
    document.getElementById("rbt").submit();
  } else {
    alert("Cancel!!!");
  }
}
</script>
</head> 
<body>
  <div class="container">
    <form id="contact" method="get" action="config">
      <h3>A-LINK SETUP</h3>
      <h4>Wifi Credientials</h4>
      <fieldset>
        <input name="ssid" placeholder="Wifi UserName // SSID" type="text" maxlength=64 tabindex="1" required autofocus>
      </fieldset>
      <fieldset>
        <input name="pass" placeholder="Wifi Password" type="password" maxlength=64 tabindex="2" required>
      </fieldset>

      <h4>A-link Server Details</h4>

      <fieldset>
        <input name="alink" placeholder="Auth Key" type="text" maxlength="32" tabindex="3" required>
      </fieldset>

      <fieldset>
        <input name="host" placeholder="HOST Name // Server Address(Optional)" type="text" tabindex="4" length=64>
      </fieldset>
      <fieldset>
        <input name="port" placeholder="Port Number(Optional)" type="Number" tabindex="5">
      </fieldset>
      <h4>AP(Hotspot) Details</h4>
      <h5> <input type='checkbox' name='checkbox' > ON / OFF </h5>
      <fieldset>
        <input name="ap_ssid" placeholder="Wifi UserName(Default Username:A-link Server)" type="text" maxlength=64 tabindex="6">
      </fieldset>
      <fieldset>
        <input name="ap_pass" placeholder="Wifi Password (Default Password:12345678)" type="text" maxlength=64 tabindex="7">
      </fieldset>
       <fieldset>
        <h4>Switch Type</h4>
        <h6> <input type='radio' name='switch' tabindex="9" value="normal"> NORMAL SWITCH <input type='radio' name='switch' tabindex="10" value="press" Checked> PRESS SWITCH <input type='radio' name='switch' tabindex="11" value="touch"> TOUCH SWITCH</h6>
      </fieldset>
      <fieldset>
        <button type="submit" id="contact-submit">Save</button>
      </fieldset>

      <p class="copyright">Designed by A-link India &copy;</p>
    </form>

    <div id="freset">
      <button id="reset" onclick="reset()">Reset</button>
      <button id="restart" onclick="reboot()">Reboot</button>
    </div>
  </div>

  <form id="rst" method="get" action="reset">
  </form>
  <form id="rbt" method="get" action="reboot"></form>
</body>
</html>
)html";

void restartMCU() {
  ESP.restart();
}

void enterConfigMode()
{
  randomSeed(ESP.getChipId());
  const uint32_t unique = random(0xFFFFF);
  char ssidBuff[64];
  snprintf(ssidBuff, sizeof(ssidBuff), "%s-%05X", PRODUCT_WIFI_SSID, unique);

  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  WiFi.softAP(ssidBuff);
  delay(500);
  IPAddress myIP = WiFi.softAPIP();
  DEBUG_PRINT(String("AP SSID: ") + ssidBuff);
  DEBUG_PRINT(String("AP IP:   ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);

  if (myIP == (uint32_t)0)
  {
    BlynkState::set(MODE_ERROR);
    return;
  }

  // Set up DNS Server
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
#ifdef WIFI_CAPTIVE_PORTAL_ENABLE
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // Point all to our IP
  server.onNotFound(handleRoot);
#else
  dnsServer.start(DNS_PORT, BOARD_CONFIG_AP_URL, WIFI_AP_IP);
  DEBUG_PRINT(String("AP URL:  ") + BOARD_CONFIG_AP_URL);
#endif

  httpUpdater.setup(&server);

  server.on("/cc", []() {
		 server.send(200, "text/html", config_form);
  });


  server.on("/config", []() {
    String ssid = server.arg("ssid");
    String ssidManual = server.arg("ssidManual");
    String pass = server.arg("pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = server.arg("alink");
    String host  = server.arg("host");
	String port  = server.arg("port");
		 ap_ssid = server.arg("ap_ssid");
		 ap_pass = server.arg("ap_pass");
	switch_type  = server.arg("switch");
	String apmode= server.arg("checkbox");
    if(apmode=="on" || apmode=="ON")
	{
		ap_mode=1;
	}
	
    if (host.length()==0)
	{
		host="blynkservergcp.ddns.net";
	}
	
	if (port.length()==0)
	{
		port="8080";
	}
	if (ap_ssid.length()==0)
	{
		ap_ssid="A-link Server";
	}
	if (ap_pass.length()==0)
	{
		ap_pass="12345678";
	}
 
    String content;
    unsigned statusCode;

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("a-link cloud: ") + token + " @ " + host + ":" + port);
	if(ap_mode=="1")
	{
		DEBUG_PRINT(String("AP MODE IS ENABLED"));
		DEBUG_PRINT(String("AP Username: ") + ap_ssid + " pass: " + ap_pass);
	}else
	{	
		DEBUG_PRINT(String("AP MODE IS DISABLED"));
	}
	DEBUG_PRINT(String("Switch Type : ") + switch_type);
  
    if (token.length() == 32 && ssid.length() > 0) {
      configStore.flagConfig = false;
      CopyString(ssid, configStore.wifiSSID);
      CopyString(pass, configStore.wifiPass);
      CopyString(token, configStore.cloudToken);
      if (host.length()) {
        CopyString(host,  configStore.cloudHost);
      }
      if (port.length()) {
        configStore.cloudPort = port.toInt();
      }
	  CopyString(ap_ssid,configStore.apssid);
		CopyString(ap_pass,configStore.appass);
    CopyString(ap_mode,configStore.amode);
    CopyString(switch_type,configStore.switchtype);
   
      content = R"json({"status":"ok","msg":"Configuration saved"})json";
      statusCode = 200;
      server.send(statusCode, "application/json", content);

      BlynkState::set(MODE_SWITCH_TO_STA);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
      statusCode = 404;
      server.send(statusCode, "application/json", content);
    }
  });
  server.on("/board_info.json", []() {
    char buff[256];
    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","vendor":"%s","tmpl_id":"%s","fw_ver":"%s","hw_ver":"%s"})json",
      BOARD_NAME,
      BOARD_VENDOR,
      BOARD_TEMPLATE_ID,
      BOARD_FIRMWARE_VERSION,
      BOARD_HARDWARE_VERSION
    );
    server.send(200, "application/json", buff);
  });
  server.on("/reset", []() {
    BlynkState::set(MODE_RESET_CONFIG);
    server.send(200, "application/json", R"json({"status":"ok","msg":"Configuration reset"})json");
  });
  server.on("/reboot", []() {
    restartMCU();
  });

  server.begin();

  while (BlynkState::is(MODE_WAIT_CONFIG) || BlynkState::is(MODE_CONFIGURING)) {
    dnsServer.processNextRequest();
    server.handleClient();
    if (BlynkState::is(MODE_WAIT_CONFIG) && WiFi.softAPgetStationNum() > 0) {
      BlynkState::set(MODE_CONFIGURING);
    } else if (BlynkState::is(MODE_CONFIGURING) && WiFi.softAPgetStationNum() == 0) {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  server.stop();
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);

  if(ap_mode==0)
  {
	WiFi.mode(WIFI_STA);
  }else{
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
	WiFi.softAP(ap_ssid, ap_pass);
  }	  
   
   
  if (!WiFi.begin(configStore.wifiSSID, configStore.wifiPass)) {
    return;
  }

  unsigned long timeoutMs = millis() + WIFI_NET_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) && (WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    if (!BlynkState::is(MODE_CONNECTING_NET)) {
      WiFi.disconnect();
      return;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterConnectCloud() {
  BlynkState::set(MODE_CONNECTING_CLOUD);

  Blynk.disconnect();
  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  Blynk.connect(0);

  unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) &&
        (Blynk.connected() == false))
  {
    Blynk.run();
    if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
      Blynk.disconnect();
      return;
    }
  }
  
  if (Blynk.connected()) {
    BlynkState::set(MODE_RUNNING);

    if (!configStore.flagConfig) {
      configStore.flagConfig = true;
      config_save();
      DEBUG_PRINT("Configuration stored to flash");
    }
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterSwitchToSTA() {
  BlynkState::set(MODE_SWITCH_TO_STA);

  DEBUG_PRINT("Switching to STA...");

  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  WiFi.softAP("A-link Server", "password");
  //below single line added by sudhir
WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  BlynkState::set(MODE_CONNECTING_NET);
}

void enterError() {
  BlynkState::set(MODE_ERROR);
  
  unsigned long timeoutMs = millis() + 10000;
  while (timeoutMs > millis() || g_buttonPressed)
  {
    delay(10);
    if (!BlynkState::is(MODE_ERROR)) {
      return;
    }
  }
  DEBUG_PRINT("Restarting after error.");
  delay(10);

  restartMCU();
}

void alink_run()
{
	
	Blynk.run();
	first_time=1;
}
