#include "Particle.h"
#include "neopixel.h"
#include "inttypes.h"
#include "stdlib.h"
#include "SPI.h"
#include "ARD1939.h"

ARD1939 j1939;

int nCounter = 0;

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN A3
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B
#define LIGHT_DELAY 10
#define FADE .5

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
  RGB.control(true);
  RGB.brightness(50);
  RGB.color(255,255,255);
  //pinMode(A3,OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  


  
  /* ROTATE RING OF COLOURS
  uint16_t i;
  for (i=0; i<PIXEL_COUNT;i++){
    strip.setPixelColor(i, 255,0,0);
    strip.show();
    delay(LIGHT_DELAY);
  }
  
  for (i=0; i<PIXEL_COUNT;i++){
    strip.setPixelColor(i, 0,0,255);
    strip.show();
    delay(LIGHT_DELAY);
  }
  
  for (i=0; i<PIXEL_COUNT;i++){
    strip.setPixelColor(i, 0,255,0);
    strip.show();
    delay(LIGHT_DELAY);
  }
  */

  /* TEST BATTERY PERCENTAGES (run 1% through 100%)*/
  uint16_t i;
  for (i=0;i<100;i++){
    setBatteryPercentage(i);
    delay(LIGHT_DELAY);
  }
  delay(1000);
}

void setBatteryPercentage(uint8_t percentage){
  uint16_t j;
  for (j=0; j<PIXEL_COUNT; j++){
    if (j<=PIXEL_COUNT * percentage/100){
      if (percentage <= 33){ // Low battery: set red
        strip.setPixelColor(j,255*FADE,0,0);
      }
      else if (percentage <= 66){ // set orange
        strip.setPixelColor(j,255*FADE,127*FADE,0);
      }
      else{ //set green
        strip.setPixelColor(j,0,255*FADE,0,0);
      }
    }
    else{
      strip.setPixelColor(j,0,0,0);
    }
  }
  strip.show();
} 