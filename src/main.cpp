//ver 2.0.0  30.09.2019
#include <Arduino.h>

//порты
    #define led1Pin 3// 40см
    #define led2Pin 5// 100см
    #define led3Pin 6// 60см
    #define pirPin  4//  датчик движения
    #define but1Pin 7// кнопка под микроволновкой
    //#define but2Pin 8// кнопка в шкафчике

//константы
    const uint8_t svetSpeed = 8; // чем больше число - тем мдленнее розжиг
    const uint32_t pirDelay = 1000*60*2; // 2 мин (затухание света при отсутствии движения 2 мин(в авторежиме))
    const uint32_t noDetectDelay = 1000*60*120; // 120 мин (автовыключение ручного режима при отсутствии любого движениия в течении 120мин(в ручном режиме))

//переменные
    bool svet = false;          // хранение состояния света
    uint32_t svetMill = 0;
    uint8_t brig1 = 0;          // 
    uint8_t brig2 = 0;          // Яркость в данный момент
    uint8_t brig3 = 0;          //
    bool pir = false;
    bool but1 = false;
    //bool but2 = false;
    uint32_t pirMill = 0;
    bool but1Flag = false;
    uint32_t but1FlagMill = 0;
    bool shortPress1 = false;
    bool longPress1 = false;
    bool manualMode = false;
    uint32_t noDetectMotionMill = 0;
    bool noDetectFlag = false;

void setup() {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);

    pinMode(pirPin,  INPUT);
    pinMode(but1Pin, INPUT);
    //pinMode(but2Pin, INPUT);

}

void readPorts(){

    pir  = digitalRead(pirPin);
    but1 = digitalRead(but1Pin);
    but2 = digitalRead(but2Pin);

}

void button(){
    
    if(but1 == true && but1Flag == false){
        but1Flag = true;
        but1FlagMill = millis();
    }
    else if(but1 == false && but1Flag == true){
        but1Flag = false;
        if(millis()-but1FlagMill<2000) {shortPress1 = true;}
        else                           {longPress1 = true; }
        but1FlagMill = 0;
    }

}

void task(){

    if(manualMode == true){ // активен ручной режим
        if(shortPress1 == true){ // вкл/выкл свет в ручную
            shortPress1 = false;
            svet = !svet;             
        }
        if(longPress1 == true){    //возврат в авторежим по долгому нажатию
            longPress1 = false;
            manualMode = false;
            brig3 = 120;
        }

        if(pir == false && noDetectFlag == false){  //если нет движения - вкл счетчик
            noDetectFlag = true;
            noDetectMotionMill = millis();
        }
        else if (pir == true && noDetectFlag == true){ // если есть движение - сбросить счетчик
            noDetectFlag = false;
            noDetectMotionMill = 0;
        }

        if(millis()-noDetectMotionMill>noDetectDelay){      // автоотключение ручного режима через 2 часа
            manualMode = false;
            noDetectMotionMill = 0;
        }

    }
    else{ //активен авто режим

        if(pir==true){                             // вкл свет при движении
            svet = true;
            pirMill = millis();

        }    
        else if(pir==false && millis()-pirMill>pirDelay){       // выкл свет если движения нет n время
            svet = false;

        }

        if(shortPress1 == true){     //вкл ручной режим по короткому нажатию
            shortPress1 = false;
            manualMode = true;
            brig2 = 120;
        }
    }
}

void svetAnalog(){
 
    if(svet==true && millis()-svetMill>svetSpeed){
        svetMill = millis();
        if(brig1 != 255)                ++brig1;
        if(brig2 != 255 && brig3 > 150) ++brig2;
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
    button();
    task();
    svetAnalog();
    writePorts();

}