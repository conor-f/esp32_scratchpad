#include <WiFiUdp.h>
#include <NTPClient.h>
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
#include "PGNCreator.h"


char LETTER_COORD_MAP[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

bool previous_board_state[8][8] = {false};
bool board_state[8][8] = {false};

PGNCreator pgnCreator;


void setup() {
  Serial.begin(115200); 

  WiFiManager wm;
  bool res = wm.autoConnect("Chessboard");
  if(!res) {
    Serial.println("Failed to connect to WiFi. Restarting");
    ESP.restart();
  } else {
    Serial.println("Successfully connected to WiFi.");
  }

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

  // Upload the PGN to Lichess
  String lichessToken = ""; // Replace with your actual Lichess API token
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

void loop() {
  // Empty loop
}
