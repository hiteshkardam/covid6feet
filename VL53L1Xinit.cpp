#include <Arduino.h>
#include "VL53L1X_api.h"

#define DEBUG 1

void sensorinit(uint8_t deviceaddress, uint16_t GPIO1pin, uint16_t TimingBudgetInMs, uint32_t InterMeasurementInMs, int16_t OffsetValue)
{
	uint8_t byteData;
	uint16_t wordData;
	VL53L1X_ERROR status = 0;

	VL53L1_RdWord(deviceaddress, 0x010F, &wordData);
	#if DEBUG
	Serial.print(F("VL53L1X: "));
	Serial.println(wordData, HEX);
	#endif
	
	status = VL53L1X_BootState(deviceaddress, &byteData);
	#if DEBUG
	Serial.print(F("VL53L1X_BootState: "));
	Serial.println(byteData);
	#endif
	
	if (byteData !=3)
	{
		delay(10);
		VL53L1X_BootState(deviceaddress, &byteData);
		if (byteData !=3)
		{
			Serial.print("CANNOT CONNECT TO VL53L1X, CHECK WIRING...");
			while(1);
		}
	}
  
	status = VL53L1X_SensorInit(deviceaddress);
	#if DEBUG
	Serial.print(F("VL53L1X_SensorInit: "));
	Serial.println(status, HEX);
	#endif
	
	if (status !=0)
	{
		Serial.print("CANNOT INITALISE, FAULTY SENSOR, CHECK WIRING OR REPLACE!");
		while(1);
	}
   
	status = VL53L1X_SetDistanceMode(deviceaddress, 2); // (1=short, 2=long)
	#if DEBUG
	Serial.print(F("VL53L1X_SetDistanceMode: "));
	Serial.println(status, HEX);
	#endif
	
	/*
	status = VL53L1X_SetDistanceThreshold (deviceaddress, 0, 3000, 3, 1); //measure only when between 0cm and 300cm(10ft)
	#if DEBUG
	Serial.print(F("VL53L1X_SetDistanceThreshold: "));
	Serial.println(status, HEX);
	#endif
	*/
  
	VL53L1X_SetTimingBudgetInMs(deviceaddress, TimingBudgetInMs);
	#if DEBUG
	Serial.print(F("VL53L1X_SetTimingBudgetInMs: "));
	Serial.println(TimingBudgetInMs);
	#endif
	
	VL53L1X_SetInterMeasurementInMs(deviceaddress, InterMeasurementInMs);
	#if DEBUG
	Serial.print(F("VL53L1X_SetInterMeasurementInMs: "));
	Serial.println(InterMeasurementInMs);
	#endif
	
	status = VL53L1X_SetOffset(deviceaddress, OffsetValue);
	#if DEBUG
	Serial.print(F("VL53L1X_SetOffset: "));
	Serial.println(OffsetValue, HEX);
	#endif
	
	status = VL53L1X_StartRanging(deviceaddress);
	#if DEBUG
	Serial.print(F("VL53L1X_StartRanging: "));
	Serial.println(status, HEX);
	#endif
}
