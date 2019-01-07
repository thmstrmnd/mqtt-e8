/* MQTT*/

#include <ESP8266WiFi.h>                          // WiFi Header  
#include <PubSubClient.h>                         // MQTT Library                         
#include <ESP8266HTTPClient.h>                    // HTTP Header
#include <ArduinoJson.h>                          // JSON formatter header | https://bblanchon.github.io/ArduinoJson/                        

const char* ssid = "ZTEH108N_3C01E";              
const char* password =  "X9425TE9";

const char* mqttServer = "m14.cloudmqtt.com";
const int mqttPort = 16120;
const char* mqttUser = "username";                // Set username
const char* mqttPassword = "password";            // Set password

const char* deviceID = "Esp8266Client02";


WiFiClient espClient;                              // Initiate WiFiClient Library
PubSubClient client(espClient);

int serialValue = 0;
int serialValueConv = 0;
int pinOut04 = D4;
int pinOut03 = D3;
char receivedChar = 0;

void setup() {
  pinMode(pinOut04, OUTPUT);
  pinMode(pinOut03, OUTPUT); 
  
  Serial.begin(115200);
  WiFi.begin(ssid, password);                     // Initialize WiFi Connection 
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }  
  
  Serial.println("Connected to the WiFi network");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    
    if (client.connect(deviceID, mqttUser, mqttPassword )) {        
      Serial.println("connected");  
    } 
    
    else  {
      Serial.print("failed with state ");
      Serial.print(client.state());
      
      delay(2000);
    }
  }
 
   client.publish("OnAndOff", "Hello from ESP8266Client02");          // Unique user id per device  
   client.subscribe("OnAndOff");
 
}
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
    
    if (receivedChar == '0')
      digitalWrite(pinOut03, LOW);
    else if (receivedChar == '1')
      digitalWrite(pinOut03, HIGH);
  }
 
  Serial.println();
  Serial.println("-----------------------");
}   

void loop() {

  if(WiFi.status() == WL_CONNECTED)  {            
    
    /* Power and Internet Connection Display */
    digitalWrite(pinOut04, HIGH);                 

    /* MQTT Client Library Connection */
    client.loop();                                // Establishes frequent connection with the MQTT broker to check messages            
   
    /* Serial Input Tests */                     
    serialValue = Serial.read();                  // Serial Input   
    serialValueConv = serialValue - 48;           // Temporary Conversion from ASCII           

    Serial.print("From Serial: ");
    Serial.println(serialValue);
              
      if (serialValueConv == 1) {
        digitalWrite(pinOut03, HIGH);   
      }
              
      else if (serialValueConv == 0)  {
        digitalWrite(pinOut03, LOW);        
      }
      
                                       
    delay(1000);        
  }
  
    else  {
      Serial.println("Error in WiFi connection");   // Throw error in connection
  }
}
