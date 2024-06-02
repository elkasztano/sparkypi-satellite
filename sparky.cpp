#include <Arduino.h>
#include "sparky.h"

void send_bit(bool b, transmissionType *t) {
  
  if (b == true) {
    
    digitalWrite(t->pin, HIGH);
    
    #ifdef BLINK
    digitalWrite(t->gr, HIGH);
    #endif
    
    delayMicroseconds(t->pulse_length * t->longfactor);
    
    digitalWrite(t->pin, LOW);
    
    delayMicroseconds(t->pulse_length * t->shortfactor);
    
    #ifdef BLINK
    digitalWrite(t->gr, LOW);
    #endif
  
  } else {
    
    digitalWrite(t->pin, HIGH);
    
    #ifdef BLINK
    digitalWrite(t->bl, HIGH);
    #endif

    delayMicroseconds(t->pulse_length * t->shortfactor);
    
    digitalWrite(t->pin, LOW);
    
    delayMicroseconds(t->pulse_length * t->longfactor);
    
    #ifdef BLINK
    digitalWrite(t->bl, LOW);
    #endif
  }
  
}

void send_sync_bit(transmissionType *t) {
  
  digitalWrite(t->pin, HIGH);
  
  #ifdef BLINK
  digitalWrite(t->re, HIGH);
  #endif

  delayMicroseconds(t->pulse_length * t->syncbit);
  
  digitalWrite(t->pin, LOW);
  
  delayMicroseconds(t->pulse_length * t->syncgap);
  
  #ifdef BLINK
  digitalWrite(t->re, LOW);
  #endif

}

void send_sequence(transmissionType *t) {
  
  for (int i=0; i<t->repeats; i++) {
    
    for (int j=0; j<(t->sequence).length(); j++) {
      
      char c = (t->sequence)[j];
      
      if (c == '1') {
        send_bit(true, t); 
      } else if (c == '0') {
        send_bit(false, t);
      } else if (c == 's') {
        send_sync_bit(t);
      }
      
    }
    
  }
}

String split_string(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found <= index; i++) {
    if(data.charAt(i)==separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1]+1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void json_echo(transmissionType *t) {
  Serial.print("{ \"sequence\" : \"");
  Serial.print(t->sequence);
  Serial.print("\", \"pulse_length\" : ");
  Serial.print(t->pulse_length);
  Serial.print(", \"repeats\" : ");
  Serial.print(t->repeats);
  Serial.print(", \"shortfactor\" : ");
  Serial.print(t->shortfactor);
  Serial.print(", \"longfactor\" : ");
  Serial.print(t->longfactor);
  Serial.print(", \"syncbit\" : ");
  Serial.print(t->syncbit);
  Serial.print(", \"syncgap\" : ");
  Serial.print(t->syncgap);
  Serial.println(" }");
}

transmissionType split_into_params(String raw_string, byte pin, byte red, byte green, byte blue) {
  transmissionType t = {
    .pin = pin,
    #ifdef BLINK
    .gr = green,
    .bl = blue,
    .re = red,
    #endif
    .sequence = split_string(raw_string, ',', 0),
    .pulse_length = split_string(raw_string, ',', 1).toInt(),
    .repeats = split_string(raw_string, ',', 2).toInt(),
    .shortfactor = split_string(raw_string, ',', 3).toInt(),
    .longfactor = split_string(raw_string, ',', 4).toInt(),
    .syncbit = split_string(raw_string, ',', 5).toInt(),
    .syncgap = split_string(raw_string, ',', 6).toInt()
  };
  return t;
}