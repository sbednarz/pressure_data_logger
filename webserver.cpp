#include "webserver.h"
#include "view.h"
#include "version.h"

MyWebServer::MyWebServer(Machine *m, uint16_t port=80): AsyncWebServer(port) {
    machine = m;
}

void MyWebServer::set_routes(void){

    on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    on("/time", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(machine->get_time(),0).c_str());
    });

    on("/pressure", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", String(machine->get_pressure(),0).c_str());
    });

    on("/data", HTTP_GET, [this](AsyncWebServerRequest *request){
        const char* mydata = machine->get_data(); 
        request->send_P(200, "text/plain", mydata);
    });

    
    on("/datasetname", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", machine->storage->get_datasetname());
    });
    

    on("/status", HTTP_GET, [this](AsyncWebServerRequest *request){
        request->send_P(200, "text/plain", machine->get_status());
    });

    
    on("/start", HTTP_GET, [this](AsyncWebServerRequest *request){
        if(request->hasArg("timestamp")) {
            String arg = request->arg("timestamp");
            Serial.printf("# webserver got timestamp: %s\n", arg.c_str());
            request->send_P(200, "text/plain", "OK");
            machine->start(arg.c_str());
        }
        else {
            request->send_P(200, "text/plain", "ERROR NO stamp");
        }
    });

    on("/stop", HTTP_GET, [this](AsyncWebServerRequest *request){
            machine->stop();
            request->send_P(200, "text/plain", "OK");
    });
    
    

    on("/download", HTTP_GET, [this](AsyncWebServerRequest *request){
            if(request->hasArg("filename")) {
                String filename = "/" + request->arg("filename");
                request->send(SD, filename.c_str(), "text/plain", true);    
            }
            else {
                request->send_P(200, "text/plain", "ERROR no filename");
            }
    });

    on("/version", HTTP_GET, [this](AsyncWebServerRequest *request){
            request->send_P(200, "text/plain", app_version);
    });

    on("/list", HTTP_GET, [this](AsyncWebServerRequest *request){
            request->send(SD, "/IDX.dat", "text/plain", true);
    });

    
}
