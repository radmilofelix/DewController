#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

//#define DEBUG

//#define WINDOWS_SYSTEM

#ifdef WINDOWS_SYSTEM
#include<windows.h>
#else
#include <unistd.h>
#endif

#define RECONNECTRETRIES 5

#define GREETING "Hello TCP client!"
#define HARTBEATCALL "YOOHOO"
#define HARTBEATRESPONSE "2u2"
#define GETDATA "gd"
#define GETDATAHEADER "11"
#define SETDATA "2813,"
#define CONFIRMATION "OK"
#define GETRPM "grpm"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void EnableSerial();
    void EnableNetwork();
    void ButtonConnect();
    void ButtonDisconnect();
    void SliderReleased();
    void Set();
    void EnableFansRPM();
    void MyTimerSlot();


private:
    Ui::MainWindow *ui;
    void OpenSerialPort();
    void CloseSerialPort();
    void ConnectToHost();
    void DisconnectFromHost();
    void SendAndReceive(QString request, QString *response, int waitForSerial, int waitForEthernet);
    int TestHeaterButton();
    void SetHeaters();
    void SetControlsEnable(bool state);
    void OnReadyRead();
    bool GetData();
    bool SendData();
    void KeepAlive();
    bool PollRPM();

    int heaters[8];
    QSerialPort *m_serial = nullptr;
    int serialTimeout;
    int readWriteDelay;

    QTimer *timer;
    QString dataBuffer;
    bool timerUpdate;
    int timerCounter; // RPM poll counter
    QTcpSocket  _socket;
    bool connected;
    bool disconnectAlarm; // true if connection fails
    int fansRPMenable;
//    int serialDisconnect;
	int reconnectRetries;


protected:

};
#endif // MAINWINDOW_H
