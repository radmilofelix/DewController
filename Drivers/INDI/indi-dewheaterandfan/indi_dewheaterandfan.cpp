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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
   USA

*/

#include "indi_dewheaterandfan.h"

std::unique_ptr<DewHeaterAndFan> dewHeaterAndFan(new DewHeaterAndFan());

DewHeaterAndFan::DewHeaterAndFan()
{
    setVersion(1, 1);
    tcpRetries=TCPRETRIES;
    connectionRetries = CONNECTIONRETRIES;
    commandRetries = COMMANDRETRIES;
}

int DewHeaterAndFan::TcpRequest(char *request, char *response, char stopChar)
{
    if (getActiveConnection() == serialConnection)
    {
        PortFD = serialConnection->getPortFD();
    }
    else if (getActiveConnection() == TCPConnection)
    {
        PortFD = TCPConnection->getPortFD();
    }

    int rc = 0, nbytes_written = 0, nbytes_read = 0;
	char errstr[MAXRBUF];

	while(tcpRetries)
	{
		tcflush(PortFD, TCIOFLUSH);
		rc = tty_write_string(PortFD, request, &nbytes_written);
		if( rc== TTY_OK )
		{
			break;
		}
		else
		{
			tcpRetries--;
			if( tcpRetries == 0)
			{
				tty_error_msg(rc, errstr, MAXRBUF);
				LOGF_ERROR("Error writing command %s: %s.", request, errstr);
				LOGF_WARN("Error writing  tcp rquest to DewHeater TCP server. Request: %s", request);
				return rc;
			}
		}
		usleep(500000);
	}
	tcflush(PortFD, TCIOFLUSH);
	rc = tty_read_section(PortFD, response, stopChar, TCP_TIMEOUT, &nbytes_read);
	if( rc != TTY_OK )
	{
		tty_error_msg(rc, errstr, MAXRBUF);
		#ifdef DEWDEBUG
			LOGF_WARN("Error reading response: %s.", errstr);
			LOGF_WARN("Error reading tcp response from RORWeatherIP device. Request: %s, Response: %s", request, response);
		#endif
		return rc;
	}
	tcpRetries = TCPRETRIES;
    response[strcspn(response, "\r")] = 0;
    response[strcspn(response, "\n")] = 0;
	response[nbytes_read]=0;
    return rc;
}

void DewHeaterAndFan::CloseConnection()
{
    Disconnect();
	INDI::PropertySwitch svp = getSwitch("CONNECTION");
	svp.onUpdate([svp, this]() mutable
	{
        if (!isConnected())
        {
            svp.setState(IPS_ALERT);
            svp.apply("Cannot change property while device is disconnected.");
            return;
        }
        auto index = svp.findOnSwitchIndex();
        if (index < 0)
            return;
        svp.setState(IPS_ALERT);
        INDI::PropertyLight light = getLight("Light Property");
    });

	deleteProperty(HeatersNP);
    deleteProperty(FansNP);
	deleteProperty(TemperatureNP);
	deleteProperty(HumidityNP);
	deleteProperty(DewpointNP);
    tcpRetries=TCPRETRIES;
    LOG_ERROR("TCP connection error, closing driver connection.");
}


