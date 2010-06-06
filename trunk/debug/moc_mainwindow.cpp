/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Jun 4 04:54:24 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      26,   11,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,
      89,   11,   11,   11, 0x08,
     113,   11,   11,   11, 0x08,
     143,  137,   11,   11, 0x08,
     180,   11,   11,   11, 0x08,
     206,   11,   11,   11, 0x08,
     232,   11,   11,   11, 0x08,
     256,   11,   11,   11, 0x08,
     284,  282,   11,   11, 0x08,
     302,   11,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     342,  340,   11,   11, 0x08,
     388,   11,   11,   11, 0x08,
     402,   11,   11,   11, 0x08,
     416,   11,   11,   11, 0x0a,
     437,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0KillProcess()\0"
    "on_edit_img_pushButton_clicked()\0"
    "on_lineEdit_editingFinished()\0"
    "on_checkBox_7_clicked()\0on_checkBox_6_clicked()\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "on_toolButton_2_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_toolButton_clicked()\0"
    "on_pushButton_3_clicked()\0i\0"
    "UpdateRegStr(int)\0UpdateOptions()\0"
    "ReadyReadStdOutSlot()\0,\0"
    "ProcessFinishedSlot(int,QProcess::ExitStatus)\0"
    "ExtractSlot()\0ReplaceSlot()\0"
    "ShowMessage(QString)\0GetThreadDone(int)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
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
        case 1: on_edit_img_pushButton_clicked(); break;
        case 2: on_lineEdit_editingFinished(); break;
        case 3: on_checkBox_7_clicked(); break;
        case 4: on_checkBox_6_clicked(); break;
        case 5: on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: on_toolButton_2_clicked(); break;
        case 7: on_pushButton_4_clicked(); break;
        case 8: on_toolButton_clicked(); break;
        case 9: on_pushButton_3_clicked(); break;
        case 10: UpdateRegStr((*reinterpret_cast< int(*)>(_a[1]))); break;
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
