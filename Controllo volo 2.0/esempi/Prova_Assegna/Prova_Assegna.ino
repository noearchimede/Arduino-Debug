#include "Assegna.hpp"

Assegna assegnaValore(Serial);

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  
  bool x1 = 0; assegnaValore(&x1);
  
  uint8_t x2 = 0; assegnaValore(&x2);
  uint16_t x3 = 0; assegnaValore(&x3);
  uint32_t x4 = 0; assegnaValore(&x4);
  
  int8_t x5 = 0; assegnaValore(&x5);
  int16_t x6 = 0; assegnaValore(&x6);
  int32_t x7 = 0; assegnaValore(&x7);
  
  char x8 = 0; assegnaValore(&x8);
  
  float x9 = 0; assegnaValore(&x9);

  Serial.println(x1);
  Serial.println(x2);
  Serial.println(x3);
  Serial.println(x4);
  Serial.println(x5);
  Serial.println(x6);
  Serial.println(x7);
  Serial.println(x8);
  Serial.println(x9);
}
