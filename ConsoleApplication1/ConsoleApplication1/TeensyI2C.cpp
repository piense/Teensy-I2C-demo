#include "stdafx.h"
#include "TeensyI2C.h"

uint8_t* buffer;
uint16_t bufferPos;
uint8_t checksum;
uint16_t frameLength;

void beginFrame()
{
	buffer = new uint8_t[I2C_BUFFER_SIZE];
	buffer[0] = I2C_CONTROL_CHAR;
	buffer[1] = I2C_FRAME_START;
	bufferPos = 4;
	//2,3 will be filled in with the length later
}

void calculateChecksum(uint16_t lastByte)
{
	checksum = 0;
	for (int i = 0; i < lastByte;i++)
		checksum ^= buffer[i];
}

void endFrame()
{
	frameLength = bufferPos;

	uint8_t length1 = (frameLength & 0xFF00) >> 8;
	uint8_t length2 = frameLength & 0xFF;

	if (length1 != I2C_CONTROL_CHAR && length2 != I2C_CONTROL_CHAR)
	{
		buffer[2] = length1;
		buffer[3] = length2;
	}
	else if (length1 == I2C_CONTROL_CHAR && length2 == I2C_CONTROL_CHAR)
	{
		buffer[2] = length1;
		buffer[3] = length1;
		buffer[4] = length2;
		buffer[5] = length2;
		memcpy(buffer + 4, buffer + 6, bufferPos - 4);
		bufferPos += 2;
	}
	else{
		if (length1 == I2C_CONTROL_CHAR){
			buffer[2] = length1;
			buffer[3] = length1;
			buffer[4] = length2;
		}
		else{
			buffer[2] = length1;
			buffer[3] = length2;
			buffer[4] = length2;
		}
		memcpy(buffer + 4, buffer + 5, bufferPos - 4);
		bufferPos += 1;
	}
	
	

	calculateChecksum(bufferPos);

	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_FRAME_END;
	buffer[bufferPos + 2] = checksum;
	buffer[bufferPos + 3] = 0x00;
	bufferPos += 4;

	
}


/*
	Each parameter shall begin with {0xD5,0xD3,0xXZ,0xYY} where ‘X’
	is the number of data bytes not including escaped data bytes,
	Z is the primitive type of the data and YY is the parameter
	identifier given above. Z is given below.

	Data Types ‘Z’:
	0 - Raw byte string
	1- Signed Integer
	2 - Unsigned Integer
	3 - Floating Type
*/

void addParameter(uint8_t index, int8_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x11;
	buffer[bufferPos + 3] = index;

	if (data == I2C_CONTROL_CHAR) {
		buffer[bufferPos + 4] = I2C_CONTROL_CHAR;
		buffer[bufferPos + 5] = I2C_CONTROL_CHAR;
		bufferPos += 6;
	}
	else
	{
		buffer[bufferPos + 4] = data;
		bufferPos += 5;
	}
}

void addParameter(uint8_t index, uint8_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;	
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x12;
	buffer[bufferPos + 3] = index;

	if (data == I2C_CONTROL_CHAR) {
		buffer[bufferPos + 4] = I2C_CONTROL_CHAR;
		buffer[bufferPos + 5] = I2C_CONTROL_CHAR;
		bufferPos += 6;
	}
	else
	{
		buffer[bufferPos + 4] = data;
		bufferPos += 5;
	}
 }

void addParameter(uint8_t index, int16_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x21;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos+1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, uint16_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x22;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, int64_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x81;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, uint64_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x82;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, int32_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x41;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, uint32_t data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x42;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	for (int i = 0; i < sizeof(data); i++)
	{
		uint8_t byte = (data & (0xFF << ((sizeof(data) - 1) * 8))) >> ((sizeof(data) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		data = data << 8;
	}
}

void addParameter(uint8_t index, float data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x43;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	uint32_t rawData;
	memcpy(&rawData, &data, 4);

	for (int i = 0; i < sizeof(rawData); i++)
	{
		uint8_t byte = (rawData & (0xFF << ((sizeof(rawData) - 1) * 8))) >> ((sizeof(rawData) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		rawData = rawData << 8;
	}
}

void addParameter(uint8_t index, double data)
{
	buffer[bufferPos] = I2C_CONTROL_CHAR;
	buffer[bufferPos + 1] = I2C_PARAMETER_START;
	buffer[bufferPos + 2] = 0x43;
	buffer[bufferPos + 3] = index;
	bufferPos += 4;

	uint64_t rawData;
	memcpy(&rawData, &data, 4);

	for (int i = 0; i < sizeof(rawData); i++)
	{
		uint8_t byte = (rawData & (0xFF << ((sizeof(rawData) - 1) * 8))) >> ((sizeof(rawData) - 1) * 8);
		if (byte == I2C_CONTROL_CHAR) {
			buffer[bufferPos] = I2C_CONTROL_CHAR;
			buffer[bufferPos + 1] = I2C_CONTROL_CHAR;
			bufferPos += 2;
		}
		else
		{
			buffer[bufferPos] = byte;
			bufferPos += 1;
		}
		rawData = rawData << 8;
	}
}