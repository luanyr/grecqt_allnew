/****************************************************************************
** Meta object code from reading C++ file 'recvthread.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../recvthread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'recvthread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RecvThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      14,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   12,   11,   11, 0x05,
      49,   37,   11,   11, 0x05,
      83,   78,   11,   11, 0x05,
     111,   78,   11,   11, 0x05,
     138,   78,   11,   11, 0x05,
     160,   78,   11,   11, 0x05,
     188,   78,   11,   11, 0x05,
     211,   78,   11,   11, 0x05,
     245,  240,   11,   11, 0x05,
     280,  270,   11,   11, 0x05,
     318,  270,   11,   11, 0x05,
     358,   78,   11,   11, 0x05,
     395,  390,   11,   11, 0x05,
     425,   78,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     453,   11,   11,   11, 0x0a,
     469,   78,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RecvThread[] = {
    "RecvThread\0\0msg\0RecvMsgToui(QString)\0"
    "bShow,value\0RecvProcessToui(bool,UINT32)\0"
    "data\0UserLoginSignal(QByteArray)\0"
    "UserOperSignal(QByteArray)\0"
    "BitSignal(QByteArray)\0InqStatusSignal(QByteArray)\0"
    "WipeSignal(QByteArray)\0"
    "WorkStatusSignal(QByteArray)\0head\0"
    "AllAckSignal(QByteArray)\0head,data\0"
    "AllCmdUiSignal(QByteArray,QByteArray)\0"
    "AllCmdSendSignal(QByteArray,QByteArray)\0"
    "SetSysBizTypeSignal(QByteArray)\0list\0"
    "DirSignal(QList<CFileAttrib>)\0"
    "TransBulkSignal(QByteArray)\0RecvStartSlot()\0"
    "TransBulkSlot(QByteArray)\0"
};

void RecvThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        RecvThread *_t = static_cast<RecvThread *>(_o);
        switch (_id) {
        case 0: _t->RecvMsgToui((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->RecvProcessToui((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< UINT32(*)>(_a[2]))); break;
        case 2: _t->UserLoginSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 3: _t->UserOperSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->BitSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 5: _t->InqStatusSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 6: _t->WipeSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 7: _t->WorkStatusSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: _t->AllAckSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 9: _t->AllCmdUiSignal((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 10: _t->AllCmdSendSignal((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 11: _t->SetSysBizTypeSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 12: _t->DirSignal((*reinterpret_cast< QList<CFileAttrib>(*)>(_a[1]))); break;
        case 13: _t->TransBulkSignal((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 14: _t->RecvStartSlot(); break;
        case 15: _t->TransBulkSlot((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData RecvThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject RecvThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_RecvThread,
      qt_meta_data_RecvThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RecvThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RecvThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RecvThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RecvThread))
        return static_cast<void*>(const_cast< RecvThread*>(this));
    return QThread::qt_metacast(_clname);
}

int RecvThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void RecvThread::RecvMsgToui(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RecvThread::RecvProcessToui(bool _t1, UINT32 _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RecvThread::UserLoginSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RecvThread::UserOperSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RecvThread::BitSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RecvThread::InqStatusSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void RecvThread::WipeSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RecvThread::WorkStatusSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void RecvThread::AllAckSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void RecvThread::AllCmdUiSignal(QByteArray _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void RecvThread::AllCmdSendSignal(QByteArray _t1, QByteArray _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void RecvThread::SetSysBizTypeSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void RecvThread::DirSignal(QList<CFileAttrib> _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void RecvThread::TransBulkSignal(QByteArray _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}
QT_END_MOC_NAMESPACE
