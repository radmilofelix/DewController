#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serial(new QSerialPort(this))
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(MyTimerSlot()));
    connect(ui->radioButtonSerial, &QRadioButton::clicked, this, &MainWindow::EnableSerial);
    connect(ui->radioButtonNetwork, &QRadioButton::clicked, this, &MainWindow::EnableNetwork);
    connect(ui->connect, &QPushButton::clicked, this, &MainWindow::ButtonConnect);
    connect(ui->disconnect, &QPushButton::clicked, this, &MainWindow::ButtonDisconnect);
    connect(ui->generalSlider, &QSlider::sliderReleased, this, &MainWindow::SliderReleased);
    connect(ui->setPushButton, &QPushButton::clicked, this, &MainWindow::Set);
    connect(ui->Fan1RPMEnabled, &QCheckBox::released, this, &MainWindow::EnableFansRPM);
    connect(ui->Fan2RPMEnabled, &QCheckBox::released, this, &MainWindow::EnableFansRPM);

    int myval = ui->sliderValue->value();
    ui->sliderValue->setValue(myval);
    ui->hostAddress->setText("192.168.111.177");
    #ifdef WINDOWS_SYSTEM
        ui->comPort->setText("15");
    #else
        ui->comPort->setText("/dev/indi-dewcontroller");
    #endif
    serialTimeout = 500; // ms
    readWriteDelay = 200; // ms
    timer->start(3000);
    timerUpdate = false;
    timerCounter = 0;
    connected = false;
	reconnectRetries = 0;
    disconnectAlarm = false;
    ui->comPort->setEnabled(true);
    ui->baud->setEnabled(true);
    ui->label_ComPort->setEnabled(true);
    ui->label_Baud->setEnabled(true);
    this->setFixedSize(QSize(800, 600));
    for(int i=0; i<8; i++)
    {
        heaters[i]=0;
    }
    fansRPMenable = 0;
    SetControlsEnable(false);
    ui->radioButtonSerial->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::EnableSerial()
{
    ui->hostAddress->setEnabled(false);
    ui->port->setEnabled(false);
    ui->label_IP->setEnabled(false);
    ui->label_Port->setEnabled(false);
    ui->label_ComPort->setEnabled(true);
    ui->label_Baud->setEnabled(true);
    ui->comPort->setEnabled(true);
    ui->baud->setEnabled(true);
}


void MainWindow::EnableNetwork()
{
    ui->comPort->setEnabled(false);
    ui->baud->setEnabled(false);
    ui->label_ComPort->setEnabled(false);
    ui->label_Baud->setEnabled(false);
    ui->label_IP->setEnabled(true);
    ui->label_Port->setEnabled(true);
    ui->hostAddress->setEnabled(true);
    ui->port->setEnabled(true);
}


void MainWindow::OpenSerialPort()
{
    int baudrate = ui->baud->value();
    #ifdef WINDOWS_SYSTEM
        QString comportString = "COM" + ui->comPort->text();
    #else
        QString comportString = ui->comPort->text();
    #endif

    m_serial->setPortName(comportString);
    m_serial->setBaudRate(baudrate);
    m_serial->setDataBits(m_serial->Data8);
    m_serial->setParity(m_serial->NoParity);
    m_serial->setStopBits(m_serial->OneStop);
    m_serial->setFlowControl(m_serial->NoFlowControl);

    QMessageBox msgBox;
    if (m_serial->open(QIODevice::ReadWrite))
    {
    }
    else
    {
        disconnectAlarm = true;
        msgBox.setText("Could not open serial port.");
        msgBox.exec();
    }
}


void MainWindow::CloseSerialPort()
{
    if (m_serial->isOpen())
        m_serial->close();
}


void MainWindow::ConnectToHost()
{
    QMessageBox msgBox;
    disconnectAlarm = false;
    _socket.connectToHost(QHostAddress(ui->hostAddress->text()), ui->port->value());
    if( !_socket.isOpen() )
    {
        msgBox.setText("Could not open TCP socket!");
        msgBox.exec();
        disconnectAlarm = true;
        ButtonDisconnect();
        return;
    }
    SendAndReceive("\r\n", &dataBuffer, 500, 100);
    if(dataBuffer.trimmed() != GREETING)
    {
        SendAndReceive(HARTBEATCALL, &dataBuffer, 500, 100);
        if(dataBuffer.trimmed() != HARTBEATRESPONSE)
        {
            msgBox.setText("Could not connect to Dew Heaters Controller TCP device. If the IP address and port are correct, please try again in 20 seconds.");
            msgBox.exec();
            disconnectAlarm = true;
            ButtonDisconnect();
            return;
        }
    }
}


