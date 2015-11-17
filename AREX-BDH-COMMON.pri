#Includes common configuration for all subdirectory .pro files.
INCLUDEPATH += ../../ .. .
WARNINGS += -Wall

QT += core
QT += network

TEMPLATE = app

# The following keeps the generated files at least somewhat separate 
# from the source files.

UI_DIR = UI
MOC_DIR = MOC
OBJECTS_DIR = OBJ
DESTDIR = BIN

QMAKE_CXXFLAGS += -std=c++11

LIBS += -lmsgpack

OTHER_FILES += \
    ../../Protocol description/protocol
    
 

HEADERS += \
    $$PWD/Infrastructure/AHP/historianalarmrequest.h \
    $$PWD/Infrastructure/AHP/historianalarmresponse.h

SOURCES += \
    $$PWD/Infrastructure/AHP/historianalarmrequest.cc \
    $$PWD/Infrastructure/AHP/historianalarmresponse.cc

FORMS +=
