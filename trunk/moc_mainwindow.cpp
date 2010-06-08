/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Jun 7 08:25:02 2010
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   11,   11,   11, 0x08,
      55,   11,   11,   11, 0x08,
      81,   11,   11,   11, 0x08,
     114,   11,   11,   11, 0x08,
     144,   11,   11,   11, 0x08,
     168,   11,   11,   11, 0x08,
     192,   11,   11,   11, 0x08,
     218,   11,   11,   11, 0x08,
     244,   11,   11,   11, 0x08,
     268,   11,   11,   11, 0x08,
     294,   11,   11,   11, 0x08,
     310,   11,   11,   11, 0x08,
     334,  332,   11,   11, 0x08,
     380,   11,   11,   11, 0x08,
     394,   11,   11,   11, 0x08,
     408,   11,   11,   11, 0x0a,
     429,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0KillProcess()\0"
    "on_save_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_edit_img_pushButton_clicked()\0"
    "on_lineEdit_editingFinished()\0"
    "on_checkBox_7_clicked()\0on_checkBox_6_clicked()\0"
    "on_toolButton_2_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_toolButton_clicked()\0"
    "on_pushButton_3_clicked()\0UpdateOptions()\0"
    "ReadyReadStdOutSlot()\0,\0"
    "ProcessFinishedSlot(int,QProcess::ExitStatus)\0"
    "ExtractSlot()\0ReplaceSlot()\0"
    "ShowMessage(QString)\0GetThreadDone(int)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: KillProcess(); break;
        case 1: on_save_pushButton_clicked(); break;
        case 2: on_pushButton_2_clicked(); break;
        case 3: on_edit_img_pushButton_clicked(); break;
        case 4: on_lineEdit_editingFinished(); break;
        case 5: on_checkBox_7_clicked(); break;
        case 6: on_checkBox_6_clicked(); break;
        case 7: on_toolButton_2_clicked(); break;
        case 8: on_pushButton_4_clicked(); break;
        case 9: on_toolButton_clicked(); break;
        case 10: on_pushButton_3_clicked(); break;
        case 11: UpdateOptions(); break;
        case 12: ReadyReadStdOutSlot(); break;
        case 13: ProcessFinishedSlot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 14: ExtractSlot(); break;
        case 15: ReplaceSlot(); break;
        case 16: ShowMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: GetThreadDone((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::KillProcess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
