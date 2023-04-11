#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"

#include "machine.h"
#include "webserver.h"
    

const char* ssid     = "SENSOR1";
const char* password = "123456789";

Storage storage;
Machine machine(&storage);
MyWebServer server(&machine, 80);

void setup(){

    Serial.begin(115200);
    machine.begin();
    storage.begin();
    machine.red_led_on();

    Serial.println("setup ....");
    Serial.println("# Setting AP (Access Point)…");
    WiFi.softAP(ssid, password,1,0,1,false); //max 1 connection
    IPAddress IP = {192, 168, 11, 1};
    IPAddress gateway = {192, 168, 11, 1};
    IPAddress NMask = {255, 255, 255, 0};
    WiFi.softAPConfig(IP, IP, NMask);
    IPAddress IPP = WiFi.softAPIP();
    Serial.print("# AP IP address: ");
    Serial.println(IPP);
    server.begin();
    server.set_routes();
    //delay(15000); // wifi connection
    machine.green_led_on();
    //storage.cleanup();
}



void loop(){
    machine.run();
}
  
  
