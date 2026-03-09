/****************************************************************************
** Meta object code from reading C++ file 'IsoSurfaceDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../../src/IsoSurfaceDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'IsoSurfaceDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IsoSurfaceDialog_t {
    QByteArrayData data[11];
    char stringdata0[175];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IsoSurfaceDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IsoSurfaceDialog_t qt_meta_stringdata_IsoSurfaceDialog = {
    {
QT_MOC_LITERAL(0, 0, 16), // "IsoSurfaceDialog"
QT_MOC_LITERAL(1, 17, 19), // "finishSetParameters"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 20), // "changeFloodParameter"
QT_MOC_LITERAL(4, 59, 19), // "changeFlowParameter"
QT_MOC_LITERAL(5, 79, 21), // "changeIsoSurfaceValue"
QT_MOC_LITERAL(6, 101, 13), // "setParameters"
QT_MOC_LITERAL(7, 115, 16), // "parameterChanged"
QT_MOC_LITERAL(8, 132, 17), // "flowNumberChanged"
QT_MOC_LITERAL(9, 150, 12), // "valueChanged"
QT_MOC_LITERAL(10, 163, 11) // "textChanged"

    },
    "IsoSurfaceDialog\0finishSetParameters\0"
    "\0changeFloodParameter\0changeFlowParameter\0"
    "changeIsoSurfaceValue\0setParameters\0"
    "parameterChanged\0flowNumberChanged\0"
    "valueChanged\0textChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IsoSurfaceDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       3,    1,   62,    2, 0x06 /* Public */,
       4,    1,   65,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   71,    2, 0x08 /* Private */,
       7,    0,   72,    2, 0x08 /* Private */,
       8,    0,   73,    2, 0x08 /* Private */,
       9,    0,   74,    2, 0x08 /* Private */,
      10,    1,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void IsoSurfaceDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IsoSurfaceDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->finishSetParameters((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->changeFloodParameter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->changeFlowParameter((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->changeIsoSurfaceValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setParameters(); break;
        case 5: _t->parameterChanged(); break;
        case 6: _t->flowNumberChanged(); break;
        case 7: _t->valueChanged(); break;
        case 8: _t->textChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IsoSurfaceDialog::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoSurfaceDialog::finishSetParameters)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IsoSurfaceDialog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoSurfaceDialog::changeFloodParameter)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (IsoSurfaceDialog::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoSurfaceDialog::changeFlowParameter)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (IsoSurfaceDialog::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoSurfaceDialog::changeIsoSurfaceValue)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IsoSurfaceDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_IsoSurfaceDialog.data,
    qt_meta_data_IsoSurfaceDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *IsoSurfaceDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IsoSurfaceDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IsoSurfaceDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int IsoSurfaceDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void IsoSurfaceDialog::finishSetParameters(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IsoSurfaceDialog::changeFloodParameter(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void IsoSurfaceDialog::changeFlowParameter(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IsoSurfaceDialog::changeIsoSurfaceValue(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
