
#include <Wire.h>
//#include <LiquidCrystal_I2C.h>

//LiquidCrystal_I2C lcd(0x3F, 16, 2); // I2C: 0x3F, 16x2 LCD
//VO →5.0V
//GND →GND
//DO →D13
//AO →A1
int nRainIn = A1;
int nRainDigitalIn = 13;
int nRainVal;
boolean bIsRaining = false;
String strRaining;
int Motor1 = 5;
int Motor2 = 6;


void setup()
{
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  Serial.begin(9600);
  Serial.println("LM393");
}

void loop()
{
  delay(3000);

  nRainVal = digitalRead(nRainIn);
  bIsRaining = !(digitalRead(nRainDigitalIn));

  if (bIsRaining) {
    strRaining = "YES";
    Serial.print("Rain  :  ");
    Serial.print(strRaining);
    Serial.println("  ");  
      //逆転（正転）伸びる
    digitalWrite(Motor1, HIGH); //５番ピンに６～２５５を出力
    digitalWrite(Motor2, LOW);
    Serial.print("のびた");
    delay(10000);
  }
  else {
    strRaining = "NO"; 
    Serial.print("Rain  :  ");
    Serial.print(strRaining);
    Serial.println("  "); 
    //正転（逆転　縮む
    digitalWrite(Motor1, LOW);
    digitalWrite(Motor2, HIGH); //6番ピンに1を出力
    Serial.print("縮んだ");
    delay(10000);
  }
//静止
  digitalWrite(Motor1, LOW);
  digitalWrite(Motor2, LOW);
  delay(1000);

  Serial.print("Level :  ");
  Serial.print(nRainVal);
  Serial.println("  ");
  delay(1000);
}

