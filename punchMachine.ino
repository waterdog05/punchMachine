#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>
#define PIN 7
#define NUMPIXELS 10
#define BRIGHTNESS 180

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(16, 2, 0x27);

int senVal = 1023;  //default shock sensor value  1023~0
int shock = 10000;  //source senVal
int power = 0;
int curPow = 0;  //store score
int maxPow = 0;  //max score
int res = 0; //0~9

void setup() {
    Serial.begin(9600);
    strip.setBrightness(BRIGHTNESS);
    strip.begin();
    strip.show();
    lcd.init();
    lcd.backlight();
}
 
void loop() {
    senVal = analogRead(A0);
    shock = map(senVal, 0, 1023, 0, 10000);
    power = 10000 - shock;  //0~10000

    if (0 <= power <1000) {
        res = 0;
    } else if (1000 <= power <2000) {
        res = 1;
    } else if (2000 <= power <3000) {
        res = 2;
    } else if (3000 <= power <4000) {
        res = 3;
    } else if (4000 <= power <5000) {
        res = 4;
    } else if (5000 <= power <6000) {
        res = 5;
    } else if (6000 <= power <7000) {
        res = 6;
    } else if (7000 <= power <8000) {
        res = 7;
    } else if (8000 <= power <9000) {
        res = 8;
    } else {
        res = 9;
    }

    curPow = power;  //current score

    if (curPow > maxPow) {  //update best score
        maxPow = curPow;
    }
    
    for (int i=0; i<res+1; i++) {  //ledstrip gauge bar
        strip.setPixelColor(i, 255, 0, 0);
    }

    Serial.print("score : ");
    Serial.println(curPow);
    Serial.print("best score : ");
    Serial.println(maxPow);

    lcd.setCursor(0, 0);
    lcd.print("score : ");
    lcd.println(curPow);
    lcd.setCursor(0, 1);
    lcd.print("best score : ");
    lcd.println(maxPow);

    delay(50);

    //reset everything
    for (int i=0; i<10; i++) {
        strip.setPixelColor(i, 0, 0, 0);
    }
    curPow = 0;
    res = 0;

    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("best score : ");
    lcd.println(maxPow);
}
