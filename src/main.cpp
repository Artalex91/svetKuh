//ver 2.0.0
#include <Arduino.h>

#define led1Pin // 40см
#define led2Pin // 100см
#define led3Pin // 60см
#define pirPin //
#define but1Pin //
#define but2Pin //

void setup() {

 pinMode(led1Pin, OUTPUT);
 pinMode(led2Pin, OUTPUT);
 pinMode(led3Pin, OUTPUT);

 pinMode(pirPin,  INPUT);
 pinMode(but1Pin, INPUT);
 pinMode(but2Pin, INPUT);

}

void svetAnalog(){
 
 

}


void loop() {

 svetAnalog();

}