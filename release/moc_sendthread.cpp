/****************************************************************************
** Meta object code from reading C++ file 'sendthread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../sendthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sendthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SendThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      40,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      45,   33,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      98,   74,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     144,   74,   11,   11, 0x0a,
     194,  173,   11,   11, 0x0a,
     225,   74,   11,   11, 0x0a,
     259,  254,   11,   11, 0x0a,
     287,  278,   11,   11, 0x0a,
     314,  307,   11,   11, 0x0a,
     341,  307,   11,   11, 0x0a,
     371,  307,   11,   11, 0x0a,
     397,   11,   11,   11, 0x0a,
     414,  307,   11,   11, 0x0a,
     442,   11,   11,   11, 0x0a,
     470,  461,   11,   11, 0x0a,
     496,  307,   11,   11, 0x0a,
     514,  307,   11,   11, 0x0a,
     536,   11,   11,   11, 0x0a,
     554,   11,   11,   11, 0x0a,
     580,  571,   11,   11, 0x0a,
     610,  603,   11,   11, 0x0a,
     632,   11,   11,   11, 0x0a,
     648,   11,   11,   11, 0x0a,
     658,   11,   11,   11, 0x0a,
     674,   11,   11,   11, 0x0a,
     686,   11,   11,   11, 0x0a,
     702,   11,   11,   11, 0x0a,
     712,   11,   11,   11, 0x0a,
     723,   11,   11,   11, 0x0a,
     736,   11,   11,   11, 0x0a,
     754,   11,   11,   11, 0x0a,
     773,  767,   11,   11, 0x0a,
     791,  767,   11,   11, 0x0a,
     828,  807,   11,   11, 0x0a,
     880,  864,   11,   11, 0x0a,
     920,  915,   11,   11, 0x0a,
     953,  943,   11,   11, 0x0a,
     996,  991,   11,   11, 0x0a,
    1026,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SendThread[] = {
    "SendThread\0\0SendMsgToui(QString)\0"
    "bShow,value\0SendProcessToui(bool,UINT32)\0"
    "strUserName,strPassword\0"
    "UserLoginSlot(QString,QString)\0"
    "UserQuitSlot()\0UserNewSlot(QString,QString)\0"
    "strUserName,strPwNew\0"
    "UserChgPwSlot(QString,QString)\0"
    "UserDelSlot(QString,QString)\0uCon\0"
    "RecConSlot(UINT32)\0uRecMode\0"
    "RecModeSlot(UINT32)\0filter\0"
    "WriteProtectSlot(QVariant)\0"
    "WriteProtectOffSlot(QVariant)\0"
    "UnInstStartSlot(QVariant)\0UnInstStopSlot()\0"
    "PlaybackStartSlot(QVariant)\0"
    "PlaybackStopSlot()\0uPbPause\0"
    "PlaybackPauseSlot(UINT32)\0DirSlot(QVariant)\0"
    "FileDelSlot(QVariant)\0CompressConSlot()\0"
    "EncryptConSlot()\0dateTime\0"
    "SetTimeSlot(QDateTime)\0filesz\0"
    "SetfileSzSlot(UINT32)\0HeartBeatSlot()\0"
    "BitSlot()\0InqStatusSlot()\0ResetSlot()\0"
    "PowerDownSlot()\0LogSlot()\0WipeSlot()\0"
    "ReportSlot()\0SelfDestorySlot()\0"
    "UpdateSlot()\0bShow\0UnmountSlot(bool)\0"
    "MountSlot(bool)\0dblLon,dblLat,dblDir\0"
    "SetCurVecSlot(double,double,double)\0"
    "uCmdTm,listFile\0MenuActionSlot(UINT32,QStringList)\0"
    "head\0AllAckSlot(QByteArray)\0head,data\0"
    "AllCmdSendSlot(QByteArray,QByteArray)\0"
    "data\0SetSysBizTypeSlot(QByteArray)\0"
    "TimerFunction()\0"
};

void SendThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SendThread *_t = static_cast<SendThread *>(_o);
        switch (_id) {
        case 0: _t->SendMsgToui((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->SendProcessToui((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< UINT32(*)>(_a[2]))); break;
        case 2: _t->UserLoginSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->UserQuitSlot(); break;
        case 4: _t->UserNewSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->UserChgPwSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 6: _t->UserDelSlot((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->RecConSlot((*reinterpret_cast< UINT32(*)>(_a[1]))); break;
        case 8: _t->RecModeSlot((*reinterpret_cast< UINT32(*)>(_a[1]))); break;
        case 9: _t->WriteProtectSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 10: _t->WriteProtectOffSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 11: _t->UnInstStartSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 12: _t->UnInstStopSlot(); break;
        case 13: _t->PlaybackStartSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 14: _t->PlaybackStopSlot(); break;
        case 15: _t->PlaybackPauseSlot((*reinterpret_cast< UINT32(*)>(_a[1]))); break;
        case 16: _t->DirSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 17: _t->FileDelSlot((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 18: _t->CompressConSlot(); break;
        case 19: _t->EncryptConSlot(); break;
        case 20: _t->SetTimeSlot((*reinterpret_cast< QDateTime(*)>(_a[1]))); break;
        case 21: _t->SetfileSzSlot((*reinterpret_cast< UINT32(*)>(_a[1]))); break;
        case 22: _t->HeartBeatSlot(); break;
        case 23: _t->BitSlot(); break;
        case 24: _t->InqStatusSlot(); break;
        case 25: _t->ResetSlot(); break;
        case 26: _t->PowerDownSlot(); break;
        case 27: _t->LogSlot(); break;
        case 28: _t->WipeSlot(); break;
        case 29: _t->ReportSlot(); break;
        case 30: _t->SelfDestorySlot(); break;
        case 31: _t->UpdateSlot(); break;
        case 32: _t->UnmountSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->MountSlot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 34: _t->SetCurVecSlot((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 35: _t->MenuActionSlot((*reinterpret_cast< UINT32(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        case 36: _t->AllAckSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 37: _t->AllCmdSendSlot((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 38: _t->SetSysBizTypeSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 39: _t->TimerFunction(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SendThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SendThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SendThread,
      qt_meta_data_SendThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SendThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SendThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SendThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SendThread))
        return static_cast<void*>(const_cast< SendThread*>(this));
    return QThread::qt_metacast(_clname);
}

int SendThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 40)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 40;
    }
    return _id;
}

// SIGNAL 0
void SendThread::SendMsgToui(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SendThread::SendProcessToui(bool _t1, UINT32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
