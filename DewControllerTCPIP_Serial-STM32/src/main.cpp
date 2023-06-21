/*
Dew heater controller serving 2 TCP clients and a Serial client on Serial2

Fo have only one TCP client, eliminate the following variables and the corresponding code:
EthernetClient client1;
int dewHeaterDriverConnect1 = 0;
unsigned long dewDriverTimer1;
bool isTCPalreadyConnected1 = false;
bool isTCPrequest1 = false;

STM32F103C8T6
MISO - A6
MOSI - A7
CLK -  A5
CS -   A4
W5500 reset - B0

It is recommended to power the W5500 with 5V.
The SPI lines will work at 3.3V for high logic level.

If W5500 is powered at 3.3V, might will still work but the
MISO line will drop to 2V for high logic level which is the low limit for it.
Variations in 3.3 power supply may render the W5500 unresponsive.

Pull the reset pin of W5500 up to Vcc with a 5Kohm resistor. Use a 5V tolerant
pin of the STM32 module to reset the W5500 at startup.
*/

/*
	TODO:	red LED to indicate that ethernet cable is not connected
			red leds on analog exits for heaters and fans - they will be modulated by the PWM signal
*/



#include "dewHeaterController.h"

//#define RPMPIN PA0
//#define HALLPIN PA0 // pin for the hall sensor
#define RPMPIN PA1
#define HALLPIN PA1 // pin for the hall sensor
#define MAGPOLES 2 // number of poles driving the Hall sensor
#define ETHLED PC13

#define DHTPIN PB11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
long rpm = 0;
int speed = 9999;
DHT dhtSensor(DHTPIN,DHTTYPE);


long GetRPM()
{
  long localRPM;
  localRPM = pulseInLong(HALLPIN, HIGH)* MAGPOLES * 2; // duration of one rotation of the fan (us)//
//  Serial.print("GetRPM: "); Serial.println(localRPM,DEC);
  if(localRPM > 0)
    localRPM = 60000000 / localRPM;
  return localRPM;
}

long GetDebouncedRPM()
{
  long localRPM;
  for(int i=0; i<20; i++)
  {
    localRPM = GetRPM();
    long diff = abs(rpm - localRPM);
   if(diff < 100 && localRPM < 2000)
   {
    rpm = localRPM;
    return rpm;
   }
  }
  if(localRPM > 2000)
  {
    rpm = 0;
  }
  else
  {
    rpm = localRPM;
  }
  return rpm;
}

void setup()
{
	dhtSensor.begin();
	pwmPin[0] = PB6;
	pwmPin[1] = PB7;
	pwmPin[2] = PB8;
	pwmPin[3] = PB9;
	pwmPin[4] = PA15;
	pwmPin[5] = PB3;
	pwmPin[6] = PB4;
	pwmPin[7] = PB5;

//	analogWriteFrequency(25000); // set PWM frequency - all timers - to 25KHz (default is 1kHz)
	analogWriteFrequency(8000); // set PWM frequency - all timers - to 8KHz (default is 1kHz)
//	analogWriteFrequency(1000); // set PWM frequency - all timers - to 1KHz - default
//	analogWriteResolution(ANALOGWRITERESOLUTION); // default

	for(int i=0; i<8; i++)
	{
		pinMode(pwmPin[i], OUTPUT);
		analogWrite(pwmPin[i],0);
	}

//  switch off ETH LED
	pinMode(ETHLED, OUTPUT);
	digitalWrite(ETHLED, LOW);

// Reset W5500 before starting
	pinMode(RESETPIN, OUTPUT);
	digitalWrite(RESETPIN, LOW);
	delay(300);
	digitalWrite(RESETPIN, HIGH);
	delay(300);

// initialize the Ethernet device
//  Ethernet.begin(mac, ip, myDns, gateway, subnet);
	Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
//	Serial2.begin(19200);  // PA3  (RX)  PA2  (TX)
	Serial.begin(115200);  // PA10  (RX)  PA9  (TX)
	Serial.setTimeout(500);
//	Serial.setTimeout(3000);
#ifdef DEBUG
	Serial.println(""); Serial.println(""); Serial.println(""); Serial.println("");
	Serial.println("Serial1 started...");
#endif

  // Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware)
	{
		digitalWrite(ETHLED, HIGH);
#ifdef DEBUG
		Serial.println("Ethernet shield was not found.  Sorry, can't connect to ethernet without hardware.");
#endif
//    while (true) // Serial connection is available, so we can proceed without ethernet
//	{
//      delay(1); // do nothing, no point running without Ethernet hardware
//    }
	}
	delay(2000);
	if (Ethernet.linkStatus() == LinkOFF)
	{
		digitalWrite(ETHLED, HIGH);
#ifdef DEBUG
    	Serial.println("Ethernet cable is not connected.");
#endif
	}

  // start listening for clients
	server.begin();

#ifdef DEBUG
	Serial.print("Dew controller IP address: ");
	Serial.println(Ethernet.localIP());
#endif

	dewControllerData.command = 0;
	sensorTemperature = 0.;
	sensorAirHumidity = 0.;
	sensorAirPressure = 0.;  
	for (int i = 0; i < 8; i++)
	{
		dewControllerData.pwmDevice[i] = 0;
		dewControllerData.pwmLimitPercent[i] = 0;
	}
}

