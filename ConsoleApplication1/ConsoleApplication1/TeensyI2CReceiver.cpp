#include "stdafx.h"
#include "TeensyI2CReceiver.h"
#include <stdint.h>

uint8_t* buffer2;
uint16_t bufferBegin;
uint16_t bufferLength;

void initReceiver()
{
	buffer2 = new uint8_t[I2C_BUFFER_SIZE];
	bufferLength = 0;
	bufferBegin = 0;
}

void receiveBytes(uint8_t* data, uint16_t length)
{
	uint16_t pos = 0;

	if (length > I2C_BUFFER_SIZE)
		return;

	//Got some bad data at some point
	if (bufferLength + length > I2C_BUFFER_SIZE)
	{
		bufferLength = 0;
		bufferBegin = 0;
	}

	if ((bufferBegin + length) <= I2C_BUFFER_SIZE)
	{
		for (int i = 0; i < length; i++)
			buffer2[i + bufferBegin] = data[i];
		bufferLength += length;
	}
	else{
		for (int i = 0; i <= (I2C_BUFFER_SIZE - bufferBegin); i++){
			buffer2[i + bufferBegin] = data[i];
		}
		int x = (I2C_BUFFER_SIZE - bufferBegin) + 1;
		bufferBegin = 0;
		for (int i = x; i < length; i++)
		{
			buffer2[x-i] = data[i];
		}
		bufferLength += length;
	}

	processBuffer();
}

void processBuffer()
{
	uint8_t* frameBuffer = new uint8_t[I2C_BUFFER_SIZE];

	int position = bufferBegin;

	for (int i = 0; i < bufferLength; i++)
		printf("%x\n", buffer2[i]);

	//Search for a full frame
	for (int i = 0; i < bufferLength; i++)
	{
		if (buffer2[(bufferBegin + i) % I2C_BUFFER_SIZE] == I2C_CONTROL_CHAR && buffer2[((bufferBegin + i + 1) % I2C_BUFFER_SIZE)] == I2C_FRAME_START)
		{
			//Found the start of a frame

			//See if we should have an ending yet
			if (i + 3 < bufferLength)
			{
				uint16_t frameLength = buffer2[(bufferBegin + i + 2) % I2C_BUFFER_SIZE];
				if (frameLength == I2C_CONTROL_CHAR)
					frameLength = (frameLength << 8) + buffer2[(bufferBegin + i + 4) % I2C_BUFFER_SIZE];
				else
					frameLength = (frameLength << 8) + buffer2[(bufferBegin + i + 3) % I2C_BUFFER_SIZE];

				if (i + 4 + frameLength <= bufferLength)
				{
					for (int x = i; x < frameLength + 4; x++)
					{
						frameBuffer[x - i] = buffer2[x % I2C_BUFFER_SIZE];
					}

					processFrame(frameBuffer, frameLength + 4);

					//Process the frame!
					i += (frameLength + 4) % I2C_BUFFER_SIZE;
				}
				else{
					i = bufferLength;
				}
			}
		}
	}
}

void processFrame(uint8_t *frameBuffer, uint16_t length)
{
	//Check the start and end headers
	if (frameBuffer[0] != I2C_CONTROL_CHAR || frameBuffer[1] != I2C_FRAME_START || frameBuffer[length - 4] != I2C_CONTROL_CHAR || frameBuffer[length - 3] != I2C_FRAME_END)
		return;

	uint8_t checksum = 0;
	for (int i = 0; i < length-4; i++)
		checksum ^= frameBuffer[i];

	if (frameBuffer[length - 2] != checksum)
		return;

	checksum += 2;
	
}