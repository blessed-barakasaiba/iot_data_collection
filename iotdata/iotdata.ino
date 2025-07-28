#include "DHT.h"
#include <WiFi.h> 

#include <HTTPClient.h>
#include <ArduinoJson.h>

#define LIGHTPIN 32 
#define PIR_PIN 27


int dhtPin = 14; 
#define DHTTYPE DHT11
DHT dht(dhtPin,DHTTYPE);

const char* ssid = "BLESSED";
const char* password = "12345678B";
const char* deviceid = "1234";

const char* serverURL = "http://192.168.43.99:8000/api/data/";

void setup() {
  Serial.begin(115200);
  dht.begin();

  //start wifi connection
  WiFi.begin(ssid,password);
  Serial.println("Connecting to Wifi");

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(PIR_PIN, INPUT);

}

void loop() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int sensorValue = analogRead(LIGHTPIN);
  float brightnessPercent = map(sensorValue, 0, 4095, 0, 100);
  int motion = digitalRead(PIR_PIN);
  const char*  detected;



  if(motion == HIGH){
    detected = "Motion detected";
  }
  else{
    detected = "No motion detected";
  }



  if(isnan(temp) || isnan(hum)){
    Serial.println("Failed to read from DHT11");
  }
  else{
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" °C | Humidity: ");
    Serial.print(hum);
    Serial.println(" %");
    Serial.print(brightnessPercent);
    Serial.println(" %");

  }
  sendDataToApi(temp, hum, brightnessPercent, detected,deviceid);
  delay(2000);
}


void sendDataToApi(float temperature, float humidity, float brightnessPercent, const char*  detected, const char*deviceid){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(serverURL);
    http.addHeader("Content-Type", "application/json");


    //craete json
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["brightnessPercent"]= brightnessPercent;
    doc["detected"] = detected;
    doc["deviceid"]=deviceid;

    String jsonString;
    serializeJson(doc, jsonString);

    //print
    Serial.println("Sending: " + jsonString);

    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("HTTP Response: " + String(httpResponseCode));
            Serial.println("Response: " + response);
        } else {
            Serial.println("Error sending POST request");
            Serial.println("HTTP Response code: " + String(httpResponseCode));
        }

    http.end();    

  }
  else{
    Serial.println("WiFi not connected");
  }
}
