#include <Wire.h>
#include "./api/vl53l1_platform.h"

VL53L1_Dev_t dev;        
VL53L1_DEV Dev = &dev;

void setup()
{
  uint8_t byteData;
  uint16_t wordData;

  Wire.begin();
  Wire.setClock(400000);
  Serial.begin(115200);

  Dev->I2cDevAddr = 0x52; //I2C address of the sensor

  //VL53L1_WaitDeviceBooted(Dev);
  //VL53L1_software_reset(Dev);

  VL53L1_RdByte(Dev, 0x010F, &byteData);
  Serial.print(F("VL53L1X Model_ID: "));
  Serial.println(byteData, HEX);
  VL53L1_RdByte(Dev, 0x0110, &byteData);
  Serial.print(F("VL53L1X Module_Type: "));
  Serial.println(byteData, HEX);
  VL53L1_RdWord(Dev, 0x010F, &wordData);
  Serial.print(F("VL53L1X: "));
  Serial.println(wordData, HEX);
}

void loop() {
  // put your main code here, to run repeatedly:

}
