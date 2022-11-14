#include <WiFi.h>
#include "ThingSpeak.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
const char* ssid = "Shelby"; // your network SSID (name)
const char* password = "djhouse008"; // your network password
WiFiClient client;
unsigned long myChannelNumber = 1877729;
const char * myWriteAPIKey = "FOUISEO16CUTE1KC";
// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;
uint32_t delayMS;

unsigned long TH;
unsigned long TL;
unsigned long C;
int CO2ppm;
// Variable to hold random value
float temperatureC;
float HUMIDITYC;
#define DHTPIN 4
#define CO2PIN 5
#define DHTTYPE  DHT22
DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(CO2PIN, INPUT);
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client); 
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
   sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}
 

// Initialize ThingSpeak

void loop() {
   if ((millis() - lastTime) > timerDelay) {

 // Connect or reconnect to WiFi
 if(WiFi.status() != WL_CONNECTED){
 Serial.print("Attempting to connect");
 while(WiFi.status() != WL_CONNECTED){
 WiFi.begin(ssid, password);
 delay(5000);
 }
 Serial.println("\nConnected.");
 }
 // Get a new temperature reading
 
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
   temperatureC = event.temperature ;
   Serial.println(temperatureC);
    Serial.println(F("°C"));
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    HUMIDITYC = event.relative_humidity;
    Serial.println(HUMIDITYC);
    Serial.println(F("%"));
  }
  //CO2 readings
  TH = pulseIn(CO2PIN, HIGH, 2008000)/1000;
  TL=1004-TH;
  Serial.print("TH:");
  Serial.println(TH);
  Serial.print("TL:");
  Serial.println(TL);
  C=TH+TL;
  Serial.println(C);
  CO2ppm = ((2000)*(TH-2))/(TH+TL-4);
  Serial.print("CO2PPM:");
  Serial.println(CO2ppm);
  if (CO2ppm >= 2000)
  {
    CO2ppm = 400;
  }
 //Serial.print("Temperature (ºC): ");
 //Serial.println(temperatureC);
 // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to
 // pieces of information in a channel. Here, we write to field 1.

 ThingSpeak.setField(1,temperatureC);
 ThingSpeak.setField(2,HUMIDITYC);
 ThingSpeak.setField(3,CO2ppm);
 int x = ThingSpeak.writeFields(myChannelNumber,myWriteAPIKey);
 if(x == 200){
 Serial.println("Channel update successful.");
 }
 else{
 Serial.println("Problem updating channel. HTTP error code " + String(x));
 }
 lastTime = millis();
 }
// delay(2000);
}
