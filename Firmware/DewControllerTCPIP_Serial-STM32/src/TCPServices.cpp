#include "TCPServices.h"


TCPServices::TCPServices(uint16_t port) : server(port)
{
    isTCPrequest = false;
    isTCPalreadyConnected = false;
    clientConnect = 0;
    clientTimer = millis();
	server.begin();
}

TCPServices::~TCPServices()
{

}

void TCPServices::Begin()
{
	server.begin();
}


void TCPServices::CheckTCPClient(char *buffer)
{
//  wait for a new client:
	if (!isTCPalreadyConnected)
  	{
  		client = server.available();
//		client = server.accept();
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

      client.println(GREETING);
	  isTCPalreadyConnected = true;

      clientConnect = 1;
	  clientTimer = millis();

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
}


void TCPServices::CheckConnections()
{
	if(clientConnect && isTCPalreadyConnected)
	{
		if( IsTcpTimerExpired(&clientTimer, TCPTIMEOUT) )
		{
			client.stop();
			clientConnect = 0;
			isTCPalreadyConnected = false;
			isTCPrequest = false;
			#ifdef DEBUG
			Serial.println("Lost connection with the dew heater TCP client, disconnecting.");
			#endif
		}
	}
}


void TCPServices::SendResponse(char* response)
{
	if(isTCPrequest)
		client.println(response);
	isTCPrequest = false;
}


bool TCPServices::IsTcpTimerExpired(unsigned long *timer, unsigned long intervalSeconds)
{
	long long timeNow = *timer;
	if( (timeNow + intervalSeconds*1000) > 4294967295 )
	{
		*timer = 0;
		return false;
	}
	timeNow = millis();

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
