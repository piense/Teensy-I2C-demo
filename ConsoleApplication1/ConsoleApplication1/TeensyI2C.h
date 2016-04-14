#include <stdint.h>

#ifndef TeensyI2C_H
#define TeensyI2C_H

#define I2C_BUFFER_SIZE 2000 
#define I2C_CONTROL_CHAR 0xD5
#define I2C_FRAME_START 0xD0
#define I2C_PARAMETER_START 0xD3
#define I2C_FRAME_END 0xD8

extern uint8_t* buffer;
extern uint16_t bufferPos;
extern uint8_t checksum;
extern uint16_t frameLength;

void beginFrame();
void calculateChecksum(uint16_t lastByte);
void endFrame();

void addParameter(uint8_t index, int8_t data);
void addParameter(uint8_t index, uint8_t data);
void addParameter(uint8_t index, int16_t data);
void addParameter(uint8_t index, uint16_t data);
void addParameter(uint8_t index, int64_t data);
void addParameter(uint8_t index, uint64_t data);
void addParameter(uint8_t index, int32_t data);
void addParameter(uint8_t index, uint32_t data);
void addParameter(uint8_t index, float data);
void addParameter(uint8_t index, double data);

#endif