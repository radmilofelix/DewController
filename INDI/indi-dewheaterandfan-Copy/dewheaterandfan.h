#ifndef __DEWHEATERANDFAN_H__
#define __DEWHEATERANDFAN_H__

/*******************************************************************************
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

*******************************************************************************/

#pragma once

#include "defaultdevice.h"



/**
 * @brief The DewHeaterAndFan class provides control for  the following devices:
 * + Dew heaters.
 * + Fans on the back of telescopes.
 */

#define DEBUG
#define TCPIPDEVICE
#define TIMERHIT_VALUE 1000 // milliseconds
#define DEW_TCP_TIMEOUT 3

class DewHeaterAndFan : public INDI::DefaultDevice
{
    public:
        DewHeaterAndFan();
        virtual ~DewHeaterAndFan() override = default;
        const char *getDefaultName() override;
        void ISGetProperties(const char *dev) override;
        virtual bool ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n) override;
        virtual bool ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n) override;
//        ModbusMaster modbus_f;
	void SendCommand(int myCommand);
	int CheckCommand(int myCommand);

    protected:
        bool initProperties() override;
        bool updateProperties() override;
        #ifdef TCPIPDEVICE
	virtual bool Handshake() override;
	int TcpRequest(char *myMessage, char *myResult); 
        void CloseConnection();
        bool TestConnection();
	#else
        bool Connect() override;
        bool Disconnect() override;
	#endif
        
        void TimerHit() override;
        void UpdateValues();
//        virtual IPState MoveFocuser(FocusDirection dir, int speed, uint16_t duration) override;
//        virtual IPState MoveAbsFocuser(uint32_t targetTicks) override;
//        virtual IPState MoveRelFocuser(FocusDirection dir, uint32_t ticks) override;
//        virtual bool SetFocuserSpeed(int speed) override;
//        virtual bool SetFocuserBacklash(int32_t steps) override;
//        virtual bool SetFocuserBacklashEnabled(bool enabled) override;

    private:
        double internalTicks { 0 };
        double initTicks { 0 };

        // Seeing in arcseconds
//        INumberVectorProperty SeeingNP;
//        INumber SeeingN[1];

        // FWHM to be used by CCD driver to draw 'fuzzy' stars
//        INumberVectorProperty FWHMNP;
//        INumber FWHMN[1];

        // Temperature in celcius degrees
        INumberVectorProperty TemperatureNP;
        INumber TemperatureN[1];
        int tcpRetries;
//	enum
//	{
//		REMOTECONTROL_ENABLE,
//		REMOTECONTROL_DISABLE,
//                REMOTECONTROL_COUNT
//	};
//        ISwitchVectorProperty RemoteControlSP;
//        ISwitch RemoteControlS[REMOTECONTROL_COUNT];

        ISwitchVectorProperty ResetSP;
        ISwitch ResetS[1];

//        ILight FocuserFaultL[1];
//        ILightVectorProperty FocuserFaultLP;	
};
#endif
