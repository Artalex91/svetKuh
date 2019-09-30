//ver 2.0.0
#include <Arduino.h>

//порты
    #define led1Pin // 40см
    #define led2Pin // 100см
    #define led3Pin // 60см
    #define pirPin //  датчик движения
    #define but1Pin // кнопка под микроволновкой
    #define but2Pin // кнопка в шкафчике

//константы
    const uint8_t svetSpeed = 8; // чем больше число - тем мдленнее розжиг

//переменные
    bool svet = false;          // хранение состояния света
    uint32_t svetMill = 0;
    uint8_t brig1 = 0;          // 
    uint8_t brig2 = 0;          // Яркость в данный момент
    uint8_t brig3 = 0;          //

void setup() {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);

    pinMode(pirPin,  INPUT);
    pinMode(but1Pin, INPUT);
    pinMode(but2Pin, INPUT);

}

void readPorts(){

    pir  = digitalRead(pirPin);
    but1 = digitalRead(but1Pin);
    but2 = digitalRead(but2Pin);

}

void svetAnalog(){
 
    if(svet==true && millis()-svetMill>svetSpeed){
        svetMill = millis();
        if(brig1 != 255)                ++brig1;
        if(brig2 != 255 && brig2 > 150) ++brig2;
        if(brig3 != 255 && brig1 > 150) ++brig3;
    }
    if(svet==false && millis()-svetMill>svetSpeed){
        svetMill = millis();
        if(brig1 != 0 && brig2 < 100) --brig1;
        if(brig2 != 0)                --brig2;
        if(brig3 != 0 && brig1 < 100) --brig3;
    }

}

void writePorts(){

    digitalWrite(led1Pin, brig1);
    digitalWrite(led2Pin, brig2);
    digitalWrite(led3Pin, brig3);

}

void loop() {

    readPorts();
    svetAnalog();
    writePorts();

}