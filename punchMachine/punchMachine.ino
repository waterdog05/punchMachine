#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>
#define PIN 7
#define NUMPIXELS 20
#define BRIGHTNESS 180

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int srtBtn = 13;
int srtVal = 1;
int senVal = 1023;  //default shock sensor value  1023~0
int shock = 10000;  //source senVal
int power = 0;
int curPow = 0;  //store score
int maxPow = 0;  //max score
int res = 10; //0~9, 10

//functions
void calcResult();
void resultLed();
void resultLcd();
void reset();

void setup() {
    Serial.begin(9600);
    pinMode(srtBtn, INPUT_PULLUP);
    strip.setBrightness(BRIGHTNESS);
    strip.begin();
    strip.show();
    lcd.init();
    lcd.backlight();
}
 
void loop() {
    srtVal = digitalRead(srtBtn);
    if (srtVal == 0) {
        if (curPow > maxPow) {  //update best score
            maxPow = curPow;
        }
        lcd.setCursor(1, 0);
        lcd.print("Bestscore:");
        lcd.print(maxPow);

        while(analogRead(0) >= 50) {
            Serial.println("wait");
            delay(1);
        }
        calcResult();
        lcd.setCursor(3, 1);
        lcd.print("Score:");
        lcd.println(lastPow);
        curPow = power;  //current score
        
        resultLed();
        resultLcd();

        if (curPow > maxPow) {  //update best score
            maxPow = curPow;
        }
        lcd.setCursor(1, 0);
        lcd.print("Bestscore:");
        lcd.print(curPow);
        reset(); 
    }
}

void calcResult() {
    senVal = analogRead(A0);
    shock = map(senVal, 0, 1023, 0, 10000);
    power = 10000 - shock;  //0~10000

    if ((50 <= power) && (power <1000)) {
        res = 0;
    } else if ((1000 <= power) && (power < 2000)) {
        res = 1;
    } else if ((2000 <= power) && (power < 3000)) {
        res = 2;
    } else if ((3000 <= power) && (power < 4000)) {
        res = 3;
    } else if ((4000 <= power) && (power < 5000)) {
        res = 4;
    } else if ((5000 <= power) && (power < 6000)) {
        res = 5;
    } else if ((6000 <= power) && (power < 7000)) {
        res = 6;
    } else if ((7000 <= power) && (power < 8000)) {
        res = 7;
    } else if ((8000 <= power) && (power < 9000)) {
        res = 8;
    } else if ((9000 <= power) && (power <= 10000)) {
        res = 9;
    } else {
      res = 10;
      power = 0;
    }
    Serial.println(res);
}

void resultLed() {
    if ((0<=res)||(res<=9)) {
      for (int i=0; i<(res+1)*2; i++) {  //ledstrip gauge bar
        strip.setPixelColor(i, 255, 0, 0);
      }
    } else if (res == 10) {
      for (int i=0; i<20; i++) {  //ledstrip gauge bar
        strip.setPixelColor(i, 0, 0, 0);
      }
    }
}

void resultLcd() {
    lcd.setCursor(3, 1);
    lcd.print("Score:");
    lcd.println(curPow);
    Serial.println(curPow);
    lcd.setCursor(1, 0);
    lcd.print("Bestscore:");
    lcd.println(maxPow);

}

void reset() {
    for (int i=0; i<20; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    power = 0;
    senVal = 1023;
    shock = 10000;
    res = 10;
}
