
/*
 * WebSocketServer_LEDcontrol.ino
 *
 *  Created on: 26.11.815
 *
 */
//Modified to act as AP
//arduino-er.blogspot.com

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <Stepper.h>

const int stepsPerRevolution = 200;  
 Stepper myStepper(stepsPerRevolution, D5,D6,D3,D4);
//Stepper myStepper(stepsPerRevolution, 5,4,0,2);  

int i = 0;
int imove = 0;
int enA = D5;
int enB = D6;

int a1 = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
int a5 = 0;
int a6 = 0;
int a7 = 0;
int a8 = 0;
int a9 = 0;
int a10 = 0;

#define OLED_RESET LED_BUILTIN  //
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


#define LED_RED     15
#define LED_GREEN   12
#define LED_BLUE    13

#define USE_SERIAL Serial

int angle = 0;
int sec = 0;

ESP8266WiFiMulti WiFiMulti;

ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void affiche(char * texte) {
  // Clear the buffer.
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println(texte);
  display.display();
}
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {


  digitalWrite(D0, HIGH);   // turn the LED on (HIGH is the voltage level)
  
    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[%u] Disconnected!\n", num);
            affiche("Deconnecte !");
            break;
        case WStype_CONNECTED: {
            IPAddress ip = webSocket.remoteIP(num);
            USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

                      // wait for a second
            // send message to client
            webSocket.sendTXT(num, "Connected");
        }
            break;
        case WStype_TEXT:
            affiche("Texte recu");
            USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);                

            if(payload[0] == '#') {
                if(payload[1] == '6') { 
                  if(payload[2] == '1') a1 = angle;
                  if(payload[2] == '2') a2 = angle;
                  if(payload[2] == '3') a3 = angle;
                  if(payload[2] == '4') a4 = angle;
                  if(payload[2] == '5') a5 = angle;
                  if(payload[2] == '6') a6 = angle;
                  if(payload[2] == '7') a7 = angle;
                  if(payload[2] == '8') a8 = angle;
                  if(payload[2] == '9') a9 = angle;
                  if(payload[2] == '0') a10 = angle;
                  }
                  
                if(payload[1] == '8') { 
                  if(payload[2] == '1') go(a1);
                  if(payload[2] == '2') go(a2);
                  if(payload[2] == '3') go(a3);
                  if(payload[2] == '4') go(a4);
                  if(payload[2] == '5') go(a5);
                  if(payload[2] == '6') go(a6);
                  if(payload[2] == '7') go(a7);
                  if(payload[2] == '8') go(a8);
                  if(payload[2] == '9') go(a9);
                  if(payload[2] == '0') go(a10);
                  }
                
                switch(payload[1]) {
                  case 49: display.println("HAUT");
                      tourne(stepsPerRevolution);
   break; 
                  case 50: display.println("GAUCHE");
                      tourne(1);
                      break;
                  case 51: display.println("BAS"); 
                      //tourne(-stepsPerRevolution);
                      go(50);
                      
                  
                  break;
                  case 52: display.println("DROITE");
                      tourne(-1);
                      break;
                  case 57: display.println("STOP"); break;
                  default: display.println("Rien"); display.println(payload[1]);
                }
                
                display.display();
            }

            break;
    }
   delay(10);

  digitalWrite(D0, LOW);    // turn the LED off by making the voltage LOW
  
  analogWrite(enA, 0);
  analogWrite(enB, 0);
}

