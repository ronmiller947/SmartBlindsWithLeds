#include <FastLED.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Servo.h>

#pragma region Wifi Definitions

#define WLAN_SSID "Raven"
#define WLAN_PASS "mmier947"

#pragma endregion

#pragma region Pin Definitions

#define SERVO_PIN D6
#define LED_PIN D7

#pragma endregion

#pragma region LED Definitions

#define NUM_LEDS 48
#define MaxDelay 100
#define MinDelay 30
#define EmergencyDelay 1
#define COLOR_ORDER GRB

#pragma endregion

#pragma region Theme Enumerations

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

#pragma endregion

#pragma region Pattern Enumerations

const int flashPattern = 0;
const int cylonPattern = 1;
const int stripesPattern = 2;
const int stripesChasePattern = 3;
const int colorWipePattern = 4;
const int waveUsingPalettePattern = 5;

//Stores an array of values to indicate if a pattern hase been shown
const int patternUsedArraySize = 6;
bool patternUsedArray[patternUsedArraySize];


#pragma endregion

#pragma region Pattern Configurations

#define FORWARD 0
#define BACKWARD 1
int direction;
CRGB color1;
CRGB color2;
CRGB color3;
int patternIterations;
int numOfPatternIteration;

unsigned long startTime;
unsigned long endTime;
unsigned long currentTime;
unsigned long patternDuration;

#pragma endregion

#pragma region Led Configurations

TBlendType currentBlending;
CRGB leds[NUM_LEDS];
uint8_t Data[NUM_LEDS];
CRGBPalette16 currentPalette;
int theme;
int count;
bool testMode;
bool ledsOff;
bool useTheme;
int emergencyRunCount;
int ledPattern;

#pragma endregion

#pragma region Web Server Configurations

MDNSResponder mdns;
ESP8266WebServer server(80);


#pragma endregion

#pragma region Servo Variables

Servo myservo;
int pos = 0;
int state = 0;
int prevstate = 0;
int dirc = 0;

#pragma endregion

#pragma region Webpage Constant

String webPage = "<html> <head> <title>Rons Smart Blinds</title> </head> <body> <style>   html {    text-align: center;   }   body{   margin-top: 50px;}    h1 {   color: #444444;   margin: 50px auto 30px;   }    h3 {   color: #444444;   margin-bottom: 50px;   }   .button {   display:inline-block;   width: 40%;   background-color: #1abc9c;   border: none;   color: white;   padding: 13px 30px;   text-decoration: none;   font-size: 25px;   margin: 0px auto 13px;   cursor: pointer;   border-radius: 4px;   }   .button-themes1{    background-color: #34495e;   }   .button-themes2{     background-color: purple;   }   .button-open {   background-color: #1abc9c;   }   .button-open:active {   background-color: #16a085;   }   .button-close {   background-color: #34495e;   }   .button-close:active {   background-color: #2c3e50;   }   .button-police {   width: 20% !important;   background-color: red;   }     .button-fire{   width: 20% !important;   background-color: orange;   }   .button-medical{   width: 20% !important;   background-color: blue;   } </style> <h1>Rons Smart Blinds</h1> <a href='open' class='button button-open'>OPEN</a> <a href='close' class='button button-close'>CLOSE</a> <br/><br/> <h2>LED's</h2> <div class='block'> <a href='turnon' class='button button-open'>ON</a> <a href='turnoff' class='button button-close'>OFF</a> </div> <br/> <h2>Emergency Themes</h2> <div class='block'> <a href='Police' class='button button-police'>Police</a> <a href='Fire' class='button button-fire'>Fire</a> <a href='Medical' class='button button-medical'>Medical</a>; </div> <br/> <h2>Theme Durations</h2> <div class='block'> <a href='dur30s' class='button button-themes1'>30 Secs</a> <a href='dur50s' class='button button-themes1'>50 Secs</a> <a href='dur1m' class='button button-themes1'>1 Min</a>; <a href='dur5m' class='button button-themes1'>5 Min</a> <a href='dur10m' class='button button-themes1'>10 Min</a>; </div> <br/> <h2>Special Themes</h2> <a href='Ravens' class='button button-themes2'>Ravens</a> <br/> <h2>Holiday Themes</h2> <div class='display:'> <a href='NewYear' class='button button-themes1'>New Years</a> <a href='Valentine' class='button button-themes1'>Valentines Day</a> <a href='StPatty' class='button button-themes1'>StPattys Day</a> <a href='Easter' class='button button-themes1'>Easter Day</a> <a href='Memorial' class='button button-themes1'>Memorial Day</a> <a href='Independence' class='button button-themes1'>Independence Day</a> <a href='Labor' class='button button-themes1'>Labor Day</a> <a href='Columbus' class='button button-themes1'>Columbus Day</a> <a href='Holloween' class='button button-themes1'>Holloween</a> <a href='Thanksgiving' class='button button-themes1'>Thanksgiving</a> <a href='Vetrens' class='button button-themes1'>Veterans Day</a> <a href='Christmas' class='button button-themes1'>Christmas</a> </div> </body> </html>";

