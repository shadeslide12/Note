/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../../../src/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[67];
    char stringdata0[1343];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 27), // "on_actionLoadMesh_triggered"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 29), // "on_actionUpdateFlow_triggered"
QT_MOC_LITERAL(4, 70, 28), // "on_actionNewWindow_triggered"
QT_MOC_LITERAL(5, 99, 27), // "on_actionOpenFile_triggered"
QT_MOC_LITERAL(6, 127, 36), // "on_actionAddScalarBarActor_tr..."
QT_MOC_LITERAL(7, 164, 31), // "on_actionAddAxisActor_triggered"
QT_MOC_LITERAL(8, 196, 34), // "on_actionAddOutlineActor_trig..."
QT_MOC_LITERAL(9, 231, 38), // "on_actionAddPointInformation_..."
QT_MOC_LITERAL(10, 270, 33), // "on_actionExport_Picture_trigg..."
QT_MOC_LITERAL(11, 304, 30), // "on_ConstHeightCheckBox_toggled"
QT_MOC_LITERAL(12, 335, 7), // "trigger"
QT_MOC_LITERAL(13, 343, 16), // "xoyViewTriggered"
QT_MOC_LITERAL(14, 360, 16), // "xozViewTriggered"
QT_MOC_LITERAL(15, 377, 16), // "yozViewTriggered"
QT_MOC_LITERAL(16, 394, 16), // "xyzViewTriggered"
QT_MOC_LITERAL(17, 411, 21), // "meshCheckBoxTriggered"
QT_MOC_LITERAL(18, 433, 24), // "contourCheckBoxTriggered"
QT_MOC_LITERAL(19, 458, 22), // "shadeCheckBoxTriggered"
QT_MOC_LITERAL(20, 481, 21), // "edgeCheckBoxTriggered"
QT_MOC_LITERAL(21, 503, 29), // "transparancyCheckBoxTriggered"
QT_MOC_LITERAL(22, 533, 25), // "lightingCheckBoxTriggered"
QT_MOC_LITERAL(23, 559, 11), // "setColorBar"
QT_MOC_LITERAL(24, 571, 29), // "selectBoundaryButtonTriggeded"
QT_MOC_LITERAL(25, 601, 17), // "showBoundaryActor"
QT_MOC_LITERAL(26, 619, 11), // "deleteSlice"
QT_MOC_LITERAL(27, 631, 13), // "cutplaneIndex"
QT_MOC_LITERAL(28, 645, 27), // "isoSurfaceCheckBoxTriggered"
QT_MOC_LITERAL(29, 673, 32), // "isoSurfaceSettingButtonTriggered"
QT_MOC_LITERAL(30, 706, 26), // "isoSurfaceChangeFlowNumber"
QT_MOC_LITERAL(31, 733, 22), // "isoSurfaceValueChanged"
QT_MOC_LITERAL(32, 756, 17), // "changeFloodNumber"
QT_MOC_LITERAL(33, 774, 23), // "slicesCheckBoxTriggered"
QT_MOC_LITERAL(34, 798, 14), // "changeCutplane"
QT_MOC_LITERAL(35, 813, 7), // "double*"
QT_MOC_LITERAL(36, 821, 15), // "makeNewCutplane"
QT_MOC_LITERAL(37, 837, 6), // "origin"
QT_MOC_LITERAL(38, 844, 6), // "normal"
QT_MOC_LITERAL(39, 851, 26), // "updateCutplaneColorMapping"
QT_MOC_LITERAL(40, 878, 8), // "minValue"
QT_MOC_LITERAL(41, 887, 8), // "maxValue"
QT_MOC_LITERAL(42, 896, 14), // "numberOfColors"
QT_MOC_LITERAL(43, 911, 8), // "isBanded"
QT_MOC_LITERAL(44, 920, 12), // "showCutplane"
QT_MOC_LITERAL(45, 933, 26), // "ConstHeightButtonTriggered"
QT_MOC_LITERAL(46, 960, 19), // "AddConstHeightPlane"
QT_MOC_LITERAL(47, 980, 6), // "height"
QT_MOC_LITERAL(48, 987, 21), // "ChangeConstHeightFlow"
QT_MOC_LITERAL(49, 1009, 4), // "flow"
QT_MOC_LITERAL(50, 1014, 20), // "AddBladeToBladePlane"
QT_MOC_LITERAL(51, 1035, 4), // "span"
QT_MOC_LITERAL(52, 1040, 27), // "ChangeBladeToBladePlaneFlow"
QT_MOC_LITERAL(53, 1068, 19), // "onSpanSliderChanged"
QT_MOC_LITERAL(54, 1088, 5), // "value"
QT_MOC_LITERAL(55, 1094, 17), // "onCopyZoneChanged"
QT_MOC_LITERAL(56, 1112, 5), // "index"
QT_MOC_LITERAL(57, 1118, 23), // "onPeriodicCopyRequested"
QT_MOC_LITERAL(58, 1142, 18), // "onProbePanelClosed"
QT_MOC_LITERAL(59, 1161, 23), // "on_Check_3Dview_toggled"
QT_MOC_LITERAL(60, 1185, 7), // "checked"
QT_MOC_LITERAL(61, 1193, 26), // "on_Check_ThreeView_toggled"
QT_MOC_LITERAL(62, 1220, 21), // "on_Check_Meri_toggled"
QT_MOC_LITERAL(63, 1242, 29), // "on_Check_BladeToBlade_toggled"
QT_MOC_LITERAL(64, 1272, 37), // "on_CBtn_BackGround_currentTex..."
QT_MOC_LITERAL(65, 1310, 4), // "text"
QT_MOC_LITERAL(66, 1315, 27) // "on_Btn_TurboInitial_clicked"

    },
    "MainWindow\0on_actionLoadMesh_triggered\0"
    "\0on_actionUpdateFlow_triggered\0"
    "on_actionNewWindow_triggered\0"
    "on_actionOpenFile_triggered\0"
    "on_actionAddScalarBarActor_triggered\0"
    "on_actionAddAxisActor_triggered\0"
    "on_actionAddOutlineActor_triggered\0"
    "on_actionAddPointInformation_triggered\0"
    "on_actionExport_Picture_triggered\0"
    "on_ConstHeightCheckBox_toggled\0trigger\0"
    "xoyViewTriggered\0xozViewTriggered\0"
    "yozViewTriggered\0xyzViewTriggered\0"
    "meshCheckBoxTriggered\0contourCheckBoxTriggered\0"
    "shadeCheckBoxTriggered\0edgeCheckBoxTriggered\0"
    "transparancyCheckBoxTriggered\0"
    "lightingCheckBoxTriggered\0setColorBar\0"
    "selectBoundaryButtonTriggeded\0"
    "showBoundaryActor\0deleteSlice\0"
    "cutplaneIndex\0isoSurfaceCheckBoxTriggered\0"
    "isoSurfaceSettingButtonTriggered\0"
    "isoSurfaceChangeFlowNumber\0"
    "isoSurfaceValueChanged\0changeFloodNumber\0"
    "slicesCheckBoxTriggered\0changeCutplane\0"
    "double*\0makeNewCutplane\0origin\0normal\0"
    "updateCutplaneColorMapping\0minValue\0"
    "maxValue\0numberOfColors\0isBanded\0"
    "showCutplane\0ConstHeightButtonTriggered\0"
    "AddConstHeightPlane\0height\0"
    "ChangeConstHeightFlow\0flow\0"
    "AddBladeToBladePlane\0span\0"
    "ChangeBladeToBladePlaneFlow\0"
    "onSpanSliderChanged\0value\0onCopyZoneChanged\0"
    "index\0onPeriodicCopyRequested\0"
    "onProbePanelClosed\0on_Check_3Dview_toggled\0"
    "checked\0on_Check_ThreeView_toggled\0"
    "on_Check_Meri_toggled\0"
    "on_Check_BladeToBlade_toggled\0"
    "on_CBtn_BackGround_currentTextChanged\0"
    "text\0on_Btn_TurboInitial_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  259,    2, 0x08 /* Private */,
       3,    0,  260,    2, 0x08 /* Private */,
       4,    0,  261,    2, 0x08 /* Private */,
       5,    0,  262,    2, 0x08 /* Private */,
       6,    0,  263,    2, 0x08 /* Private */,
       7,    0,  264,    2, 0x08 /* Private */,
       8,    0,  265,    2, 0x08 /* Private */,
       9,    0,  266,    2, 0x08 /* Private */,
      10,    0,  267,    2, 0x08 /* Private */,
      11,    1,  268,    2, 0x08 /* Private */,
      13,    0,  271,    2, 0x08 /* Private */,
      14,    0,  272,    2, 0x08 /* Private */,
      15,    0,  273,    2, 0x08 /* Private */,
      16,    0,  274,    2, 0x08 /* Private */,
      17,    0,  275,    2, 0x08 /* Private */,
      18,    0,  276,    2, 0x08 /* Private */,
      19,    0,  277,    2, 0x08 /* Private */,
      20,    0,  278,    2, 0x08 /* Private */,
      21,    0,  279,    2, 0x08 /* Private */,
      22,    0,  280,    2, 0x08 /* Private */,
      23,    6,  281,    2, 0x08 /* Private */,
      24,    0,  294,    2, 0x08 /* Private */,
      25,    3,  295,    2, 0x08 /* Private */,
      26,    1,  302,    2, 0x08 /* Private */,
      28,    0,  305,    2, 0x08 /* Private */,
      29,    0,  306,    2, 0x08 /* Private */,
      30,    1,  307,    2, 0x08 /* Private */,
      31,    1,  310,    2, 0x08 /* Private */,
      32,    1,  313,    2, 0x08 /* Private */,
      33,    0,  316,    2, 0x08 /* Private */,
      34,    3,  317,    2, 0x08 /* Private */,
      36,    2,  324,    2, 0x08 /* Private */,
      39,    4,  329,    2, 0x08 /* Private */,
      44,    2,  338,    2, 0x08 /* Private */,
      45,    0,  343,    2, 0x08 /* Private */,
      46,    1,  344,    2, 0x08 /* Private */,
      48,    1,  347,    2, 0x08 /* Private */,
      50,    1,  350,    2, 0x08 /* Private */,
      52,    1,  353,    2, 0x08 /* Private */,
      53,    1,  356,    2, 0x08 /* Private */,
      55,    1,  359,    2, 0x08 /* Private */,
      57,    0,  362,    2, 0x08 /* Private */,
      58,    0,  363,    2, 0x08 /* Private */,
      59,    1,  364,    2, 0x08 /* Private */,
      61,    1,  367,    2, 0x08 /* Private */,
      62,    1,  370,    2, 0x08 /* Private */,
      63,    1,  373,    2, 0x08 /* Private */,
      64,    1,  376,    2, 0x08 /* Private */,
      66,    0,  379,    2, 0x08 /* Private */,

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
    QMetaType::Void, QMetaType::Bool,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int, QMetaType::Int, QMetaType::Double, QMetaType::Double,    2,    2,    2,    2,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Bool,    2,    2,    2,
    QMetaType::Void, QMetaType::Int,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Double,    2,
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 35, 0x80000000 | 35, QMetaType::Int,    2,    2,    2,
    QMetaType::Void, 0x80000000 | 35, 0x80000000 | 35,   37,   38,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Int, QMetaType::Bool,   40,   41,   42,   43,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    2,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   47,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Void, QMetaType::Double,   51,
    QMetaType::Void, QMetaType::Int,   49,
    QMetaType::Void, QMetaType::Int,   54,
    QMetaType::Void, QMetaType::Int,   56,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   60,
    QMetaType::Void, QMetaType::Bool,   60,
    QMetaType::Void, QMetaType::Bool,   60,
    QMetaType::Void, QMetaType::Bool,   60,
    QMetaType::Void, QMetaType::QString,   65,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionLoadMesh_triggered(); break;
        case 1: _t->on_actionUpdateFlow_triggered(); break;
        case 2: _t->on_actionNewWindow_triggered(); break;
        case 3: _t->on_actionOpenFile_triggered(); break;
        case 4: _t->on_actionAddScalarBarActor_triggered(); break;
        case 5: _t->on_actionAddAxisActor_triggered(); break;
        case 6: _t->on_actionAddOutlineActor_triggered(); break;
        case 7: _t->on_actionAddPointInformation_triggered(); break;
        case 8: _t->on_actionExport_Picture_triggered(); break;
        case 9: _t->on_ConstHeightCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->xoyViewTriggered(); break;
        case 11: _t->xozViewTriggered(); break;
        case 12: _t->yozViewTriggered(); break;
        case 13: _t->xyzViewTriggered(); break;
        case 14: _t->meshCheckBoxTriggered(); break;
        case 15: _t->contourCheckBoxTriggered(); break;
        case 16: _t->shadeCheckBoxTriggered(); break;
        case 17: _t->edgeCheckBoxTriggered(); break;
        case 18: _t->transparancyCheckBoxTriggered(); break;
        case 19: _t->lightingCheckBoxTriggered(); break;
        case 20: _t->setColorBar((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6]))); break;
        case 21: _t->selectBoundaryButtonTriggeded(); break;
        case 22: _t->showBoundaryActor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 23: _t->deleteSlice((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->isoSurfaceCheckBoxTriggered(); break;
        case 25: _t->isoSurfaceSettingButtonTriggered(); break;
        case 26: _t->isoSurfaceChangeFlowNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->isoSurfaceValueChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 28: _t->changeFloodNumber((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->slicesCheckBoxTriggered(); break;
        case 30: _t->changeCutplane((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 31: _t->makeNewCutplane((*reinterpret_cast< double*(*)>(_a[1])),(*reinterpret_cast< double*(*)>(_a[2]))); break;
        case 32: _t->updateCutplaneColorMapping((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 33: _t->showCutplane((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 34: _t->ConstHeightButtonTriggered(); break;
        case 35: _t->AddConstHeightPlane((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 36: _t->ChangeConstHeightFlow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->AddBladeToBladePlane((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 38: _t->ChangeBladeToBladePlaneFlow((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->onSpanSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->onCopyZoneChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->onPeriodicCopyRequested(); break;
        case 42: _t->onProbePanelClosed(); break;
        case 43: _t->on_Check_3Dview_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 44: _t->on_Check_ThreeView_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 45: _t->on_Check_Meri_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: _t->on_Check_BladeToBlade_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 47: _t->on_CBtn_BackGround_currentTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 48: _t->on_Btn_TurboInitial_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 49)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 49;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
