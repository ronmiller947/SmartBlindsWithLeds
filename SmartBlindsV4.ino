#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include "index.h"


#define SERVO_PIN D6
#define LED_PIN D7
#define NUM_LEDS 48
#define MaxDelay 100
#define MinDelay 30
#define EmergencyDelay 1
#define COLOR_ORDER GRB

#define WLAN_SSID "Raven"
#define WLAN_PASS "mmier947"

//Themes
const int NewYear = 1;
const int Valentine = 2;
const int StPatty = 3;
const int Easter = 4;
const int Memorial = 5;
const int Independence = 6;
const int Labor = 7;
const int Columbus = 8;
const int Holloween = 9;
const int Thanksgiving = 10;
const int Vetrens = 11;
const int Christmas = 12;
const int Ravens = 13;
const int Police = 14;
const int Fire = 15;
const int Medical = 16;

//Colors

//Animation
TBlendType currentBlending;

//Palettes
CRGB leds[NUM_LEDS];
uint8_t Data[NUM_LEDS];
CRGBPalette16 currentPalette;

//Led Control Variable
int theme;
int count;
bool testMode;
bool ledsOff;
bool useTheme;

//Web Server
MDNSResponder mdns;
ESP8266WebServer server(80);
String webPage = "";

//Blind Tilt (Servo)
Servo myservo;
int pos = 0;
int state = 0;
int prevstate = 0;
int dirc = 0;

void setup() {
  webPage += "<h1>Rons Smart Blinds</h1><p>Blinds<a href=\"open\"><button>OPEN</button></a>&nbsp;<a href=\"close\"><button>CLOSE</button></a></p><p>Leds<a href=\"NewYear\"><button>NewYear</button></a>&nbsp;<a href=\"Valentine\"><button>Valentine</button></a>&nbsp;<a href=\"StPatty\"><button>StPatty</button></a>&nbsp;<a href=\"Easter\"><button>Easter</button></a>&nbsp;<a href=\"Memorial\"><button>Memorial</button></a>&nbsp;<a href=\"Independence\"><button>Independence</button></a>&nbsp;<a href=\"Labor\"><button>Labor</button></a>&nbsp;<a href=\"Columbus\"><button>Columbus</button></a>&nbsp;<a href=\"Holloween\"><button>Holloween</button></a>&nbsp;<a href=\"Thanksgiving\"><button>Thanksgiving</button></a>&nbsp;<a href=\"Vetrens\"><button>Vetrens</button></a>&nbsp;<a href=\"Christmas\"><button>Christmas</button></a>&nbsp;<a href=\"Ravens\"><button>Ravens</button></a>&nbsp;<a href=\"Police\"><button>Police</button></a>&nbsp;<a href=\"Fire\"><button>Fire</button></a>&nbsp;<a href=\"Medical\"><button>Medical</button></a></p>";

  delay(1000);
  Serial.begin(9600);
  delay(500);
  Serial.println("Blind Startup Sequence");
  delay(500);
  Serial.println();

  //WIFI SETUP
  SetupWifi();

  //SERVER SETUP
  ServerSetup();
  
  //OTA SETUP
  SetupOTA();
  
  //LED SETUP  
  SetLedDefaults();
  
}

void loop() {
  ArduinoOTA.handle();
  if (state != prevstate) {
    Serial.println("State change!");
    servo_move();
  }
  prevstate = state;
  server.handleClient();
  
  if (!ledsOff) {
    if (testMode) {
      LedTestMode();
    }
    if(useTheme) {
      fill_data_array();
      RunLedTheme();
    }
    
  }
  else
  {
    turnOffLeds();
  }
}
