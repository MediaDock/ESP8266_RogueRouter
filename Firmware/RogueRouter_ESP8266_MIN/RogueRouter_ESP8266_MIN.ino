/*
                #####
              ##########
             ###     ###
             ###     ###
             ###     ###
             ###     ###
             ###     ###
             ###     ##########
             ###     ##################
             ###     ###     ##############
             ###     ###     ####     #######
             ###     ###     ####     ###   ###
  ######     ###     ###     ####     ###   ######
###########  ###     ###     ####     ###       ###
####     #######                      ###       ###
#######      ###                                 ###
  #####                                          ###
  #######                                        ###
     ####                                        ###
      ######                                     ###
         ###                                  #####
         #######                             ####
            ####                             ###
             #######                     #######
                ####                     ####
                #############################
                #############################
                Based on: Sparkfun ESP8266 Thing Dev
                Based on the following examples: 
                https://github.com/sparkfun/Rogue_Router    
                This is a reduced Version of the SDAPServer of the Rogue Router Example on Github.
                Its still not perfect but it should work for our Project.          
*/

/*
  SDAPServer - A Sloppy Amalgam of SDWebServer and WiFiAccessPoint

  Copyright (c) 2015 Hristo Gochkov. All rights reserved.
  Modified 2016 Nick Poole.
  This file is part of the ESP8266WebServer library for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Have a FAT Formatted SD Card connected to the SPI port of the ESP8266
  The web root is the SD Card root folder
  File extensions with more than 3 charecters are not supported by the SD Library
  File Names longer than 8 charecters will be truncated by the SD library, so keep filenames shorter
  index.htm is the default index (works on subfolders as well)

  upload the contents of SdRoot to the root of the SDcard and access the editor by going to http://esp8266sd.local/edit

*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <SPI.h>
#include <SD.h>

const char WiFiAPPSK[] = "";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

static bool hasSD = false;


void returnOK() {
  Serial.println("returnOK");
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  Serial.println("returnFail");
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path) {
  Serial.println("loadFromSdCard entered");
  String dataType = "text/html";
  if (path.endsWith("/")) path += "index.htm";

  File dataFile = SD.open("/index.htm");

  if(dataFile.isDirectory()){
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open("/index.htm");
  }
   Serial.println(dataType);

  if (!dataFile)
    return false;

  if (server.hasArg("download")) dataType = "application/octet-stream";

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }

  dataFile.close();
  return true;
}


void handleNotFound() {
  Serial.println("handleNotFound entered");  
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("Rougerouter Setup Started");
  setupWiFi();

  server.begin();
  server.on("/edit", HTTP_POST, []() {
    returnOK();
  });
  server.onNotFound([]() {
    loadFromSdCard("");
  });

  server.begin();

  if (SD.begin(SS)) {

    hasSD = true;
    Serial.print("hasSD:");
    Serial.println(hasSD);
  }
}

void setupWiFi() {
  WiFi.mode(WIFI_AP);
  Serial.println("Rougerouter Setup Wifi Started");
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) + String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
  macID.toUpperCase();
  String AP_NameString = "AP_Test" + macID;

  char AP_NameChar[AP_NameString.length() + 1];
  memset(AP_NameChar, 0, AP_NameString.length() + 1);

  for (int i = 0; i < AP_NameString.length(); i++)
    AP_NameChar[i] = AP_NameString.charAt(i);

  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(AP_NameChar, WiFiAPPSK);

  dnsServer.start(DNS_PORT, "*", apIP);
  Serial.print("DNS_PORT:");
  Serial.println(DNS_PORT);
  Serial.print("AccessPointIP:");
  Serial.println(apIP);
}

void loop(void) {
  dnsServer.processNextRequest();
  server.handleClient();
}
