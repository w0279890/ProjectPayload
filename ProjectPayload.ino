#include <SD.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h> 

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#include "GPS.h"
#include "myFunctions.h"

Servo releaseServo;
int timer = 0;
const int servoPin = 8; 
const int chipSelect = 10;

void setup() {
  
  /* Initialise GPS/GSM shield */
  pinMode(3,OUTPUT);//The default digital driver pins for the GSM and GPS mode
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  delay(1500);
  digitalWrite(5,LOW);
  digitalWrite(3,LOW);//Enable GSM mode
  digitalWrite(4,HIGH);//Disable GPS mode
  delay(2000);
  Serial.begin(9600); 
  delay(5000);//GPS ready

  /* Activate GPS */
  activateGPS();
  
  /* Initialise the sensor */
  bmp.begin();
  
  /* Initialise the SD card */
  SD.begin(chipSelect);

  /* Servo control */
  pinMode(servoPin, OUTPUT);
  releaseServo.attach(servoPin);
  releaseServo.write(120);
  releaseServo.detach();
}

void loop() {
  
  /* Write GPS data to the SD card */
  UTC();
  latitude();
  lat_dir();
  longitude();
  lon_dir();
  altitude();

  /* Write sensor data to the SD card */
  savePressure();
  saveTemperature();
  saveAltitude();
  delay(3000);
  timer++;

  /*Release servo*/
  if (millis() > 10000) {
    payloadRelease(releaseServo, servoPin);
  }

  /* Text location every couple of minutes*/
  if (timer == 6) {
      sendMessage();
      activateGPS();
      timer = 0;
  }
}