bool DewHeaterAndFan::initProperties()
{
    DefaultDevice::initProperties();

    // Heater duty cycles
    HeatersNP[0].fill("HEATER1", "1-Primary Mirror", "%g", 0, 255, 10, 0);
    HeatersNP[1].fill("HEATER2", "2-Secondary Mirror", "%g", 0, 255, 10, 0);
    HeatersNP[2].fill("HEATER3", "3-Telescope Camera", "%g", 0, 255, 10, 0);
    HeatersNP[3].fill("HEATER4", "4-Guider Lens", "%g", 0, 255, 10, 0);
    HeatersNP[4].fill("HEATER5", "5-Guider Camera", "%g", 0, 255, 10, 0);
    HeatersNP[5].fill("HEATER6", "Heater 6", "%g", 0, 255, 10, 0);
    HeatersNP.fill(getDeviceName(), "HEATERS", "Heaters", MAIN_CONTROL_TAB, IP_RW, 0, IPS_IDLE);

    // Fans duty cycles
    FansNP[0].fill("FAN1", "1-Primary Mirror", "%g", 0, 255, 10, 0);
    FansNP[1].fill("FAN2", "Fan 2", "%g", 0, 255, 10, 0);
    FansNP.fill(getDeviceName(), "FANS", "Fans", MAIN_CONTROL_TAB, IP_RW, 0, IPS_IDLE);
    
    // Temperatures
    TemperatureNP[0].fill("TEMPERATURE", "Temperature [°C]", "%3.2f", -50., 70., 0., 0.);
    TemperatureNP.fill(getDeviceName(), "TEMPERATURE", "Temperature", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);

    // Humidity
    HumidityNP[0].fill("HUMIDITY", "Humidity [%]", "%3.2f", 0., 100., 0., 0.);
    HumidityNP.fill(getDeviceName(), "HUMIDITY", "Humidity", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);
    
    // Dew point
    DewpointNP[0].fill("DEWPOINT", "Dew point [°C]", "%3.2f", -50., 70., 0., 0.);
    DewpointNP.fill(getDeviceName(), "DEWPOINT", "Dew point", MAIN_CONTROL_TAB, IP_RO, 0, IPS_IDLE);

    if (heaterConnection & CONNECTION_SERIAL)
    {
        serialConnection = new Connection::Serial(this);
        serialConnection->registerHandshake([&]()
        {
            return Handshake();
        });
        serialConnection->setDefaultBaudRate(Connection::Serial::B_115200);
        registerConnection(serialConnection);
    }

    if (heaterConnection & CONNECTION_TCP)
    {
        TCPConnection = new Connection::TCP(this);
        TCPConnection->setDefaultHost("192.168.1.1");
        TCPConnection->setDefaultPort(10001);
        TCPConnection->registerHandshake([&]()
        {
            return Handshake();
        });
        registerConnection(TCPConnection);
	}

    return true;
}

bool DewHeaterAndFan::updateProperties()
{
    DefaultDevice::updateProperties();

    if (isConnected())
    {
        defineProperty(HeatersNP);
        defineProperty(FansNP);
        defineProperty(TemperatureNP);
        defineProperty(HumidityNP);
        defineProperty(DewpointNP);
    }
    else
    {
        deleteProperty(HeatersNP);
        deleteProperty(FansNP);
        deleteProperty(TemperatureNP);
        deleteProperty(HumidityNP);
        deleteProperty(DewpointNP);
    }

    return true;
}

const char *DewHeaterAndFan::getDefaultName()
{
    return (const char *)"Dew Heaters and Fan";
}

bool DewHeaterAndFan::Handshake()
{

    if (isSimulation())
        return true;
    if(!TestConnection())
    {
        LOG_ERROR("Handshake:Connection error.");
        return false;
    }
    return true;
}

bool DewHeaterAndFan::ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {

    }

    return INDI::DefaultDevice::ISNewSwitch(dev, name, states, names, n);
}

bool DewHeaterAndFan::ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
	
	
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if (strcmp(name, HeatersNP.getName()) == 0)
        {
			HeatersNP.update(values, names, n);
            HeatersNP.setState(IPS_BUSY);
            HeatersNP.apply();
			SetData();
            return true;
        }


        if (strcmp(name, FansNP.getName()) == 0)
        {
			FansNP.update(values, names, n);
            FansNP.setState(IPS_BUSY);
            FansNP.apply();
			SetData();
            return true;
        }
    }

    return INDI::DefaultDevice::ISNewNumber(dev, name, values, names, n);
}

bool DewHeaterAndFan::TestConnection()
{
    return SendCommand( HARTBEATCALL, HARTBEATRESPONSE, "\n");
}

