#-------------------------------------------------
#
# Project created by QtCreator 2015-03-03T16:01:42
#
#-------------------------------------------------

!include(../../AREX-BDH-COMMON.pri) {
    error( "Couldn't find the common.pri file!" )
}

QT       += sql concurrent xml


TARGET = AREX-BDH-SERVER

SOURCES += main.cpp \
    ../../Domain/alarm.cpp \
    ../../Domain/alarmpoint.cpp \
    ../../Domain/configuration.cpp \
    ../../Domain/variable.cpp \
    ../../Domain/variablepoint.cpp \
    ../../Infrastructure/AHP/ahpframe.cpp \
    ../../Infrastructure/AHP/ahpframefactory.cpp \
    ../../Infrastructure/AHP/ahpserver.cpp \
    ../../Infrastructure/AHP/connection.cpp \
    ../../Infrastructure/AHP/historianvariablerequest.cpp \
    ../../Infrastructure/AHP/historianvariableresponse.cpp \
    ../../Infrastructure/AHP/realtimevariablerequest.cpp \
    ../../Infrastructure/AHP/realtimevariableresponse.cpp \
    ../../Infrastructure/AHP/variablerequest.cpp \
    ../../Infrastructure/bdhservercontroller.cpp \
    ../../Infrastructure/dataacquisition.cpp \
    ../../Infrastructure/iserializable.cpp \
    ../../Infrastructure/realtimecache.cpp \
    ../../Infrastructure/serializer.cpp \
    ../../Infrastructure/sqlitedatabase.cpp \
    ../../Test/sqlitedatabasetest.cpp \
    ../../Infrastructure/AHP/ahpchannel.cpp \
    ../../Infrastructure/AHP/variablesconfigured.cpp \
    ../../Infrastructure/tcpproxy.cpp \
    ../../Infrastructure/xmlloader.cpp \
    ../../Infrastructure/globalfunctions.cpp

HEADERS += \
    ../../Common/globals.h \
    ../../Domain/alarm.h \
    ../../Domain/alarm_states_enum.h \
    ../../Domain/alarmpoint.h \
    ../../Domain/configuration.h \
    ../../Domain/value_datatype_enum.h \
    ../../Domain/variable.h \
    ../../Domain/variablepoint.h \
    ../../Infrastructure/AHP/ahp_commands_enum.h \
    ../../Infrastructure/AHP/ahp_states_enum.h \
    ../../Infrastructure/AHP/ahpframe.h \
    ../../Infrastructure/AHP/ahpframefactory.h \
    ../../Infrastructure/AHP/ahpserver.h \
    ../../Infrastructure/AHP/connection.h \
    ../../Infrastructure/AHP/historianvariablerequest.h \
    ../../Infrastructure/AHP/historianvariableresponse.h \
    ../../Infrastructure/AHP/realtimevariablerequest.h \
    ../../Infrastructure/AHP/realtimevariableresponse.h \
    ../../Infrastructure/AHP/variablerequest.h \
    ../../Infrastructure/bdhservercontroller.h \
    ../../Infrastructure/dataacquisition.h \
    ../../Infrastructure/iserializable.h \
    ../../Infrastructure/realtimecache.h \
    ../../Infrastructure/serializer.h \
    ../../Infrastructure/sqlitedatabase.h \
    ../../Test/sqlitedatabasetest.h \
    ../../Infrastructure/AHP/ahpchannel.h \
    ../../Infrastructure/AHP/variablesconfigured.h \
    ../../Infrastructure/tcpproxy.h \
    ../../Infrastructure/xmlloader.h \
    ../../Infrastructure/globalfunctions.h


