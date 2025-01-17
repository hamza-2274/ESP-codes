#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266_Light_Control";
const char* password = "12345678";

// GPIO Pins for LEDs
const int redLight = D2;
const int blueLight = D0;
const int greenLight = D1;
const int multiColorLights[] = {D3, D4, D5};
const int numMultiColorLights = sizeof(multiColorLights) / sizeof(multiColorLights[0]);

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(redLight, OUTPUT);
  pinMode(blueLight, OUTPUT);
  pinMode(greenLight, OUTPUT);
  for (int i = 0; i < numMultiColorLights; i++) {
    pinMode(multiColorLights[i], OUTPUT);
  }

  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/green", handleGreenLight);
  server.on("/red", handleRedLight);

  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

void handleGreenLight() {
  digitalWrite(greenLight, HIGH);
  digitalWrite(redLight, LOW);
  digitalWrite(blueLight, LOW);
  for (int i = 0; i < numMultiColorLights; i++) {
    digitalWrite(multiColorLights[i], HIGH);
  }
  server.send(200, "text/plain", "Green light activated. All multi-color lights turned ON.");
}

void handleRedLight() {
  digitalWrite(redLight, HIGH);
  digitalWrite(greenLight, LOW);
  for (int i = 0; i < numMultiColorLights; i++) {
    digitalWrite(multiColorLights[i], LOW);
  }

  server.send(200, "text/plain", "Red light activated. All multi-color lights turned OFF. Blue light blinking.");

  for (int i = 0; i < 100; i++) { // Blink blue light for 5 seconds (2 blinks per second)
    digitalWrite(blueLight, HIGH);
    delay(250);
    digitalWrite(blueLight, LOW);
    delay(250);
  }
}
