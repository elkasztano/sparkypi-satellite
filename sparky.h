#include <Arduino.h>
#define BLINK
#define STATE

typedef struct {
  byte pin;
  #ifdef BLINK
  byte gr;
  byte bl;
  byte re;
  #endif
  String sequence;
  uint16_t pulse_length;
  uint8_t repeats;
  uint16_t shortfactor;
  uint16_t longfactor;
  uint16_t syncbit;
  uint16_t syncgap;
} transmissionType;

void send_bit(bool b, transmissionType *t);

void send_sync_bit(transmissionType *t);

void send_sequence(transmissionType *t);

void json_echo(transmissionType *t);

transmissionType split_into_params(String raw_string, byte pin, byte red, byte green, byte blue);