void setup() {
  
  pinMode(D0, OUTPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
     
  
    //USE_SERIAL.begin(921600);
    USE_SERIAL.begin(115200);

    //USE_SERIAL.setDebugOutput(true);

  pinMode(12, OUTPUT); // D6 = PIN 12 ; 
  pinMode(13, OUTPUT); // D7 = Pin 13 ;
  pinMode(3, OUTPUT);
  pinMode(15, OUTPUT);


  pinMode(5, OUTPUT); // A D1 PWM control 0 - 623
pinMode(4, OUTPUT); // B D2 PWM control 0 - 623
pinMode(0, OUTPUT); // A D3 Direction 0/1
pinMode(2, OUTPUT); // B D4 Direction 0/1

//pin initialisation
analogWrite(5, 0);
analogWrite(4, 0);

digitalWrite(0, 1); 
digitalWrite(2, 1);


  digitalWrite(D0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(D0, LOW);    // turn the LED off by making the voltage LOW
  delay(50);
  
  myStepper.setSpeed(60);
    
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);


affiche("C'est parti !");

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
        USE_SERIAL.flush();
  display.setCursor(0,0);
  display.println("BOOT WAIT...");
  display.display();
        delay(1000);
    }
        
  
    /*
    WiFiMulti.addAP("SSID", "passpasspass");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }
    */

    WiFi.softAP("jesus", "12345678");
    IPAddress myIP = WiFi.softAPIP();
    USE_SERIAL.print("AP IP address: ");
    USE_SERIAL.println(myIP);
    
  display.clearDisplay();
  display.display();
                display.setTextSize(2);
                display.setTextColor(WHITE);
                display.setCursor(0,0);
                //display.println("Connecté !");
                display.println("");
                display.println("Ip adress: ");
                display.println(myIP);
                
        affiche("Connecté");
        delay(100);

    // start webSocket server
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    //if(MDNS.begin("esp8266")) {
    //    USE_SERIAL.println("MDNS responder started");
    //}

    // handle index
    server.on("/", []() {
        // send index.html
        // replace for better looking
        server.send(200, "text/html", 
        "<html><head><script>"
        "var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);"
        "connection.onopen = function () {  connection.send('Connect ' + new Date()); };"
        "connection.onerror = function (error) {    console.log('WebSocket Error ', error);};"
        "connection.onmessage = function (e) {  console.log('Server: ', e.data);};"
        "function sendMsg(msg) { connection.send(msg); }  "
        "function sendRGB() {  "
        "var r = parseInt(document.getElementById('r').value).toString(16);  "
        "var g = parseInt(document.getElementById('g').value).toString(16);  "
        "var b = parseInt(document.getElementById('b').value).toString(16);  "
        "if(r.length < 2) { r = '0' + r; }   "
        "if(g.length < 2) { g = '0' + g; }   "
        "if(b.length < 2) { b = '0' + b; }   "
        "var rgb = '#'+r+g+b;    "
        "console.log('RGB: ' + rgb); "
        "connection.send(rgb); }"
        "</script><style type=\"text/css\">button { width: 100%; height: 20%; text-align: center; font-size: 30px;}</style></head><body id=\"page\">"
        "<button id=\"h\" onclick=\"sendMsg('#9');\">Stop</button><br/>"
        "<button id=\"h\" onclick=\"sendMsg('#1');\" onrelease=\"sendMsg('#10');\">Haut</button><br/>"
        "<button id=\"g\" onclick=\"sendMsg('#2');\" onrelease=\"sendMsg('#20');\">Gauche</button><br/>"
        "<button id=\"b\" onclick=\"sendMsg('#3');\" onrelease=\"sendMsg('#30');\">Bas</button><br/>"
        "<button id=\"d\" onclick=\"sendMsg('#4');\" onrelease=\"sendMsg('#40');\">Droite</button><br/>"
        "<script>"
        "var input = document.getElementById(\"page\"); input.addEventListener(\"keydown\", function(event) { event.preventDefault();"
        "if (event.keyCode === 13) { alert('#10');}"
        "if (event.keyCode === 37) { sendMsg('#20'); console.log('gauche');}"
        "if (event.keyCode === 38) { sendMsg('#10'); console.log('haut');}"
        "if (event.keyCode === 39) { sendMsg('#40'); console.log('droite');}"
        "if (event.keyCode === 40) { sendMsg('#30'); console.log('bas');}"
        "if (event.keyCode === 112) { sendMsg('#61'); console.log('61');}"
        "if (event.keyCode === 113) { sendMsg('#62'); console.log('62');}"
        "if (event.keyCode === 114) { sendMsg('#63'); console.log('63');}"
        "if (event.keyCode === 115) { sendMsg('#64'); console.log('64');}"
        "if (event.keyCode === 116) { sendMsg('#65'); console.log('65');}"
        "if (event.keyCode === 117) { sendMsg('#66'); console.log('66');}"
        "if (event.keyCode === 118) { sendMsg('#67'); console.log('67');}"
        "if (event.keyCode === 119) { sendMsg('#68'); console.log('68');}"
        "if (event.keyCode === 120) { sendMsg('#69'); console.log('69');}"
        "if (event.keyCode === 121) { sendMsg('#60'); console.log('60');}"
        "if (event.keyCode === 49) { sendMsg('#81'); console.log('81');}"
        "if (event.keyCode === 50) { sendMsg('#82'); console.log('82');}"
        "if (event.keyCode === 51) { sendMsg('#83'); console.log('83');}"
        "if (event.keyCode === 52) { sendMsg('#84'); console.log('84');}"
        "if (event.keyCode === 53) { sendMsg('#85'); console.log('85');}"
        "if (event.keyCode === 54) { sendMsg('#86'); console.log('86');}"
        "if (event.keyCode === 55) { sendMsg('#87'); console.log('87');}"
        "if (event.keyCode === 56) { sendMsg('#88'); console.log('88');}"
        "if (event.keyCode === 57) { sendMsg('#89'); console.log('89');}"
        "if (event.keyCode === 48) { sendMsg('#80'); console.log('80');}"
        "});"
        "</script>"
        "</body></html>");
    });


    server.begin();

    // Add service to MDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);

  delay(1000);                       // wait for a second
                      tourne(-stepsPerRevolution);
  delay(1000);   
                      tourne(stepsPerRevolution);
  delay(1000);                       // wait for a second
                      tourne(-stepsPerRevolution);
  delay(1000);   
}

void loop() {
    
  
    webSocket.loop();
    server.handleClient();
  
}

void tourne(int steps) {
  myStepper.step(steps);
  angle += steps;
  if(angle>=200) angle -= 200;
  if(angle<= -200) angle += 200;
            USE_SERIAL.printf("Steps: [%d]", steps);
            USE_SERIAL.printf("Angle: [%d]", angle);
}

void go(int getAngle) {
  int na = getAngle - angle;
  tourne(na);
}

