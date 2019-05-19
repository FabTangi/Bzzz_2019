Ce projet est très largement basé sur [ttgo-tbeam-ttn-tracker](https://github.com/kizniche/ttgo-tbeam-ttn-tracker)

Le but de ce projet est d'émettre vers [The Things Network](http://thethingsnetwork.org/) la tension de la batterie et le poids mesuré grâce à un module HX711

Pour commencer, installer les dépendences ci-dessous, puis  editer ```src/credentials.h``` pour utiliser ```USE_ABP``` ou ```USE_OTAA``` et les clés et identifiants de votre Application's Device 
crées sur [The Things Network](https://www.thethingsnetwork.org/). 

On peututiliser l'intégration [AllThingsTalkMaker](https://maker.allthingstalk.com) pour grapher et conserver les données

Compiler le code  et pousser le sur votre [TTGO T-Beam](https://wiki.fablab-lannion.org/index.php?title=WemosTTGO).

Allumer le teminal et l'écran devrait vous donner des informations...

#### Dépendances Bibliothèques Arduino

 - [mcci-catena/arduino-lmic](https://github.com/mcci-catena/arduino-lmic) version 2.3.2
 - [HX711](https://github.com/bogde/HX711) version 0.7.2.0
 - [ThingPulse/esp8266-oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306) version 4.0.0

### A Faire

 - Sauver les paramètres OTAA en cas de deep sleep. Semble  fait ici : https://github.com/dermatthias/Lora-TTNMapper-T-Beam

 -Sauver le nombre de trames en ABP et l'utiliser après deep sleep

 -Valider la consommation / autonomie
