#include <driver/adc.h>

int MUXER_INPUT_PIN = 33;
int S0 = 17;
int S1 = 5;
int S2 = 18;
int S3 = 19;

char LETTER_COORD_MAP[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

int previous_board_state[8][8];
int current_board_state[8][8];




void setup() {
  Serial.begin(115200); 

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_11); // considering MUXER_INPUT_PIN as 33 
}


void update_board_state() {
  int change_locations[3] = {0};
  int change_count = 0;

  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      previous_board_state[i][j] = current_board_state[i][j];
      current_board_state[i][j] = readMux((8 * i) + j);
      /*
      if (abs(current_board_state[i][j] - previous_board_state[i][j]) > 30) {
        change_locations[change_count] = (10 * i) + j;
        change_count++;
      }
      */
    }
  }
}

void print_board_changes() {
  for (int i=0; i<8; i++) {
    for (int j=0; j<8; j++) {
      if (abs(current_board_state[i][j] - previous_board_state[i][j]) > 30) {
        Serial.print("Change Detected: ");
        Serial.print(LETTER_COORD_MAP[i]);
        Serial.println(j+1);
      }
    }
  }
}

int readMux(int channel) {
  /*
   * Reads from a specified input on the muxer.
   * Taken from: http://adam-meyer.com/arduino/CD74HC4067
   */
  // Exit early as we only have 3 sensors configured:
  if (channel >= 3) {
    return 0;
  }

  int controlPin[] = {S0, S1, S2, S3};

  int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  //loop through the 4 sig
  for(int i = 0; i < 4; i ++){
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  return adc1_get_raw(ADC1_CHANNEL_5);
}

void loop() {
  update_board_state();
  print_board_changes();
  delay(100);

  /*
  for(int i = 0; i < 3; i ++){
    Serial.print("Value at channel ");
    Serial.print(i);
    Serial.print(" is : ");
    Serial.println(readMux(i));
    delay(1000);
  }
  */
}
