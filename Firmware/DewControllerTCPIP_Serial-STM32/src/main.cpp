/*
Dew heater controller serving 2 TCP clients and a Serial client on Serial2

To have only one TCP client, eliminate the following variables and the corresponding code:
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


#include "dewHeaterController.h"

#define RPMPIN1 PA0
#define RPMPIN2 PA1
#define MAGPOLES 2 // number of poles driving the Hall sensor
#define ETHLED PC13

#define DHTPIN PB11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
long rpm = 0;
int speed = 9999;
DHT dhtSensor(DHTPIN,DHTTYPE);


void setup()
{
	dhtSensor.begin();
	pwmPin[0] = PB6;
	pwmPin[1] = PB7;
	pwmPin[2] = PB8;
	pwmPin[3] = PB3;
	pwmPin[4] = PA15;
	pwmPin[5] = PB9;
	pwmPin[6] = PB4;
	pwmPin[7] = PB5;

	analogWriteFrequency(25000); // set PWM frequency - all timers - to 25KHz (default is 1kHz)
//	analogWriteFrequency(8000); // set PWM frequency - all timers - to 8KHz (default is 1kHz)
//	analogWriteFrequency(1000); // set PWM frequency - all timers - to 1KHz - default
//	analogWriteResolution(ANALOGWRITERESOLUTION); // default

	for(int i=0; i<8; i++)
	{
		pinMode(pwmPin[i], OUTPUT);
		analogWrite(pwmPin[i],0);
	}

// Reset W5500 before starting
	pinMode(RESETPIN, OUTPUT);
	digitalWrite(RESETPIN, LOW);
	delay(300);
	digitalWrite(RESETPIN, HIGH);
	delay(300);

//  switch off ETH LED
	pinMode(ETHLED, OUTPUT);
	digitalWrite(ETHLED, LOW);

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
		digitalWrite(ETHLED, LOW);
#ifdef DEBUG
		Serial.println("Ethernet shield was not found.  Sorry, can't connect to ethernet without hardware.");
#endif
	}
	else
	{
		digitalWrite(ETHLED, HIGH);
	}
	delay(2000);
	if (Ethernet.linkStatus() == LinkOFF)
	{
		digitalWrite(ETHLED, LOW);
#ifdef DEBUG
    	Serial.println("Ethernet cable is not connected.");
#endif
	}
	else
	{
		digitalWrite(ETHLED, HIGH);
	}

  // start listening for clients
//	server.begin();

#ifdef DEBUG
	Serial.print("Dew controller IP address: ");
	Serial.println(Ethernet.localIP());
#endif

	dewControllerData.command = 0;
	dewControllerData.fanRPMcapability = 0;
	dewControllerData.temperature = 0;
	dewControllerData.airHumidity = 0;
	dewControllerData.airPressure = 0;
	for (int i = 0; i < 8; i++)
	{
		dewControllerData.pwmDevice[i] = 0;
	}
} // setup()


long GetRPM(int fanNumber)
{
  long localRPM;
  switch(fanNumber)
  {
	  case 1:
	  	if(dewControllerData.fanRPMcapability & 1)
		{
			localRPM = pulseInLong(RPMPIN1, HIGH)* MAGPOLES * 2; // duration of one rotation of the fan (us)//
		}
		else
		{
			localRPM = 0;
		}
	  break;
	  
	  case 2:
	  	if(dewControllerData.fanRPMcapability & 2)
		{
			localRPM = pulseInLong(RPMPIN2, HIGH)* MAGPOLES * 2; // duration of one rotation of the fan (us)//
		}
		else
		{
			localRPM = 0;
		}
	  break;
	  
	  default:
	  localRPM = 0;
  }
  if(localRPM > 0)
  {
    localRPM = 60000000 / localRPM;
  }
  return localRPM;
}


long GetDebouncedRPM(int fanNumber)
{
  switch(fanNumber)
  {
	case 1:
		if(dewControllerData.pwmDevice[6] < 1 || !(dewControllerData.fanRPMcapability & 1 ))
		{
			return 0;
		}
	break;

	case 2:
		if(dewControllerData.pwmDevice[7] < 1 || !(dewControllerData.fanRPMcapability & 2 ))
		{
			return 0;
		}
	break;
  }
  long localRPM;
  for(int i=0; i<20; i++)
  {

    localRPM = GetRPM(fanNumber);
   	long diff = abs(rpm - localRPM);
	if(diff < 100 && localRPM < 2000)
	{
		rpm = localRPM;
		return rpm;
	}
  }
  if(localRPM > 2500)
  {
    rpm = 0;
  }
  else
  {
    rpm = localRPM;
  }
  return rpm;
}


void PackData()
{
	PollDhtSensor();
	char dataElement[100];
	buffer[0] = 0;
	itoa( dewControllerData.command, dataElement, 10);
	strcat(buffer, dataElement);
	strcat(buffer, ",");
	for (int i = 1; i < 9; i++)
	{
		itoa( dewControllerData.pwmDevice[i-1], dataElement, 10);
		strcat(buffer, dataElement);
		strcat(buffer, ",");
	}
		for (int i = 9; i < 15; i++)
	{
		switch (i)
		{
		case 9:
			itoa( dewControllerData.fanRPMcapability, dataElement, 10);
			break;
		case 10:
			itoa( dewControllerData.temperature, dataElement, 10);
			break;
		case 11:
			itoa( dewControllerData.airHumidity, dataElement, 10);
			break;
		case 12:
			itoa( dewControllerData.airPressure, dataElement, 10);
			break;
		case 13:
			itoa( dewControllerData.rpm1, dataElement, 10);
			break;
		case 14:
			itoa( dewControllerData.rpm2, dataElement, 10);
			break;
		default:
			break;
		}
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
				dewControllerData.fanRPMcapability = atoi(localString);
				break;
			default:
				break;
			}
			dataCounter++;
		}

	}
}


void SetHeatersAndFans()
{
	for(int i=0; i<8; i++)
	{
		analogWrite(pwmPin[i], dewControllerData.pwmDevice[i]);
	}
}


//*
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
	Serial.print("temperature: "); Serial.println(dewControllerData.temperature/100);
	Serial.print("airHumidity: "); Serial.println(dewControllerData.airHumidity/100);
	Serial.println();
	Serial.print("airPressure: "); Serial.println(dewControllerData.airPressure);
	Serial.println();

	Serial.print("Fans RPM capability: "); Serial.println(dewControllerData.fanRPMcapability);
	Serial.println();

	Serial.print("RPM 1: "); Serial.println(dewControllerData.rpm1);
	Serial.print("RPM 2: "); Serial.println(dewControllerData.rpm2);
	Serial.println();


}
#endif
//*/


