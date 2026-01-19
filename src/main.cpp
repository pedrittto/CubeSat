#include <Arduino.h>

const int PIN_RED=12;
const int PIN_GREEN=27;
const int PIN_YELLOW=14;
const int PIN_BTN=4;

const int STAN_RED=1;
const int STAN_YELLOW=2;
const int STAN_GREEN=3;
const int STAN_OFF=0;
const int STAN_PREPARING=4;

int aktualnyStan=STAN_OFF;

unsigned long czasOstatniejZmiany=0;

const int CZAS_RED=2000;
const int CZAS_YELLOW=1000;
const int CZAS_GREEN=1000;
const int CZAS_DELAY=3000;

void setup (){
  Serial.begin(115200);

  digitalWrite(PIN_RED,LOW);
  digitalWrite(PIN_YELLOW,LOW);
  digitalWrite(PIN_GREEN,HIGH);

  pinMode(PIN_RED,OUTPUT);
  pinMode(PIN_YELLOW,OUTPUT);
  pinMode(PIN_GREEN,OUTPUT);

  pinMode(PIN_BTN,INPUT_PULLUP);

}

void loop(){
  unsigned long aktualnyCzas=millis();

  switch (aktualnyStan){

    case STAN_OFF:
    if (digitalRead(PIN_BTN)==LOW){
      aktualnyStan=STAN_PREPARING;
      czasOstatniejZmiany=aktualnyCzas;

    }
    break;
    
    case STAN_PREPARING:
      if (aktualnyCzas - czasOstatniejZmiany >= 500) {
        if (digitalRead(PIN_BTN) == LOW) {
          aktualnyStan = STAN_OFF;
          digitalWrite(PIN_GREEN, HIGH);
          delay(200);
          break;
        }
      }

      if (aktualnyCzas - czasOstatniejZmiany >= CZAS_DELAY) {
        aktualnyStan = STAN_YELLOW;
        czasOstatniejZmiany = aktualnyCzas;
        digitalWrite(PIN_GREEN, LOW);
        digitalWrite(PIN_YELLOW, HIGH);
      } else {
        if ((aktualnyCzas % 500) < 250) {
          digitalWrite(PIN_GREEN, HIGH);
        } else {
          digitalWrite(PIN_GREEN, LOW);
        }
      }
      break;
    
    case STAN_YELLOW:
    if (aktualnyCzas-czasOstatniejZmiany>=CZAS_YELLOW){
      aktualnyStan=STAN_RED;
      czasOstatniejZmiany=aktualnyCzas;
      digitalWrite(PIN_YELLOW,HIGH);
      digitalWrite(PIN_RED,HIGH);
      digitalWrite(PIN_GREEN,LOW);
    } 
    break;

    case STAN_RED:
    if (aktualnyCzas-czasOstatniejZmiany>=CZAS_RED){
      aktualnyStan=STAN_OFF;
      czasOstatniejZmiany=aktualnyCzas;
      digitalWrite(PIN_YELLOW,LOW);
      digitalWrite(PIN_RED,LOW);
      digitalWrite(PIN_GREEN,HIGH);
    } 
    break;

  }
}