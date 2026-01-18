#include <Arduino.h>

const int PIN_RED=12;
const int PIN_GREEN=27;
const int PIN_YELLOW=14;

const int STAN_RED=1;
const int STAN_YELLOW=2;
const int STAN_GREEN=3;

int aktualnyStan=STAN_RED;

unsigned long czasOstatniejZmiany=0;

const int CZAS_RED=1000;
const int CZAS_YELLOW=500;
const int CZAS_GREEN=1000;

void setup (){
  Serial.begin(115200);

  digitalWrite(PIN_RED,HIGH);
  digitalWrite(PIN_YELLOW,LOW);
  digitalWrite(PIN_GREEN,LOW);

  pinMode(PIN_RED,OUTPUT);
  pinMode(PIN_YELLOW,OUTPUT);
  pinMode(PIN_GREEN,OUTPUT);

}

void loop(){
  unsigned long aktualnyCzas=millis();

  switch (aktualnyStan){

    case STAN_RED:
    if (aktualnyCzas-czasOstatniejZmiany>=CZAS_RED){
      aktualnyStan=STAN_YELLOW;
      czasOstatniejZmiany=aktualnyCzas;
      digitalWrite(PIN_RED,HIGH);
      digitalWrite(PIN_YELLOW,HIGH);
    }
    break;

    case STAN_YELLOW:
    if (aktualnyCzas-czasOstatniejZmiany>=CZAS_YELLOW){
      aktualnyStan=STAN_GREEN;
      czasOstatniejZmiany=aktualnyCzas;
      digitalWrite(PIN_YELLOW,LOW);
      digitalWrite(PIN_RED,LOW);
      digitalWrite(PIN_GREEN,HIGH);
    } 
    break;

    case STAN_GREEN:
    if (aktualnyCzas-czasOstatniejZmiany>=CZAS_GREEN){
      aktualnyStan=STAN_RED;
      czasOstatniejZmiany=aktualnyCzas;
      digitalWrite(PIN_GREEN,LOW);
      digitalWrite(PIN_RED,HIGH);
  }
  break ;

  }
}