#ifndef PLUGIN_SYSTEM_H
#define PLUGIN_SYSTEM_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include <functional>

class Plugin {
public:
    String name;
    String version;
    String description;
    JsonObject config;
    
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual JsonObject getStatus() = 0;
    virtual bool configure(JsonObject settings) = 0;
};

class PluginSystem {
private:
    std::vector<Plugin*> plugins;
    
public:
    bool loadPlugin(const char* filename);
    bool unloadPlugin(const char* pluginName);
    void setupPlugins();
    void loopPlugins();
    JsonArray getPluginsList();
};

#endif 