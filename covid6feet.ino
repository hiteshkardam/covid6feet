#include <Wire.h>
#include "VL53L1X_api.h"

VL53L1X_ERROR status = 0;
uint8_t state;
uint16_t Dev = 0x52; //I2C address of the sensor

void setup()
{
  uint8_t byteData;
  
  uint16_t wordData;
  
  int16_t OffsetValue = 5;

  //The TB is the time required by the sensor to make one distance measurement.
  //The TB values available are [15, 20, 33, 50, 100, 200, 500].
  //This function must be called after VL53L1X_SetDistanceMode.
  //Note: 15 ms only works with Short distance mode.
  uint16_t TimingBudgetInMs = 100;
  
  //Intermeasurement period must be >/= timing budget.
  //This condition is not checked by the API.
  uint32_t InterMeasurementInMs = 150;

                                                            
  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);
  
  VL53L1_RdWord(Dev, 0x010F, &wordData);
  Serial.print(F("VL53L1X: "));
  Serial.println(wordData, HEX);

   status = VL53L1X_BootState(Dev, &byteData);
   Serial.print(F("VL53L1X_BootState: "));
   Serial.println(byteData);

   if (byteData !=3)
   {
    Serial.print("CANNOT CONNECT TO VL53L1X, CHECK WIRING...");
    while(1);
   }
   
   status = VL53L1X_SensorInit(Dev);
   Serial.print(F("VL53L1X_SensorInit: "));
   Serial.println(status, HEX);

   if (status !=0)
   {
    Serial.print("CANNOT INITALISE, FAULTY SENSOR, CHECK WIRING OR REPLACE!");
    while(1);
   }
   
   status = VL53L1X_SetDistanceMode(Dev, 2); // (1=short, 2=long) 
   Serial.print(F("VL53L1X_SetDistanceMode: "));
   Serial.println(status, HEX);

   VL53L1X_SetTimingBudgetInMs(Dev, TimingBudgetInMs);
   Serial.print(F("VL53L1X_SetTimingBudgetInMs: "));
   Serial.println(TimingBudgetInMs);
   
   VL53L1X_SetInterMeasurementInMs(Dev, InterMeasurementInMs);
   Serial.print(F("VL53L1X_SetInterMeasurementInMs: "));
   Serial.println(InterMeasurementInMs);

   status = VL53L1X_SetOffset(Dev, OffsetValue);
   Serial.print(F("VL53L1X_SetOffset: "));
   Serial.println(OffsetValue, HEX);
   
   status = VL53L1X_StartRanging(Dev);
   Serial.print(F("VL53L1X_StartRanging: "));
   Serial.println(status, HEX);
}

void loop()
{
  uint8_t isDataReady;
  static uint16_t distance;
  
  VL53L1X_CheckForDataReady(Dev, &isDataReady);
  if (isDataReady == 1)
  {
    VL53L1X_GetDistance(Dev, &distance);
    Serial.print(F("Distance(mm): "));
    Serial.println(distance);
  }

  VL53L1X_ClearInterrupt(Dev);
}
