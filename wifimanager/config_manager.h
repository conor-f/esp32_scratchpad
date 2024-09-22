#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Preferences.h>

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    void begin();
    String getLichessApiKey();
    bool getLichessDirectMode();
    String getBackendUrl();

    void setLichessApiKey(const String& apiKey);
    void setLichessDirectMode(bool directMode);
    void setBackendUrl(const String& url);

private:
    Preferences preferences;

    // Default values
    const char* defaultLichessApiKey = "default_api_key";
    bool defaultLichessDirectMode = false;
    const char* defaultBackendUrl = "http://default.backend.url";

    // Config keys
    const char* keyLichessApiKey = "lichess_api_key";
    const char* keyLichessDirectMode = "lichess_direct_mode";
    const char* keyBackendUrl = "backend_url";
};

#endif // CONFIG_MANAGER_H
