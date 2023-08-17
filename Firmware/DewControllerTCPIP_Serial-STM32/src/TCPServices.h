#ifndef TCPSERVICES__H
#define TCPSERVICES__H

#include <Ethernet.h>
#include <sys/Time.h>

#define TCPTIMEOUT 10 // sec
#define GREETING "Hello TCP client!"


//#define ESP8266BOARD

#ifndef ESP8266BOARD
	#include <SPI.h>
	#include <Ethernet.h>
#else
	#include <ESP8266WiFi.h>
#endif



class TCPServices
{
    public:

    TCPServices(uint16_t port);
    ~TCPServices();

    void Begin();
    void CheckTCPClient(char *buffer);
    void CheckConnections();
    void SendResponse(char* response);
    bool IsTcpTimerExpired(unsigned long *timer, unsigned long interval);

    bool isTCPrequest;
    bool isTCPalreadyConnected;
    int clientConnect;
    unsigned long clientTimer;

	#ifndef ESP8266BOARD
		EthernetServer server;
		EthernetClient client;
	#else
		WiFiServer server;
		WiFiClient client;
	#endif

};


#endif
