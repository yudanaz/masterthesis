/****************************************************************************
** Meta object code from reading C++ file 'rgb_nir_depth_capture.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../RGB_NIR_Depth_capture/rgb_nir_depth_capture.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rgb_nir_depth_capture.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RGB_NIR_Depth_Capture_t {
    QByteArrayData data[10];
    char stringdata[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RGB_NIR_Depth_Capture_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RGB_NIR_Depth_Capture_t qt_meta_stringdata_RGB_NIR_Depth_Capture = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 19),
QT_MOC_LITERAL(2, 42, 0),
QT_MOC_LITERAL(3, 43, 11),
QT_MOC_LITERAL(4, 55, 11),
QT_MOC_LITERAL(5, 67, 6),
QT_MOC_LITERAL(6, 74, 32),
QT_MOC_LITERAL(7, 107, 24),
QT_MOC_LITERAL(8, 132, 31),
QT_MOC_LITERAL(9, 164, 35)
    },
    "RGB_NIR_Depth_Capture\0startImgAcquisition\0"
    "\0imagesReady\0RGBDNIR_MAP\0images\0"
    "on_btn_startAcquisition_released\0"
    "on_btn_saveImgs_released\0"
    "on_btn_stopAcquisition_released\0"
    "on_checkBox_showAllChannels_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RGB_NIR_Depth_Capture[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   45,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,
       8,    0,   50,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RGB_NIR_Depth_Capture::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RGB_NIR_Depth_Capture *_t = static_cast<RGB_NIR_Depth_Capture *>(_o);
        switch (_id) {
        case 0: _t->startImgAcquisition(); break;
        case 1: _t->imagesReady((*reinterpret_cast< RGBDNIR_MAP(*)>(_a[1]))); break;
        case 2: _t->on_btn_startAcquisition_released(); break;
        case 3: _t->on_btn_saveImgs_released(); break;
        case 4: _t->on_btn_stopAcquisition_released(); break;
        case 5: _t->on_checkBox_showAllChannels_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RGBDNIR_MAP >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RGB_NIR_Depth_Capture::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RGB_NIR_Depth_Capture::startImgAcquisition)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject RGB_NIR_Depth_Capture::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_RGB_NIR_Depth_Capture.data,
      qt_meta_data_RGB_NIR_Depth_Capture,  qt_static_metacall, 0, 0}
};


const QMetaObject *RGB_NIR_Depth_Capture::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RGB_NIR_Depth_Capture::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RGB_NIR_Depth_Capture.stringdata))
        return static_cast<void*>(const_cast< RGB_NIR_Depth_Capture*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int RGB_NIR_Depth_Capture::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void RGB_NIR_Depth_Capture::startImgAcquisition()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
