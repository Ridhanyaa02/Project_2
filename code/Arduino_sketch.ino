#include <Wire.h>       
#include <Adafruit_Sensor.h>
//#include <Adafruit_BMP280.h> 
#include <Adafruit_BME280.h>
#include <DHT.h>       

//DHT setup
#define DHTPIN 8         
#define DHTTYPE DHT11    
DHT dht(DHTPIN, DHTTYPE);

//BMP setup
//Adafruit_BMP280 bmp; 
Adafruit_BME280 bme;
//Adafruit_BME280 bme(BME_CS); // hardware SPI   

void setup() {
  Serial.begin(9600);    
  dht.begin();          

  //Initialize BMP
  if (!bme.begin(0x76)) { 
    Serial.println("Not connected");
 
  }
}

void loop() {
  //Read DHT11
  float dhtTemp = dht.readTemperature(); 
  float dhtHum = dht.readHumidity();     

  //Read BMP
  float bmpTemp = bme.readTemperature(); 
  float bmpPress = bme.readPressure() / 100.0F; 

  //Check DHT readings
  if (isnan(dhtTemp) || isnan(dhtHum)) {
    Serial.println("Error DHT11");
  } else {
    //Send to serial
    Serial.print("DHT11 Temp: ");
    Serial.print(dhtTemp);
    Serial.print(" °C, Humidity: ");
    Serial.print(dhtHum);
    Serial.print(" %, BMP280 Temp: ");
    Serial.print(bmpTemp);
    Serial.print(" °C, Pressure: ");
    Serial.print(bmpPress);
    Serial.println(" hPa");
  }

  delay(60000); //wait 1 min
}
