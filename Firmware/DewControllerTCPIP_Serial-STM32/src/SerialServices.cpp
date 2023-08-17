#include "SerialServices.h"
#include <Arduino.h>

SerialServices::SerialServices()
{
    isSerialRequest = false;
}

SerialServices::~SerialServices()
{

}

void SerialServices::CheckSerialClient(char *buffer)
{
    if (Serial.available() > 0)
    {
        //at least one character has been received.
        //read the whole string.
  		isSerialRequest = true;
		int bufferLen = Serial.readBytesUntil(0, buffer, 1024);
//		int bufferLen = Serial.readBytesUntil(13, buffer, 1024);
		buffer[bufferLen] = 0;
	    buffer[strcspn(buffer, "\r")] = 0;
	    buffer[strcspn(buffer, "\n")] = 0;
//		Serial.println(buffer);
    }
}


void SerialServices::SendResponse(char* response)
{
	if(isSerialRequest)
		Serial.println(response);

//	buffer[0] = 0;
	isSerialRequest = false;
}


