#include "PGNCreator.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

PGNCreator::PGNCreator() : current_move_number(1), is_white_move(true) {}

void PGNCreator::init_pgn() {
  is_white_move = true;
  current_move_number = 1;

  String iso_time = get_date_string();
  String date = iso_time.substring(0, 10);
  date.replace("-", ".");

  pgn =
    "[Event \"OTB Game\"]\n"
    "[Site \"Berlin, Berlin GERMANY\"]\n"
    "[Date \"" + date + "\"]\n"
    "[Round \"Someones!\"]\n"
    "[White \"Someone\"]\n"
    "[Black \"An Opponent\"]\n"
    "[Annotator \"Some Cool Zappy Name\"]\n";
}

bool PGNCreator::upload_to_lichess(const String& token) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  HTTPClient http;
  http.begin("https://lichess.org/api/import");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Authorization", "Bearer " + token);

  String postData = "pgn=" + pgn;
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    String errorResponse = http.getString();
    Serial.println("Error on sending POST: " + String(httpResponseCode));
    Serial.println("Error response: " + errorResponse);
  }

  http.end();
  return httpResponseCode == 200;
}

bool PGNCreator::upload_to_lichess_study(const String& token, const String& studyName) {
  /*
  * TODO This doesn't work as lichess don't allow you to create a study via API.
  * Resolve this issue by just forcing the user to have already created the
  * study
  */
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return false;
  }

  HTTPClient http;
  http.begin("https://lichess.org/api/study");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", "Bearer " + token);

  String postData = "{\"name\":\"" + studyName + "\", \"pgn\":\"" + pgn + "\"}";
  int httpResponseCode = http.POST(postData);

  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println("Response: " + response);
  } else {
    String errorResponse = http.getString();
    Serial.println("Error on sending POST: " + String(httpResponseCode));
    Serial.println("Error response: " + errorResponse);
  }

  http.end();
  return httpResponseCode == 200;
}

void PGNCreator::add_move(String move) {
  if (is_white_move) {
    char buffer[10];
    sprintf(buffer, "%d. ", current_move_number);
    pgn += buffer;
  } else {
    pgn += " ";
  }

  pgn += move;

  if (!is_white_move) {
    current_move_number++;
    pgn += "\n";
  }

  is_white_move = !is_white_move;
}

void PGNCreator::end_game(Result result) {
  String result_string = "[Result \"";

  switch (result) {
    case WHITE_WIN:
      pgn += "1-0\n";
      result_string += "1-0";
      break;
    case BLACK_WIN:
      pgn += "0-1\n";
      result_string += "0-0";
      break;
    case DRAW:
      pgn += "1/2-1/2\n";
      result_string += "1/2-1/2";
      break;
  }

  result_string += "\"]\n";

  int position = 0;
  for (int i = 0; i < 7; i++) {
    position = pgn.indexOf('\n', position) + 1;
  }

  String lhs = pgn.substring(0, position);
  String rhs = pgn.substring(position);

  pgn = lhs + result_string + rhs;
}

String PGNCreator::get_pgn() const {
  return pgn;
}

String PGNCreator::epochToISO8601(unsigned long epoch) {
  struct tm ts;
  char buf[30];
  time_t t = epoch;
  gmtime_r(&t, &ts);
  strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &ts);
  return String(buf);
}

String PGNCreator::get_date_string() {
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org");
  timeClient.begin();
  timeClient.setTimeOffset(7200);
  timeClient.update();
  unsigned long epoch_time = timeClient.getEpochTime();
  return epochToISO8601(epoch_time);
}
