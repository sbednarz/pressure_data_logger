#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <Arduino.h>
#include <WiFi.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include "machine.h"
#include "storage.h"


class MyWebServer : public AsyncWebServer {
    public:
        MyWebServer(Machine *m, uint16_t port);
        void set_routes(void);

    private:
        Machine* machine;
        Storage* storage;
};

#endif
