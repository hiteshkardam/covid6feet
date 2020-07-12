
/* 
* This file is part of VL53L1 Platform 
* 
* Copyright (c) 2016, STMicroelectronics - All Rights Reserved 
* 
* License terms: BSD 3-clause "New" or "Revised" License. 
* 
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this 
* list of conditions and the following disclaimer. 
* 
* 2. Redistributions in binary form must reproduce the above copyright notice, 
* this list of conditions and the following disclaimer in the documentation 
* and/or other materials provided with the distribution. 
* 
* 3. Neither the name of the copyright holder nor the names of its contributors 
* may be used to endorse or promote products derived from this software 
* without specific prior written permission. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
* 
*/

#include <Arduino.h>
#include <Wire.h>

#include "vl53l1_platform.h"
#include "vl53l1X_api.h"
#include <string.h>
#include <time.h>
#include <math.h>

int8_t VL53L1_WriteMulti( uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count) {
  while (count > 0)
  {
    Wire.beginTransmission(dev >> 1);
    Wire.write((index >> 8) & 0xff);
    Wire.write(index & 0xff);

    uint8_t writing = 0;

    while (count > 0 && Wire.write(*pdata) != 0)
    {
      pdata++;
      writing++;
      count--;
    }

    if (writing == 0 || Wire.endTransmission() != 0) { return 1; }
    index += writing;
  }
	return 0;
}

int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count){
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	if (Wire.endTransmission() != 0) { return 1; }
	
	while (count > 0)
	{
		uint8_t reading = Wire.requestFrom(dev >> 1, count);
		if (reading == 0) { return 1; }
		count -= reading;
		
		while (reading-- > 0)
		{
			*pdata++ = Wire.read();
		}
	}
	
	return 0;
}

int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	Wire.write(data);
	
	return (Wire.endTransmission() == 0 ? 0 : 1);
}

int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	Wire.write((data >> 8) & 0xff);
	Wire.write(data & 0xff);
	
	return (Wire.endTransmission() == 0 ? 0 : 1);
}

int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	Wire.write((data >> 24) & 0xff);
	Wire.write((data >> 16) & 0xff);
	Wire.write((data >> 8) & 0xff);
	Wire.write(data & 0xff);
	
	return (Wire.endTransmission() == 0 ? 0 : 1);
}

int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	if (Wire.endTransmission() != 0) { return 1; }
	if (Wire.requestFrom(dev >> 1, 1) != 1) { return 1; }
	*data = Wire.read();

	return 0;
}

int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	if (Wire.endTransmission() != 0) { return 1; }
	if (Wire.requestFrom(dev >> 1, 2) != 2) { return 1; }
	*data = (uint16_t)Wire.read() << 8;
	*data |= Wire.read();
	
	return 0;
}

int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data) {
	Wire.beginTransmission(dev >> 1);
	Wire.write((index >> 8) & 0xff);
	Wire.write(index & 0xff);
	if (Wire.endTransmission() != 0) { return 1; }
	if (Wire.requestFrom(dev >> 1, 4) != 4) { return 1; }
	*data = (uint32_t)Wire.read() << 24;
	*data |= (uint32_t)Wire.read() << 16;
	*data |= (uint16_t)Wire.read() << 8;
	*data |= Wire.read();
	
	return 0;
}

int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms){
	delay(wait_ms);
	
	return 0;
}
