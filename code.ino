#include <ArduinoJson.h>    // Lib to format JSON Document
#include "EspMQTTClient.h"  // Lib to comunicate MQTT from ESP
#include <WiFi.h>

//Doc: https://github.com/plapointe6/EspMQTTClient

char  FilaJson[100];
char  HospJson[100];


int Pin_pot1 = 34;
int Pin_pot2 = 35;

float FilaPessoas;
int Hospital;



//MQTT and WiFi configuration
EspMQTTClient client
(
  "Wokwi-GUEST",                //nome da sua rede Wi-Fi
  "",           //senha da sua rede Wi-Fi
  "mqtt.tago.io",       //Endereço do servidor MQTT
  "Default",            //User é sempre default pois vamos usar token
  "b2e8dd20-062d-49c6-a440-763ffe8637df",              // Código do Token
  "esp32",              //Nome do device
  1883                  //Porta de comunicação padrao
);

void setup()
{
  
  pinMode(Pin_pot1, INPUT);
  pinMode(Pin_pot2, INPUT);
   Serial.begin(9600);
  
  Serial.println("Conectando WiFi");
  while(!client.isWifiConnected()){
    Serial.print('.');
    client.loop();
    delay(1000);
  }
  Serial.println("Conectado!");

  Serial.println("Conectando com o broker MQTT");
  while(!client.isMqttConnected()){
    Serial.print('.');
    client.loop();
    delay(1000);
  }
  Serial.println("Conectado!");
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{}

//loop do programa
void loop()
{

  FilaPessoas = map(analogRead(Pin_pot1),0,1023,0,25);
  Hospital = map(analogRead(Pin_pot2),0,1023,0,4);

  Serial.println("Pessoas:");
  Serial.println(FilaPessoas);
  Serial.println("Hospital:");
  Serial.println(Hospital);



  StaticJsonDocument<300> documentoJson;
  documentoJson["variable"] = "Fila";
  documentoJson["value"] = FilaPessoas;
  serializeJson(documentoJson, FilaJson);

  documentoJson["variable"] = "Hospital";
  documentoJson["value"] = Hospital;
  serializeJson(documentoJson, HospJson);

  Serial.println("Enviando os dados");
  Serial.println(FilaJson);
  Serial.println(HospJson);
  
  client.publish("topicHap", FilaJson); 
  client.publish("topicHap", HospJson); 
  delay(5000);

  client.loop();
}