void MainWindow::DisconnectFromHost()
{
    _socket.disconnectFromHost();
}


void MainWindow::ButtonConnect()
{
    if( ui->radioButtonSerial->isChecked() )
    {
        OpenSerialPort();
    }
    if( ui->radioButtonNetwork->isChecked() )
    {
        ConnectToHost();
    }
    if(disconnectAlarm)
    {
        ButtonDisconnect();
        return;
    }
    if(disconnectAlarm == true)
    {
        disconnectAlarm = false;
    }
    ui->radioButtonNetwork->setEnabled(false);
    ui->radioButtonSerial->setEnabled(false);
    ui->comPort->setEnabled(false);
    ui->baud->setEnabled(false);
    ui->hostAddress->setEnabled(false);
    ui->port->setEnabled(false);
    ui->connect->setEnabled(false);
    ui->label_ComPort->setEnabled(false);
    ui->label_Baud->setEnabled(false);
    ui->disconnect->setEnabled(true);
    GetData();
    SetControlsEnable(true);
    connected = true;
}


void MainWindow::ButtonDisconnect()
{
    connected = false;
    QMessageBox msgBox;
    if( ui->radioButtonSerial->isChecked() )
    {
        CloseSerialPort();
    }

    if( ui->radioButtonNetwork->isChecked() )
    {
        DisconnectFromHost();
    }
    ui->radioButtonNetwork->setEnabled(true);
    ui->radioButtonSerial->setEnabled(true);
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);

    SetControlsEnable(false);

    if ( ui->radioButtonSerial->isChecked() )
    {
        EnableSerial();
    }
    if ( ui->radioButtonNetwork->isChecked() )
    {
        EnableNetwork();
    }
}


void MainWindow::SendAndReceive(QString request, QString *response, int waitForSerial, int waitForEthernet)
{
#ifdef DEBUG
    qDebug() << request.trimmed();
#endif
    if( ui->radioButtonSerial->isChecked() )
    {
        m_serial->write(request.toStdString().data());
        m_serial->waitForBytesWritten(500);
        this->thread()->msleep(readWriteDelay);
        m_serial->waitForReadyRead(waitForSerial);
    }

    if( ui->radioButtonNetwork->isChecked() )
    {
        _socket.write(request.toStdString().data());
        _socket.waitForBytesWritten(100);
        this->thread()->msleep(readWriteDelay);
        _socket.waitForReadyRead(waitForEthernet);
    }

    QByteArray data;

    if( ui->radioButtonSerial->isChecked() )
    {
        data = m_serial->readAll();
    }

    if( ui->radioButtonNetwork->isChecked() )
    {
        data = _socket.readAll();
    }

    response->clear();
    if( !data.isEmpty() )
    {
        response->append(data);
    }
#ifdef DEBUG
    qDebug() << response->trimmed();
#endif
}


void MainWindow::Set()
{
    int sliderValue = ui->sliderValue->value();
    heaters[TestHeaterButton()-1] = sliderValue;
    SetHeaters();
    SendData();
//    GetData();
}


void MainWindow::SliderReleased()
{
    int sliderValue = ui->sliderValue->value();
    heaters[TestHeaterButton()-1] = sliderValue;
    SetHeaters();
    SendData();
}

bool MainWindow::PollRPM()
{
    timerCounter++;
    if(timerCounter > 5)
    {
        timerCounter = 0;
    }

    if(timerCounter == 0 && (ui->Fan1RPMEnabled->isChecked() || ui->Fan2RPMEnabled->isChecked()) )
    {
        SendAndReceive(GETRPM, &dataBuffer, 500, 1000);
        if(dataBuffer.trimmed() != CONFIRMATION)
        {
            return false;
        }
    }
	return true;
}

