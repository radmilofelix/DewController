#ifndef DEWHEATERCONTROLLER__H
#define DEWHEATERCONTROLLER__H

#include <cstdio>
#include <stdlib.h>
#include <string.h>   //strlen  
#include <errno.h>  
#include <unistd.h>   //close  
#include <sys/stat.h>
#include <sys/Time.h>
#include <chrono>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h> 
#include <Adafruit_BME280.h>

#include <SPI.h>
#include <Ethernet.h>

#include "TCPServices.h"
#include "SerialServices.h"

//#define DEBUG

#define DHCP
#define TCPPORT 20002
#define TCPPORT1 20003
#define ANALOGWRITERESOLUTION 8 // default

#define RESETPIN PB0

#define HARTBEATCALL "YOOHOO"
#define HARTBEATRESPONSE "2u2"
#define GETDATA "gd"
#define GETDATAHEADER 11
#define SETDATA "2813,"
#define CONFIRMATION "OK"
#define GETRPM "grpm"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] =
{
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x01 // black DHT
//  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 // white DHT
};

#ifndef DHCP
    IPAddress ip(192, 168, 111, 176); // black DHT
//    IPAddress ip(192, 168, 111, 177); // white DHT

//    IPAddress myDns(192, 168, 111, 1);
//    IPAddress gateway(192, 168, 111, 1);
//    IPAddress subnet(255, 255, 255, 0);
#endif

struct DEWCONTROLLERDATA
{   
	int command;
	int pwmDevice[8];
	int fanRPMcapability; // bit0 = 1 Fan1 has rpm capability; bit1 = 1 -> Fan2 has rpm capability 
	int temperature;
	int airHumidity;
	int airPressure;
	long rpm1;
	long rpm2;
} dewControllerData;

int pwmPin[32];

// Initialise the second serial port on STM32
HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)

TCPServices tcpservice(TCPPORT);
TCPServices tcpservice1(TCPPORT1);
SerialServices serialservices;


int dewHeaterDriverConnect = 0; 
int dewHeaterDriverConnect1 = 0;
unsigned long dewDriverTimer;
unsigned long dewDriverTimer1;
bool isTCPalreadyConnected = false;
bool isTCPalreadyConnected1 = false;
bool isTCPrequest = false;
bool isTCPrequest1 = false;
bool isSerialrequest = false;
char buffer[1025];  // 1k communication buffer

void setup();
void ReportDewHeaterDriverConnect();
void ProcessRequests();
void SetHeatersAndFans();
void SelectAndSend(char *response);
int main ( int argument, char const *argv[] );
void PollDhtSensor();
long GetRPM(int fanNumber);
long GetDebouncedRPM(int fanNumber);
void HartBeat();

#endif
