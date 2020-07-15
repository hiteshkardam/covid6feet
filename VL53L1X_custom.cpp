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
	Serial.println(OffsetValue);
	#endif
	
	status = VL53L1X_StartRanging(deviceaddress);
	#if DEBUG
	Serial.print(F("VL53L1X_StartRanging: "));
	Serial.println(status, HEX);
	#endif
	
	Serial.println(F("-----------------------------------------------------------------------------"));
}

//VL53L1X_StopRanging(uint16_t dev) function stops the ranging.

//VL53L1X_SetDistanceMode distance mode. Short mode, DM = 1, Long mode, DM = 2.
//VL53L1X_GetDistanceMode current distance mode.

//VL53L1X_SetTimingBudget TB in milliseconds. The TB values available are [15, 20, 33, 50, 100, 200, 500].
//GetTimingBudgetInMs current TB.
//Note: 15 ms only with Short distance mode. Called after VL53L1X_SetDistanceMode.

//VL53L1X_SetInterMeasurementInMs intermeasurement period in milliseconds.
//VL53L1X_GetIntermeasurementInMs curent IMP.
//Note: The IMP must be greater than or equal to the TB.

//The VL53L1X_StartRanging make a distance measurement.

//|---------|---------|------------------|--------->
//0         900      1800               3000              
//    t3         t2            t1             t0

//t0
//DistanceMode		2(long)
//TimingBudget		50
//InterMeasurement	1000

//t1
//DistanceMode		2(long)
//TimingBudget		100
//InterMeasurement	110

//t2
//DistanceMode		2(long)
//TimingBudget		50
//InterMeasurement	60

//t3
//DistanceMode		1(short)
//TimingBudget		15
//InterMeasurement	20


void settiming(uint16_t devadr, uint8_t timingmode)
{
	uint16_t currDistanceMode;
	uint16_t currIM;
	uint16_t currTimingBudgetInMs;
	
	/*
	VL53L1X_StopRanging(devadr);
	VL53L1X_GetDistanceMode(devadr, uint16_t *currDistanceMode);
	VL53L1X_GetInterMeasurementInMs(devadr, uint16_t * currIM);
	VL53L1X_GetTimingBudgetInMs(devadr, uint16_t *currTimingBudgetInMs);
	*/
	
	switch (timingmode)
	{
		case 0:
		VL53L1X_SetDistanceMode(devadr, 2);	
		VL53L1X_SetInterMeasurementInMs(devadr, 50);
		VL53L1X_SetTimingBudgetInMs(devadr, 1000);
		Serial.print("Timing Mode:");
		Serial.println(timingmode);
		break;
		
		case 1:
		VL53L1X_SetDistanceMode(devadr, 1);	
		VL53L1X_SetInterMeasurementInMs(devadr, 100);
		VL53L1X_SetTimingBudgetInMs(devadr, 110);
		Serial.print("Timing Mode:");
		Serial.println(timingmode);
		break;
		
		case 2:
		VL53L1X_SetDistanceMode(devadr, 2);	
		VL53L1X_SetInterMeasurementInMs(devadr, 50);
		VL53L1X_SetTimingBudgetInMs(devadr, 60);
		Serial.print("Timing Mode:");
		Serial.println(timingmode);
		break;
		
		case 3:
		VL53L1X_SetDistanceMode(devadr, 1);	
		VL53L1X_SetInterMeasurementInMs(devadr, 15);
		VL53L1X_SetTimingBudgetInMs(devadr, 20);
		Serial.print("Timing Mode:");
		Serial.println(timingmode);
		break;
		
		default:
		break;
	}
	
	VL53L1X_StartRanging(devadr);
}
