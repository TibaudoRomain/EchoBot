/****************************************************************************
** Meta object code from reading C++ file 'server.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../includes/server.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'server.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN6ServerE_t {};
} // unnamed namespace

template <> constexpr inline auto Server::qt_create_metaobjectdata<qt_meta_tag_ZN6ServerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Server",
        "message_received",
        "",
        "message",
        "msg",
        "newConnection",
        "howmany",
        "serverState",
        "state",
        "client_disconnected",
        "read_data",
        "new_client",
        "on_msg_clicked",
        "on_portchanged",
        "port",
        "on_results",
        "uint8_t",
        "angleA",
        "angleB",
        "angleC",
        "angleD",
        "angleE"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'message_received'
        QtMocHelpers::SignalData<void(message)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'newConnection'
        QtMocHelpers::SignalData<void(int)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 6 },
        }}),
        // Signal 'serverState'
        QtMocHelpers::SignalData<void(bool)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 8 },
        }}),
        // Slot 'client_disconnected'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'read_data'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'new_client'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_msg_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_portchanged'
        QtMocHelpers::SlotData<void(int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 14 },
        }}),
        // Slot 'on_results'
        QtMocHelpers::SlotData<void(uint8_t, uint8_t, uint8_t, uint8_t, uint8_t)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 16, 17 }, { 0x80000000 | 16, 18 }, { 0x80000000 | 16, 19 }, { 0x80000000 | 16, 20 },
            { 0x80000000 | 16, 21 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Server, qt_meta_tag_ZN6ServerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Server::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ServerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ServerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6ServerE_t>.metaTypes,
    nullptr
} };

void Server::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Server *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->message_received((*reinterpret_cast< std::add_pointer_t<message>>(_a[1]))); break;
        case 1: _t->newConnection((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->serverState((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->client_disconnected(); break;
        case 4: _t->read_data(); break;
        case 5: _t->new_client(); break;
        case 6: _t->on_msg_clicked(); break;
        case 7: _t->on_portchanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->on_results((*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<uint8_t>>(_a[5]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Server::*)(message )>(_a, &Server::message_received, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Server::*)(int )>(_a, &Server::newConnection, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Server::*)(bool )>(_a, &Server::serverState, 2))
            return;
    }
}

const QMetaObject *Server::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Server::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6ServerE_t>.strings))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int Server::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Server::message_received(message _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void Server::newConnection(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Server::serverState(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP
