/*
STM32F103C8T6
MISO - A6
MOSI - A7
CLK -  A5
CS -   A4
*/


/*
 Chat Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino WIZnet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified for STM32 on 16 jan 2023
 by Radmilo Felix


It is recommended to power the W5500 with 5V.
The SPI lines will work at 3.3V for high logic level.

If W5500 is powered at 3.3V, might will still work but the
MISO line will drop to 2V for high logic level which is the low limit for it.
Variations in 3.3 power supply may render the W5500 unresponsive.

Pull the reset pin of W5500 up to Vcc with a 5Kohm resistor. Use a 5V tolerant
pin of the STM32 module to reset the W5500 at startup.
*/



#include <SPI.h>
#include <Ethernet.h>
#include "dewHeaterController.h"
#define RESETPIN PB11
#define ETHLED PC13

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 111, 177);
//IPAddress myDns(192, 168, 111, 1);
//IPAddress gateway(192, 168, 111, 1);
//IPAddress subnet(255, 255, 255, 0);



// Initialise the second serial port on STM32
HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)

// connect using telnet defaults on port 20003
EthernetServer server(20003);
bool alreadyConnected = false; // whether or not the client was connected previously

void setup()
{
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
  Serial2.begin(19200);  // PA3  (RX)  PA2  (TX)
  Serial2.println("Serial2 started...");
//  Serial.begin(9600);
//   while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
	digitalWrite(ETHLED, HIGH);
    Serial2.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  sleep(2);
  if (Ethernet.linkStatus() == LinkOFF)
  {
	digitalWrite(ETHLED, HIGH);
    Serial2.println("Ethernet cable is not connected.");
  }

  // start listening for clients
  server.begin();

  Serial2.print("Chat server address:");
  Serial2.println(Ethernet.localIP());
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

//	Serial2.print(*timer);
//	Serial2.print("   ");
//	Serial2.println(timeNow);

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













/*
void SendParameters(FILE *myFile, char *fileName, char *lockFileName)
{
	char rdBuffer[100];
	strcpy(rdBuffer,"\0");
	int result;
	result=ReadWithMutex(rdBuffer, WeatherDataDim, myFile, fileName, lockFileName);
//	result=ReadWithMutex(rdBuffer, 40, myFile, fileName, lockFileName);
	if (result==0)
	{
		strcpy(rdBuffer,"Error");
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
		return;
	}
//	if (result<0)
//	{
//		strcpy(rdBuffer,"Wait");
//		send(sd, rdBuffer, strlen(rdBuffer), 0 );
//		return;
//	}
	send(sd, rdBuffer, strlen(rdBuffer), 0 );
}
//*/






void CheckConnections()
{
	#ifdef DEBUG
	Serial2.println("Checking connections\n");
	#endif
//	if(dewHeaterDriverConnect)
	{
		if( IsTimerExpired(&dewDriverTimer, 30) )
		{
			client.stop();
			alreadyConnected = false;
			dewHeaterDriverConnect = 0;
			Serial2.println("Lost connection with the dew heater INDI driver, disconnecting.");
		}
	}
}



void ProcessRequests()
{
#ifdef DEBUG
	if(strlen(buffer))
	{
		Serial2.print("Received: ");
		Serial2.println(buffer);
	}
#endif
	if(!strcmp(buffer,"DewHeaterController-Connect"))
	{
		dewDriverTimer = millis();
		dewHeaterDriverConnect = 1;
		char rdBuffer[30];
		strcpy(rdBuffer, "DewHeaterController-Connect-Ok");
		client.println(rdBuffer);
	}

/*
	if(!strcmp(buffer,"getRoofPosition"))
	{
		char rdBuffer[15];
		int result;
		strcpy(rdBuffer,"\0");
		result=ReadWithMutex(rdBuffer, 3, pRoofPosition, (char*)ROOFPOSITION, (char*)ROOFPOSITIONLOCK);
		if (result==0)
		{
			strcpy(rdBuffer,"Error");
			send(sd, rdBuffer, strlen(rdBuffer), 0 );
			return;
		}
		if (result<0)
		{
			strcpy(rdBuffer,"Wait");
			send(sd, rdBuffer, strlen(rdBuffer), 0 );
			return;
		}
		#ifdef DEBUG
//		printf("Roof position sent: %s\n", rdBuffer);
		#endif
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
	}
	if(!strcmp(buffer,"mountParked"))
	{
		char rdBuffer[5];
		char wBuffer[5];
		strcpy(wBuffer,"1");
		WriteWithMutex(wBuffer, pMountStatus, (char*)MOUNTSTATUS, (char*)MOUNTSTATUSLOCK);
		strcpy(rdBuffer,"OK");
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
	}
	if(!strcmp(buffer,"mountUnParked"))
	{
		char rdBuffer[5];
		char wBuffer[5];
		pMountStatus = fopen (MOUNTSTATUS , "w");
		strcpy(wBuffer,"0");
		WriteWithMutex(wBuffer, pMountStatus, (char*)MOUNTSTATUS, (char*)MOUNTSTATUSLOCK);
		strcpy(rdBuffer,"OK");
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
	}
	if(!strcmp(buffer,"openRoof"))
	{
		char rdBuffer[5];
		char wBuffer[15];
		pRoofCommand=fopen(ROOFCOMMAND,"w");
		strcpy(wBuffer,"Open");
		WriteWithMutex(wBuffer, pRoofCommand, (char*)ROOFCOMMAND, (char*)ROOFCOMMANDLOCK);
		strcpy(rdBuffer,"OK");
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
	}
	if(!strcmp(buffer,"closeRoof"))
	{
		char rdBuffer[5];
		char wBuffer[15];
		pRoofCommand=fopen(ROOFCOMMAND,"w");
		strcpy(wBuffer,"Close");
		WriteWithMutex(wBuffer, pRoofCommand, (char*)ROOFCOMMAND, (char*)ROOFCOMMANDLOCK);
		strcpy(rdBuffer,"OK");
		send(sd, rdBuffer, strlen(rdBuffer), 0 );
	}
	if(!strcmp(buffer,"getWeather"))
	{
		SendParameters(fpWeatherData, (char*)WEATHERDATA, (char*)WEATHERDATALOCK);
	}
*/
	buffer[0] = 0;
}



void loop()
{
  // wait for a new client:
  if (!alreadyConnected)
  {
  	client = server.available();
  }

  // when the client sends the first byte, say hello:
  if (client)
  {
    if (!alreadyConnected)
    {
      // clear out the input buffer:
      client.flush();
      Serial2.println("We have a new client");
      client.println("Hello, Astro Server!");
      alreadyConnected = true;
	  dewDriverTimer = millis();
    }
    if (client.available() > 0)
	{
      // read the message from the client:
      int bufferLen = client.read((uint8_t*)buffer, 1024);
	  buffer[bufferLen] = 0;
    }
	 ProcessRequests();
//	 CheckConnections();
  }
}
