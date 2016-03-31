int letGoPlz = 0;
byte gsmDriverPin[3] = { 3, 4, 5 };

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

/* Functions */
void writeSD(float dataString, String unit) {
  
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(unit);
    dataFile.println(dataString);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void payloadRelease(Servo myservo, const int servoPin) {

  if (letGoPlz == 0) {
    myservo.attach(servoPin);
    myservo.write(0);
    delay(1000);
    myservo.detach();
    delay(15);
    letGoPlz++;
  }
}

/* Saving sensor data to the SD card functions */
void savePressure() {

  /* Get a new sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);
  
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure) {
    
    /* Save atmospheric pressue in hPa */
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("hPa: ");
      dataFile.println(event.pressure);
      dataFile.close();
    }

    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}

void saveTemperature() {

  /* Get a new sensor event */
  float temperature;
  bmp.getTemperature(&temperature);
  
  /* Display the results (barometric pressure is measure in hPa) */
  if (temperature) {
    
    /* Save temperature in celsius */
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("Tmp: ");
      dataFile.println(temperature);
      dataFile.close();
    }

    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}

void saveAltitude() {

  /* Get a new sensor event */
  sensors_event_t event;
  bmp.getEvent(&event);
  float seaLevelPressure = 1005;
  float alti = bmp.pressureToAltitude(seaLevelPressure,event.pressure);
  
  /* Display the results (barometric pressure is measure in hPa) */
  if (alti) {
    
    /* Save altitude pressue in meters */
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.print("ALT: ");
      dataFile.println(alti);
      dataFile.println("");
      dataFile.println("");
      dataFile.close();
    }

    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
  }
}

/* Switch between GSM and GPS functions*/
void activateGPS() {

  Serial.println("");
  Serial.println("");
  Serial.println("AT");   
  delay(2000);
  //turn on GPS power supply
  Serial.println("AT+CGPSPWR=1");
  delay(1000);
  //reset GPS in autonomy mode
  Serial.println("AT+CGPSRST=1");
  delay(1000);
 
  digitalWrite(4,LOW);//Enable GPS mode
  digitalWrite(3,HIGH);//Disable GSM mode
  delay(5000);
}

void sendMessage() {
  
  digitalWrite(4,HIGH);//Disable GPS mode
  digitalWrite(3,LOW); //Enable GSM mode
  delay(5000);
  
  Serial.println("");
  Serial.println("");
  Serial.println("AT"); //Send AT command  
  delay(2000);
  Serial.println("AT");   
  delay(2000);
  
  
  Serial.println("AT+CMGF=1");
  delay(1000);
  Serial.println("AT+CMGS=\"19024144910\""); //Send message to this number
  delay(1000);
  
  Serial.print("Lat:  "); //the message will be latitute
  Serial.print(latBuff, 7);
  Serial.print(" ");
  Serial.println(lat_val);
  
  Serial.print("Lon: -"); //the message will be longitute
  Serial.print(lonBuff, 7);
  Serial.print(" ");
  Serial.println(lon_val);
  
  delay(5000);
  Serial.write(26);
}