void MainWindow::KeepAlive()
{
    SendAndReceive(HARTBEATCALL, &dataBuffer, 500, 100); // hartbeat
    if(dataBuffer.trimmed() != HARTBEATRESPONSE)
    {
        reconnectRetries++;
        if( reconnectRetries > RECONNECTRETRIES )
        {
#ifdef DEBUG
            qDebug() << "Disconnect";
#endif
            ButtonDisconnect();
#ifdef DEBUG
            qDebug() << "Connect";
#endif
            ButtonConnect();
            if( disconnectAlarm )
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle("Connection error");
                msgBox.setInformativeText(QString("Lost connection to the device!"));
                msgBox.exec();
                reconnectRetries = 0;
            }
            else
            {
#ifdef DEBUG
                qDebug() << "Hartbeat after reconnect";
#endif
                SendAndReceive(HARTBEATCALL, &dataBuffer, 500, 100); // hartbeat
                if(dataBuffer.trimmed() != HARTBEATRESPONSE)
                {
                    QMessageBox msgBox;
                    msgBox.setWindowTitle("Hartbeat error");
                    msgBox.setInformativeText(QString("Wrong hartbeat answer after reconnect! Disconnecting device."));
                    msgBox.exec();
                    ButtonDisconnect();
                    reconnectRetries = 0;
                }
            }
        }
        return;
    }
    else
    {
        reconnectRetries = 0;
    }
}


void MainWindow::MyTimerSlot()
{
    if(!connected)
    {
        return;
    }
    KeepAlive();
    if( !PollRPM())
    {
        KeepAlive();
    }
    if( !GetData())
    {
        KeepAlive();
    }
}

int MainWindow::TestHeaterButton()
{
    if(ui->radioButtonH1->isChecked())
        return 1;

    if(ui->radioButtonH2->isChecked())
        return 2;

    if(ui->radioButtonH3->isChecked())
        return 3;

    if(ui->radioButtonH4->isChecked())
        return 4;

    if(ui->radioButtonH5->isChecked())
        return 5;

    if(ui->radioButtonH6->isChecked())
        return 6;

    if(ui->radioButtonH7->isChecked())
        return 7;

    if(ui->radioButtonH8->isChecked())
        return 8;

    return 0;
}

void MainWindow::SetHeaters()
{
    for(int i=0; i<8; i++)
    {
        switch(i+1)
        {
        case 1:
            ui->heater1->setValue(heaters[i]);
            break;
        case 2:
            ui->heater2->setValue(heaters[i]);
            break;
        case 3:
            ui->heater3->setValue(heaters[i]);
            break;
        case 4:
            ui->heater4->setValue(heaters[i]);
            break;
        case 5:
            ui->heater5->setValue(heaters[i]);
            break;
        case 6:
            ui->heater6->setValue(heaters[i]);
            break;
        case 7:
            ui->heater7->setValue(heaters[i]);
            break;
        case 8:
            ui->heater8->setValue(heaters[i]);
            break;
        }
    }
}

void MainWindow::EnableFansRPM()
{
    fansRPMenable = ui->Fan1RPMEnabled->isChecked() + ui->Fan2RPMEnabled->isChecked() * 2;
    SendData();
}

bool MainWindow::GetData()
{
    SendAndReceive(GETDATA, &dataBuffer, 500, 1000);
    if(dataBuffer.left(2) != GETDATAHEADER)
    {
        return false;
    }

    QString dataValue = dataBuffer.section(",",1,1);

    heaters[0] = dataValue.toInt();
    dataValue = dataBuffer.section(",",2,2);
    heaters[1] = dataValue.toInt();
    dataValue = dataBuffer.section(",",3,3);
    heaters[2] = dataValue.toInt();
    dataValue = dataBuffer.section(",",4,4);
    heaters[3] = dataValue.toInt();
    dataValue = dataBuffer.section(",",5,5);
    heaters[4] = dataValue.toInt();
    dataValue = dataBuffer.section(",",6,6);
    heaters[5] = dataValue.toInt();
    dataValue = dataBuffer.section(",",7,7);
    heaters[6] = dataValue.toInt();
    dataValue = dataBuffer.section(",",8,8);
    heaters[7] = dataValue.toInt();
    SetHeaters();

    dataValue = dataBuffer.section(",",9,9);
    fansRPMenable = dataValue.toInt();

    if(fansRPMenable & 1)
    {
        ui->Fan1RPMEnabled->setChecked(true);
        ui->rpm1Box->setEnabled(true);
        ui->label_2->setEnabled(true);
    }
    else
    {
        ui->Fan1RPMEnabled->setChecked(false);
        ui->rpm1Box->setEnabled(false);
        ui->label_2->setEnabled(false);
    }

    if(fansRPMenable & 2)
    {
        ui->Fan2RPMEnabled->setChecked(true);
        ui->rpm2Box->setEnabled(true);
        ui->label_4->setEnabled(true);
    }
    else
    {
        ui->Fan2RPMEnabled->setChecked(false);
        ui->rpm2Box->setEnabled(false);
        ui->label_4->setEnabled(false);
    }

    dataValue = dataBuffer.section(",",10,10);
    ui->temperatureBox->setValue(dataValue.toInt()/100);
    dataValue = dataBuffer.section(",",11,11);
    ui->humidityBox->setValue(dataValue.toInt()/100);
    dataValue = dataBuffer.section(",",12,12);
    ui->airPressureBox->setValue(dataValue.toInt()/100);
    dataValue = dataBuffer.section(",",13,13);
    ui->rpm1Box->setValue(dataValue.toInt());
    dataValue = dataBuffer.section(",",14,14);
    ui->rpm2Box->setValue(dataValue.toInt());
    return true;
}