/*
 * HTML PAGE BEFORE CONVERSION TO STRING
 * <html>
<head>
<title>Rons Smart Blinds</title>
</head>
<body>
<style>
  html { 
  text-align: center;
  }
  body{
  margin-top: 50px;} 
  h1 {
  color: #444444;
  margin: 50px auto 30px;
  } 
  h3 {
  color: #444444;
  margin-bottom: 50px;
  }
  .button {
  display:inline-block;
  width: 40%;
  background-color: #1abc9c;
  border: none;
  color: white;
  padding: 13px 30px;
  text-decoration: none;
  font-size: 25px;
  margin: 0px auto 13px;
  cursor: pointer;
  border-radius: 4px;
  }
  .button-themes1{
   background-color: #34495e;
  }
  .button-themes2{
    background-color: purple;
  }
  .button-open {
  background-color: #1abc9c;
  }
  .button-open:active {
  background-color: #16a085;
  }
  .button-close {
  background-color: #34495e;
  }
  .button-close:active {
  background-color: #2c3e50;
  }
  .button-police {
  width: 20% !important;
  background-color: red;
  }  
  .button-fire{
  width: 20% !important;
  background-color: orange;
  }
  .button-medical{
  width: 20% !important;
  background-color: blue;
  }
</style>
<h1>Rons Smart Blinds</h1>
<a href='open' class='button button-open'>OPEN</a>
<a href='close' class='button button-close'>CLOSE</a>
<br/><br/>
<h2>LED's</h2>
<div class='block'>
<a href='turnon' class='button button-open'>ON</a>
<a href='turnoff' class='button button-close'>OFF</a>
</div>
<br/>
<h2>Emergency Themes</h2>
<div class='block'>
<a href='Police' class='button button-police'>Police</a>
<a href='Fire' class='button button-fire'>Fire</a>
<a href='Medical' class='button button-medical'>Medical</a>;
</div>
<br/>
<h2>Theme Durations</h2>
<div class='block'>
<a href='dur30s' class='button button-themes1'>30 Secs</a>
<a href='dur50s' class='button button-themes1'>50 Secs</a>
<a href='dur1m' class='button button-themes1'>1 Min</a>;
<a href='dur5m' class='button button-themes1'>5 Min</a>
<a href='dur10m' class='button button-themes1'>10 Min</a>;
</div>
<br/>
<h2>Special Themes</h2>
<a href='Ravens' class='button button-themes2'>Ravens</a>
<br/>
<h2>Holiday Themes</h2>
<div class='display:'>
<a href='NewYear' class='button button-themes1'>New Years</a>
<a href='Valentine' class='button button-themes1'>Valentines Day</a>
<a href='StPatty' class='button button-themes1'>StPattys Day</a>
<a href='Easter' class='button button-themes1'>Easter Day</a>
<a href='Memorial' class='button button-themes1'>Memorial Day</a>
<a href='Independence' class='button button-themes1'>Independence Day</a>
<a href='Labor' class='button button-themes1'>Labor Day</a>
<a href='Columbus' class='button button-themes1'>Columbus Day</a>
<a href='Holloween' class='button button-themes1'>Holloween</a>
<a href='Thanksgiving' class='button button-themes1'>Thanksgiving</a>
<a href='Vetrens' class='button button-themes1'>Veterans Day</a>
<a href='Christmas' class='button button-themes1'>Christmas</a>
</div>
</body>
</html>
 * 
 */

#pragma endregion

#pragma region Scetch Setup

void setup() {
 
  emergencyRunCount = 0;

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
  
  patternUsedArray[0] = true;
  patternUsedArray[1] = false;
  patternUsedArray[2] = false;
  patternUsedArray[3] = false;
  patternUsedArray[4] = false;
  patternUsedArray[5] = false;
  startTime = 0;
  endTime = 0;
  currentTime = 0;
  patternDuration = 10000; 


  SetLedDefaults();
  
}

#pragma endregion

#pragma region Scetch Loop

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
    if (useTheme) {
      fill_data_array();
      RunLedTheme();
    }

  }
  else
  {
    turnOffLeds();
  }
  Serial.print("patternDuration");
  Serial.println(patternDuration);
}

#pragma endregion

