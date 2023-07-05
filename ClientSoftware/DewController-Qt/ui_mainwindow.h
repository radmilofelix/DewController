/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QSlider *generalSlider;
    QRadioButton *radioButtonSerial;
    QRadioButton *radioButtonNetwork;
    QSpinBox *sliderValue;
    QSpinBox *baud;
    QLabel *label_ComPort;
    QLabel *label_Baud;
    QLabel *label_IP;
    QLabel *label_Port;
    QSpinBox *port;
    QPushButton *connect;
    QPushButton *disconnect;
    QPushButton *setPushButton;
    QLineEdit *hostAddress;
    QFrame *line;
    QFrame *line_2;
    QFrame *line_3;
    QFrame *line_4;
    QFrame *line_5;
    QLineEdit *comPort;
    QSpinBox *heater1;
    QSpinBox *heater2;
    QSpinBox *heater3;
    QSpinBox *heater4;
    QSpinBox *heater5;
    QSpinBox *heater6;
    QSpinBox *heater7;
    QSpinBox *heater8;
    QRadioButton *radioButtonH1;
    QRadioButton *radioButtonH2;
    QRadioButton *radioButtonH3;
    QRadioButton *radioButtonH4;
    QRadioButton *radioButtonH5;
    QRadioButton *radioButtonH6;
    QRadioButton *radioButtonH7;
    QRadioButton *radioButtonH8;
    QSpinBox *temperatureBox;
    QSpinBox *humidityBox;
    QSpinBox *airPressureBox;
    QSpinBox *rpm1Box;
    QSpinBox *rpm2Box;
    QCheckBox *Fan2RPMEnabled;
    QCheckBox *Fan1RPMEnabled;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QButtonGroup *buttonGroupHeaters;
    QButtonGroup *buttonGroupComm;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setEnabled(true);
        MainWindow->resize(800, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        generalSlider = new QSlider(centralwidget);
        generalSlider->setObjectName("generalSlider");
        generalSlider->setEnabled(false);
        generalSlider->setGeometry(QRect(19, 254, 761, 20));
        generalSlider->setMaximum(255);
        generalSlider->setOrientation(Qt::Horizontal);
        radioButtonSerial = new QRadioButton(centralwidget);
        buttonGroupComm = new QButtonGroup(MainWindow);
        buttonGroupComm->setObjectName("buttonGroupComm");
        buttonGroupComm->addButton(radioButtonSerial);
        radioButtonSerial->setObjectName("radioButtonSerial");
        radioButtonSerial->setGeometry(QRect(312, 23, 71, 18));
        radioButtonSerial->setChecked(true);
        radioButtonNetwork = new QRadioButton(centralwidget);
        buttonGroupComm->addButton(radioButtonNetwork);
        radioButtonNetwork->setObjectName("radioButtonNetwork");
        radioButtonNetwork->setGeometry(QRect(104, 23, 91, 18));
        sliderValue = new QSpinBox(centralwidget);
        sliderValue->setObjectName("sliderValue");
        sliderValue->setEnabled(false);
        sliderValue->setGeometry(QRect(307, 221, 81, 22));
        sliderValue->setAlignment(Qt::AlignCenter);
        sliderValue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sliderValue->setMaximum(255);
        sliderValue->setSingleStep(1);
        baud = new QSpinBox(centralwidget);
        baud->setObjectName("baud");
        baud->setEnabled(false);
        baud->setGeometry(QRect(300, 76, 81, 22));
        baud->setAlignment(Qt::AlignCenter);
        baud->setButtonSymbols(QAbstractSpinBox::NoButtons);
        baud->setMaximum(1000000);
        baud->setValue(115200);
        label_ComPort = new QLabel(centralwidget);
        label_ComPort->setObjectName("label_ComPort");
        label_ComPort->setEnabled(false);
        label_ComPort->setGeometry(QRect(220, 49, 81, 20));
        label_Baud = new QLabel(centralwidget);
        label_Baud->setObjectName("label_Baud");
        label_Baud->setEnabled(false);
        label_Baud->setGeometry(QRect(220, 80, 51, 16));
        label_IP = new QLabel(centralwidget);
        label_IP->setObjectName("label_IP");
        label_IP->setEnabled(false);
        label_IP->setGeometry(QRect(40, 49, 16, 16));
        label_Port = new QLabel(centralwidget);
        label_Port->setObjectName("label_Port");
        label_Port->setEnabled(false);
        label_Port->setGeometry(QRect(40, 78, 41, 16));
        port = new QSpinBox(centralwidget);
        port->setObjectName("port");
        port->setEnabled(false);
        port->setGeometry(QRect(95, 76, 81, 22));
        port->setFrame(true);
        port->setAlignment(Qt::AlignCenter);
        port->setButtonSymbols(QAbstractSpinBox::NoButtons);
        port->setMaximum(65535);
        port->setValue(20002);
        connect = new QPushButton(centralwidget);
        connect->setObjectName("connect");
        connect->setGeometry(QRect(175, 119, 80, 20));
        disconnect = new QPushButton(centralwidget);
        disconnect->setObjectName("disconnect");
        disconnect->setEnabled(false);
        disconnect->setGeometry(QRect(175, 148, 80, 18));
        setPushButton = new QPushButton(centralwidget);
        setPushButton->setObjectName("setPushButton");
        setPushButton->setEnabled(false);
        setPushButton->setGeometry(QRect(404, 220, 60, 24));
        hostAddress = new QLineEdit(centralwidget);
        hostAddress->setObjectName("hostAddress");
        hostAddress->setEnabled(false);
        hostAddress->setGeometry(QRect(74, 48, 131, 20));
        hostAddress->setAlignment(Qt::AlignCenter);
        line = new QFrame(centralwidget);
        line->setObjectName("line");
        line->setGeometry(QRect(25, 172, 380, 4));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(centralwidget);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(20, 9, 4, 164));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(centralwidget);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(25, 7, 380, 4));
        line_3->setMinimumSize(QSize(380, 0));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        line_4 = new QFrame(centralwidget);
        line_4->setObjectName("line_4");
        line_4->setGeometry(QRect(399, 9, 16, 164));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_5 = new QFrame(centralwidget);
        line_5->setObjectName("line_5");
        line_5->setGeometry(QRect(208, 13, 16, 100));
        line_5->setFrameShape(QFrame::VLine);
        line_5->setFrameShadow(QFrame::Sunken);
        comPort = new QLineEdit(centralwidget);
        comPort->setObjectName("comPort");
        comPort->setGeometry(QRect(287, 50, 113, 20));
        comPort->setAlignment(Qt::AlignCenter);
        heater1 = new QSpinBox(centralwidget);
        heater1->setObjectName("heater1");
        heater1->setGeometry(QRect(123, 352, 35, 22));
        heater1->setAlignment(Qt::AlignCenter);
        heater1->setReadOnly(true);
        heater1->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater1->setMaximum(255);
        heater2 = new QSpinBox(centralwidget);
        heater2->setObjectName("heater2");
        heater2->setGeometry(QRect(123, 402, 35, 22));
        heater2->setAlignment(Qt::AlignCenter);
        heater2->setReadOnly(true);
        heater2->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater2->setMaximum(255);
        heater3 = new QSpinBox(centralwidget);
        heater3->setObjectName("heater3");
        heater3->setGeometry(QRect(123, 455, 35, 22));
        heater3->setAlignment(Qt::AlignCenter);
        heater3->setReadOnly(true);
        heater3->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater3->setMaximum(255);
        heater4 = new QSpinBox(centralwidget);
        heater4->setObjectName("heater4");
        heater4->setGeometry(QRect(290, 352, 35, 22));
        heater4->setAlignment(Qt::AlignCenter);
        heater4->setReadOnly(true);
        heater4->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater4->setMaximum(255);
        heater5 = new QSpinBox(centralwidget);
        heater5->setObjectName("heater5");
        heater5->setGeometry(QRect(290, 402, 35, 22));
        heater5->setAlignment(Qt::AlignCenter);
        heater5->setReadOnly(true);
        heater5->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater5->setMaximum(255);
        heater6 = new QSpinBox(centralwidget);
        heater6->setObjectName("heater6");
        heater6->setGeometry(QRect(290, 454, 35, 22));
        heater6->setAlignment(Qt::AlignCenter);
        heater6->setReadOnly(true);
        heater6->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater6->setMaximum(255);
        heater7 = new QSpinBox(centralwidget);
        heater7->setObjectName("heater7");
        heater7->setGeometry(QRect(533, 368, 35, 22));
        heater7->setWrapping(false);
        heater7->setAlignment(Qt::AlignCenter);
        heater7->setReadOnly(true);
        heater7->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater7->setMaximum(255);
        heater8 = new QSpinBox(centralwidget);
        heater8->setObjectName("heater8");
        heater8->setGeometry(QRect(531, 441, 35, 22));
        heater8->setAlignment(Qt::AlignCenter);
        heater8->setReadOnly(true);
        heater8->setButtonSymbols(QAbstractSpinBox::NoButtons);
        heater8->setMaximum(255);
        radioButtonH1 = new QRadioButton(centralwidget);
        buttonGroupHeaters = new QButtonGroup(MainWindow);
        buttonGroupHeaters->setObjectName("buttonGroupHeaters");
        buttonGroupHeaters->addButton(radioButtonH1);
        radioButtonH1->setObjectName("radioButtonH1");
        radioButtonH1->setGeometry(QRect(56, 353, 69, 18));
        radioButtonH1->setChecked(true);
        radioButtonH2 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH2);
        radioButtonH2->setObjectName("radioButtonH2");
        radioButtonH2->setGeometry(QRect(56, 403, 69, 18));
        radioButtonH3 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH3);
        radioButtonH3->setObjectName("radioButtonH3");
        radioButtonH3->setGeometry(QRect(56, 456, 69, 18));
        radioButtonH4 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH4);
        radioButtonH4->setObjectName("radioButtonH4");
        radioButtonH4->setGeometry(QRect(216, 353, 69, 18));
        radioButtonH5 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH5);
        radioButtonH5->setObjectName("radioButtonH5");
        radioButtonH5->setGeometry(QRect(216, 403, 69, 18));
        radioButtonH6 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH6);
        radioButtonH6->setObjectName("radioButtonH6");
        radioButtonH6->setGeometry(QRect(216, 456, 69, 18));
        radioButtonH7 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH7);
        radioButtonH7->setObjectName("radioButtonH7");
        radioButtonH7->setGeometry(QRect(453, 371, 69, 18));
        radioButtonH8 = new QRadioButton(centralwidget);
        buttonGroupHeaters->addButton(radioButtonH8);
        radioButtonH8->setObjectName("radioButtonH8");
        radioButtonH8->setGeometry(QRect(453, 444, 69, 18));
        temperatureBox = new QSpinBox(centralwidget);
        temperatureBox->setObjectName("temperatureBox");
        temperatureBox->setGeometry(QRect(507, 106, 35, 22));
        temperatureBox->setAlignment(Qt::AlignCenter);
        temperatureBox->setReadOnly(true);
        temperatureBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        temperatureBox->setMaximum(100);
        humidityBox = new QSpinBox(centralwidget);
        humidityBox->setObjectName("humidityBox");
        humidityBox->setGeometry(QRect(591, 106, 35, 22));
        humidityBox->setAlignment(Qt::AlignCenter);
        humidityBox->setReadOnly(true);
        humidityBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        humidityBox->setMaximum(100);
        airPressureBox = new QSpinBox(centralwidget);
        airPressureBox->setObjectName("airPressureBox");
        airPressureBox->setGeometry(QRect(665, 106, 35, 22));
        airPressureBox->setAlignment(Qt::AlignCenter);
        airPressureBox->setReadOnly(true);
        airPressureBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        airPressureBox->setMaximum(2000);
        rpm1Box = new QSpinBox(centralwidget);
        rpm1Box->setObjectName("rpm1Box");
        rpm1Box->setGeometry(QRect(590, 368, 60, 22));
        rpm1Box->setAlignment(Qt::AlignCenter);
        rpm1Box->setReadOnly(true);
        rpm1Box->setButtonSymbols(QAbstractSpinBox::NoButtons);
        rpm1Box->setMaximum(3000);
        rpm2Box = new QSpinBox(centralwidget);
        rpm2Box->setObjectName("rpm2Box");
        rpm2Box->setGeometry(QRect(588, 441, 62, 22));
        rpm2Box->setAlignment(Qt::AlignCenter);
        rpm2Box->setReadOnly(true);
        rpm2Box->setButtonSymbols(QAbstractSpinBox::NoButtons);
        rpm2Box->setMaximum(3000);
        Fan2RPMEnabled = new QCheckBox(centralwidget);
        Fan2RPMEnabled->setObjectName("Fan2RPMEnabled");
        Fan2RPMEnabled->setGeometry(QRect(454, 424, 73, 18));
        Fan1RPMEnabled = new QCheckBox(centralwidget);
        Fan1RPMEnabled->setObjectName("Fan1RPMEnabled");
        Fan1RPMEnabled->setGeometry(QRect(454, 352, 72, 18));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(538, 353, 37, 12));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(608, 353, 37, 12));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(538, 425, 37, 12));
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(608, 425, 37, 12));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(129, 336, 37, 12));
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(129, 386, 37, 12));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(129, 440, 37, 12));
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(295, 440, 37, 12));
        label_9 = new QLabel(centralwidget);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(295, 386, 37, 12));
        label_10 = new QLabel(centralwidget);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(295, 336, 37, 12));
        label_11 = new QLabel(centralwidget);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(498, 86, 67, 16));
        label_12 = new QLabel(centralwidget);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(588, 86, 53, 16));
        label_13 = new QLabel(centralwidget);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(664, 86, 55, 16));
        label_14 = new QLabel(centralwidget);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(225, 276, 320, 16));
        label_14->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(centralwidget);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(318, 204, 61, 16));
        label_15->setAlignment(Qt::AlignCenter);
        label_16 = new QLabel(centralwidget);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(415, 469, 237, 16));
        QFont font;
        font.setBold(true);
        label_16->setFont(font);
        label_16->setAlignment(Qt::AlignCenter);
        label_17 = new QLabel(centralwidget);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(423, 481, 225, 16));
        label_17->setFont(font);
        label_17->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);
        QObject::connect(generalSlider, &QSlider::valueChanged, sliderValue, &QSpinBox::setValue);
        QObject::connect(sliderValue, &QSpinBox::valueChanged, generalSlider, &QSlider::setValue);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        radioButtonSerial->setText(QCoreApplication::translate("MainWindow", "Serial", nullptr));
        radioButtonNetwork->setText(QCoreApplication::translate("MainWindow", "Network", nullptr));
        label_ComPort->setText(QCoreApplication::translate("MainWindow", "COM Port: ", nullptr));
        label_Baud->setText(QCoreApplication::translate("MainWindow", "BAUD:", nullptr));
        label_IP->setText(QCoreApplication::translate("MainWindow", "IP:", nullptr));
        label_Port->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        connect->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        disconnect->setText(QCoreApplication::translate("MainWindow", "Disconnect", nullptr));
        setPushButton->setText(QCoreApplication::translate("MainWindow", "SET PWM", nullptr));
        radioButtonH1->setText(QCoreApplication::translate("MainWindow", "Heater 1", nullptr));
        radioButtonH2->setText(QCoreApplication::translate("MainWindow", "Heater 2", nullptr));
        radioButtonH3->setText(QCoreApplication::translate("MainWindow", "Heater 3", nullptr));
        radioButtonH4->setText(QCoreApplication::translate("MainWindow", "Heater 4", nullptr));
        radioButtonH5->setText(QCoreApplication::translate("MainWindow", "Heater 5", nullptr));
        radioButtonH6->setText(QCoreApplication::translate("MainWindow", "Heater 6", nullptr));
        radioButtonH7->setText(QCoreApplication::translate("MainWindow", "Fan 1", nullptr));
        radioButtonH8->setText(QCoreApplication::translate("MainWindow", "Fan 2", nullptr));
        Fan2RPMEnabled->setText(QCoreApplication::translate("MainWindow", "RPM", nullptr));
        Fan1RPMEnabled->setText(QCoreApplication::translate("MainWindow", " RPM", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "RPM", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "RPM", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "PWM", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Temperature", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Humidity", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Pressure", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "PWM slider for  selected device", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "PWM value", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Do not enable RPM reading", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "if the fan has no rpm capabilities!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
