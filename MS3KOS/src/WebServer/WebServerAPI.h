#ifndef _WEBSERVERAPI_H
#define _WEBSERVERAPI_H

#include "Base64.h"

void handleGETInfo(void)
{
	msSystem.slogln("handleGETInfo");

	String response = "{";
	response += "\"api\":{";
	response += "\"versions\":[1],";
	response += "\"prefix\":\"v\"";
	response += "},";
	response += "\"init\": {";
	response += "\"settings\":[";
	response += "\"server\",";
	response += "\"ap\",";
	response += "\"wifi/list\",";
	response += "\"wifi/preferred\"";
	response += "],";
	response += "\"info\":[";
	response += "\"about\",";
	response += "\"status\"";
	response += "]";
	response += "}";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}


void handleGETAbout(void)
{
	msSystem.slogln("handleGETAbout");

	String response = "{";
	response += "\"type\":\"MagicShifter3000\",";
	response += "\"format\":\"BGRA\",";
	response += "\"version\":" + String(MS3KOS_VERSION) + ",";
	response += "\"leds\":" + String(MAX_LEDS) + ",";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response += "\"flashid\":" + String(ESP.getFlashChipId()) + ",";
	response += "\"flashsize\":" + String(ESP.getFlashChipSize());
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETStatus(void)
{
	msSystem.slogln("handleGETStatus");

	int adValue = msGlobals.ggLastADValue;
	float voltage = msSystem.getBatteryVoltage();

	uint32_t ip = (uint32_t) msSystem.getIP();

	String response = "{";
	response += "\"id\":" + String(ESP.getChipId()) + ",";
	response +=
		"\"uptime\":" + String(millis() - msGlobals.ggBootTime) + ",";
	response += "\"adValue\":" + String(adValue) + ",";
	response += "\"voltage\":" + String(voltage) + ",";
	response += "\"ggAccelTime\":" + String(msGlobals.ggAccelTime) + ",";
	response += "\"ggLFrameTime\":" + String(msGlobals.ggLFrameTime) + ",";
	response +=
		"\"ggLastFrameMicros\":" + String(msGlobals.ggLastFrameMicros) +
		",";
	response +=
		"\"ggCurrentMicros\":" + String(msGlobals.ggCurrentMicros) + ",";
	response += "\"accelRaw\":[";
	response += String(msGlobals.ggAccelCount[XAXIS]) + ",";
	response += String(msGlobals.ggAccelCount[YAXIS]) + ",";
	response += String(msGlobals.ggAccelCount[ZAXIS]);
	response += "],";
	response += "\"msGlobals.ggAccel\":[";
	response += String(msGlobals.ggAccel[XAXIS]) + ",";
	response += String(msGlobals.ggAccel[YAXIS]) + ",";
	response += String(msGlobals.ggAccel[ZAXIS]);
	response += "],";
	response += "\"ip\":\"";
	response += String(0xFF & (ip >> 0)) + ".";
	response += String(0xFF & (ip >> 8)) + ".";
	response += String(0xFF & (ip >> 16)) + ".";
	response += String(0xFF & (ip >> 24)) + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

bool parseAPInfoFromServerArgs(APAuth & apInfo)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "ssid") == 0) {
			l_safeStrncpy(apInfo.ssid, msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.ssid));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "pwd")
				   == 0) {
			l_safeStrncpy(apInfo.password,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(apInfo.password));
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}

bool parseSysLogInfoFromServerArgs(ServerConfig & sysLogHost)
{
	bool success = true;
	for (int i = 0; i < msSystem.msESPServer.args(); i++) {
		msSystem.slogln("argName: ");
		msSystem.slogln(msSystem.msESPServer.argName(i));

		msSystem.slogln("arg: ");
		msSystem.slogln(msSystem.msESPServer.arg(i));

		if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") == 0) {
			l_safeStrncpy(sysLogHost.hostname,
						  msSystem.msESPServer.arg(i).c_str(),
						  sizeof(sysLogHost.hostname));
		} else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
				   == 0) {
			sysLogHost.port = atoi(msSystem.msESPServer.arg(i).c_str());
		} else {
			msSystem.slogln("arg is not known!");
			success = false;
		}
	}
	return success;
}


