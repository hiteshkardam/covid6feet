#include <Wire.h>
#include "VL53L1X_api.h"
#include "VL53L1Xinit.h"

#define DEBUG 1

uint8_t gotreading = 0;
uint8_t deviceaddress = 0x52; //I2C address of the sensor

//Interrupt routine
void IRAM_ATTR ISR()
{
  gotreading = 1;
}


void setup()
{
  uint16_t GPIO1pin = 23;

  //The TB is the time required by the sensor to make one distance measurement.
  //The TB values available are [15, 20, 33, 50, 100, 200, 500].
  //This function must be called after VL53L1X_SetDistanceMode.
  //Note: 15 ms only works with Short distance mode.
  uint16_t TimingBudgetInMs = 200;
  
  //Intermeasurement period must be >/= timing budget.
  //This condition is not checked by the API.
  uint32_t InterMeasurementInMs = 500;
  
  int16_t OffsetValue = 10;

  pinMode(GPIO1pin, INPUT_PULLUP);
  attachInterrupt(GPIO1pin, ISR, FALLING);

  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  int init = sensorinit(deviceaddress, GPIO1pin, TimingBudgetInMs, InterMeasurementInMs, OffsetValue);
  if (init)
  while(1);
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
