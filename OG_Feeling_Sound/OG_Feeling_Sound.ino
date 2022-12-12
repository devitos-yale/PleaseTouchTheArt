#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiScan.h>
#include <ETH.h>
#include <WiFiClient.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiGeneric.h>

//#include "WiFi.h"
//#include "WiFiUdp.h"

// WiFi network name and password:
const char * networkName = "yale wireless";
const char * networkPswd = "";

//IP address to send UDP data to:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "172.29.129.22";
const int udpPort = 8092;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

byte command[27] = {0x20, 0x00, 0x00, 0x00, 0x16, 0x02, 0x62, 0x3A, 0xD5, 0xED, 0xA3, 0x01, 0xAE, 0x08, 0x2D, 0x46, 0x61, 0x41, 0xA7, 0xF6, 0xDC, 0xAF, 0xD3, 0xE6, 0x00, 0x00, 0x1E};


// variables will change:
int buttonState1 = 0;  // variable for reading the pushbutton status
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;

unsigned char val = 0;

void setup() {
  Serial.begin(115200);

  int numberOfNetworks = WiFi.scanNetworks();
 
  for(int i =0; i<numberOfNetworks; i++){
 
      Serial.print("Network name: ");
      Serial.println(WiFi.SSID(i));
      Serial.print("Signal strength: ");
      Serial.println(WiFi.RSSI(i));
      Serial.println("-----------------------");
 
  }

  connectToWiFi(networkName, networkPswd);

  
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);

  pinMode(11, OUTPUT);

 
}

void loop() {
  // read the state of the pushbutton value:
  int key = 5000;

  buttonState1 = digitalRead(2);
  buttonState2 = digitalRead(4);
  buttonState3 = digitalRead(5);
  buttonState4 = digitalRead(16);

  int p1 = analogRead(33); //P1 sensor
  delay(10); // delay in between analog reads for stability
  int p2 = analogRead(32); //P2 sensor
  delay(10);
  int p3 = analogRead(35); //P3 sensor

  //only send data when connected
  if(connected){
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.print(key);  // USES .print INSTEAD OF .write
    udp.endPacket();
    
    udp.beginPacket(udpAddress,udpPort);
    udp.print(buttonState1);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(buttonState2);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(buttonState3);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(buttonState4);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(p1);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(p2);  // USES .print INSTEAD OF .write
    udp.endPacket();

    udp.beginPacket(udpAddress,udpPort);
    udp.print(p3);  // USES .print INSTEAD OF .write
    udp.endPacket();
  }
  //Wait for 1 second
  delay(10);
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);

  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
    }
}
