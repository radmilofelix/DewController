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

//#include "commons.h"

#include <SPI.h>
#include <Ethernet.h>


#define DEBUG

#define DEWHEATERCONTROLLERTIMEOUT 20000 // ms
#define COMPORT 20002
#define COMPORT1 20003
#define TCPTIMEOUT 10 // sec
#define ANALOGWRITERESOLUTION 8 // default

#define RESETPIN PB0

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 111, 177);
//IPAddress myDns(192, 168, 111, 1);
//IPAddress gateway(192, 168, 111, 1);
//IPAddress subnet(255, 255, 255, 0);

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
//int pwmPin[8];
//float sensorTemperature;
//float sensorAirHumidity;
//float sensorAirPressure;
//int maxPWMvalue = pow(2, ANALOGWRITERESOLUTION) - 1;


// Initialise the second serial port on STM32
HardwareSerial Serial2(USART2);   // PA3  (RX)  PA2  (TX)

EthernetServer server(COMPORT);
EthernetClient client;

EthernetServer server1(COMPORT1);
EthernetClient client1;

int dewHeaterDriverConnect = 0; 
int dewHeaterDriverConnect1 = 0;
unsigned long dewDriverTimer;
unsigned long dewDriverTimer1;
bool isTCPalreadyConnected = false;
bool isTCPalreadyConnected1 = false;
bool isTCPrequest = false;
bool isTCPrequest1 = false;
bool isSerialrequest = false;
//float temperatureDht;
//float humidityDht;
char buffer[1025];  // 1k communication buffer
//DHT dhtSensor;


/*
static bool is_high1;  // is the input currently HIGH?
static uint32_t time_of_rise1;  // when did it rise?
static bool is_high2;  // is the input currently HIGH?
static uint32_t time_of_rise2;  // when did it rise?
//*/

bool IsTimerExpired(unsigned long *timer, unsigned long interval);
void ReportDewHeaterDriverConnect();
void ProcessRequests();
void SetHeatersAndFans();
void SendResponse(char* response);
int main ( int argument, char const *argv[] );
void PollDhtSensor();
long GetRPM(int fanNumber);
long GetDebouncedRPM(int fanNumber);

#endif
