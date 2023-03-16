#include <ESP8266WiFi.h>
#include <Wire.h>

const char* ssid = "Taekwondo";
const char* password = "password";

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  WiFi.softAP(ssid, password);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = server.available();
  float pressure1 = 10.50;
  float pressure2 = 20.50;
  int person1 = 10;
  int person2 = 20;

  Wire.beginTransmission(0x60);
  Wire.write(0x08);
  Wire.endTransmission();

  Wire.requestFrom(0x60, 2);
  if (Wire.available() == 2) {
    int pressure1_raw = Wire.read() << 8 | Wire.read();
    pressure1 = pressure1 + pressure1_raw / 16384.0;
    person1++;
    Serial.println(person1);
  }
  
  Wire.beginTransmission(0x61);
  Wire.write(0x08);
  Wire.endTransmission();

  Wire.requestFrom(0x61, 2);
  if (Wire.available() == 2) {
    int pressure2_raw = Wire.read() << 8 | Wire.read();
    pressure2 = pressure2 + pressure2_raw / 16384.0;
    person2++;
  }

  if (client) {
    Serial.println("New client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);

    if (request.indexOf("/pressure1") != -1) {
      sendPressure1(client, pressure1);
    } else if (request.indexOf("/pressure2") != -1) {
      sendPressure2(client, pressure2);
    }else if (request.indexOf("/person1") != -1) {
      sendPerson1(client, person1);
    } else if (request.indexOf("/person2") != -1) {
      sendPerson2(client, person2);
    } else {
      client.println("HTTP/1.1 404 Not Found");
      client.println("Content-Type: text/html");
      client.println("Access-Control-Allow-Origin: *");
      client.println("Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept");
      client.println("");
      client.println("<html><body><h1>Page not found</h1></body></html>");
    }
  }
}

void sendPressure1(WiFiClient client, float pressure1) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.println(pressure1);
}

void sendPressure2(WiFiClient client, float pressure2) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.print(pressure2);
}

void sendPerson1(WiFiClient client, int person1) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.print(person1);
}

void sendPerson2(WiFiClient client, int person2) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("");
  client.print(person2);
}