bool IsTimerExpired(unsigned long *timer, unsigned long intervalSeconds)
{
	long long timeNow = *timer;
	if( (timeNow + intervalSeconds*1000) > 4294967295 )
	{
		*timer = 0;
		return false;
	}
	timeNow = millis();

#ifdef DEBUG
//	Serial.print("timer   timeNow: ");
//	Serial.print(*timer);
//	Serial.print("   ");
//	Serial.println(timeNow);
//	Serial.print("TimeNow - Timer:                                "); Serial.println(timeNow-*timer);
#endif

	if( (timeNow - *timer + intervalSeconds*1000) > 4294967295)
	{
		return false;
	}
	if( (timeNow - *timer) > (intervalSeconds * 1000) )
	{
		*timer = timeNow;
		return true;
	}
	return false;
}

void PackData()
{

//#ifdef DEBUG
//sensorTemperature = 1.1;
//sensorAirHumidity = 85.3;
//sensorAirPressure = 1002.5;
//for (int i = 0; i < 8; i++)
//{
//	dewControllerData.pwmDevice[i] = i+10;
//	dewControllerData.pwmLimitPercent[i] = i+50;
//}
//#endif

	char dataElement[100];
	buffer[0] = 0;
	dewControllerData.temperature = sensorTemperature * 10;
	dewControllerData.airHumidity = sensorAirHumidity * 10;
	dewControllerData.airPressure = sensorAirPressure * 10;
	itoa( dewControllerData.command, dataElement, 10);
	strcat(buffer, dataElement);
	strcat(buffer, ",");
	for (int i = 1; i < 9; i++)
	{
		itoa( dewControllerData.pwmDevice[i-1], dataElement, 10);
		strcat(buffer, dataElement);
		strcat(buffer, ",");
	}
		for (int i = 9; i < 12; i++)
	{
		switch (i)
		{
		case 9:
			itoa( dewControllerData.temperature, dataElement, 10);
			break;
		case 10:
			itoa( dewControllerData.airHumidity, dataElement, 10);
			break;
		case 11:
			itoa( dewControllerData.airPressure, dataElement, 10);
			break;
		default:
			break;
		}
		strcat(buffer, dataElement);
		strcat(buffer, ",");
	}
	for (int i = 12; i < 20; i++)
	{
		itoa( dewControllerData.pwmLimitPercent[i-12], dataElement, 10);
		strcat(buffer, dataElement);
		strcat(buffer, ",");
	}
	int i = strlen(buffer);
	buffer[i-1] = 0;
}

void UnpackData(char* dataString)
{
	char localString[100];
	int charCounter = 0;
	int dataCounter = 0;
	int d = strlen(dataString);
	for (int i = 0; i < d; i++)
	{
		if(dataString[i] != ',')
		{
			localString[charCounter] = dataString[i];
			charCounter++;
		}
		else
		{
			localString[charCounter] = 0;
			charCounter = 0;
			switch (dataCounter)
			{
			case 0:
				dewControllerData.command = atoi(localString);
				break;
			case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
				dewControllerData.pwmDevice[dataCounter-1] = atoi(localString);
				break;
			case 9:
				dewControllerData.temperature = atoi(localString);
				break;
			case 10:
				dewControllerData.airHumidity = atoi(localString);
				break;
			case 11:
				dewControllerData.airPressure = atoi(localString);
				break;
			case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19:
				dewControllerData.pwmLimitPercent[dataCounter-12] = atoi(localString);
				break;

			default:
				break;
			}
			dataCounter++;
		}

	}
	localString[charCounter] = 0;
	dewControllerData.pwmLimitPercent[7] = atoi(localString);
	sensorTemperature = (float)dewControllerData.temperature / 10;
	sensorAirHumidity = (float)dewControllerData.airHumidity / 10;
	sensorAirPressure = (float)dewControllerData.airPressure / 10; 
}

