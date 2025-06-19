#ifndef __DEWHEATERANDFAN_H__
#define __DEWHEATERANDFAN_H__

/*
    Dew Heater and Fan Controller
    Copyright (C) 2023 Radmilo Felix

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

*/

#include "connectionplugins/connectionserial.h"
#include "connectionplugins/connectiontcp.h"
#include "indicom.h"

#include <cmath>
#include <cstring>
#include <memory>
#include <termios.h>
#include <unistd.h>

#include <ctime>
#include <cerrno>
#include <regex>

#pragma once
#include <defaultdevice.h>
#include "StringParse.h"

#define DEWDEBUG


// Network
#define TIMERHIT_VALUE 1000 // milliseconds
#define TCPRETRIES 5
#define CONNECTIONRETRIES 5
#define COMMANDRETRIES 5
#define TCP_TIMEOUT 5
#define TCPSTOPCHAR '\n'

//      Communication
#define HARTBEATCALL "YOOHOO\n"
#define HARTBEATRESPONSE "2u2"
#define GETDATA "gd"
#define GETDATAHEADER "11"
#define SETDATA "2813,"
#define CONFIRMATION "OK"
#define GETRPM "grpm"
#define DATADELIMITER ","


namespace Connection
{
	class Serial;
};

class DewHeaterAndFan : public INDI::DefaultDevice
{
  public:
    DewHeaterAndFan();
    virtual ~DewHeaterAndFan() = default;

    virtual const char *getDefaultName() override;
    virtual bool initProperties() override;
    virtual bool updateProperties() override;
    virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
    virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
    virtual void TimerHit() override;
    bool TestConnection();
    std::string buffer;

    typedef enum
    {
        CONNECTION_NONE   = 1 << 0,
        CONNECTION_SERIAL = 1 << 1,
        CONNECTION_TCP    = 1 << 2
    } HeaterConnection;


  private:
    bool SendCommand(std::string request, std::string expectedResponse, std::string delimiter);
	uint8_t heaterConnection { CONNECTION_SERIAL | CONNECTION_TCP };

	int TcpRequest(char *request, char *response, char stopChar);
    int tcpRetries;
    int connectionRetries;
    int commandRetries;
    void CloseConnection();
    bool Handshake();
	void GetData();
	void SetData();
    Connection::TCP *TCPConnection{ nullptr };
    Connection::Serial *serialConnection{ nullptr };
    int PortFD{ -1 };

	INDI::PropertyNumber HeatersNP {6};
	INDI::PropertyNumber FansNP {2};
	INDI::PropertyNumber TemperatureNP {1};
	INDI::PropertyNumber HumidityNP {1};
	INDI::PropertyNumber DewpointNP {1};
};

#endif
