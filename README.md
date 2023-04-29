# Based On 
## The following Build: 
https://github.com/sparkfun/Rogue_Router

# install the esp8266 into your Arduino IDE
esp8266 core Version 2.7.3 worked for me. 
But it was very tedious to get this running... 
The software is working with some older dependencies wich arent working with the latest updates. 
In my case that meant to downgrade some of the dependencies. 

Put this into your Additional Boards Manager URL in case you need to install the esp 8266 core into your Arduino IDE. 
http://arduino.esp8266.com/stable/package_esp8266com_index.json

# Rogue_Router

Solar Access Point Fileserver for SparkFun Thing
Featured here: https://www.sparkfun.com/news/2009

*3D Files* - Models for 3D Printed Enclosure

*Firmware* - SDAPServer code for SparkFun Thing

*Img* - Pics of the enclosure

## Firmware Versions

### SDAPServer

...is a sloppy amalgam of SDWebServer and WiFiAccessPoint, orginially from Hristo Gochkov.


### SDAPServerCaptive

...is a sloppy amalgam of SDWebServer and WiFiAccessPoint with some DNSServer thrown in to add Captive Portal functionality. 

Captive Portal modifications are dependant on latest DNSServer library via: 
https://github.com/knovoselic/Arduino/tree/dns_server_improvements/hardware/esp8266com/esp8266/libraries/DNSServer
with patch by "blackie" from:
http://www.esp8266.com/viewtopic.php?f=32&t=3618&start=36#p21726


