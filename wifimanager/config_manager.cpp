#include "config_manager.h"

ConfigManager::ConfigManager() {
    // Constructor
}

ConfigManager::~ConfigManager() {
    // Destructor
    preferences.end();
}

void ConfigManager::begin() {
    preferences.begin("config", false);
}

String ConfigManager::getLichessApiKey() {
    return preferences.getString(keyLichessApiKey, defaultLichessApiKey);
}

bool ConfigManager::getLichessDirectMode() {
    return preferences.getBool(keyLichessDirectMode, defaultLichessDirectMode);
}

String ConfigManager::getBackendUrl() {
    return preferences.getString(keyBackendUrl, defaultBackendUrl);
}

void ConfigManager::setLichessApiKey(const String& apiKey) {
    preferences.putString(keyLichessApiKey, apiKey);
}

void ConfigManager::setLichessDirectMode(bool directMode) {
    preferences.putBool(keyLichessDirectMode, directMode);
}

void ConfigManager::setBackendUrl(const String& url) {
    preferences.putString(keyBackendUrl, url);
}
