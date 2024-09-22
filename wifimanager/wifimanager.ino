#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "config_manager.h"

ConfigManager configManager;


void setup() {
    Serial.begin(115200);

    configManager.begin();

    Serial.println("Beginning WiFi setup...");
    WiFiManager wm;

    // Tries to auto-connect to stored WiFi network. If it fails, it will
    // create a network called Chessboard, and demand you give it WiFi details.
    bool res = wm.autoConnect("Chessboard");

    if(!res) {
        Serial.println("Failed to connect to WiFi. Restarting");
        ESP.restart();
    } else {
        Serial.println("Successfully connected to WiFi.");
    }

    Serial.println(configManager.getLichessApiKey());
}

void loop() {

}