void handleGetSettings(void)
{
	msSystem.slogln("handleGetSettings");

	ServerConfig config;
	msSystem.Settings.getServerConfig(&config);

	String response = "<!DOCTYPE html><html><head></head><body>";;
	response += "<h3>Urls</h3>";
	response += "<ul>";
	response += "<li><a href=\"/settings\">this page (/settings)</a></li>";
	response +=
		"<li><a href=\"/settings/server\">server settings (/settings/server)</a></li>";
	response +=
		"<li><a href=\"/settings/ap\">accesspoint settings (/settings/ap)</a></li>";

	response += "<li><a href=\"/info\">the schema page (/info)</a></li>";

	response +=
		"<li><a href=\"/info/about\">about info page (/info/about)</a></li>";
	response +=
		"<li><a href=\"/info/status\">status output page (/info/status)</a></li>";

	response +=
		"<li><a href=\"/settings/wifi/list\">list of saved wifis (/settings/wifi/list)</a></li>";
	response +=
		"<li><a href=\"/settings/wifi/preferred\">preferred wifi (/settings/wifi/preferred)</a></li>";

	response += "</ul>";
	response += "</body></html>";
	msSystem.msESPServer.send(200, "text/html", response);
}

void handleGETServerSettings(void)
{
	msSystem.slogln("handleGETServerSettings");

	ServerConfig config;
	msSystem.Settings.getServerConfig(&config);

	String response = "{";
	response += "\"host\":";
	response += "\"";
	response += config.hostname;
	response += "\"";
	response += ",";
	response += "\"port\":";
	response += config.port;
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTServerSettings(void)
{
	msSystem.slogln("handlePOSTServerSettings");
	if (msSystem.msESPServer.args() >= 2) {
		ServerConfig config;
		msSystem.Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			if (strcmp(msSystem.msESPServer.argName(i).c_str(), "host") ==
				0) {
				l_safeStrncpy(config.hostname,
							  msSystem.msESPServer.arg(i).c_str(),
							  sizeof(config.hostname));
			} else
				if (strcmp(msSystem.msESPServer.argName(i).c_str(), "port")
					== 0) {
				config.port = atoi(msSystem.msESPServer.arg(i).c_str());
			} else {
				success = false;
			}
		}

		if (success) {
			msSystem.Settings.setServerConfig(&config);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void handleGETAPSettings(void)
{
	msSystem.slogln("handleGETAPSettings");

	APAuth apInfo;
	msSystem.Settings.getAPConfig(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPSettings(void)
{
	msSystem.slogln("handlePOSTAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		APAuth apInfo;
		msSystem.Settings.getAPConfig(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setAPConfig");
			msSystem.Settings.setAPConfig(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}


void handleGETSysLogHostSettings(void)
{
	msSystem.slogln("handleGETSysLogHostSettings");

	ServerConfig sysLogInfo;
	msSystem.Settings.getSyslogConfig(&sysLogInfo);

	String response = "{";
	response += "\"syslog_host\":";
	response += "\"";
	response += sysLogInfo.hostname;
	response += "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTSysLogSettings(void)
{
	msSystem.slogln("handlePOSTSysLogSettings");

	if (msSystem.msESPServer.args() >= 2) {
		// load old settings
		ServerConfig sysLogInfo;
		msSystem.Settings.getSyslogConfig(&sysLogInfo);

		if (parseSysLogInfoFromServerArgs(sysLogInfo)) {
			msSystem.slogln("saving setSysLogConfig");
			msSystem.Settings.setSyslogConfig(&sysLogInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}



void handleGETPreferredAPSettings(void)
{
	msSystem.slogln("handleGETPreferredAPSettings");

	APAuth apInfo;
	msSystem.Settings.getPreferredAP(&apInfo);

	String response = "{";
	response += "\"ssid\":";
	response += "\"";
	response += apInfo.ssid;
	response += "\"";
	//response += ",";
	//response += "\"pwd\":";
	//response += "\"" + apInfo.pwd + "\"";
	response += "}";
	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTPreferredAPSettings(void)
{
	msSystem.slogln("handlePOSTPreferredAPSettings");

	if (msSystem.msESPServer.args() >= 2) {
		APAuth apInfo;
		msSystem.Settings.getPreferredAP(&apInfo);

		if (parseAPInfoFromServerArgs(apInfo)) {
			msSystem.slogln("saving setAPConfig");
			msSystem.Settings.setPreferredAP(&apInfo);
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "unknown args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}


///////////////////////////////////////
//////////////////////////////////////////

void handleGETAPList(void)
{
	msSystem.slogln("handleGETAPList");

	APAuth apInfo;
	msSystem.Settings.getPreferredAP(&apInfo);

	String response = "[";

	msSystem.Settings.resetAPList();
	bool firstAP = true;
	while (msSystem.Settings.getNextAP(&apInfo)) {
		if (!firstAP) {
			response += ",";
		}
		firstAP = false;

		response += "{\"ssid\":";
		response += "\"";
		response += apInfo.ssid;
		response += "\"";
		//response += ",";
		//response += "\"pwd\":";
		//response += "\"" + apInfo.pwd + "\"";
		response += "}";
	}
	response += "]";
	msSystem.Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETWLANList(void)
{
	msSystem.slogln("handleGETWLANList");

	String response = "[";

	int n = WiFi.scanNetworks();
	msSystem.slogln("scan done");
	if (n == 0) {
		msSystem.slogln("\"no networks found\"");
	} else {
		//msSystem.slogln(n);
		msSystem.slogln(" networks found");
		bool firstAP = true;
		for (int i = 0; i < n; ++i) {
			if (!firstAP) {
				response += ",";
			}
			firstAP = false;

			response += "{\"ssid\":";
			response += "\"";
			response += WiFi.SSID(i);
			response += "\"";
			response += ",";
			response += "\"rssi\":";
			response += WiFi.RSSI(i);
			response += ",";
			response += "\"free\":";
			response +=
				(WiFi.encryptionType(i) ==
				 ENC_TYPE_NONE) ? "true" : "false";
			response += "}";
		}
	}


	response += "]";
	msSystem.Settings.resetAPList();

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handlePOSTAPListAdd(void)
{
	msSystem.slogln("handlePOSTAPListAdd");

	if (msSystem.msESPServer.args() >= 2) {
		APAuth apInfo;
		memset(apInfo.ssid, 0, sizeof(apInfo.ssid));
		memset(apInfo.password, 0, sizeof(apInfo.password));

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("adding wifi");
				msSystem.Settings.addAP(&apInfo);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}

void handlePOSTAPListDelete(void)
{
	msSystem.slogln("handlePOSTAPListDelete");

	if (msSystem.msESPServer.args() >= 1) {
		APAuth apInfo;
		strcpy(apInfo.ssid, "");
		strcpy(apInfo.password, "");

		if (parseAPInfoFromServerArgs(apInfo)) {
			if (!strcmp(apInfo.ssid, "") == 0) {
				msSystem.slogln("deleting wifi");
				msSystem.Settings.deleteAP(apInfo.ssid);
			}
			msSystem.msESPServer.send(200, "text/plain", "OK");
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "args missing!");
	}
}


void handleSetMode(void)
{
	msSystem.slogln("handleSetMode");
	if (msSystem.msESPServer.args() == 1) {
		ServerConfig config;
		msSystem.Settings.getServerConfig(&config);

		bool success = true;
		for (int i = 0; i < msSystem.msESPServer.args(); i++) {
			msSystem.slogln("argName: ");
			msSystem.slogln(msSystem.msESPServer.argName(i));

			msSystem.slogln("arg: ");
			msSystem.slogln(msSystem.msESPServer.arg(i));

			int newMode = 
				atoi(msSystem.msESPServer.arg(i).c_str());
			
			msSystem.setMode(newMode);

		}

		if (success) {
			String response = "{";
			response += "\"mode\":";
			response += "\"";
			response += msGlobals.ggCurrentMode;
			response += "\"";
			response += "}";
			msSystem.msESPServer.send(200, "text/plain", response);
		} else {
			msSystem.msESPServer.send(500, "text/plain", "invalid args!");
		}
	} else {
		msSystem.msESPServer.send(500, "text/plain", "argument missing!");
	}
}

void respondREQTime()
{
	int currentTime =
		msGlobals.ggTime + (millis() - msGlobals.ggTimePostedAt);

	String response = "{";
	response += "\"time\":" + String(currentTime) + ",";
	response += "\"postedAt\":" + String(msGlobals.ggTimePostedAt);
	response += "}";

	msSystem.msESPServer.send(200, "text/plain", response);
}

void handleGETTime()
{
	msSystem.slogln("handleGETTime");

	respondREQTime();
}

void handlePOSTTime()
{
	msSystem.slogln("handlePOSTTime");

	if (msSystem.msESPServer.argName(0) == "t") {
		msGlobals.ggTime = atoi(msSystem.msESPServer.arg(0).c_str());
		msGlobals.ggTimePostedAt = millis();
	}

	respondREQTime();
}

void handleLedSet()
{
	byte ledData[MAX_LEDS * 5];

	String message = "LedSet\n\n";

	if (msSystem.msESPServer.args() == 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		unsigned int inputLen = (int) msSystem.msESPServer.arg(0).length();
		msSystem.slogln("inputLen: ");
		msSystem.slogln(String(inputLen));

		if (inputLen > sizeof(ledData))
			inputLen = sizeof(ledData);

		msSystem.slogln(String(inputLen));

		unsigned int dataLen =
			base64_decode((char *) ledData, input, inputLen);

		for (int i = 0; i < dataLen; i += 5) {
			byte idx = ledData[i];
			msSystem.slogln("idx: ");
			msSystem.slogln(String((int) idx));
			if (idx >= MAX_LEDS)
				continue;
			msSystem.slogln("data+1: ");
			msSystem.slogln(String((int) ledData[i + 1]));

			msGlobals.ggRGBLEDBuf[idx * 4] = ledData[i + 1];
			msGlobals.ggRGBLEDBuf[idx * 4 + 1] = ledData[i + 2];
			msGlobals.ggRGBLEDBuf[idx * 4 + 2] = ledData[i + 3];
			msGlobals.ggRGBLEDBuf[idx * 4 + 3] = ledData[i + 4];
		}
	} else {
		message += "argument missing or too many arguments!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


void handleLedsSet()
{
	msSystem.slogln("handleLedsSet");

	String message = "LedsSet\n\n";

	if (msSystem.msESPServer.args() >= 1) {
		const char *input = msSystem.msESPServer.arg(0).c_str();
		int inputLen = BASE64_ENC_LEN(RGB_BUFFER_SIZE);

		base64_decode((char *) msGlobals.ggRGBLEDBuf, input, inputLen);

		message += "done";
	} else {
		message += "argument missing!";
	}
	msSystem.msESPServer.send(200, "text/plain", message);
}


/*
struct UIConfig
{
  int timeoutHighPower;
  int timeoutLowPower;
  int defaultBrightness;
};
*/

void handleGetUISettings(void)
{
  msSystem.slogln("handleGetUISettings");

  UIConfig config;
  msSystem.Settings.getUIConfig(&config);

  String response = "{";
    response += "\"timeoutHighPower\":";
    response += config.timeoutHighPower;
    response += ",";
    response += "\"timeoutLowPower\":";
    response += config.timeoutLowPower;
    response += ",";
    response += "\"defaultBrightness\":";
    response += config.defaultBrightness;
  response += "}";
  msSystem.msESPServer.send(200, "text/plain", response);
}

void handleSetUISettings(void)
{
  msSystem.slogln("handleSetUISettings");
  if (msSystem.msESPServer.args() >= 1)
  {
    UIConfig config;
    msSystem.Settings.getUIConfig(&config);

    bool success = true;
    for (int i = 0; i < msSystem.msESPServer.args(); i++)
    {
      msSystem.slogln("argName: ");
      msSystem.slogln(msSystem.msESPServer.argName(i));

      msSystem.slogln("arg: ");
      msSystem.slogln(msSystem.msESPServer.arg(i));

      if (strcmp(msSystem.msESPServer.argName(i).c_str(), "timeoutHighPower") == 0)
      {
        config.timeoutHighPower = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "timeoutLowPower") == 0)
      {
        config.timeoutLowPower = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else if (strcmp(msSystem.msESPServer.argName(i).c_str(), "defaultBrightness") == 0)
      {
        config.defaultBrightness = atoi(msSystem.msESPServer.arg(i).c_str());
      }
      else
      {
        success = false;
      }
    }

    if (success)
    {
      msSystem.Settings.setUIConfig(&config);
      msSystem.msESPServer.send (200, "text/plain", "OK");
    }
    else
    {
      msSystem.msESPServer.send(500, "text/plain", "invalid args!");
    }
  }
  else
  {
    msSystem.msESPServer.send ( 500, "text/plain", "argument(s) missing!");
  }
}

#endif