#pragma region Emergency Led Methods

void emergencyLeds(int theme) {

  switch (theme) {
    case Police: {
        CHSV color1(0, 255, 255);    //red
        CHSV color2(160, 255, 255); //blue
        tiggerEmergencyLights(color1, color2);
        break;
      }
    case Fire: {
        CHSV color1(0, 255, 255);    //red
        CHSV color2(32, 255, 255); //Yellow
        tiggerEmergencyLights(color1, color2);
        break;
      }
    case Medical: {
        CHSV color1(0, 0, 255);    //White
        CHSV color2(160, 255, 255); //blue
        tiggerEmergencyLights(color1, color2);
        break;
      }
  }
}

void tiggerEmergencyLights(CHSV color1, CHSV color2) {
  int delayArr[8] = {50, 63, 75, 88, 100, 113, 125, 138};
  int currentDelay = random(0, 7);

  if (emergencyRunCount >= 4) {
    emergencyRunCount = 0;
  }
  else
  {
    emergencyRunCount = emergencyRunCount + 1;
  }
  switch (emergencyRunCount)
  {
    case 2:
      for (int i = 0; i < NUM_LEDS; i++) {
        int centerLed = NUM_LEDS / 2;
        if (i < centerLed) {
          leds[i] = color2;
        } else {
          leds[i] = color1;
        }
      }
      FastLED.show();
      FastLED.delay(delayArr[currentDelay]);
      break;

    case 3:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color1;
      };
      FastLED.show();
      FastLED.delay(delayArr[currentDelay]);
      break;

    case 4:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = color2;
      };
      FastLED.show();
      FastLED.delay(delayArr[currentDelay]);
      break;

    default:
      for (int i = 0; i < NUM_LEDS; i++) {
        int centerLed = NUM_LEDS / 2;
        if (i < centerLed) {
          leds[i] = color1;
        } else {
          leds[i] = color2;
        }

      };
      FastLED.show();
      FastLED.delay(delayArr[currentDelay]);
      break;
  }
}

