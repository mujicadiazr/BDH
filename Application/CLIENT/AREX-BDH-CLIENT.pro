#-------------------------------------------------
#
# Project created by QtCreator 2015-03-30T00:53:31
#
#-------------------------------------------------

# Check if the common pri file exists
! include(../../AREX-BDH-COMMON.pri) {
    error( "Couldn't find the common.pri file!" )
}

QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AREX-BDH-CLIENT


SOURCES += main.cpp \
    ../../Domain/alarm.cpp \
    ../../Domain/alarmpoint.cpp \
    ../../Domain/configuration.cpp \
    ../../Domain/variable.cpp \
    ../../Domain/variablepoint.cpp \
    ../../Infrastructure/AHP/ahpclient.cpp \
    ../../Infrastructure/AHP/ahpframe.cpp \
    ../../Infrastructure/AHP/ahpframefactory.cpp \
    ../../Infrastructure/AHP/historianvariablerequest.cpp \
    ../../Infrastructure/AHP/historianvariableresponse.cpp \
    ../../Infrastructure/AHP/realtimevariablerequest.cpp \
    ../../Infrastructure/AHP/realtimevariableresponse.cpp \
    ../../Infrastructure/AHP/variablerequest.cpp \
    ../../Infrastructure/iserializable.cpp \
    ../../Infrastructure/serializer.cpp \
    ../../Infrastructure/AHP/ahpchannel.cpp \
    ../../Presentation/clientview.cpp \
    ../../Presentation/historicplot.cpp \
    ../../Infrastructure/AHP/variablesconfigured.cpp \
    ../../Presentation/realtimeplot.cpp \
    ../../Presentation/qabstractplotter.cpp \
    ../../Infrastructure/clientworker.cpp \
    ../../Presentation/connectdialog.cpp \
    ../../Infrastructure/AHP/mt.cc


HEADERS  += \
    ../../Common/globals.h \
    ../../Domain/alarm.h \
    ../../Domain/alarm_states_enum.h \
    ../../Domain/alarmpoint.h \
    ../../Domain/configuration.h \
    ../../Domain/integervariablepoint.h \
    ../../Domain/value_datatype_enum.h \
    ../../Domain/variable.h \
    ../../Domain/variablepoint.h \
    ../../Infrastructure/AHP/ahp_commands_enum.h \
    ../../Infrastructure/AHP/ahpclient.h \
    ../../Infrastructure/AHP/ahpframe.h \
    ../../Infrastructure/AHP/ahpframefactory.h \
    ../../Infrastructure/AHP/historianvariablerequest.h \
    ../../Infrastructure/AHP/historianvariableresponse.h \
    ../../Infrastructure/AHP/realtimevariablerequest.h \
    ../../Infrastructure/AHP/realtimevariableresponse.h \
    ../../Infrastructure/AHP/variablerequest.h \
    ../../Infrastructure/iserializable.h \
    ../../Infrastructure/serializer.h \
    ../../Infrastructure/AHP/ahpchannel.h \
    ../../Presentation/clientview.h \
    ../../Presentation/historicplot.h \
    ../../Infrastructure/AHP/variablesconfigured.h \
    ../../Presentation/realtimeplot.h \
    ../../Presentation/qabstractplotter.h \
    ../../Infrastructure/clientworker.h \
    ../../Presentation/connectdialog.h \
    ../../Infrastructure/AHP/mt.h

FORMS    += \
    ../../Presentation/clientview.ui \
    ../../Presentation/connectdialog.ui


CONFIG += qwt


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/qwt-6.1.2/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../usr/local/qwt-6.1.2/lib/debug/ -lqwt
else:unix: LIBS += -L$$PWD/../../../../../../../usr/local/qwt-6.1.2/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../../usr/local/qwt-6.1.2/include
DEPENDPATH += $$PWD/../../../../../../../usr/local/qwt-6.1.2/include

RESOURCES += \
    resources.qrc
