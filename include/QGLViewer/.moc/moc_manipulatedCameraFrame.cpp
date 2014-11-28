/****************************************************************************
** Meta object code from reading C++ file 'manipulatedCameraFrame.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../manipulatedCameraFrame.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manipulatedCameraFrame.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_qglviewer__ManipulatedCameraFrame_t {
    QByteArrayData data[12];
    char stringdata[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_qglviewer__ManipulatedCameraFrame_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_qglviewer__ManipulatedCameraFrame_t qt_meta_stringdata_qglviewer__ManipulatedCameraFrame = {
    {
QT_MOC_LITERAL(0, 0, 33),
QT_MOC_LITERAL(1, 34, 11),
QT_MOC_LITERAL(2, 46, 0),
QT_MOC_LITERAL(3, 47, 5),
QT_MOC_LITERAL(4, 53, 16),
QT_MOC_LITERAL(5, 70, 3),
QT_MOC_LITERAL(6, 74, 2),
QT_MOC_LITERAL(7, 77, 4),
QT_MOC_LITERAL(8, 82, 18),
QT_MOC_LITERAL(9, 101, 11),
QT_MOC_LITERAL(10, 113, 7),
QT_MOC_LITERAL(11, 121, 9)
    },
    "qglviewer::ManipulatedCameraFrame\0"
    "setFlySpeed\0\0speed\0setSceneUpVector\0"
    "Vec\0up\0spin\0initFromDOMElement\0"
    "QDomElement\0element\0flyUpdate\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qglviewer__ManipulatedCameraFrame[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a,
       4,    1,   42,    2, 0x0a,
       7,    0,   45,    2, 0x09,
       8,    1,   46,    2, 0x0a,
      11,    0,   49,    2, 0x08,

 // slots: parameters
    QMetaType::Void, QMetaType::QReal,    3,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,

       0        // eod
};

void qglviewer::ManipulatedCameraFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ManipulatedCameraFrame *_t = static_cast<ManipulatedCameraFrame *>(_o);
        switch (_id) {
        case 0: _t->setFlySpeed((*reinterpret_cast< qreal(*)>(_a[1]))); break;
        case 1: _t->setSceneUpVector((*reinterpret_cast< const Vec(*)>(_a[1]))); break;
        case 2: _t->spin(); break;
        case 3: _t->initFromDOMElement((*reinterpret_cast< const QDomElement(*)>(_a[1]))); break;
        case 4: _t->flyUpdate(); break;
        default: ;
        }
    }
}

const QMetaObject qglviewer::ManipulatedCameraFrame::staticMetaObject = {
    { &ManipulatedFrame::staticMetaObject, qt_meta_stringdata_qglviewer__ManipulatedCameraFrame.data,
      qt_meta_data_qglviewer__ManipulatedCameraFrame,  qt_static_metacall, 0, 0}
};


const QMetaObject *qglviewer::ManipulatedCameraFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qglviewer::ManipulatedCameraFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_qglviewer__ManipulatedCameraFrame.stringdata))
        return static_cast<void*>(const_cast< ManipulatedCameraFrame*>(this));
    return ManipulatedFrame::qt_metacast(_clname);
}

int qglviewer::ManipulatedCameraFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ManipulatedFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
