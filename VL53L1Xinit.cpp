#include <Arduino.h>
#include "VL53L1X_api.h"

int sensorinit(uint8_t deviceaddress, uint16_t GPIO1pin, uint16_t TimingBudgetInMs, uint32_t InterMeasurementInMs, int16_t OffsetValue)
{
	uint8_t byteData;
	uint16_t wordData;
	VL53L1X_ERROR status = 0;

	VL53L1_RdWord(deviceaddress, 0x010F, &wordData);
	#ifdef DEBUG
	Serial.print(F("VL53L1X: "));
	Serial.println(wordData, HEX);
	#endif
	
	status = VL53L1X_BootState(deviceaddress, &byteData);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_BootState: "));
	Serial.println(byteData);
	#endif
	
	if (byteData !=3)
	{
		Serial.print("CANNOT CONNECT TO VL53L1X, CHECK WIRING...");
		return 1;
	}
	
	status = VL53L1X_SensorInit(deviceaddress);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_SensorInit: "));
	Serial.println(status, HEX);
	#endif
	
	if (status !=0)
	{
		Serial.print("CANNOT INITALISE, FAULTY SENSOR, CHECK WIRING OR REPLACE!");
		return 1;
	}
	
	status = VL53L1X_SetDistanceMode(deviceaddress, 2); // (1=short, 2=long)
	#ifdef DEBUG
	Serial.print(F("VL53L1X_SetDistanceMode: "));
	Serial.println(status, HEX);
	#endif
	
	VL53L1X_SetTimingBudgetInMs(deviceaddress, TimingBudgetInMs);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_SetTimingBudgetInMs: "));
	Serial.println(TimingBudgetInMs);
	#endif
	
	VL53L1X_SetInterMeasurementInMs(deviceaddress, InterMeasurementInMs);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_SetInterMeasurementInMs: "));
	Serial.println(InterMeasurementInMs);
	#endif
	
	status = VL53L1X_SetOffset(deviceaddress, OffsetValue);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_SetOffset: "));
	Serial.println(OffsetValue, HEX);
	#endif
	
	status = VL53L1X_StartRanging(deviceaddress);
	#ifdef DEBUG
	Serial.print(F("VL53L1X_StartRanging: "));
	Serial.println(status, HEX);
	#endif
	
	return 0;
}