void SetHeatersAndFans()
{
	for(int i=0; i<8; i++)
	{
		if(dewControllerData.pwmDevice[i] > maxPWMvalue)
			dewControllerData.pwmDevice[i] = maxPWMvalue;
		int PWMlimit = 	(maxPWMvalue * dewControllerData.pwmLimitPercent[i] / 100);
		if( dewControllerData.pwmDevice[i] > PWMlimit)
			analogWrite(pwmPin[i], PWMlimit);
		else
			analogWrite(pwmPin[i], dewControllerData.pwmDevice[i]);
		Serial.print("i = ");
		Serial.print(i);
		Serial.print("          ");
		Serial.print("pwmPin = ");
		Serial.print(pwmPin[i]);
		Serial.print("          ");
		Serial.print("pwmValue = ");
		Serial.print(dewControllerData.pwmDevice[i]);
		Serial.print("          ");
		Serial.print("pwmLimit = ");
		Serial.print(PWMlimit);
		Serial.print("          ");
		Serial.print("maxPWMvalue = ");
		Serial.println(maxPWMvalue);
	}
}

#ifdef DEBUG
void DisplayData()
{
	PollDhtSensor();

	Serial.print("Command: "); Serial.println(dewControllerData.command);
	for (int i = 0; i < 8; i++)
	{
		Serial.print("pwmDevice ");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(dewControllerData.pwmDevice[i]);
	}

	Serial.println();
	Serial.print("temperature: "); Serial.println(temperatureDht);
	Serial.print("airHumidity: "); Serial.println(humidityDht);
	Serial.println();


//	Serial.print("temperature: "); Serial.println(dewControllerData.temperature);
//	Serial.print("airHumidity: "); Serial.println(dewControllerData.airHumidity);

	Serial.print("airPressure: "); Serial.println(dewControllerData.airPressure);
	Serial.println();

	for (int i = 0; i < 8; i++)
	{
		Serial.print("pwmLimitPercent ");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(dewControllerData.pwmLimitPercent[i]);
	}
}
#endif

void CheckConnections()
{
	if(dewHeaterDriverConnect && isTCPalreadyConnected)
	{
		if( IsTimerExpired(&dewDriverTimer, TCPTIMEOUT) )
		{
			client.stop();
			dewHeaterDriverConnect = 0;
			isTCPalreadyConnected = false;
			isTCPrequest = false;
			#ifdef DEBUG
			Serial.println("Lost connection with the dew heater TCP client, disconnecting.");
			#endif
		}
	}

	if(dewHeaterDriverConnect1 && isTCPalreadyConnected1)
	{
		if( IsTimerExpired(&dewDriverTimer1, 30) )
		{
			client1.stop();
			dewHeaterDriverConnect1 = 0;
			isTCPalreadyConnected1 = false;
			isTCPrequest1 = false;
			#ifdef DEBUG
			Serial.println("Lost connection with the dew heater TCP client, disconnecting.");
			#endif
		}
	}
}

void SendResponse(char* response)
{
	if(isTCPrequest)
		client.println(response);
	if(isTCPrequest1)
		client1.println(response);
	if(isSerialrequest)
		Serial.println(response);

	buffer[0] = 0;
	isSerialrequest = false;
	isTCPrequest = false;
	isTCPrequest1 = false;
}

void PollDhtSensor()
{
	temperatureDht=dhtSensor.readTemperature();
	humidityDht=dhtSensor.readHumidity();
}

