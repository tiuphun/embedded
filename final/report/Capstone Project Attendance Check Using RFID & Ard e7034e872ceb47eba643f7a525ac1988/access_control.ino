#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Org_01.h"
#include "RTClib.h"
#include "oled.h"

#define ACCESS 8
#define DELY 7
/*
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS: Pin 10
* RST: Pin 9
*/
#define SS_PIN 10
#define RST_PIN 9


RFID rfid(SS_PIN, RST_PIN);
 
unsigned char buffer[5]; 
unsigned char master[5] = { 0xd3, 0x5b, 0x16, 0x34, 0xaa };
char username[3][20] = {"Vuducan"};
 
unsigned char i;

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

// RTC
RTC_DS1307 rtc;


void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("RFID initialized");
  Serial.println("Reading...");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

	delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  Serial.println("OLED initialized");
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  // text display tests
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Failure is always an option");
    
  // Clear the buffer.
  display.clearDisplay();
  Serial.println("Setup complete");
  
  pinMode(ACCESS, OUTPUT); 
  pinMode(DELY, OUTPUT);  
}
 
void loop()
{
  DateTime now = rtc.now();
  idleScreen();

  if (rfid.isCard()){
    if (rfid.readCardSerial()){
      for (i = 0; i < 5; i++) {
        buffer[i] = rfid.serNum[i];
      }
      Serial.println("Card input detected: ");
      for (i = 0; i < 5; i++) {
        if (buffer[i] != master[i]) break;
      }
      if (i == 5){               
        Serial.println("Access");
        digitalWrite(ACCESS, HIGH);
        delay(2000);
        digitalWrite(ACCESS, LOW);
      } else {
        digitalWrite(DELY, HIGH);
        delay(2000);
        digitalWrite(DELY, LOW);
      }
    } 
    rfid.halt();
  }
}

void idleScreen(){
  DateTime now = rtc.now();
    display.clearDisplay();
    display.drawBitmap(0, 0,  cat_bg, 128, 64, 1);
    display.drawLine(0, 10, 128, 10, 1);
    //display.drawLine(70, 0, 70, 10, 1);
    display.setTextColor(SH110X_WHITE);
    display.setFont(NULL);
    display.setTextSize(1);
    display.setCursor(0, 2);
    display.print("B1-301");
    display.setCursor(70, 1);
    display.print(now.day(), DEC);
    display.print("-");
    display.print(now.month(), DEC);
    display.print("-");
    display.print(now.year(), DEC);
    display.setCursor(46, 40);
    display.setFont(&Org_01);
    display.setTextSize(2);
    display.print(now.hour(), DEC);
    display.print(":");
    display.print(now.minute(), DEC);
    display.setCursor(30, 62);
    display.setTextSize(1);
    display.setFont(NULL);
    display.print("Good morning!");
    display.display();
}

void accessScreen(){
  DateTime now = rtc.now();
    display.clearDisplay();
    display.drawBitmap(0, 0,  cat_bg, 128, 64, 1);
    display.drawLine(0, 10, 128, 10, 1);
    display.drawLine(70, 0, 70, 10, 1);
    display.setTextColor(SH110X_WHITE);
    display.setFont(NULL);
    display.setTextSize(1);
    display.setCursor(0, 2);
    display.print("B1-301");
    display.setCursor(70, 1);
    display.print(now.day(), DEC);
    display.print("-");
    display.print(now.month(), DEC);
    display.print("-");
    display.print(now.year(), DEC);
    display.setCursor(20, 40);
    display.setTextSize(2);
    display.print(username[0]);
    
    display.setCursor(30, 62);
    display.setTextSize(1);
    display.setFont(NULL);
    display.print("Access Granted!");
    display.display();
}