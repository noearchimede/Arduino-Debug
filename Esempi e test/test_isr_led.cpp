#include <Arduino.h>

#include "Led.hpp"

Led led;

void setup() {
  led.begin(13);
  Serial.begin(115200);
}

void loop() {
  unsigned long i, f;

  i = micros();
  
  led.abilita();//1/////////////////////
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//2
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//3
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//4
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//5
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//6
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//7
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//8
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//9
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();//10
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  led.abilita();
  
  f = micros();

  Serial.print("spento:\t");
  Serial.println(f - i);
  a();
  b();
  c();

  while (1);
}



void a() {
  unsigned long i, f;

  i = micros();
  for (unsigned long i = 0; i != 1000000; i++) led.controlla();
  f = micros();

  Serial.print("spento:\t");
  Serial.println(f - i);
}


void b() {
  unsigned long i, f;

  led.accendi();
  i = micros();
  for (unsigned long i = 0; i != 1000000; i++) led.controlla();
  f = micros();

  Serial.print("manual:\t");
  Serial.println(f - i);
}


void c() {
  unsigned long i, f;

  led.accendi(3000);
  i = micros();
  for (unsigned long i = 0; i != 1000000; i++) led.controlla();
  f = micros();

  Serial.print("acceso:\t");
  Serial.println(f - i);
}

