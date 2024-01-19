/****************************************************************************
** Meta object code from reading C++ file 'svgview.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.6.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "svgview.h"
#include <QtCore/qmetatype.h>

#if __has_include(<QtCore/qtmochelpers.h>)
#include <QtCore/qtmochelpers.h>
#else
QT_BEGIN_MOC_NAMESPACE
#endif


#include <memory>

#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'svgview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.6.0. It"
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
struct qt_meta_stringdata_CLASSSvgViewENDCLASS_t {};
static constexpr auto qt_meta_stringdata_CLASSSvgViewENDCLASS = QtMocHelpers::stringData(
    "SvgView",
    "zoomChanged",
    "",
    "setAntialiasing",
    "antialiasing",
    "setViewBackground",
    "enable",
    "setViewOutline",
    "zoomIn",
    "zoomOut",
    "resetZoom"
);
#else  // !QT_MOC_HAS_STRING_DATA
struct qt_meta_stringdata_CLASSSvgViewENDCLASS_t {
    uint offsetsAndSizes[22];
    char stringdata0[8];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[16];
    char stringdata4[13];
    char stringdata5[18];
    char stringdata6[7];
    char stringdata7[15];
    char stringdata8[7];
    char stringdata9[8];
    char stringdata10[10];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_CLASSSvgViewENDCLASS_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_CLASSSvgViewENDCLASS_t qt_meta_stringdata_CLASSSvgViewENDCLASS = {
    {
        QT_MOC_LITERAL(0, 7),  // "SvgView"
        QT_MOC_LITERAL(8, 11),  // "zoomChanged"
        QT_MOC_LITERAL(20, 0),  // ""
        QT_MOC_LITERAL(21, 15),  // "setAntialiasing"
        QT_MOC_LITERAL(37, 12),  // "antialiasing"
        QT_MOC_LITERAL(50, 17),  // "setViewBackground"
        QT_MOC_LITERAL(68, 6),  // "enable"
        QT_MOC_LITERAL(75, 14),  // "setViewOutline"
        QT_MOC_LITERAL(90, 6),  // "zoomIn"
        QT_MOC_LITERAL(97, 7),  // "zoomOut"
        QT_MOC_LITERAL(105, 9)   // "resetZoom"
    },
    "SvgView",
    "zoomChanged",
    "",
    "setAntialiasing",
    "antialiasing",
    "setViewBackground",
    "enable",
    "setViewOutline",
    "zoomIn",
    "zoomOut",
    "resetZoom"
};
#undef QT_MOC_LITERAL
#endif // !QT_MOC_HAS_STRING_DATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSSvgViewENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   56,    2, 0x06,    1 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       3,    1,   57,    2, 0x0a,    2 /* Public */,
       5,    1,   60,    2, 0x0a,    4 /* Public */,
       7,    1,   63,    2, 0x0a,    6 /* Public */,
       8,    0,   66,    2, 0x0a,    8 /* Public */,
       9,    0,   67,    2, 0x0a,    9 /* Public */,
      10,    0,   68,    2, 0x0a,   10 /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject SvgView::staticMetaObject = { {
    QMetaObject::SuperData::link<QGraphicsView::staticMetaObject>(),
    qt_meta_stringdata_CLASSSvgViewENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSSvgViewENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSSvgViewENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<SvgView, std::true_type>,
        // method 'zoomChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setAntialiasing'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setViewBackground'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'setViewOutline'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'zoomIn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zoomOut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resetZoom'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void SvgView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SvgView *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->zoomChanged(); break;
        case 1: _t->setAntialiasing((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 2: _t->setViewBackground((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->setViewOutline((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->zoomIn(); break;
        case 5: _t->zoomOut(); break;
        case 6: _t->resetZoom(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SvgView::*)();
            if (_t _q_method = &SvgView::zoomChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject *SvgView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SvgView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSSvgViewENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QGraphicsView::qt_metacast(_clname);
}

int SvgView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void SvgView::zoomChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
