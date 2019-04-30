#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
#define SCREEN_HEIGHT 64
#define SCREEN_WIDTH 128
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme;

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
const int sharpLEDPin = 7;
const int sharpVoPin = A0;
float voMeasured = 0;
float calcVoltage = 0;
float dustDensity = 0;

int powerFet = 9;
int builtLed = 13;

void setup() {
  pinMode(powerFet, OUTPUT);
  digitalWrite(powerFet, HIGH);
  pinMode(sharpLEDPin, OUTPUT);
  pinMode(builtLed, OUTPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.fillRect(0,0,128,64, WHITE);
  display.fillRect(3,3,122,58,INVERSE);
  display.drawLine(0, 20, 128, 20, WHITE);
  display.display(); 
  bool status;
  status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
}

void printValue(String text,float value, String units){ //opciona funkcija, nije neophodna
  display.fillRect(3,3,122,16, BLACK);
  display.fillRect(3,22,122,39, BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print(text);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(value, 0);
  display.setTextSize(2);
  display.setCursor(55,43);
  display.print(units);
  display.display();
}
void eraseField(){
  display.fillRect(3,3,122,16, BLACK);
  display.fillRect(3,22,122,39, BLACK);
}

void loop() {


  digitalWrite(sharpLEDPin,LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(sharpVoPin);
  delayMicroseconds(deltaTime);
  digitalWrite(sharpLEDPin,HIGH);
  delayMicroseconds(sleepTime);
  calcVoltage = voMeasured * (3.3 / 1024);
  dustDensity = 0.17 * calcVoltage - 0.1;
  

  //printValue("Temperatura",bme.readTemperature()," C");
  eraseField();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print("Temperatura");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(bme.readTemperature(), 0);
  display.setTextSize(1);
  display.setCursor(80,50);
  display.print(" C");
  display.display();
  
  delay(2000);
 
  //printValue("Relativna Vlaznost",bme.readHumidity()," %");
  eraseField();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print("Relativna Vlaznost");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(bme.readHumidity(), 0);
  display.setTextSize(1);
  display.setCursor(80,50);
  display.print(" %");
  display.display();
  
  delay(2000);

  //printValue("Vazdusni Pritisak",bme.readPressure() / 100.0F," mbar");
  eraseField();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print("Vazdusni Pritisak");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(bme.readPressure() / 100.0F, 0);
  display.setTextSize(1);
  display.setCursor(80,50);
  display.print(" mbar");
  display.display();
  
  delay(2000);


  //printValue("Nadmorska Visiona",bme.readAltitude(SEALEVELPRESSURE_HPA)," m");
  eraseField();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print("Nadmorska Visiona");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(bme.readAltitude(SEALEVELPRESSURE_HPA), 0);
  display.setTextSize(1);
  display.setCursor(80,50);
  display.print(" m");
  display.display();
  
  delay(2000);



  //printValue("Gustina Prasine",dustDensity," ug/m3");
  display.fillRect(3,3,122,16, BLACK);
  display.fillRect(3,22,122,39, BLACK);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(8,8);
  display.print("Gustina Prasine");
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(8,35);
  display.print(dustDensity,1);
  display.setTextSize(1);
  display.setCursor(80,50);
  display.print(" ug/m3");
  display.display();
  
  delay(2000);


  if(millis() > 40000){
  digitalWrite(powerFet, LOW);
  digitalWrite(builtLed, HIGH);
  }
}
