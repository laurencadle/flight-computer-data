#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <SD.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp;

File flightComputerData;
const int chipselect = 10;

void setup() {
  
  //pinMode(LED_BUILTIN, OUTPUT); 
  //digitalWrite(LED_BUILTIN, LOW);
  
  // put your setup code here, to run once:
  Serial.begin(9600);
  //while ( !Serial ) { delay(100); }   
  
  Serial.println(F("BMP280 test"));
  unsigned status;

  status = bmp.begin(0x76);
  
  if (!status) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    Serial.print("SensorID was: 0x"); Serial.println(bmp.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) { delay(10); }
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     
                      Adafruit_BMP280::SAMPLING_X2,    
                      Adafruit_BMP280::SAMPLING_X16,   
                      Adafruit_BMP280::FILTER_X16,      
                      Adafruit_BMP280::STANDBY_MS_500); 
  }

  if (!SD.begin(10)) {  
    Serial.println("SD card initialization failed!");
    while (1) { delay(10); }
  }
  Serial.println("SD card initialized");
  
  flightComputerData = SD.open("data.txt", FILE_WRITE);

  //if (flightComputerData) {
  //  while (flightComputerData.available()) {
  //    Serial.write(flightComputerData.read());
  //  }

  //digitalWrite(LED_BUILTIN, HIGH);

}



void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);  
  //delay(500);                      
  //digitalWrite(LED_BUILTIN, LOW);   
  //delay(500);

  digitalWrite(LED_BUILTIN, HIGH);

  Serial.print(F("Temperature = "));
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  Serial.print(F("Approx altitude = "));
  Serial.print(bmp.readAltitude(1013.25)); 
  Serial.println(" m");

  Serial.println();
  delay(250);

  String datastring = "";
  if (flightComputerData) {
    flightComputerData.println(F("Approx altitude = "));
    flightComputerData.print(bmp.readAltitude(1013.25));
    flightComputerData.println(" m");
    
    flightComputerData.println(F("Approx pressure = "));
    flightComputerData.print(bmp.readPressure());
    flightComputerData.println(" Pa");

    flightComputerData.println(F("Approx temperature = "));
    flightComputerData.print(bmp.readTemperature());
    flightComputerData.println(" *C");

    flightComputerData.flush(); 
  }
}