void add_glitter()
{
  int chance_of_glitter =  5; // percent of the time that we add glitter
  int number_of_glitters = 3; // number of glitter sparkles to add

  int r = random8(100);
  if ( r < chance_of_glitter ) {
    for ( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}

#pragma endregion

#pragma region Led Control Methods

void RunLedTheme() {

  //Check if the theme triggered if for an emergency
  bool emergencyTheme = theme == Medical || theme == Fire || theme == Police;

  //If the theme is an emergency run emergency theme and exit this method
  if (emergencyTheme) {
      emergencyLeds(theme);
      return;
  }

  // Set colorPalette to correct theme and set 3 colors to be used for theme
  SetVariablesForTheme();

  // Set Pattern Variables 
  int simultaneous; 
  int cycles;
  int speed; 
  int width; 
  int count;
  int perIteration;

  GetThemeSpecificPatternSettings(); //This will handle changing the led pattern if required for the chosen theme

  //Perform pattern using themes colors based on the current pattern itteration. 
  switch (ledPattern)
  {
    case flashPattern:
      count = 1;
      //The Lower the number the faster the animation
      speed = random(100, 251);
      flash(color1, count, speed);
      flash(color2, count, speed);
      flash(color3, count, speed);
      break;
    case cylonPattern:
      width = 10;
      if (theme == Ravens) {
        cylonRavens();
      } else {
        //The Lower the number the faster the animation
        speed = random(5, 30);
        cylon(color1, width, speed);
        cylon(color2, width, speed);
        cylon(color3, width, speed);
      }
      
      
      break;
    case stripesPattern:
      width = 5;
      perIteration = 5;

      if (numOfPatternIteration < perIteration) {
        stripes(color1, color2, width);
      }
      else if (numOfPatternIteration < (perIteration * 2)){
        stripes(color2, color3, width);
      }
      else if (numOfPatternIteration < (perIteration * 3)){
        stripes(color3, color1, width);
      }
      else if (numOfPatternIteration < (perIteration * 4)){
        stripes(color1, color3, width);
      }
      else if (numOfPatternIteration < (perIteration * 5)){
        stripes(color2, color1, width);
      }
      else if (numOfPatternIteration < (perIteration * 6)){
        stripes(color3, color2, width);
      }
      else if (numOfPatternIteration > (perIteration * 6)){
        numOfPatternIteration = 0;
      }
      
      FastLED.delay(20); 
      break;
    case stripesChasePattern:
      cycles = 3;
      //The Lower the number the faster the animation
      speed = random(150, 250);
      stripesChase(color1, cycles, speed);
      stripesChase(color2, cycles, speed);
      stripesChase(color3, cycles, speed);
      break;
    case colorWipePattern:
      //The Lower the number the faster the animation
      speed = random(3, 15);
      direction = direction == FORWARD ? BACKWARD : FORWARD;
      colorWipe(color1, speed, direction);
      colorWipe(color2, speed, direction);
      colorWipe(color3, speed, direction);
      break;
    case waveUsingPalettePattern:
      waveUsingPalette();
      FastLED.show(); 
      FastLED.delay(20); 
      break;
  }

  //Increment Pattern iterations
  bool checkTimer = checkPatternTimer();
  numOfPatternIteration += 1;
  delay(5);
  
  if (checkTimer) {
Serial.print("currentTime: ");
Serial.println(currentTime);
Serial.print("startTime: ");
Serial.println(startTime);
Serial.print("endTime: ");
Serial.println(endTime);
    //Reset iteration count in case pattern needs it (currently only 1)
    numOfPatternIteration = 0;
    //If pattern iterations is complete go to next pattern
    ledPattern = RandomGetNextPattern();
    GetThemeSpecificPatternSettings();
    
    //Clear the current pattern
    simultaneous = NUM_LEDS; //How many leds to perform action on at a time
    cycles = 1; //Number of cycles to run pattern
    speed = 20; // How long to delay each pattern, determins how fast pattern will run
    //disolve(simultaneous, cycles, speed);
    //delay(100);
    startTime = 0;
    endTime = 0;



  }  
}

void GetThemeSpecificPatternSettings() {
  //Check for Theme specific pattern settings
  bool showFlashPattern = true;
  bool showcylonPattern = true;
  bool showstripesPattern = true;
  bool showstripesChasePattern = true;
  bool showcolorWipePattern = true;
  bool showwaveUsingPalettePattern = true;


  //Theme Specific Logic
  if (theme == Memorial || theme == Independence || theme == Labor || theme == Columbus || theme == Vetrens) {    
    //The patriotic theme colors (Red White and Blue) create a police appearance when using the flash pattern
    showFlashPattern = false;
  }
  
  if (theme == Ravens) {    
    //showFlashPattern = false;
    //showstripesChasePattern = false;
    //showstripesPattern = false;
    //showcylonPattern = false;
    //showcolorWipePattern = false;
    ledPattern = waveUsingPalettePattern;
  }

  if (showFlashPattern == false && ledPattern == flashPattern
    || showcylonPattern == false && ledPattern == cylonPattern
    || showstripesPattern == false && ledPattern == stripesPattern
    || showstripesChasePattern == false && ledPattern == stripesChasePattern
    || showcolorWipePattern == false && ledPattern == colorWipePattern
    || showwaveUsingPalettePattern == false && ledPattern == waveUsingPalettePattern
  ) 
  {
    ledPattern = RandomGetNextPattern();
  }
}

int RandomGetNextPattern() {
  
  patternIterations = 0;
  
  bool allUsed = true;

  //Check if all patterns have been used
  for (int i = 0; i < patternUsedArraySize; i++) {
    if (patternUsedArray[i] == false) {
      allUsed = false;
      break;
    }
  }

  // If all patterns have been used then reset the array
  if (allUsed) {
      patternUsedArray[0] = false;
      patternUsedArray[1] = false;
      patternUsedArray[2] = false;
      patternUsedArray[3] = false;
      patternUsedArray[4] = false;
      patternUsedArray[5] = false;
  }

  //Try to set the next pattern randomly. 
  int tryCount = 0;
  int oldPattern = ledPattern;
  ledPattern = random(0, 6);

  //If inital random call fails to identify an unused pattern that is not the last used pattern
  while (patternUsedArray[ledPattern] == true && ledPattern != oldPattern) {
    ledPattern = ledPattern = random(0, 6);
    tryCount += 1;

    //If unable to identfiy a random pattern that hasnt been used after 20 attempts then set led pattern to the first unused pattern that is not the last used pattern. 
    if (tryCount == 20) {
      for (int i = 0; i < patternUsedArraySize; i++) {
        if (patternUsedArray[i] == false) {
          ledPattern = i;
          if(ledPattern != oldPattern)
          {
            break;
          }
        }
      }
    }
  }
  patternUsedArray[ledPattern] = true;

Serial.print("Pattern: ");
Serial.println(ledPattern);

  return ledPattern;
}

bool checkPatternTimer() {
  bool timerDone = false;
  currentTime = millis();
  
  //If timer is unitiated
  if (startTime == 0 || endTime == 0) {
    startTime = millis();
    endTime = millis() + patternDuration;
    return false;
  }
  
  //If its time to change the pattern
  if (currentTime >= endTime) {
    startTime = currentTime;
    endTime = millis() + patternDuration;
    return true;
  }

  return false;
}

void SetVariablesForTheme() {
  //Set Palette to be used by waveUsingPalette Theme
  switch (theme)
  {
    case NewYear: NewYearTheme_gPalette(); add_glitter(); break;
    case Valentine: ValentineTheme_gPalette(); break;
    case StPatty: StPattyTheme_gPalette(); break;
    case Easter: EasterTheme_gPalette(); break;
    case Memorial: PatrioticTheme_gPalette(); add_glitter(); break;
    case Independence: PatrioticTheme_gPalette(); add_glitter(); break;
    case Labor: PatrioticTheme_gPalette(); add_glitter(); break;
    case Columbus: PatrioticTheme_gPalette(); add_glitter(); break;
    case Holloween: HolloweenTheme_gPalette(); add_glitter(); break;
    case Thanksgiving: ThanksgivingTheme_gPalette(); break;
    case Vetrens: PatrioticTheme_gPalette(); add_glitter(); break;
    case Christmas: ChristmasTheme_gPalette(); break;
    case Ravens: RavensTheme_gPalette(); add_glitter(); break;
  }
}

// Set defult settings for Leds
void SetLedDefaults() {
  delay(3000);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  theme = 1;
  count = 1;
  ledsOff = false;
  testMode = false;
  useTheme = true;
  patternIterations = 0;
}

void turnOffLeds() {
  FastLED.clear();
  FastLED.show();
}

void LedTestMode() {
  //For Testing Themes Cycle through themes
  Serial.print("current theme: ");
  Serial.println(theme);
  Serial.print("current count: ");
  Serial.println(count);
  count += 1;
  if (theme >= 14 && count >= 1)
  {
    Serial.println("previous theme: " + theme);
    Serial.println("previous count: " + count);
    if (theme >= 16)
    {
      theme = 1;
    }
    else
    {
      theme += 1;
    }
    count = 0;
    Serial.println("new theme: " + theme);
    Serial.println("new count: " + count);
  }
  else if (count >= 400)
  {
    Serial.println("previous theme: " + theme);
    Serial.println("previous count: " + count);
    if (theme >= 16) {
      theme = 1;
    }
    else
    {
      theme += 1;
    }
    count = 0;
    Serial.println("new theme: " + theme);
    Serial.println("new count: " + count);
  }
}

void fill_data_array()
{
  static uint8_t startValue = 0;
  startValue = startValue + 2;
  uint8_t value = startValue;
  for ( int i = 0; i < NUM_LEDS; i++) {
    Data[i] = triwave8( value); // convert value to an up-and-down wave
    value += 7;
  }
}

#pragma endregion

#pragma region Led Themes Palettes

void PatrioticTheme_gPalette() {
  color1 = CRGB::FireBrick;
  color2 = CRGB::Gray;
  color3 = CRGB::MidnightBlue;

  currentPalette = CRGBPalette16(
                     CRGB::Black, CRGB::Black,
                     color1, color1, color1, color1,
                     CRGB::Gray,  CRGB::Gray, CRGB::Gray, CRGB::Gray,
                     color3, color3, color3, color3,
                     CRGB::Black, CRGB::Black
                   );
}

void NewYearTheme_gPalette() {
  color1 = CRGB::White;
  color2 = CRGB::Purple;
  color3 = CRGB::Goldenrod;
  currentPalette = CRGBPalette16(
                     CRGB::White, CRGB::White,
                     CRGB::Purple, CRGB::Purple, CRGB::Purple, CRGB::Purple, 
                     CRGB::Goldenrod, CRGB::Goldenrod, CRGB::Goldenrod, CRGB::Goldenrod,
                     CRGB::Purple, CRGB::Purple, CRGB::Purple, CRGB::Purple,
                     CRGB::White, CRGB::White
                   );
}

void ValentineTheme_gPalette() {
  color1 = CRGB::MediumVioletRed;
  color2 = CRGB::Fuchsia;
  color3 = CRGB::DeepPink;
  currentPalette = CRGBPalette16(
                     CRGB::DarkRed, CRGB::DarkRed,
                     CRGB::MediumVioletRed, CRGB::MediumVioletRed,
                     CRGB::DeepPink, CRGB::DeepPink,
                     CRGB::Fuchsia,
                     CRGB::Purple, CRGB::Purple,
                     CRGB::Fuchsia,
                     CRGB::DeepPink, CRGB::DeepPink,
                     CRGB::MediumVioletRed, CRGB::MediumVioletRed,
                     CRGB::DarkRed, CRGB::DarkRed
                   );
}

void StPattyTheme_gPalette() {
  color1 = CRGB::Green;
  color2 = CRGB::White;
  color3 = CRGB::Goldenrod;
  currentPalette = CRGBPalette16(
                     CRGB::Green, CRGB::Green,
                     CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green,
                     CRGB::White, CRGB::White, CRGB::White, CRGB::White,
                     CRGB::Goldenrod, CRGB::Goldenrod, CRGB::Goldenrod, CRGB::Goldenrod,
                     CRGB::Green, CRGB::Green
                   );
}

void EasterTheme_gPalette() {
  color1 = CRGB::Cyan;
  color2 = CRGB::Chartreuse;
  color3 = CRGB::DeepPink;
  currentPalette = CRGBPalette16(
                     CRGB::Blue, CRGB::Blue,
                     CRGB::DeepPink, CRGB::DeepPink, CRGB::DeepPink, CRGB::DeepPink,
                     CRGB::Cyan, CRGB::Cyan, CRGB::Cyan, CRGB::Cyan,
                     CRGB::Chartreuse, CRGB::Chartreuse, CRGB::Chartreuse, CRGB::Chartreuse,
                     CRGB::Blue, CRGB::Blue
                   );
}

void HolloweenTheme_gPalette() {
  color1 = CHSV(18, 255, 255);
  color2 = CRGB::Indigo;
  color3 = CRGB::Green;
  currentPalette = CRGBPalette16(
                     CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, 
                     CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green, CRGB::Green,
                     color1, color1, color1, color1, color1,
                     CRGB::Indigo, CRGB::Indigo, CRGB::Indigo
                   );
}

void ThanksgivingTheme_gPalette() {
  color1 = CRGB::DarkRed;
  color2 = CRGB::SeaGreen;
  color3 = CRGB::DarkOrange;
  currentPalette = CRGBPalette16(
                     CRGB::Gray, CRGB::Gray,
                     CRGB::SeaGreen, CRGB::SeaGreen, CRGB::SeaGreen, CRGB::SeaGreen,
                     CRGB::DarkOrange, CRGB::DarkOrange, CRGB::DarkOrange, CRGB::DarkOrange,
                     CRGB::DarkRed, CRGB::DarkRed, CRGB::DarkRed, CRGB::DarkRed,
                     CRGB::Gray, CRGB::Gray
                   );
}

void ChristmasTheme_gPalette() {
  color1 = CRGB::DeepSkyBlue;
  color2 = CRGB::DarkGreen;
  color3 = CRGB::Red;
  currentPalette = CRGBPalette16(   CRGB::DeepSkyBlue, CRGB::DeepSkyBlue,
                                    CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, CRGB::DarkGreen, 
                                    CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red, CRGB::Red,
                                    CRGB::DeepSkyBlue, CRGB::DeepSkyBlue
                                );
}

void RavensTheme_gPalette() {
  


  color1 = CRGB(71, 0, 88);
  color2 = CRGB(148, 0, 185);
  color3 = CRGB::Maroon;
  currentPalette = CRGBPalette16(
                     CRGB::Red, CRGB::Black, CRGB::Black, CRGB::Black,
                     CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo,
                     CRGB::Indigo, CRGB::Indigo, CRGB::Indigo, CRGB::Indigo,
                     CRGB::Black, CRGB::Black, CRGB::Black, CRGB::Black
                   );
}

#pragma endregion

#pragma region Led Patterns

// Renders colors from palete displaying color palette in as 16 led waves that travel ledt to right
void waveUsingPalette()
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette , Data[i], 128, LINEARBLEND);
  }
}

