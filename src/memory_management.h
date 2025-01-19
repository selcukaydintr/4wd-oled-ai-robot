#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include <Arduino.h>
#include <esp32-hal.h>

class MemoryManager {
public:
    static void checkMemory() {
        if (Serial) {
            Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
            if (psramFound()) {
                Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());
            }
        }
    }
    
    static void optimizeMemory() {
        // Bellek sızıntılarını önlemek için periyodik temizlik
        if (ESP.getFreeHeap() < 10000) { // Düşük bellek durumu
            ESP.getFreeHeap();
            delay(1); // yield() yerine daha güvenli
        }
    }
};

#endif 