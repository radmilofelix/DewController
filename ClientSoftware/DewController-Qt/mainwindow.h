#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
#include <QThread>

#define WINDOWS_SYSTEM

#ifdef WINDOWS_SYSTEM
#include<windows.h>
#else
#include <unistd.h>
#endif

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
    void GetData();
    void SendData();


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
    int heaters[8];
    QSerialPort *m_serial = nullptr;
    int serialTimeout;
    int readWriteDelay;
    int timerId;
    QTimerEvent *event;
    QString dataBuffer;
    bool timerUpdate;
    int timerCounter;
    QTcpSocket  _socket;
    void OnReadyRead();
    bool connected;
    bool disconnectAlarm;
    int fansRPMenable;


protected:
//    void mtimerEvent();
    void timerEvent(QTimerEvent *event);

};
#endif // MAINWINDOW_H
