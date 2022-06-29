/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "c:/Users/taylor.macdonald/LEDBatteryReader/src/LEDBatteryReader.ino"
#include "Particle.h"
#include "neopixel.h"
#include "inttypes.h"
#include "stdlib.h"
#include "SPI.h"
#include "ARD1939.h"

void setup();
void loop();
void errorLights();
void charging(uint8_t percentage);
void setBatteryPercentage(uint8_t percentage);
#line 8 "c:/Users/taylor.macdonald/LEDBatteryReader/src/LEDBatteryReader.ino"
ARD1939 j1939;

int nCounter = 0;

SYSTEM_MODE(AUTOMATIC);

// IMPORTANT: Set pixel COUNT, PIN and TYPE
#define PIXEL_PIN A3
#define PIXEL_COUNT 24
#define PIXEL_TYPE WS2812B
#define LIGHT_DELAY 10
#define FADE .05
#define CHARGING_LED_QTY 2

Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
  RGB.control(true);
  RGB.brightness(50);
  RGB.color(255,255,255);
  //pinMode(A3,OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'


  // Set the serial interface baud rate
  Serial.begin(MONITOR_BAUD_RATE);
  
  // Initialize the J1939 protocol including CAN settings
  if(j1939.Init(SYSTEM_TIME) == 0)
    Serial.print("CAN Controller Init OK.\n\r\n\r");
  else
    Serial.print("CAN Controller Init Failed.\n\r");
    
 // Set the preferred address and address range
 j1939.SetPreferredAddress(SA_PREFERRED);
 j1939.SetAddressRange(ADDRESSRANGEBOTTOM, ADDRESSRANGETOP);
 
 // Set the message filter
 //j1939.SetMessageFilter(59999);
 
 //j1939.SetMessageFilter(65242);
 //j1939.SetMessageFilter(65259);
 //j1939.SetMessageFilter(65267);
 
 // Set the NAME
 j1939.SetNAME(NAME_IDENTITY_NUMBER,
               NAME_MANUFACTURER_CODE,
               NAME_FUNCTION_INSTANCE,
               NAME_ECU_INSTANCE,
               NAME_FUNCTION,
               NAME_VEHICLE_SYSTEM,
               NAME_VEHICLE_SYSTEM_INSTANCE,
               NAME_INDUSTRY_GROUP,
               NAME_ARBITRARY_ADDRESS_CAPABLE);
}

void loop() {
      // J1939 Variables
  byte nMsgId;
  byte nDestAddr;
  byte nSrcAddr;
  byte nPriority;
  byte nJ1939Status;
  
  int nMsgLen;
  
  long lPGN;
  
  byte pMsg[J1939_MSGLEN];
  
  // Variables for proof of concept tests
  //byte msgFakeNAME[] = {0, 0, 0, 0, 0, 0, 0, 0};
  byte msgLong[] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45};
  
  // Establish the timer base in units of milliseconds
  delay(SYSTEM_TIME);
  
  // Call the J1939 protocol stack
  nJ1939Status = j1939.Operate(&nMsgId, &lPGN, &pMsg[0], &nMsgLen, &nDestAddr, &nSrcAddr, &nPriority);
 
  
  
  // Send out a periodic message with a length of more than 8 bytes
  // RTS/CTS Session
  if(nJ1939Status == NORMALDATATRAFFIC)
  {
    nCounter++;
    
    if(nCounter == (int)(5000/SYSTEM_TIME))
    {
      nSrcAddr = j1939.GetSourceAddress();
      j1939.Transmit(6, 59999, nSrcAddr, 0x33, msgLong, 15);
      nCounter = 0;
      
    }// end if
  
  }// end if
  
  // Check for reception of PGNs for our ECU/CA
  if(nMsgId == J1939_MSG_APP)
  {
    // Check J1939 protocol status
    switch(nJ1939Status)
    {
      case ADDRESSCLAIM_INPROGRESS:
      
        break;
        
      case NORMALDATATRAFFIC:
      
        // Determine the negotiated source address
        //byte nAppAddress;
        //nAppAddress = j1939.GetSourceAddress();
        
       break;
        
      case ADDRESSCLAIM_FAILED:
      
        break;
      
    }// end switch(nJ1939Status)
    
  }// end if


  /* Testing: ROTATE RING OF COLOURS
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

  /* Testing: EMULATE BATTERY PERCENTAGES (run 1% through 100%)
  uint16_t k;
  for (k=0;k<100;k++){
    setBatteryPercentage(k);
    delay(LIGHT_DELAY);
  }
  delay(1000);*/

  /* Testing: EMULATE CHARGING FROM 1 TO 100 
  uint16_t m;
  for (m=0;m<100;m++){
    charging(m);
  }*/

  /* Testing: ERROR LIGHTS
  uint16_t i;
  for(i=0;i<10;i++){ // run error segment 10x
    errorLights();
  }*/
}

void errorLights(){ // red light then no light 
  uint16_t j;
  for (j=0;j<PIXEL_COUNT;j++){ //set all pixels red
    strip.setPixelColor(j,255,0,0);
  }
  strip.show();//shows changes on LED strip
  delay(1000);
  for (j=0;j<PIXEL_COUNT;j++){ //set all pixels off
    strip.setPixelColor(j,0,0,0);
  }
  strip.show();//shows changes on LED strip
  delay(1000);
}

void charging(uint8_t percentage){ //pass in a percentage (0-100) that the battery is currently at (when charging).
  setBatteryPercentage(percentage);//calls setBatteryPercentage function
  delay(1000);
  setBatteryPercentage(percentage + CHARGING_LED_QTY * 100/PIXEL_COUNT);//calls setBatteryPercentage function with enough additional percent to activate more LEDs
  delay(1000);
}

void setBatteryPercentage(uint8_t percentage){ //pass in a percentage (0-100) that the battery is current at (not charging)
  uint16_t j;
  for (j=0; j<PIXEL_COUNT; j++){
    if (j<=PIXEL_COUNT * percentage/100){
      if (percentage <= 33){ // Low battery: set red
        strip.setPixelColor(j,255*FADE,0,0);
      }
      else if (percentage <= 66){ // set orange
        strip.setPixelColor(j,255*FADE,127*FADE,0);
      }
      else{ //set green if batter is above 66%
        strip.setPixelColor(j,0,255*FADE,0,0);
      }
    }
    else{ //sets the rest of the LEDs (if there are any) to off
      strip.setPixelColor(j,0,0,0);
    }
  }
  strip.show(); //shows changes on LED strip
} 