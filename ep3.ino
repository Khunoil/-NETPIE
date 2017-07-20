
// Microgear
#include <AuthClient.h>
#include <debug.h>
#include <MicroGear.h>
#include <MQTTClient.h>
#include <PubSubClient.h>
#include <SHA1.h>


//Node WIFI
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

// DHT11
#include <DHT.h>
#include <DHT_U.h>


const char* ssid     = "iPhoneJT";
const char* password = "jak123456";

#define APPID   " " //นำ APPID จาก NETPIE มาใส่
#define KEY     " " //APP KEY(Device Key) ของ NETPIE
#define SECRET  " " //APP SECRET(Device Key) ของ NETPIE

#define ALIAS   "NodeMCU1"
#define TargetWeb "HTML_web"

DHT dht(9, DHT11);

int S = A0;




WiFiClient client;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() 
{
 
  
    microgear.on(CONNECTED,onConnected);

    dht.begin();
    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

   
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{

  
  
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("connected");

        Serial.println("Clone Fleur");
       int Humidity = dht.readHumidity();
       int Temp = dht.readTemperature();  // Read temperature as Celsius (the default)
        Serial.print(" Temperature: ");
        Serial.print(Temp);
        Serial.print(" C ");
        Serial.print(" Humidity: ");
        Serial.print(Humidity);
        Serial.print(" RH ");

        int soil= analogRead(S);
        // for Arduino
        //Checksoil = map(Checksoil,0,710,100,0);
        // for NodeWIFI
        soil = map(soil,0,1024,100,0);
        Serial.print(" Moisture: ");
        Serial.print(soil);
        Serial.println(" %");

        
         

        char msg[128];
    sprintf(msg,"%d,%d,%d", (int)Temp , (int)Humidity , (int)soil );
       Serial.println(msg);    

       microgear.chat(TargetWeb , msg);
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(1500);
}
