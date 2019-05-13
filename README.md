It uses a RFM95 by HopeRF and the MCCI LoRaWAN LMIC stack. This sample code is configured to connect to The Things Network using the US 915 MHz frequency by default, but can be changed to EU 868 MHz.

To start, install the dependencies, below. Then edit ```src/credentials.h``` to use either ```USE_ABP``` or ```USE_OTAA``` and the Keys/EUIs for your Application's Device from [The Things Network](https://www.thethingsnetwork.org/). Add the TTN Mapper integration to your Application (and optionally the Data Storage integration if you want to access the GPS location information yourself). Compile the code and upload it to your TTGO T-Beam. Turn on the device and once a GPS lock is acquired, the device will start sending data.

#### Arduino Library Dependencies

 - [mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic)
 - HX711
 - [ThingPulse/esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)

### The TTGO T-Beam development platform

