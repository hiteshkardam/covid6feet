#include <Wire.h>
#include "VL53L1X_api.h"
//#include <Math.h>

#include "VL53L1Xinit.h"
#include "WS2812Bwrapper.h"

uint8_t deviceaddress = 0x52;         //I2C address of the sensor
uint16_t GPIO1pin = 4;                //VL53L1X interrupt pin
uint16_t TimingBudgetInMs = 200;      
uint32_t InterMeasurementInMs = 1000; //Reading in every 1s
int16_t OffsetValue = 10;             

uint8_t gotreading = 0;
static uint16_t distance;
static uint16_t prevdistance = distance;
static uint16_t deltadistance = 0;
static uint16_t laststate = 0;

//Interrupt routine
void IRAM_ATTR ISR()
{
  gotreading = 1;
}


void setup()
{
	Serial.println(F("-----------------------------------------------------------------------------"));
	
	initWS2812B();
	
	pinMode(GPIO1pin, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(GPIO1pin), ISR, FALLING);
	
	Wire.begin();
	Wire.setClock(400000);
	Serial.begin(115200);
	
	sensorinit(deviceaddress, GPIO1pin, TimingBudgetInMs, InterMeasurementInMs, OffsetValue);
}

void execrepeat(uint16_t withlaststate)
{
  Serial.println(F("execrepeat"));
  Serial.print(F("withlaststate: "));
  Serial.println(withlaststate);
  switch (withlaststate)
  {
    case 0:
    searching();
    break;
   
    case 1:
    showcolour(0,255,0);
    break;
   
    case 2:
    showcolour(255,0,0);
    break;
    
    case 3:
    redflash();
    break;
  }
}


void loop()
{
  if (gotreading == 1)
  {
    VL53L1X_GetDistance(deviceaddress, &distance);
    deltadistance = abs(distance-prevdistance);

     /*
     * 0-900: buzzer plus red flash
     * 900-1800: red solid
     * 1800-3000:green solid
     * 3000+: green searching
     */
     
     Serial.print(F("Distance(mm): "));
     Serial.print(distance);
     Serial.print(F("\tPrevDistance(mm): "));
     Serial.print(prevdistance);
     Serial.print(F("\t\tDeltaDistance(mm): "));
     Serial.println(deltadistance);
    
    if (deltadistance > 50)
    {
      Serial.println(F("deltadistance > 50"));
      if (distance < prevdistance) // Coming closer to sensor
      {
        if (distance>=3000 && laststate!=0)
        {
           Serial.println(F("distance>=3000"));
           searching();
           laststate = 0;
        }
        else if (distance<3000 && distance>=1800 && laststate!=1)
        {
           Serial.println(F("distance<3000 && distance>=1800"));
           searchingfadetocolour(0,255,0);
           togreen();
           showcolour(0,255,0);
           laststate = 1;
        }
        else if (distance<1800 && distance>=900 && laststate!=2)
        {
          Serial.println(F("distance<1800 && distance>=900"));
          if (laststate==0)
          searchingfadetocolour(255,0,0);
          tored();
          showcolour(255,0,0);
          laststate = 2;
        }
        else if (distance<900 && laststate!=3)
        {
          Serial.println(F("distance<900 && distance>=0"));
          searchingfadetocolour(255,0,0);
          tored();
          redflash();
          laststate = 3;
        }
        else
        {
          execrepeat(laststate);
        }
        Serial.print(F("laststate coming closer: "));
        Serial.println(laststate);
      }
      else if (distance > prevdistance) //Moving away from sensor
      {
        if (distance>=0 && distance<900 && laststate!=3)
        {
          Serial.println(F("distance>=0 && distance<900"));
          if (laststate==0)
          searchingfadetocolour(255,0,0);
          tored();
          redflash();
          
          laststate = 3;
        }
        else if (distance>=900 && distance<1800 && laststate!=2)
        {
          Serial.println(F("distance>=900 && distance<180"));
          showcolour(255,0,0);
          laststate = 2;
        }
        else if (distance>=1800 && distance<3000 && laststate!=1)
        {
           Serial.println(F("distance>=1800 && distance<3000"));
           togreen();
           showcolour(0,255,0);
           laststate = 1;
        }
        else if (distance>3000 && laststate!=0)
        {
           Serial.println(F("distance>3000"));
           searchingfadeout();
           searching();
           laststate = 0;
        }
        else
        {
          execrepeat(laststate);
        }
        Serial.print(F("laststate going away: "));
        Serial.println(laststate);
      }
        prevdistance = distance;
     }
     else
     {
      Serial.println(F("deltadistance < 50"));
      execrepeat(laststate);
     }
     
     gotreading = 0;
     Serial.println(F("-----------------------------------------------------------------------------"));
  }
  VL53L1X_ClearInterrupt(deviceaddress);
  
  //searching();
  //searchingfadein();
  //showcolour(0,255,0); //green
  //tored();
  //showcolour(255,0,0); //red
  //delay(500);
  //redflash();
  //redflash();
  //redflash();
  //showcolour(255,0,0); //red
  //delay(300);
  //togreen();
  //showcolour(0,255,0); //green
  //searchingfadeout();
  //searching();
}