// Changes all LEDS to given color
void allColor(CRGB c) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = c;
  }
  FastLED.show();
}

// Random disolve colors
void disolve(int simultaneous, int cycles, int speed) {
  for (int i = 0; i < cycles; i++) {
    for (int j = 0; j < simultaneous; j++) {
      int idx = random(NUM_LEDS+1);
      leds[idx] = CRGB::Black;
    }
    FastLED.show();
    delay(speed);
  }

  allColor(CRGB::Black);
}
// Sliding bar across LEDs using ravens mix
void cylonRavens() {
  
  int width = 21;
  int centerIdx = 10;
  int rightEyeIdx = 7;
  int leftEyeIdx = 13;
  
  int speed = 100;
  CRGB darkPurple = CRGB(71, 0, 88);
  CRGB red = CRGB::Maroon;
  CRGB lightPurple = CRGB(148, 0, 185);
  CRGB black = CRGB(0, 0, 0);

  // First slide the leds in one direction
  for (int i = 0; i <= NUM_LEDS - width; i++) {
    for (int j = 0; j < width; j++) {
      if (i == centerIdx) {
        leds[i + j] = black;
      }
      else if (i == rightEyeIdx || i == leftEyeIdx){
        leds[i + j] = red;
      }
      else {
        leds[i + j] = darkPurple;
      }
    }

    FastLED.show();

    // now that we've shown the leds, reset to black for next loop
    for (int j = 0; j < 5; j++) {
      leds[i + j] = CRGB::Black;
    }
    delay(speed);
  }

  // Now go in the other direction.
  for (int i = NUM_LEDS - width; i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      if (i == centerIdx) {
        leds[i + j] = black;
      }
      else if (i == rightEyeIdx || i == leftEyeIdx){
        leds[i + j] = red;
      }
      else {
        leds[i + j] = darkPurple;
      }
    }
    FastLED.show();
    for (int j = 0; j < width; j++) {
      leds[i + j] = CRGB::Black;
    }

    delay(speed);
  }
}

