//ver 2.0.1  03.10.2019
#include <Arduino.h>

//порты
    #define led1Pin 6// 40см
    #define led2Pin 5// 100см
    #define led3Pin 3// 60см
    #define pirPin  4//  датчик движения
    #define but1Pin 2// кнопка под микроволновкой

//константы
    const uint8_t svetSpeed = 10; // чем больше число - тем мдленнее розжиг
    const uint32_t pirDelay = 120000; // 2 мин (затухание света при отсутствии движения 2 мин(в авторежиме))
    const uint32_t noDetectDelayShort = 3600000; // 1час задержка в ручном режиме по короткому нажатию
    const uint32_t noDetectDelayLong = 14400000; // 4часа задержка в ручном при долгом нажатии

//переменные
    uint32_t noDetectDelay = 3600000; // 60 мин (автовыключение ручного режима при отсутствии любого движениия в течении 60мин(в ручном режиме))
    bool svet = false;          // хранение состояния света
    uint32_t svetMill = 0;
    uint8_t brig1 = 0;          // 
    uint8_t brig2 = 0;          // Яркость в данный момент
    uint8_t brig3 = 0;          //
    bool pir = false;
    bool but1 = false;
    uint32_t pirMill = 0;
    bool but1Flag = false;
    uint32_t but1FlagMill = 0;
    bool shortPress1 = false;
    bool longPress1 = false;
    bool manualMode = false;
    uint32_t noDetectMotionMill = 0;
    bool noDetectFlag = false;
    bool firstCycleM = false;
    bool pirFlag = false;
    uint16_t longPressDelay = 1000; // задержка долгого нажатия

void setup() {
    pinMode(led1Pin, OUTPUT);
    pinMode(led2Pin, OUTPUT);
    pinMode(led3Pin, OUTPUT);

    pinMode(pirPin,  INPUT);
    pinMode(but1Pin, INPUT);

}

void readPorts(){

    pir  = digitalRead(pirPin);
    but1 = digitalRead(but1Pin);

}

void button(){
    
    if(but1 == true && but1Flag == false){
        but1Flag = true;
        but1FlagMill = millis();
    }
    else if(but1 == false && but1Flag == true){
        but1Flag = false;
        if(millis()-but1FlagMill<longPressDelay) {shortPress1 = true;}
        else                           {longPress1 = true; }
        but1FlagMill = 0;
    }

    if(but1 == true && millis() - but1FlagMill > longPressDelay) brig3 = 5;

}

void task(){

    if(manualMode == true){ // активен ручной режим
        if(firstCycleM == false) {noDetectMotionMill = millis(); firstCycleM = true;}// при входе в авторежим

        if(shortPress1 == true){ // вкл/выкл свет в ручную
            shortPress1 = false;
            svet = !svet;             
        }
        if(longPress1 == true){    //возврат в авторежим по долгому нажатию
            longPress1 = false;
            noDetectDelay = noDetectDelayShort;
            manualMode = false;
            brig3 = 5;
            noDetectFlag = false;
        }

        if(pir == false && noDetectFlag == false){  //если нет движения - вкл счетчик
            noDetectFlag = true;
            noDetectMotionMill = millis();
        }
        else if (pir == true && noDetectFlag == true){ // если есть движение - сбросить счетчик
            noDetectFlag = false;
            noDetectMotionMill = millis();
        }

        if(millis()-noDetectMotionMill>noDetectDelay){      // автоотключение ручного режима через 2 часа
            manualMode = false;
            noDetectFlag = false;
            noDetectDelay = noDetectDelayShort;
            noDetectMotionMill = millis();
        }

    }
    else{ //активен авто режим
        if(firstCycleM == true) firstCycleM = false; // сброс флага

        if(pir==true && pirFlag == false){                             // вкл свет при движении
            pirFlag = true;
            svet = true;
            pirMill = millis();

        }    
        if(pir==false && pirFlag == true){
            pirFlag = false;
            pirMill = millis();
        }

        else if(svet==true && pir==false && millis()-pirMill>pirDelay){       // выкл свет если движения нет n время
            svet = false;

        }

        if(shortPress1 == true){     //вкл ручной режим по короткому нажатию
            shortPress1 = false;
            noDetectDelay = noDetectDelayShort;
            manualMode = true;
            brig2 = 5;
        }
        if(longPress1 == true){     //вкл ручной режим по длинному нажатию
            longPress1 = false;
            noDetectDelay = noDetectDelayLong;
            manualMode = true;
            brig1 = 5;
        }
    }
}

void svetAnalog(){
 
    if(svet==true && millis()-svetMill>svetSpeed){
        svetMill = millis();
        if(brig1 != 255)                ++brig1;
        if(brig2 != 255 && brig3 > 100) ++brig2;
        if(brig3 != 255 && brig1 > 100) ++brig3;
    }
    if(svet==false && millis()-svetMill>svetSpeed){
        svetMill = millis();
        if(brig1 != 0 && brig2 < 100) --brig1;
        if(brig2 != 0)                --brig2;
        if(brig3 != 0 && brig1 < 100) --brig3;
    }

}

void writePorts(){

    analogWrite(led1Pin, brig1);
    analogWrite(led2Pin, brig2);
    analogWrite(led3Pin, brig3);

}

void loop() {

    readPorts();
    button();
    task();
    svetAnalog();
    writePorts();

}