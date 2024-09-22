#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include "PGNCreator.h"

String HOSTNAME = "esp32-chessboard";
PGNCreator pgnCreator;
void add_moves_to_game() {
  pgnCreator.init_pgn();

  // Play a valid 5-move chess game
  pgnCreator.add_move("e4");
  pgnCreator.add_move("e5");
  pgnCreator.add_move("Nf3");
  pgnCreator.add_move("Nc6");
  pgnCreator.add_move("Bb5");
  pgnCreator.add_move("a6");
  pgnCreator.add_move("Ba4");
  pgnCreator.add_move("Nf6");
  pgnCreator.add_move("O-O");
  pgnCreator.add_move("Be7");

  // End the game as a win for white
  pgnCreator.end_game(WHITE_WIN);
}

void upload_game() {
  // Upload the PGN to Lichess
  String lichessToken = "lip_gZtEd03ONprcrlUMSL4a"; // Replace with your actual Lichess API token
  if (pgnCreator.upload_to_lichess(lichessToken)) {
    Serial.println("PGN uploaded successfully.");
  } else {
    Serial.println("Failed to upload PGN.");
  }
  // Upload the PGN to Lichess as a study
  String studyName = "My Chess Study"; // Replace with your desired study name
  if (pgnCreator.upload_to_lichess_study(lichessToken, studyName)) {
    Serial.println("PGN uploaded to study successfully.");
  } else {
    Serial.println("Failed to upload PGN to study.");
  }
}

void init_wifi() {
  /*
   * Blocks until WiFi connection established.
   */
  WiFiManager wm;
  wm.setHostname(HOSTNAME);
  bool res = wm.autoConnect("Chessboard");
  if(!res) {
    Serial.println("Failed to connect to WiFi. Restarting");
    ESP.restart();
  } else {
    Serial.println("Successfully connected to WiFi.");
  }
}

void init_ota() {
  /*
   * Setups up the OTA update mechanism.
   */
  ArduinoOTA.setHostname(HOSTNAME.c_str());
  // ArduinoOTA.setPasword("SuperSecurePasswordHere");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();
}

void setup() {
  Serial.begin(115200); 

  init_wifi();
  init_ota();
}


// We don't want to print constantly, only every ~5s.
// We can't use delay as if an OTA update happens during that delay period,
// The update won't be found.
unsigned long print_watcher = 0;

void loop() {
  ArduinoOTA.handle();

  unsigned long elapsed = millis();

  if (elapsed -  print_watcher >= 5000) {
    Serial.println("Basic...");
    Serial.println("ADVANCED!...");
    print_watcher = elapsed;
  }
}