void ProcessRequests()
{
#ifdef DEBUG
	if(strlen(buffer))
	{
		Serial.print("\r\nReceived: ");
		Serial.println(buffer);
	}
#endif

	if(!strcmp(buffer,"DHCC"))
//	if(!strcmp(buffer,"DewHeaterController-Connect"))
	{
		Serial.println("DewHeater connetc received");
		if(isTCPrequest)
		{
			dewDriverTimer = millis();
			dewHeaterDriverConnect = 1;
		}
		if(isTCPrequest1)
		{
			dewDriverTimer1 = millis();
			dewHeaterDriverConnect1 = 1;
		}
		char txBuffer[50];
		strcpy(txBuffer, "DewHeaterController-Connect-Ok");
		isSerialrequest = true;
		SendResponse(txBuffer);
		return;
	}

//	if(!strcmp(buffer,"GetData"))
	if(!strcmp(buffer,"gd"))
	{
		dewControllerData.command = 11;
		PackData();
		SendResponse(buffer);
		return;
	}

	if (buffer[0] == '2' && buffer[1] == '7' && buffer[2] == '3'&& buffer[3] == '5' && buffer[4] == ',')
                      //UnpackData("2735,11,22,33,44,55,66,111,222,11,853,9025,51,52,53,54,55,56,57,58");
					  // Serial.println(GetDebouncedRPM(), DEC);

					  // 2735,0,0,0,0,0,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,2,2,2,2,2,2,2,2,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,4,4,4,4,4,4,4,4,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,8,8,8,8,8,8,8,8,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,16,16,16,16,16,16,16,16,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,32,32,32,32,32,32,32,32,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,64,64,64,64,64,64,64,64,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,127,127,127,127,127,127,127,127,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,255,255,255,255,255,255,255,255,255,853,9025,100,100,100,100,100,100,100,100


					  // 2735,127,0,0,0,0,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,127,0,0,0,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,127,0,0,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,0,127,0,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,0,0,127,0,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,0,0,0,127,0,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,0,0,0,0,127,0,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,0,0,0,0,0,0,0,127,11,853,9025,100,100,100,100,100,100,100,100


					  // 2735,257,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,255,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,201,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,201,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,127,22,33,44,55,66,111,222,11,853,9025,100,100,100,100,100,100,100,100
					  // 2735,100,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,64,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,32,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,16,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,8,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,4,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,2,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,1,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,201,22,33,44,55,66,111,222,11,853,9025,100,52,53,54,55,56,57,58
					  // 2735,257,22,33,44,55,66,111,1,11,853,9025,50,52,53,54,55,56,57,58
					  // 2735,255,22,33,44,55,66,111,255,11,853,9025,51,52,53,54,55,56,57,100
					  // 2735,255,22,33,44,55,66,111,255,11,853,9025,50,52,53,54,55,56,57,50
					  // 2735,255,22,33,44,55,66,111,0,11,853,9025,50,52,53,54,55,56,57,58
					  // 2735,255,22,33,44,55,66,111,4095,11,853,9025,50,52,53,54,55,56,57,50
					  // 2735,255,22,33,44,55,66,111,1,11,853,9025,50,52,53,54,55,56,57,58
	{
		UnpackData(buffer);
		SetHeatersAndFans();
		SendResponse((char*)"OK");
		#ifdef DEBUG
		DisplayData();
		#endif
		return;
	}

	buffer[0] = 0;
}


void DisplayTmperature()
{
	PollDhtSensor();
	Serial.print("temperature: "); Serial.println(temperatureDht);
	Serial.print("airHumidity: "); Serial.println(humidityDht);
	Serial.println();
}


void loop()
{
// wait for a new client:
  if (!isTCPalreadyConnected)
  {
  	client = server.available();
 // 	Serial.println("Client1 server available.");
  }

  if (!isTCPalreadyConnected1)
  {
  	client1 = server1.available();
 // 	Serial.println("Client2 server available.");
  }

  // when the client sends the first byte, say hello:
  if (client)
  {
    if (!isTCPalreadyConnected)
    {
      // clear out the input buffer:
      client.flush();
#ifdef DEBUG
      Serial.println("We have a new client-0");
#endif
      client.println("Hello, astro client!");
	  isTCPalreadyConnected = true;
      dewHeaterDriverConnect = 1;
	  dewDriverTimer = millis();
    }
    if (client.available() > 0)
	{
      // read the message from the client:
      int bufferLen = client.read((uint8_t*)buffer, 1024);
	  buffer[bufferLen] = 0;
	  buffer[strcspn(buffer, "\r")] = 0;
	  buffer[strcspn(buffer, "\n")] = 0;
	  isTCPrequest = true;
    }
  }

  if (client1)
  {
    if (!isTCPalreadyConnected1)
    {
      // clear out the input buffer:
      client1.flush();
#ifdef DEBUG
      Serial.println("We have a new client-1");
#endif
      client1.println("Hello, astro client-1!");
	  isTCPalreadyConnected1 = true;
      dewHeaterDriverConnect1 = 1;
	  dewDriverTimer1 = millis();
    }
    if (client1.available() > 0)
	{
      // read the message from the client:
      int bufferLen = client1.read((uint8_t*)buffer, 1024);
	  buffer[bufferLen] = 0;
	  buffer[strcspn(buffer, "\r")] = 0;
	  buffer[strcspn(buffer, "\n")] = 0;
	  isTCPrequest1 = true;
    }
  }

    if (Serial.available() > 0)
    {
        //at least one character has been received.
        //read the whole string.
  		isSerialrequest = true;
		int bufferLen = Serial.readBytesUntil(0, buffer, 1024);
//		int bufferLen = Serial.readBytesUntil(13, buffer, 1024);
		buffer[bufferLen] = 0;
	    buffer[strcspn(buffer, "\r")] = 0;
	    buffer[strcspn(buffer, "\n")] = 0;
    }   
//	Serial.print("Debounced RPM: "); Serial.println(GetDebouncedRPM(), DEC);
	ProcessRequests();
 	isTCPrequest = false;
	isTCPrequest1 = false;
//	CheckConnections();

delay(4000);
DisplayTmperature();


}


