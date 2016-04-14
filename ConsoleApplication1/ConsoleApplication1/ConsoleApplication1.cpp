// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "TeensyI2C.h"
#include "TeensyI2CReceiver.h"


using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	beginFrame();
	addParameter(4, uint8_t(20));
	addParameter(5, uint8_t(20));
	addParameter(6, int16_t(2046));
	addParameter(7, float(-5000));
	endFrame();

	initReceiver();

	receiveBytes(buffer, bufferPos);


	int a;
	cin >> a;
	return 0;
}

