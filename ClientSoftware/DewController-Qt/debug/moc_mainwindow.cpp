/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "EnableSerial",
    "",
    "EnableNetwork",
    "ButtonConnect",
    "ButtonDisconnect",
    "SliderReleased",
    "Set",
    "EnableFansRPM",
    "GetData",
    "SendData"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[11];
    char stringdata1[13];
    char stringdata2[1];
    char stringdata3[14];
    char stringdata4[14];
    char stringdata5[17];
    char stringdata6[15];
    char stringdata7[4];
    char stringdata8[14];
    char stringdata9[8];
    char stringdata10[9];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSMainWindowENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSMainWindowENDCLASS_t qt_meta_stringdata_CLASSMainWindowENDCLASS = {
    {
        QT_MOC_LITERAL(0, 10),  // "MainWindow"
        QT_MOC_LITERAL(11, 12),  // "EnableSerial"
        QT_MOC_LITERAL(24, 0),  // ""
        QT_MOC_LITERAL(25, 13),  // "EnableNetwork"
        QT_MOC_LITERAL(39, 13),  // "ButtonConnect"
        QT_MOC_LITERAL(53, 16),  // "ButtonDisconnect"
        QT_MOC_LITERAL(70, 14),  // "SliderReleased"
        QT_MOC_LITERAL(85, 3),  // "Set"
        QT_MOC_LITERAL(89, 13),  // "EnableFansRPM"
        QT_MOC_LITERAL(103, 7),  // "GetData"
        QT_MOC_LITERAL(111, 8)   // "SendData"
    },
    "MainWindow",
    "EnableSerial",
    "",
    "EnableNetwork",
    "ButtonConnect",
    "ButtonDisconnect",
    "SliderReleased",
    "Set",
    "EnableFansRPM",
    "GetData",
    "SendData"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      11,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x08,    1 /* Private */,
       3,    0,   69,    2, 0x08,    2 /* Private */,
       4,    0,   70,    2, 0x08,    3 /* Private */,
       5,    0,   71,    2, 0x08,    4 /* Private */,
       6,    0,   72,    2, 0x08,    5 /* Private */,
       7,    0,   73,    2, 0x08,    6 /* Private */,
       8,    0,   74,    2, 0x08,    7 /* Private */,
       9,    0,   75,    2, 0x08,    8 /* Private */,
      10,    0,   76,    2, 0x08,    9 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'EnableSerial'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'EnableNetwork'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'ButtonConnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'ButtonDisconnect'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SliderReleased'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Set'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'EnableFansRPM'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'GetData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'SendData'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->EnableSerial(); break;
        case 1: _t->EnableNetwork(); break;
        case 2: _t->ButtonConnect(); break;
        case 3: _t->ButtonDisconnect(); break;
        case 4: _t->SliderReleased(); break;
        case 5: _t->Set(); break;
        case 6: _t->EnableFansRPM(); break;
        case 7: _t->GetData(); break;
        case 8: _t->SendData(); break;
        default: ;
        }
    }
    (void)_a;
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