// Sliding bar across LEDs
void cylon(CRGB c, int width, int speed) {
  // First slide the leds in one direction
  for (int i = 0; i <= NUM_LEDS - width; i++) {
    for (int j = 0; j < width; j++) {
      leds[i + j] = c;
    }

    FastLED.show();

    // now that we've shown the leds, reset to black for next loop
    for (int j = 0; j < 5; j++) {
      leds[i + j] = CRGB::Black;
    }
    delay(speed);
  }

  // Now go in the other direction.
  for (int i = NUM_LEDS - width; i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      leds[i + j] = c;
    }
    FastLED.show();
    for (int j = 0; j < width; j++) {
      leds[i + j] = CRGB::Black;
    }

    delay(speed);
  }
}

// Flashes given color
// If c==NULL, random color flash
void flash(CRGB c, int count, int speed){
  for(int i=0; i<count; i++){
    allColor(c);
    delay(speed);
    //allColor(CRGB::Black);
    //delay(speed);
  }
}

// Display alternating stripes
void stripes(CRGB c1, CRGB c2, int width) {
  for (int i = 0; i < NUM_LEDS; i++) {
    if (i % (width * 2) < width) {
      leds[i] = c1;
    }
    else {
      leds[i] = c2;
    }
  }
  FastLED.show();
}

