#include <Wire.h>
#include <SPI.h>
#include <FastLED.h>

#define NUM_LEDS 112
#define NUM_LEDS2 15
#define DATA_PIN 6
#define DATA_PIN1 7
#define DATA_PIN2 4

char IData;
int fadeAmount = 5; //Can set this to 5, 10, 15, 20, 25
int brightness = 0;

CRGB leds1[NUM_LEDS];
CRGB leds2[NUM_LEDS];
CRGB leds3[NUM_LEDS2];

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 1000;
void setup()
{
  startMillis = millis();
  Serial.begin(9600);
 // Wire.begin(8);
  //Wire.onReceive(receiveEvent);
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds1, NUM_LEDS);// Elevator Left
  FastLED.addLeds<WS2812B, DATA_PIN1, GRB>(leds2, NUM_LEDS); //Elevator Right
  FastLED.addLeds<WS2812B, DATA_PIN2, GRB>(leds3, NUM_LEDS2); //Electrical Board
}

void loop()
{
 switch(IData){
    case 'r': //All Red
      allColor(CRGB::Red);
      break;
    case 'g': //All Green
      allColor(CRGB::Green);
      break;
    case 'b': //All Blue
      allColor(CRGB::Blue);
      break;
    case 'u': 
      rainbowAll();
      break;
    case 'c': // Rainbow E Board, Blink Green at top
      rainbowEBoard();
      colorWipeF(CRGB::Green, 46, 10);
      colorWipeB(CRGB::Green, 57, 10);
      break;
    case 'h': //Rainbow E Board, Blink Red at Bottom
      
      break;
    case 'w':
      
      break;
    case 'o': 
      allColor(CRGB::Black);
      break;

     //pulse(CRGB::Blue, 0, 10);
  

rainbowElevator(0, 10);
  //colorWipe(CRGB::Blue, 11, 10);
 // blink(CRGB::Red);
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

void allColor(CRGB color){
  for(int i = 0; i < NUM_LEDS; i++){
      leds1[i] = color;
      leds2[i] = color;
      leds3[i] = color;
    }  
    FastLED.show();
}
//Color Wipes Forward
void colorWipeF(CRGB color, int first, int num){
  for(uint16_t i = 0; i < num; i++){
      uint8_t c = (millis() / 5) + (i * 10);
      if(c > 128) 
      {c = 0;
      leds1[i + first] = color;
      leds2[i+ first] = color;
      }
      else {
        leds1[i +first] = CRGB::Black;
        leds2[i + first] = CRGB::Black;
       }
    }
}
//Color Wipes Backward
void colorWipeB(CRGB color, int first, int num){
  for(uint16_t i =0; i < num; i++){
    uint8_t c = (millis() / 5) + (i * 10);
    if(c > 128){
      leds1[num-1-i];
      leds2[num-1-i];
    }
    else{
        leds1[i + first] = CRGB::Black;
        leds2[i + first] = CRGB::Black;
      }  
  }
}

//Need to test
void pulse(CRGB c, int first, int num){
  currentMillis = millis();
  for(int i = 0; i < num; i++){
    if(currentMillis - startMillis >= period){
      leds1[i] = c;
      leds[i].fadeLightBy(brightness);
      startMillis = currentMillis;
      }
    }
    FastLED.show();
    brightness = brightness + fadeAmount;
    if(brightness == 0 || brightness == 255){
        fadeAmount =-fadeAmount;
      }
      delay(20);
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
