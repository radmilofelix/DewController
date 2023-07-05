#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_serial(new QSerialPort(this))
{
    ui->setupUi(this);
    timerId = startTimer(3000);
//    timerId = startTimer(1000);

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
        ui->comPort->setText("13");
    #else
        ui->comPort->setText("/dev/ttyUSB0");
    #endif
    serialTimeout = 500; // ms
    readWriteDelay = 200; // ms
    timerUpdate = false;
    timerCounter = 0;
    connected = false;
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
    QString comportString = "COM" + ui->comPort->text();

    m_serial->setPortName(comportString);
    m_serial->setBaudRate(baudrate);
    m_serial->setDataBits(m_serial->Data8);
    m_serial->setParity(m_serial->NoParity);
    m_serial->setStopBits(m_serial->OneStop);
    m_serial->setFlowControl(m_serial->NoFlowControl);

    QMessageBox msgBox;
    if (m_serial->open(QIODevice::ReadWrite))
    {
        msgBox.setText("Serial port connected.");
        msgBox.exec();
    }
    else
    {
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
    if(dataBuffer.trimmed() != "Hello, astronomer!")
    {
        msgBox.setText("Could not connect to Dew Heaters Controller TCP device. Please try again in 20 seconds.");
        msgBox.exec();
        disconnectAlarm = true;
        ButtonDisconnect();
        return;
    }
    msgBox.setText("Connected to TCP host.");
    msgBox.exec();
}


void MainWindow::DisconnectFromHost()
{
    _socket.disconnectFromHost();
}


void MainWindow::ButtonConnect()
{
    if(disconnectAlarm == true)
        disconnectAlarm = false;
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

    SetControlsEnable(true);

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
        return;
    }
    GetData();
    connected = true;
}


void MainWindow::ButtonDisconnect()
{
    connected = false;
    QMessageBox msgBox;
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


    if( ui->radioButtonSerial->isChecked() )
    {
        CloseSerialPort();
        msgBox.setText("Serial port disconnected.");
        msgBox.exec();
    }

    if( ui->radioButtonNetwork->isChecked() )
    {
        DisconnectFromHost();
        msgBox.setText("Disconnected from network device.");
        msgBox.exec();
    }
}


void MainWindow::SendAndReceive(QString request, QString *response, int waitForSerial, int waitForEthernet)
{
    dataBuffer.clear();
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
//        _socket.waitForReadyRead(15000);
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

    if( !data.isEmpty() )
    {
        response->clear();
        response->append(data);
    }
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


void MainWindow::timerEvent(QTimerEvent *event)
{
    if(!connected)
    {
        return;
    }
    if(timerCounter == 0)
    {
        SendAndReceive("grpm", &dataBuffer, 500, 1000);
    }
    if(!connected)
    {
        return;
    }
    SendAndReceive("YOOHOO", &dataBuffer, 500, 100); // hartbeat
//    qDebug() << dataBuffer.trimmed();
    if(dataBuffer.trimmed() != "2u2")
    {
        QMessageBox msgBox;
        msgBox.setText("Wrong connection response, disconnecting device-timerEvent");
        msgBox.exec();
        ButtonDisconnect();
    }
    timerCounter++;
    if(timerCounter > 5)
    {
        timerCounter = 0;
    }

    GetData();
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

void MainWindow::GetData()
{
    SendAndReceive("gd", &dataBuffer, 500, 100);
    if(dataBuffer.left(2) != "11")
    {
        QMessageBox msgBox;
        msgBox.setText("Wrong answer from device upon data request");
        msgBox.exec();
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
}

void MainWindow::SendData()
{
    QString sendString = "2813,";
    for(int i=0; i<8; i++)
    {
        sendString += QString::number(heaters[i]) + ",";
    }
    sendString += QString::number(fansRPMenable) + ",";
    QString receiveString;
    SendAndReceive(sendString, &receiveString, 500, 100);
    if(receiveString.trimmed() != "OK")
    {
        QMessageBox msgBox;
        msgBox.setText("Wrong answer from device upon changing values! : " + receiveString);
        msgBox.exec();
    }
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
