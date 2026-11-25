/****************************************************************************
** Meta object code from reading C++ file 'BedsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.12.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../Class/UI/Header/BedsWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'BedsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.12.0. It"
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
struct qt_meta_tag_ZN10BedsWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto BedsWidget::qt_create_metaobjectdata<qt_meta_tag_ZN10BedsWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "BedsWidget",
        "add_bed_clicked",
        "",
        "remove_bed_clicked",
        "plant_clicked",
        "collect_clicked",
        "water_clicked",
        "fertilize_clicked",
        "toggle_light_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'add_bed_clicked'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'remove_bed_clicked'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'plant_clicked'
        QtMocHelpers::SignalData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'collect_clicked'
        QtMocHelpers::SignalData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'water_clicked'
        QtMocHelpers::SignalData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fertilize_clicked'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'toggle_light_clicked'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<BedsWidget, qt_meta_tag_ZN10BedsWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject BedsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BedsWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BedsWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10BedsWidgetE_t>.metaTypes,
    nullptr
} };

void BedsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<BedsWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->add_bed_clicked(); break;
        case 1: _t->remove_bed_clicked(); break;
        case 2: _t->plant_clicked(); break;
        case 3: _t->collect_clicked(); break;
        case 4: _t->water_clicked(); break;
        case 5: _t->fertilize_clicked(); break;
        case 6: _t->toggle_light_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::add_bed_clicked, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::remove_bed_clicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::plant_clicked, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::collect_clicked, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::water_clicked, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::fertilize_clicked, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (BedsWidget::*)()>(_a, &BedsWidget::toggle_light_clicked, 6))
            return;
    }
}

const QMetaObject *BedsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BedsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10BedsWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BedsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void BedsWidget::add_bed_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BedsWidget::remove_bed_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BedsWidget::plant_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BedsWidget::collect_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BedsWidget::water_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void BedsWidget::fertilize_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void BedsWidget::toggle_light_clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}
QT_WARNING_POP
