#include <Wire.h>
#include <SPI.h>
#include <FastLED.h>

#define NUM_LEDS 112
#define NUM_LEDS2 15
#define DATA_PIN 6
#define DATA_PIN1 7
#define DATA_PIN2 4

char IData;

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS2];

void setup()
{
  //Starts the serial library
  Serial.begin(9600);
  //Sets up the led strips
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds1, NUM_LEDS);// Elevator Left
  FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds2, NUM_LEDS); //Elevator Right
  FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds3, NUM_LEDS2); //Electrical Board
  //Starts receiving data over i2c
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

void loop()
{
 switch(IData){
    case 'r': 
      AllRed();
      break;
    case 'g': //All Green
      AllGreen();
      break;
    case 'b': //All Blue
      AllBlue();
      break;
    case 'u': //All Rainbow
      RainbowAllStrips();
      break;
    case 'c': // Rainbow E Board, Blink Green at top
      ElevatorUp();
      break;
    case 'h': //Rainbow E Board, Blink Red at Bottom
      ElevatorDown();
      break;
    case 'w': //Random Chase
      RandomChase();
      break;
    case 'o': //All Leds Off
      AllOff();
      break;
  }
  FastLED.show();
}

void receiveEvent(int howMany){
  while(Wire.available()){
      char c;
      c = Wire.read();
      Serial.println(c);
      IData = c;
  }
}

//****** Color Patterns ********

void allColor(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
      leds1[i] = color;
      leds2[i] = color;
      leds3[i] = color;
    }  
    FastLED.show();
}
//Color Wipes Forward
void colorWipeF(CRGB color, int first, int num, CRGB leds){
  for(uint16_t i = 0; i < num; i++){
      uint8_t c = (millis() / 5) + (i * 10);
      if(c > 128) 
      {c = 0;
      leds[i + first] = color;
      }
      else {
        leds[i +first] = CRGB::Black;
       }
    }
}
//Color Wipes Backward
void colorWipeB(CRGB color, int first, int num, CRGB leds){
  for(uint16_t i =0; i < num; i++){
    uint8_t c = (millis() / 5) + (i * 10);
    if(c > 128){
      leds[num-1-i];
    }
    else{
        leds[i + first] = CRGB::Black;
      }  
  }
}

void rainbowAll(){
   for(uint16_t i=0; i<NUM_LEDS; i++){
      leds1[i] = CHSV((millis() / 10) - (i * 3), 255, 255);
      leds2[i] = CHSV((millis() / 10) - (i * 3), 255, 255);
      leds3[i] = CHSV((millis() / 10) - (i * 3), 255, 255);
    }  
    FastLED.show();
}

void rainbowElevator(int first, int num){
  for(uint16_t i=0; i<num; i++){
    leds1[i + first] = CHSV((millis() / 10) - (i * 3), 255, 255);
    leds2[i + first] = CHSV((millis() / 10) - (i * 3), 255, 255);
  }
  FastLED.show();
}

void rainbowEBoard(){
  for(uint16_t i=0; i< NUM_LEDS; i++){
      leds3[i] = CHSV((millis() / 10) - (i * 3), 255, 255);
    }  
}

CRGB Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return CRGB(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return CRGB(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return CRGB(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

CRGB randomColor(){
  return Wheel(random(256)); 
}

//*********User Modes ***********
void AllRed(){
  allColor(CRGB::Red);
  FastLED.show();
  Serial.println("All Red");
}

void AllBlue(){
  allColor(CRGB::Blue);
  FastLED.show();
  Serial.println("All Blue");
}

void AllGreen(){
  allColor(CRGB::Green);
  FastLED.show();
  Serial.println("All Green");  
}

void RainbowAllStrips(){
  rainbowAll();
  FastLED.show();
  Serial.println("Rainbow All");
}

void ElevatorUp(){
   rainbowEBoard();
   colorWipeB(CRGB::Green, 40, 16, leds1);
   colorWipeF(CRGB::Green, 73, 16, leds1); // might need to change to 53
   colorWipeB(CRGB::Green, 40, 16, leds2);
   colorWipeF(CRGB::Green, 73, 16, leds2); // might need to change to 53
   rainbowElevator(0, 39);
   rainbowElevator(73, 39);
   FastLED.show();
   Serial.println("Elevator is up");
}

void ElevatorDown(){
  rainbowEBoard();
  colorWipeF(CRGB::Red, 0, 16, leds1);
  colorWipeF(CRGB::Red, 0, 16, leds2);
  colorWipeB(CRGB::Red, 112, 16, leds1);
  colorWipeB(CRGB::Red, 112, 16, leds2);
  rainbowElevator(17, 36); // might need to fix
  rainbowElevator(53, 16);
  FastLED.show();
  Serial.println("Elevator is down");
}

void RandomChase(){
  colorWipeF(randomColor, 0, 112, leds1);
  colorWipeF(randomColor, 0, 112, leds2);
  FastLED.show();
  Serial.println("Random Chase");
}

void AllOff(){
  allColor(CRGB::Black);
  Serial.println("All Off");  
}
