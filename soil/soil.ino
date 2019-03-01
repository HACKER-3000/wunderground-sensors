#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
const char* ssid     = "lol";
const char* password = "alsolol";
const int sleepTimeS = 300; //18000 for Half hour, 300 for 5 minutes etc.
//Get sensor data
float soiltempf = 0;
float soilmoisture = 0;
float rawsoilmoisture = 0;
// device index
#define DEVICE_INDEX 0

// Data wire is plugged into port D2 on the ESP8266
#define ONE_WIRE_BUS D2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

///////////////Weather////////////////////////
char server [] = "weatherstation.wunderground.com";
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";
char ID [] = "IBADDRRH12";
char PASSWORD [] = "looooool";

void setup() {
  sensors.begin();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

}
void loop() {
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  soiltempf = sensors.getTempFByIndex(DEVICE_INDEX);
  rawsoilmoisture = analogRead(A0);
  //Send data to Weather Underground
  Serial.print("connecting to ");
  Serial.println(server);
  WiFiClient client;
  if (!client.connect(server, 80)) {
    Serial.println("Conection Fail");
    return;
  }
  client.print(WEBPAGE);
  client.print("ID=");
  client.print(ID);
  client.print("&PASSWORD=");
  client.print(PASSWORD);
  client.print("&dateutc=");
  client.print("now");
  client.print("&soiltempf=");
  client.print(soiltempf);
  client.print("&soilmoisture =");
  client.print(soilmoisture);
  client.print("&softwaretype=H3soilsens&action=updateraw&realtime=1&rtfreq=2.5");
  client.println();
  delay(2500);
  sleepMode();

}

void sleepMode() {
  Serial.print(F("Sleeping..."));
  ESP.deepSleep(sleepTimeS * 1000000);
}
