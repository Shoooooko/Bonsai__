#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include <Avatar.h>
#include <faces/DogFace.h>

//avatar setting
using namespace m5avatar;

Avatar avatar;

//avatar face setting
Face* faces[2];
const int facesSize = sizeof(faces) / sizeof(Face*);
int faceIdx = 0;

//avatar expression setting
const Expression expressions[] = {
  Expression::Doubt,
  Expression::Happy
};
const int expressionsSize = sizeof(expressions) / sizeof(Expression);
int idx = 0;

//avatar color setting
ColorPalette* cps[2];
const int cpsSize = sizeof(cps) / sizeof(ColorPalette*);
int cpsIdx = 0;

//blynk setting
char auth[] = "xxxx";
char ssid[] = "xxxx";
char pass[] = "xxxx";

//motor
int redPin = 21;
int greenPin = 22;
//analogread
int nRainIn = 36;
//digitalread
int nRainDigitalIn = 26;

//check from rain detector
boolean bIsRaining = false;
//check from blynk
boolean blynkRaining = false;

//blynk lcd setting
WidgetLCD lcd(V2);

void setup() {
    // Debug console
    M5.begin();
    Serial.begin(115200);

    // Speaker OFF
    dacWrite(25, 0);

    //Blynk start
    Blynk.begin(auth, ssid, pass);

    pinMode(nRainIn, INPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(nRainDigitalIn, INPUT);

    // Setup a function to be called every second
    delay(100);

    M5.Lcd.setBrightness(30);
    M5.Lcd.clear();

    faces[0] = new DogFace();
    faces[1] = avatar.getFace();

    cps[0] = new ColorPalette();
    cps[1] = new ColorPalette();
    cps[0]->set(COLOR_PRIMARY, TFT_YELLOW);
    cps[0]->set(COLOR_BACKGROUND, TFT_DARKCYAN);
    cps[1]->set(COLOR_PRIMARY, TFT_DARKGREY);
    cps[1]->set(COLOR_BACKGROUND, TFT_WHITE);

    avatar.init();
    avatar.setColorPalette(*cps[0]);

    //m5stack lcd
    M5.Lcd.println("Connecting to ...");
    M5.Lcd.print(ssid);

    //blynk lcd
    lcd.clear();
    lcd.print(0, 0, "Bonsai is");
}

int value = 0;

//blynk button to select rainy mode or sunny mode
BLYNK_WRITE(V0){
  int button = param[0].asInt();

  if(button == 1){
    blynkRaining = true;
  } else {
    blynkRaining = false;
  }
}

//send analog value to blynk level meter
BLYNK_READ(V1){
  int rainValue = analogRead(nRainIn);
  Blynk.virtualWrite(V1, rainValue);
}

void loop() {
  M5.update();

  Blynk.run();

  //when to close or open bonsai
  //rainy(bIsRaining == true) : close
  //sunny(bIsRaining == false) : open
  if(blynkRaining || digitalRead(nRainDigitalIn) == LOW){
    bIsRaining = true;
  } else {
    bIsRaining = false;
  }

  if(bIsRaining){
    digitalWrite(greenPin, HIGH);
    digitalWrite(redPin, LOW);

    //set blue color and doubt expression
    avatar.setColorPalette(*cps[0]);
    avatar.setExpression(expressions[0]);
  } else {
    digitalWrite(greenPin, LOW);
    digitalWrite(redPin, HIGH);

    //set white color and doubt expression
    avatar.setColorPalette(*cps[1]);
    avatar.setExpression(expressions[1]);
  }
  Serial.print(nRainIn);

  //select human or dog
  if (M5.BtnA.wasPressed())
  {
    avatar.setFace(faces[faceIdx]);
    faceIdx = (faceIdx + 1) % facesSize;
  }


  //display in blynk to know the state of bonsai
  if(bIsRaining){
    lcd.print(2, 1, "Closed");
  } else {
    lcd.print(2, 1, "Opened");
  }

  delay(500);

}
