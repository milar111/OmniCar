#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Hash.h>

const char* ssid = "OMNI BOT";
const char* password = "123456789";

// Motor pins
#define AIN1 15
#define AIN2 2
#define BIN1 16
#define BIN2 4
#define CIN1 17
#define CIN2 5

char Data;

void setupMotors() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(CIN1, OUTPUT);
  pinMode(CIN2, OUTPUT);
  stopMotors();
}

void setup() {
  Serial.begin(9600);
  setupMotors();
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(myIP);

  server.on("/", []() {
    server.send(200, "text/html", INDEX_HTML);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  checkMotors();
  delay(5000); // Delay to avoid spamming the serial output
  webSocket.loop();
  server.handleClient();
}

void checkMotors() {
  if (Data == 'F') moveForward();
  else if (Data == 'B') moveBackward();
  else if (Data == 'L') turnLeft();
  else if (Data == 'R') turnRight();
  else if (Data == 'S') stopMotors();
  else if (Data == 'a') topRight();
  else if (Data == 'b') topLeft();
  else if (Data == 'c') bottomRight();
  else if (Data == 'd') bottomLeft();
  else if (Data == 'o') goLeft();
  else if (Data == 'f') goRight();
}

// WebSocket server
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer server(80);

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0">
    <title>OMNI BOT</title>
    <style>
      body {
        background-color: #343a40;
        font-family: Arial, Helvetica, Sans-Serif;
        color: #000000;
      }
      .button {
        border: none;
        color: black;
        padding: 17px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 16px;
        margin: 4px 2px;
        cursor: pointer;
        border-radius: 12px;
        width: 100%;
      }
      .green { background-color: #28a745; }
      .red { background-color: #dc3545; }
      .yellow { background-color: #ffc107; }
      .blue { background-color: #007bff; }
      .button-container {
        text-align: center;
        margin: 20px 0;
      }
    </style>
    <script>
      var websock;
      function start() {
        websock = new WebSocket('ws://' + window.location.hostname + ':81/');
        websock.onopen = function(evt) { console.log('websock open'); };
        websock.onclose = function(evt) { console.log('websock close'); };
        websock.onerror = function(evt) { console.log(evt); };
        websock.onmessage = function(evt) {
          console.log(evt);
          var e = document.getElementById('ledstatus');
          if (evt.data === 'ledon') {
            e.style.color = 'red';
          } else if (evt.data === 'ledoff') {
            e.style.color = 'black';
          } else {
            console.log('unknown event');
          }
        };
      }
      function buttonclick(e) {
        websock.send(e.id);
      }
    </script>
  </head>
  <body onload="start();">
    <div class="button-container">
      <button id="F" class="button green" onclick="buttonclick(this);">Forward</button>
    </div>
    <div class="button-container">
      <button id="L" class="button green" onclick="buttonclick(this);">Left</button>
      <button id="S" class="button red" onclick="buttonclick(this);">Stop</button>
      <button id="R" class="button green" onclick="buttonclick(this);">Right</button>
    </div>
    <div class="button-container">
      <button id="B" class="button green" onclick="buttonclick(this);">Backward</button>
    </div>
    <div class="button-container">
      <button id="a" class="button blue" onclick="buttonclick(this);">Forward Right</button>
      <button id="b" class="button blue" onclick="buttonclick(this);">Forward Left</button>
      <button id="c" class="button blue" onclick="buttonclick(this);">Backward Right</button>
      <button id="d" class="button blue" onclick="buttonclick(this);">Backward Left</button>
    </div>
    <div class="button-container">
      <button id="o" class="button yellow" onclick="buttonclick(this);">CCW</button>
      <button id="f" class="button yellow" onclick="buttonclick(this);">CW</button>
    </div>
  </body>
</html>
)rawliteral";

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * Payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      break;
    }
    case WStype_TEXT:
      Data = Payload[0];
      Serial.println(Data);
      webSocket.broadcastTXT(Payload, length);
      break;
    case WStype_BIN:
      hexdump(Payload, length);
      webSocket.sendBIN(num, Payload, length);
      break;
    default:
      break;
  }
}

// Motor control functions

// Stop all motors
void stopMotors() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, LOW);
}

// Move forward
void moveForward() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, LOW);
}

// Move backward
void moveBackward() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, LOW);
}

// Turn left
void turnLeft() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(CIN1, HIGH);
  digitalWrite(CIN2, LOW);
}

// Turn right
void turnRight() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, HIGH);
}

void goLeft(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, HIGH);
  digitalWrite(CIN2, LOW);
}

void goRight(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, HIGH);
}

void bottomRight(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, HIGH);
}

void topLeft(){
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, HIGH);
  digitalWrite(CIN2, LOW);
}

void topRight(){
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  digitalWrite(CIN1, LOW);
  digitalWrite(CIN2, HIGH);
}
