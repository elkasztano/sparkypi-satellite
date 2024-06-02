#include "sparky.h";

String readString, sequence, pulse_length_string, repeats_string;
const byte PIN = 10;

const long BAUD_RATE = 115200;

#ifdef BLINK
const byte RED = 2;
const byte GREEN = 3;
const byte BLUE = 4;
#endif

#ifdef STATE
const byte RPIN5 = 5;
const byte RPIN6 = 6;
#endif

void setup() {
  
  Serial.begin(BAUD_RATE);
  
  pinMode(PIN, OUTPUT);

  #ifdef BLINK  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  #endif
  
  #ifdef STATE
  pinMode(RPIN5, INPUT);
  pinMode(RPIN6, INPUT);
  #endif

}

void loop() {
  while (Serial.available()) {
    delay(3);
    if (Serial.available() > 0) {
      char ch = Serial.read();
      readString += ch;
    }
  }

  if (readString.length() > 0) {

    if (readString.startsWith("blink")) {
      
      #ifdef BLINK
      // This is just for testing purposes.
      // You can add your own functions here.
      digitalWrite(GREEN, HIGH);
      delay(100);
      digitalWrite(GREEN, LOW);
      digitalWrite(RED, HIGH);
      delay(100);
      digitalWrite(RED, LOW);
      Serial.println("blinked ;)");
      #endif

      #ifndef BLINK
      Serial.println("blink not available");
      #endif
    
    } else if (readString.startsWith("reqswitch")) {
      
      #ifdef STATE
      // For testing purposes only.
      // Send back state of digitalPin 5 and 6. (i.e. two bits)
      Serial.println( ( digitalRead(RPIN5) | digitalRead(RPIN6) << 1 ) );
      #endif

      #ifndef STATE
      Serial.println("reqswitch not available");
      #endif

    } else {
      
      transmissionType t = split_into_params(readString, PIN, 
      
      #ifdef BLINK
        RED, GREEN, BLUE
      #endif

      #ifndef BLINK
        0, 0, 0
      #endif
      
      );
      
      send_sequence(&t);
      json_echo(&t);
    
    }
    readString="";
  }

}
