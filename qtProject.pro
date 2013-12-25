#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T15:16:43
#
#-------------------------------------------------

QT       += core gui
CFLAGS  = -std=c++0x
QMAKE_CXXFLAGS += -std=c++11

DESTDIR = build
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
RCC_DIR = build

TARGET = run
TEMPLATE = app


SOURCES +=\
    src/AppGUI.cpp \
    src/models/Person.cpp \
    src/models/Mail.cpp \
    src/models/Headers.cpp \
    src/models/Email.cpp \
    src/models/Graph.cpp \
    src/views/settingswindow.cpp \
    src/views/mainwindow.cpp \
    src/views/loadfilewindow.cpp \
    src/views/helpwindow.cpp \
    src/utils/GenericException.cpp \
    src/views/graphspace.cpp \
    src/controllers/parser.cpp




HEADERS  += \
    src/models/Containers.hpp \
    src/utils/Exceptions.hpp \
    src/views/settingswindow.h \
    src/views/mainwindow.h \
    src/views/loadfilewindow.h \
    src/views/helpwindow.h \
    src/models/Graph.h \
    src/views/graphspace.h




FORMS    += \
    src/views/settingswindow.ui \
    src/views/mainwindow.ui \
    src/views/loadfilewindow.ui \
    src/views/helpwindow.ui



OTHER_FILES +=

RESOURCES += \
    src/assets/images.qrc