void PollDhtSensor()
{
	dewControllerData.temperature = dhtSensor.readTemperature()*100;
	dewControllerData.airHumidity = dhtSensor.readHumidity()*100;
}


void HartBeat()
{
	char txBuffer[50];
	strcpy(txBuffer, HARTBEATRESPONSE);

	if(serialservices.isSerialRequest)
	{
		serialservices.SendResponse(txBuffer);
	}

	if(tcpservice.isTCPrequest)
	{
		tcpservice.clientTimer = millis();
		tcpservice.clientConnect = 1;
		tcpservice.SendResponse(txBuffer);
	}
	if(tcpservice1.isTCPrequest)
	{
		tcpservice1.clientTimer = millis();
		tcpservice1.clientConnect = 1;
		tcpservice1.SendResponse(txBuffer);
	}
}


void SelectAndSend(char *response)
{
	if(serialservices.isSerialRequest)
	{
		serialservices.SendResponse(response);
	}
	if(tcpservice.isTCPrequest)
	{
		tcpservice.SendResponse(response);
	}
	if(tcpservice1.isTCPrequest)
	{
		tcpservice1.SendResponse(response);
	}
	buffer[0] = 0;
}


void ProcessRequests()
{
#ifdef DEBUG
	if(strlen(buffer))
	{
//		Serial.print("\r\nReceived: ");
//		Serial.println(buffer);
	}
#endif

	if(!strcmp(buffer,HARTBEATCALL))
	{
		HartBeat();
		return;
	}

	if(!strcmp(buffer,GETDATA))
	{
		dewControllerData.command = GETDATAHEADER;
		PackData();
		SelectAndSend(buffer);
		return;
	}

	if(!strcmp(buffer,GETRPM))
	{
		dewControllerData.rpm1 = GetDebouncedRPM(1);
		dewControllerData.rpm2 = GetDebouncedRPM(2);
		SelectAndSend((char*)CONFIRMATION);
	}
	
	if ( !strncmp(buffer, SENDDATA, 4) )
//	if (buffer[0] == '2' && buffer[1] == '8' && buffer[2] == '1'&& buffer[3] == '3' && buffer[4] == ',')
                      //UnpackData("2735,11,22,33,44,55,66,111,222,11,853,9025,51,52,53,54,55,56,57,58");
					  // Serial.println(GetDebouncedRPM(), DEC);

					  // 2735,0,0,0,0,0,0,0,0,0,
					  // 2735,2,2,2,2,2,2,2,2,0,
					  // 2735,4,4,4,4,4,4,4,4,0,
					  // 2735,8,8,8,8,8,8,8,8,0,
					  // 2735,16,16,16,16,16,16,16,16,0
					  // 2735,32,32,32,32,32,32,32,32,0
					  // 2735,64,64,64,64,64,64,64,64,0,
					  // 2735,127,127,127,127,127,127,127,127,0,
					  // 2735,255,255,255,255,255,255,255,255,0,


	{
		UnpackData(buffer);
		SetHeatersAndFans();
		SelectAndSend((char*)CONFIRMATION);
		#ifdef DEBUG
		DisplayData();
		#endif
		return;
	}
	buffer[0] = 0;
}


void loop()
{
	tcpservice.CheckTCPClient(buffer);
	tcpservice1.CheckTCPClient(buffer);
	serialservices.CheckSerialClient(buffer);
	ProcessRequests();
	tcpservice.CheckConnections();
	tcpservice1.CheckConnections();
}