bool MainWindow::SendData()
{
    QString sendString = SETDATA;
    for(int i=0; i<8; i++)
    {
        sendString += QString::number(heaters[i]) + ",";
    }
    sendString += QString::number(fansRPMenable) + ",";
    QString receiveString;
    SendAndReceive(sendString, &receiveString, 500, 100);
    if(receiveString.trimmed() != CONFIRMATION)
    {
        return false;
    }
    return true;
}

void MainWindow::SetControlsEnable(bool state)
{
    ui->generalSlider->setEnabled(state);
    ui->sliderValue->setEnabled(state);
    ui->setPushButton->setEnabled(state);
    ui->label->setEnabled(state);
    ui->label_2->setEnabled(state);
    ui->label_3->setEnabled(state);
    ui->label_4->setEnabled(state);
    ui->label_5->setEnabled(state);
    ui->label_6->setEnabled(state);
    ui->label_7->setEnabled(state);
    ui->label_8->setEnabled(state);
    ui->label_9->setEnabled(state);
    ui->label_10->setEnabled(state);
    ui->label_11->setEnabled(state);
    ui->label_12->setEnabled(state);
    ui->label_13->setEnabled(state);
    ui->label_14->setEnabled(state);
    ui->label_15->setEnabled(state);
    ui->label_16->setEnabled(state);
    ui->label_17->setEnabled(state);
    ui->label_18->setEnabled(state);

    if(state)
    {
        ui->label_16->setText("<font color=#FF0000>Do not enable RPM reading if the fan</font>");
        ui->label_17->setText("<font color=#FF0000>has no rpm capabilities or it is not connected!</font>");
        ui->label_18->setText("<span style=' font-size:9pt; font-weight:1000; color:#0000FF;'>CONNECTED</span>");
    }
    else
    {
        ui->label_16->setText("<font color='white'>Do not enable RPM reading if the fan</font>");
        ui->label_17->setText("<font color='white'>has no rpm capabilities or it is not connected!</font>");
        ui->label_18->setText("<span style=' font-size:9pt; font-weight:1000; color:#FF0000;'>DISCONNECTED</span>");
    }

    ui->radioButtonH1->setEnabled(state);
    ui->radioButtonH2->setEnabled(state);
    ui->radioButtonH3->setEnabled(state);
    ui->radioButtonH4->setEnabled(state);
    ui->radioButtonH5->setEnabled(state);
    ui->radioButtonH6->setEnabled(state);
    ui->radioButtonH7->setEnabled(state);
    ui->radioButtonH8->setEnabled(state);

    ui->heater1->setEnabled(state);
    ui->heater2->setEnabled(state);
    ui->heater3->setEnabled(state);
    ui->heater4->setEnabled(state);
    ui->heater5->setEnabled(state);
    ui->heater6->setEnabled(state);
    ui->heater7->setEnabled(state);
    ui->heater8->setEnabled(state);

    ui->Fan1RPMEnabled->setEnabled(state);
    ui->Fan2RPMEnabled->setEnabled(state);

    ui->rpm1Box->setEnabled(state);
    ui->rpm2Box->setEnabled(state);

    ui->temperatureBox->setEnabled(state);
    ui->humidityBox->setEnabled(state);
    ui->airPressureBox->setEnabled(state);
}
