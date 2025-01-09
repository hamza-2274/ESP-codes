#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "ESP8266_LED_Control";
const char* password = "12345678";

const int ledPins[] = {16, 5, 4, 0, 2, 14, 12, 13, 15};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

ESP8266WebServer server(80);
String currentPattern = "off";

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  WiFi.softAP(ssid, password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.on("/chaser", handleChaser);
  server.on("/blink", handleBlinkAll);
  server.on("/altblink", handleAltBlink);
  server.on("/off", handleOff);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = "<html>\n";
  html += "<head><title>ESP8266 LED Control</title></head>\n";
  html += "<body>\n";
  html += "<h1>Wi-Fi Controlled LED Patterns</h1>\n";
  html += "<button onclick=\"location.href='/chaser'\">Chaser</button><br><br>\n";
  html += "<button onclick=\"location.href='/blink'\">Blink All</button><br><br>\n";
  html += "<button onclick=\"location.href='/altblink'\">Alternating Blink</button><br><br>\n";
  html += "<button onclick=\"location.href='/off'\">Turn Off</button><br><br>\n";
  html += "</body></html>\n";
  server.send(200, "text/html", html);
}

void handleChaser() {
  currentPattern = "chaser";
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(ledPins[j], HIGH);
      delay(200);
      digitalWrite(ledPins[j], LOW);
    }
  }
  server.send(200, "text/plain", "Chaser Pattern Activated");
}

void handleBlinkAll() {
  currentPattern = "blink";
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(500);
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    delay(500);
  }
  server.send(200, "text/plain", "Blink All Pattern Activated");
}

void handleAltBlink() {
  currentPattern = "altblink";
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < numLeds; j += 2) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(500);
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(ledPins[j], LOW);
    }
    for (int j = 1; j < numLeds; j += 2) {
      digitalWrite(ledPins[j], HIGH);
    }
    delay(500);
    for (int j = 0; j < numLeds; j++) {
      digitalWrite(ledPins[j], LOW);
    }
  }
  server.send(200, "text/plain", "Alternating Blink Pattern Activated");
}

void handleOff() {
  currentPattern = "off";
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  server.send(200, "text/plain", "All LEDs Turned Off");
}