void DewHeaterAndFan::TimerHit()
{
    if (!isConnected())
    {
		LOGF_WARN( "TimerHit: %s", "Device disconnected" );
        return; //  No need to reset timer if we are not connected anymore
	}
    if( !TestConnection() )
    {
		if( connectionRetries == 0 )
		{
			CloseConnection();
		}
		else
		{
			connectionRetries--;
		}
	}
	else
	{
		connectionRetries = CONNECTIONRETRIES;
		GetData();
	}

    SetTimer(getCurrentPollingPeriod());
}


bool DewHeaterAndFan::SendCommand(std::string request, std::string expectedResponse, std::string delimiter)
{
    char pRES[MAXRBUF] = {0};
    while (commandRetries)
    {
        TcpRequest( (char*)request.c_str(), pRES, TCPSTOPCHAR);
		buffer = pRES;
        StringParse stringParse( buffer, delimiter);
        if( stringParse.GetElement(0) == expectedResponse)
        {
            commandRetries = COMMANDRETRIES;
            return true;
        }
        else
        {
			if( stringParse.GetElement(0) == "OUTOFRANGE" )
			{
				commandRetries = COMMANDRETRIES;
				return false;
			}
			if( stringParse.GetElement(0) == "NODELIMITER" )
			{
				if( buffer == expectedResponse )
				{
					commandRetries = COMMANDRETRIES;
					return true;
				}
			}
		}
        commandRetries--;
        usleep(500000);
    }
	LOGF_WARN( "SendCommand error, request: %s, response: %s, expected: %s", request.c_str(), buffer.c_str(), expectedResponse.c_str() ); 
    commandRetries = COMMANDRETRIES;
    return false;
}


void DewHeaterAndFan::GetData()
{
	if( SendCommand( GETDATA, GETDATAHEADER, DATADELIMITER))
	{
		StringParse stringParse( buffer, DATADELIMITER);
		float temperature = (float)stoi(stringParse.GetElement(10))/100 ;
		float humidity = (float)stoi(stringParse.GetElement(11))/100;
		float dewpoint = temperature -  ((100. - humidity)/5.);
		HeatersNP[0].setValue(stoi( stringParse.GetElement(1) ));
		HeatersNP[1].setValue(stoi( stringParse.GetElement(2) ));
		HeatersNP[2].setValue(stoi( stringParse.GetElement(3) ));
		HeatersNP[3].setValue(stoi( stringParse.GetElement(4) ));
		HeatersNP[4].setValue(stoi( stringParse.GetElement(5) ));
		HeatersNP[5].setValue(stoi( stringParse.GetElement(6) ));
		HeatersNP.setState(IPS_OK);
		HeatersNP.apply();

		FansNP[0].setValue(stoi( stringParse.GetElement(7) ));
		FansNP[1].setValue(stoi( stringParse.GetElement(8) ));
		FansNP.setState(IPS_OK);
		FansNP.apply();
		
        TemperatureNP[0].setValue(temperature);
        TemperatureNP.setState(IPS_OK);
        TemperatureNP.apply();
        
        
        HumidityNP[0].setValue(humidity);
        HumidityNP.setState(IPS_OK);
        HumidityNP.apply();

        DewpointNP[0].setValue(dewpoint);
        DewpointNP.setState(IPS_OK);
        DewpointNP.apply();
	}
	else
	{
		LOGF_WARN( "GetData: %s", "Response wrong" );
	}
}
	
void DewHeaterAndFan::SetData()
{
	std::string dataBuffer;
	dataBuffer = SETDATA;
	for(int i=0; i<6; i++)
	{
		dataBuffer += std::to_string( (int)HeatersNP[i].getValue()) ;
		dataBuffer += DATADELIMITER;
	}
	for(int i=0; i<2; i++)
	{
		dataBuffer += std::to_string( (int)FansNP[i].getValue());
		dataBuffer += DATADELIMITER;
	}
	SendCommand(dataBuffer, CONFIRMATION,  DATADELIMITER);
}
