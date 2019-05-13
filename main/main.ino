/*

Main module

# Modified by Kyle T. Gabriel to fix issue with incorrect GPS data for TTNMapper

Copyright (C) 2018 by Xose Pérez <xose dot perez at gmail dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "configuration.h"
#include <rom/rtc.h>
#include <WiFi.h>
#include <soc/rtc.h>

uint8_t txBuffer[4];

const uint8_t vbatPin = 35; //35
float VBAT; // battery voltage from ESP32 ADC read
float poids; 

// Message counter, stored in RTC memory, survives deep sleep
RTC_DATA_ATTR uint32_t count = 0;

// -----------------------------------------------------------------------------
// Application
// -----------------------------------------------------------------------------

void send() {
    char buffer[40];

     //Add some extra info like battery, temperature....
    // Battery Voltage
    VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
    Serial.println("Voltage: " + String(VBAT));            
    snprintf(buffer, sizeof(buffer), "Battery: %10.3f\n", VBAT);
    screen_print(buffer);     
    //Poids   
    poids = get_weigth();    
    snprintf(buffer, sizeof(buffer), "Poids en gr: %10.3f\n", poids);
    screen_print(buffer);
    //Charge Lora
    txBuffer[0] = highByte((int)round(VBAT*100));
    txBuffer[1] = lowByte((int)round(VBAT*100)); 
    txBuffer[2] = highByte((int)round(poids*100));
    txBuffer[3] = lowByte((int)round(poids*100));

    #if LORAWAN_CONFIRMED_EVERY > 0
        bool confirmed = (count % LORAWAN_CONFIRMED_EVERY == 0);
    #else
        bool confirmed = false;
    #endif

    ttn_cnt(count);
    ttn_send(txBuffer, sizeof(txBuffer), LORAWAN_PORT, confirmed);
    
    count++;
}

void sleep() {
    #if SLEEP_BETWEEN_MESSAGES

        // Show the going to sleep message on the screen
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "Sleeping in %3.1fs\n", (MESSAGE_TO_SLEEP_DELAY / 1000.0));
        screen_print(buffer);

        // Wait for MESSAGE_TO_SLEEP_DELAY millis to sleep
        delay(MESSAGE_TO_SLEEP_DELAY);

        // Turn off screen
        screen_off();

        // Set the user button to wake the board
        sleep_interrupt(BUTTON_PIN, LOW);

        // We sleep for the interval between messages minus the current millis
        // this way we distribute the messages evenly every SEND_INTERVAL millis
        uint32_t sleep_for = (millis() < SEND_INTERVAL) ? SEND_INTERVAL - millis() : SEND_INTERVAL;
        sleep_millis(sleep_for);

        //Set Frequency to 80M again, as it is not kept during sleep  
        rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    

    #endif
}

void callback(uint8_t message) {
    if (EV_JOINING == message) screen_print("Joining TTN...\n");
    if (EV_JOINED == message) screen_print("TTN joined!\n");
    if (EV_JOIN_FAILED == message) screen_print("TTN join failed\n");
    if (EV_REJOIN_FAILED == message) screen_print("TTN rejoin failed\n");
    if (EV_RESET == message) screen_print("Reset TTN connection\n");
    if (EV_LINK_DEAD == message) screen_print("TTN link dead\n");
    if (EV_ACK == message) screen_print("ACK received\n");
    if (EV_PENDING == message) screen_print("Message discarded\n");
    if (EV_QUEUED == message) screen_print("Message queued\n");

    if (EV_TXCOMPLETE == message) {
        screen_print("Message sent\n");
        sleep();
    }

    if (EV_RESPONSE == message) {

        screen_print("[TTN] Response: ");

        size_t len = ttn_response_len();
        uint8_t data[len];
        ttn_response(data, len);

        char buffer[6];
        for (uint8_t i=0; i<len; i++) {
            snprintf(buffer, sizeof(buffer), "%02X", data[i]);
            screen_print(buffer);
        }
        screen_print("\n");
    }
}

uint32_t get_count() {
    return count;
}

void setup() {
    // Debug
    int freq1,freq2;
    #ifdef DEBUG_PORT
        DEBUG_PORT.begin(SERIAL_BAUD);
    #endif
    freq1 = rtc_clk_cpu_freq_get();
    rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
    freq2 = rtc_clk_cpu_freq_get();
    Serial.println(freq1);
    Serial.println(freq2);
    // Buttons & LED
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_PIN, OUTPUT);

    // Hello
    DEBUG_MSG(APP_NAME " " APP_VERSION "\n");

    // Display
    screen_setup();

    //Disable radiozz`
    WiFi.mode(WIFI_OFF); 
    btStop();

    // Show logo on first boot
    if (0 == count) {
        screen_print(APP_NAME " " APP_VERSION, 0, 0);
        screen_show_logo();
        screen_update();
        delay(LOGO_DELAY);
    }

    // TTN setup
    DEBUG_MSG("TTN Setup" "\n");
    if (!ttn_setup()) {
        screen_print("[ERR] Radio module not found!\n");
        DEBUG_MSG("Radio Module not found\n");
        delay(MESSAGE_TO_SLEEP_DELAY);
        screen_off();
        sleep_forever();
    }

    ttn_register(callback);
    DEBUG_MSG("TTN Join\n");
    ttn_join();
    ttn_sf(LORAWAN_SF);
    ttn_adr(LORAWAN_ADR);
    DEBUG_MSG("TTN Finished\n");
}

void loop() {   
    ttn_loop();
    screen_loop();

    // Send every SEND_INTERVAL millis
    static uint32_t last = 0;
    static bool first = true;
    if (0 == last || millis() - last > SEND_INTERVAL) {
        last = millis();
        first = false;
        send();
        
        }
    
}
