/****************************************************************************
** Meta object code from reading C++ file 'ScaleFactorDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../../src/ScaleFactorDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ScaleFactorDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ScaleFactorDialog_t {
    QByteArrayData data[9];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ScaleFactorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ScaleFactorDialog_t qt_meta_stringdata_ScaleFactorDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ScaleFactorDialog"
QT_MOC_LITERAL(1, 18, 14), // "SetScaleFactor"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 5), // "scale"
QT_MOC_LITERAL(4, 40, 28), // "ChangeVectorComponentsNumber"
QT_MOC_LITERAL(5, 69, 4), // "int*"
QT_MOC_LITERAL(6, 74, 22), // "vectorComponentsNumber"
QT_MOC_LITERAL(7, 97, 18), // "ApplyButtonClicked"
QT_MOC_LITERAL(8, 116, 20) // "ComboBoxIndexChanged"

    },
    "ScaleFactorDialog\0SetScaleFactor\0\0"
    "scale\0ChangeVectorComponentsNumber\0"
    "int*\0vectorComponentsNumber\0"
    "ApplyButtonClicked\0ComboBoxIndexChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ScaleFactorDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       4,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   40,    2, 0x08 /* Private */,
       8,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ScaleFactorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ScaleFactorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SetScaleFactor((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->ChangeVectorComponentsNumber((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 2: _t->ApplyButtonClicked(); break;
        case 3: _t->ComboBoxIndexChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ScaleFactorDialog::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScaleFactorDialog::SetScaleFactor)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ScaleFactorDialog::*)(int * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ScaleFactorDialog::ChangeVectorComponentsNumber)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ScaleFactorDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ScaleFactorDialog.data,
    qt_meta_data_ScaleFactorDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ScaleFactorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ScaleFactorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ScaleFactorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ScaleFactorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void ScaleFactorDialog::SetScaleFactor(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ScaleFactorDialog::ChangeVectorComponentsNumber(int * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
