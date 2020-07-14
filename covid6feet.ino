#include <Wire.h>
#include "VL53L1X_api.h"

#include "VL53L1Xinit.h"
#include "WS2812Bwrapper.h"

uint8_t deviceaddress = 0x52;         //I2C address of the sensor
uint16_t GPIO1pin = 4;                //VL53L1X interrupt pin
uint16_t TimingBudgetInMs = 200;      
uint32_t InterMeasurementInMs = 1000; //Reading in every 1s
int16_t OffsetValue = 10;             

uint8_t gotreading = 0;

//Interrupt routine
void IRAM_ATTR ISR()
{
  gotreading = 1;
}


void setup()
{
  initWS2812B();
  
  pinMode(GPIO1pin, INPUT_PULLUP);
  attachInterrupt(GPIO1pin, ISR, FALLING);

  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  sensorinit(deviceaddress, GPIO1pin, TimingBudgetInMs, InterMeasurementInMs, OffsetValue);
}


void loop()
{
  static uint16_t distance;
  
  if (gotreading == 1)
  {
    VL53L1X_GetDistance(deviceaddress, &distance);
    gotreading = 0;
    Serial.print(F("Distance(mm): "));
    Serial.println(distance);
  }
  VL53L1X_ClearInterrupt(deviceaddress);
}
