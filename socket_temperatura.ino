#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>
#include <DHT.h>
#include "ConexionWifi.h"

// Instancias
DHT dht(D1, DHT11);
WebSocketsServer webSocket(81);
ConexionWifi conexionWifi("nombre de red", "password");

// Variables
String out = "{}";
String outTemp = "{}";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        out = getData();
        Serial.println("connect, send => " + out);
        webSocket.broadcastTXT(out);
      }
      break;

    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      break;

    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      hexdump(payload, length);
      break;
  }
}

String getData() {
  float humedad = dht.readHumidity();
  float temperature = dht.readTemperature();
  return "{\"data\":{\"temperatura\":" + String(temperature) + ",\"humedad\":" + String(humedad) + "}}";
}

void setup() {

  Serial.begin(115200);
  dht.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  conexionWifi.conectar();
}

void loop() {

  webSocket.loop();

  out = getData();

  if (out != outTemp) {
    Serial.println(out);
    webSocket.broadcastTXT(out);
    outTemp = out;
  }

  delay(500);
}