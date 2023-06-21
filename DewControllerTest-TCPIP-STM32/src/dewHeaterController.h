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

#define DEBUG

#define DEWHEATERCONTROLLERTIMEOUT 20000 // ms
#define COMPORT 20003



struct DEWCONTROLLERDATA
{   
	int pwmHeater1;
	int pwmHeater2;
	int pwmHeater3;
	int pwmHeater4;
	int pwmHeater5;
	int pwmHeater6;
	int pwmFan1;
	int pwmFan2;

    float temperature;  // s,n
    float airPressure;          // p,v
    float airHumidity;          // h,k
    float gasResistanceRaw;   // g
};

EthernetClient client;
int dewHeaterDriverConnect = 0; // socket descriptors for the 2 drivers// socket descriptors for the 2 drivers
unsigned long dewDriverTimer;
char buffer[1025];  // 1k data buffer
char message[500]; // a message

bool IsTimerExpired(unsigned long *timer, unsigned long interval);
void ReportDewHeaterDriverConnect();
void ProcessRequests();
int main ( int argument, char const *argv[] );

#endif