// Theater-style crawling of stripes
void stripesChase(CRGB c, int cycles, int speed){ 

  for (int j=0; j<cycles; j++) {  
    for (int q=0; q < 3; q++) {
      for (int i=0; i < NUM_LEDS; i=i+3) {
        int pos = i+q;
        leds[pos] = c;    //turn every third pixel on
      }
      FastLED.show();

      delay(speed);

      for (int i=0; i < NUM_LEDS; i=i+3) {
        leds[i+q] = CRGB::Black;        //turn every third pixel off
      }
    }
  }
}

// Wipes color from end to end
void colorWipe(CRGB c, int speed, int direction){
  for(int i=0; i<NUM_LEDS; i++){
    if(direction == FORWARD){
      leds[i] = c;
    }
    else{
      leds[NUM_LEDS-1-i] = c;
    }
    FastLED.show();
    delay(speed);
  }
}

#pragma endregion

#pragma region Arduino OTA

//Setup Arduino OTA
void SetupOTA() {
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

#pragma endregion

#pragma region Server Control Methods


//Configure Server For Blinds
void ServerSetup() {
  //webPage = "<h1>Rons Smart Blinds</h1><p>Blinds<a href=\"open\"><button>OPEN</button></a>&nbsp;<a href=\"close\"><button>CLOSE</button></a></p><p>Leds<a href=\"NewYear\"><button>NewYear</button></a>&nbsp;<a href=\"Valentine\"><button>Valentine</button></a>&nbsp;<a href=\"StPatty\"><button>StPatty</button></a>&nbsp;<a href=\"Easter\"><button>Easter</button></a>&nbsp;<a href=\"Memorial\"><button>Memorial</button></a>&nbsp;<a href=\"Independence\"><button>Independence</button></a>&nbsp;<a href=\"Labor\"><button>Labor</button></a>&nbsp;<a href=\"Columbus\"><button>Columbus</button></a>&nbsp;<a href=\"Holloween\"><button>Holloween</button></a>&nbsp;<a href=\"Thanksgiving\"><button>Thanksgiving</button></a>&nbsp;<a href=\"Vetrens\"><button>Vetrens</button></a>&nbsp;<a href=\"Christmas\"><button>Christmas</button></a>&nbsp;<a href=\"Ravens\"><button>Ravens</button></a>&nbsp;<a href=\"Police\"><button>Police</button></a>&nbsp;<a href=\"Fire\"><button>Fire</button></a>&nbsp;<a href=\"Medical\"><button>Medical</button></a></p>";

  server.on("/", []() {
    server.send(200, "text/html", webPage);
  });
  server.on("/open", []() {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP OPEN COMMAND RECEIVED");
    dirc = 0;  // direction for servo to run
    state = 2; // sets current state
  });
  server.on("/close", []() {
    server.send(200, "text/html", webPage);
    Serial.println("HTTP CLOSE COMMAND RECEIVED");
    dirc = 180; // direction for servo to run
    state = 1;  // sets current state
  });
  server.on("/NewYear", []() {
    server.send(200, "text/html", webPage);
    theme = NewYear;
  });
  server.on("/Valentine", []() {
    server.send(200, "text/html", webPage);
    theme = Valentine;
  });
  server.on("/StPatty", []() {
    server.send(200, "text/html", webPage);
    theme = StPatty;
  });
  server.on("/Easter", []() {
    server.send(200, "text/html", webPage);
    theme = Easter;
  });
  server.on("/Memorial", []() {
    server.send(200, "text/html", webPage);
    theme = Memorial;
  });
  server.on("/Independence", []() {
    server.send(200, "text/html", webPage);
    theme = Independence;
  });
  server.on("/Labor", []() {
    server.send(200, "text/html", webPage);
    theme = Labor;
  });
  server.on("/Columbus", []() {
    server.send(200, "text/html", webPage);
    theme = Columbus;
  });
  server.on("/Holloween", []() {
    server.send(200, "text/html", webPage);
    theme = Holloween;
  });
  server.on("/Thanksgiving", []() {
    server.send(200, "text/html", webPage);
    theme = Thanksgiving;
  });
  server.on("/Vetrens", []() {
    server.send(200, "text/html", webPage);
    theme = Vetrens;
  });
  server.on("/Christmas", []() {
    server.send(200, "text/html", webPage);
    theme = Christmas;
  });
  server.on("/Ravens", []() {
    server.send(200, "text/html", webPage);
    theme = Ravens;
  });
  server.on("/Police", []() {
    server.send(200, "text/html", webPage);
    theme = Police;
  });
  server.on("/Fire", []() {
    server.send(200, "text/html", webPage);
    theme = Fire;
  });
  server.on("/Medical", []() {
    server.send(200, "text/html", webPage);
    theme = Medical;
  });
  server.on("/dur30s", []() {
    server.send(200, "text/html", webPage);
    patternDuration = 30000;
  });
  server.on("/dur50s", []() {
    server.send(200, "text/html", webPage);
    patternDuration = 50000;
  });
  server.on("/dur1m", []() {
    server.send(200, "text/html", webPage);
    patternDuration = 60000;
  });
    server.on("/dur5m", []() {
    server.send(200, "text/html", webPage);
    patternDuration = 300000;
  });
    server.on("/dur10m", []() {
    server.send(200, "text/html", webPage);
    patternDuration = 600000;
  });
    server.on("/turnoff", []() {
    server.send(200, "text/html", webPage);
    ledsOff = true;
  });
    server.on("/turnon", []() {
    server.send(200, "text/html", webPage);
    ledsOff = false;
  });

  server.begin();
  Serial.println("HTTP server started");
}

void setDurationFromServer(unsigned long milisecsRequested) {
  unsigned long startTime;

}

#pragma endregion

#pragma region Servo Control Methods

void servo_move() {
  Serial.println("State Change. Rotating Servo");
  if ( dirc == 180) {
    myservo.attach(SERVO_PIN);              // energize servo
    delay(50);
    for (pos = 0; pos <= 179; pos += 1) {   // goes from 0 degrees to 90 degrees in steps of 1 degree CHANGE 90 TO MATCH ANGLE OF TILT DESIRED
      myservo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(30);                           // waits 30ms between each degree to slow rotation speed
    }
    delay(50);
    myservo.detach();                      // movement finished so detach servo to conserve power
  }
  else if (dirc == 0) {
    myservo.attach(SERVO_PIN);              // energize servo
    delay(50);
    for (pos = 179; pos >= 0; pos -= 1) {   // goes from 90 degrees to 0 degrees in steps of 1 degree CHANGE 90 TO MIRROR ANGLE OF TILT DESIRED ABOVE
      myservo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(30);                           // waits 30ms between each degree to slow rotation speed
    }
    delay(50);
    myservo.detach();                      // movement finished so detach servo to conserve power
  }

  Serial.println("Returning to main loop");
  return;
}

#pragma endregion

#pragma region Wifi Control Methods

//Setup Wifi
void SetupWifi() {
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting");
  delay(500);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }
}

#pragma endregion
