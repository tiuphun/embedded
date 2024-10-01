#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Org_01.h"
#include "RTClib.h"
#include "oled.h"

#define ACCESS 7
#define DELY 6
#define BUZZER 5
#define CS_SD 4 
#define SS_PIN 10
#define RST_PIN 9
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RTC_DS1307 rtc;
RFID rfid(SS_PIN, RST_PIN);
// Create a file to store the data
File myFile;

unsigned char buffer[5]; 
unsigned char master[5] = { 0xd3, 0x5b, 0x16, 0x34, 0xaa };
unsigned char i;
String uidString;
// Define check in time
const int checkInHour = 15;
const int checkInMinute = 40;

//Variable to hold user check in
int userCheckInHour;
int userCheckInMinute;

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  Serial.println("RFID initialized");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if(!SD.begin(1, CS_SD)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("MicroSD initialized");

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
  Serial.println("Setup completed");
  
  pinMode(ACCESS, OUTPUT); 
  pinMode(DELY, OUTPUT);  
  pinMode(BUZZER, OUTPUT); 
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
      for (int k = 0; k < 5; k++) {
        Serial.print(buffer[k], HEX);
        Serial.print(" ");
      }
      Serial.println("");

      uidString = String(buffer[0]) + " " + String(buffer[1]) + " " + String(buffer[2]) + " " + String(buffer[3]);

      // Sound the buzzer when a card is read
      tone(BUZZER, 4000); 
      delay(100);        
      noTone(BUZZER);

      for (i = 0; i < 5; i++) {
        if (buffer[i] != master[i]) break;
      }
      
      if (i == 5){               
        Serial.println("Access Granted!");
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

void logCard() {
  // Enables SD card chip select pin
  digitalWrite(CS_SD,LOW);
  myFile = SD.open("data.txt", FILE_READ);
  if (!myFile) {
    // If the file doesn't exist, create it
    Serial.println("File does not exist, creating file.");}
  // Open file
  myFile=SD.open("data.txt", FILE_WRITE);

  // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");
    myFile.print(uidString);
    myFile.print(", ");   

    String line = myFile.readStringUntil('\n');
    // line.trim();  // Remove any trailing newline or whitespace characters
    Serial.println(line);
    // Check if the UID is in the current line
    if (line.startsWith(uidString)) {
      Serial.print("UID found in line: ");
      Serial.println(line);
    }
    else {
      Serial.println("New UID");
    }
      
    
    // Save time on SD card
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.println(now.minute(), DEC);
    
    // Print time on Serial monitor
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.println(now.minute(), DEC);
    Serial.println("sucessfully written on SD card");
    myFile.close();

    // Save check in time;
    userCheckInHour = now.hour();
    userCheckInMinute = now.minute();
  }
  else {
    Serial.println("error opening data.txt");  
  }
  // Disables SD card chip select pin  
  digitalWrite(CS_SD,HIGH);
}

void verifyCheckIn(){ 
  if((userCheckInHour < checkInHour)||((userCheckInHour==checkInHour) && (userCheckInMinute <= checkInMinute))){
    digitalWrite(greenLED, HIGH);
    delay(2000);
    digitalWrite(greenLED,LOW);
    Serial.println("You're welcome!");
  }
  else{
    digitalWrite(redLED, HIGH);
    delay(2000);
    digitalWrite(redLED,LOW);
    Serial.println("You are late...");
  }
}
