#include <ESP8266WiFi.h>
#include "DHT.h"
#define DHTPIN          2   //Pin to attach the DHT
#define DHTTYPE DHT22       //type of DTH  
const char* ssid     = "lol";
const char* password = "loooooolo";
const int sleepTimeS = 500; //18000 for Half hour, 300 for 5 minutes etc.
//Get sensor data
float indoortempf = 0;
float indoorhumidity = 0;
float tempc = 0;
///////////////Weather////////////////////////
char server [] = "weatherstation.wunderground.com";
char WEBPAGE [] = "GET /weatherstation/updateweatherstation.php?";
char ID [] = "IBADDRRH12";
char PASSWORD [] = "looooooooooooooooo";
DHT dht(DHTPIN, DHTTYPE);               
           


void setup()
{
  Serial.begin(115200);
  dht.begin();
  delay(1000);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void loop(){  
  //Get sensor data
  float tempc = dht.readTemperature(); 
  float indoortempf =  (tempc * 9.0)/ 5.0 + 32.0; 
  float indoorhumidity = dht.readHumidity();
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
    client.print("&tempf=");
    client.print(indoortempf);
    client.print("&humidity=");
    client.print(indoorhumidity);
    client.print("&softwaretype=H3indoor&action=updateraw&realtime=1&rtfreq=2.5");
    client.println();
    delay(2500); 
    sleepMode();    

}

void sleepMode(){
  Serial.print(F("Sleeping..."));
  ESP.deepSleep(sleepTimeS * 1000000);
}
