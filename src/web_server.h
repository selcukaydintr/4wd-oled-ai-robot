#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "plugin_system.h"

class WebInterface {
private:
    AsyncWebServer server;
    PluginSystem& pluginSystem;
    
    void setupRoutes();
    void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);
    
public:
    WebInterface(int port, PluginSystem& ps) : server(port), pluginSystem(ps) {}
    void begin();
    void handleClient();
};

#endif 