#ifndef PGN_CREATOR_H
#define PGN_CREATOR_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

enum Result {
  WHITE_WIN,
  BLACK_WIN,
  DRAW,
};

class PGNCreator {
public:
  PGNCreator();
  void init_pgn();
  void add_move(String move);
  void end_game(Result result);
  String get_pgn() const;
  bool upload_to_lichess(const String& token);
  bool upload_to_lichess_study(const String& token, const String& studyName);

private:
  String epochToISO8601(unsigned long epoch);
  String get_date_string();

  String pgn;
  int current_move_number;
  bool is_white_move;
};

#endif // PGN_CREATOR_H
