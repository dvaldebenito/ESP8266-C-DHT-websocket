#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;

class ConexionWifi {

private:
  char* nombre;
  char* password;

public:
  ConexionWifi(char* _nombre, char* _password) {
    nombre = _nombre;
    password = _password;
  }
  
  void conectar() {
    WiFiMulti.addAP(nombre, password);
    while (WiFiMulti.run() != WL_CONNECTED) {
      delay(100);
    }
    Serial.println(WiFi.localIP());
  }
  
};