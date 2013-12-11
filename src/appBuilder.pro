#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T15:16:43
#
#-------------------------------------------------

QT       += core gui
CFLAGS  = -std=c++0x
QMAKE_CXXFLAGS += -std=c++11


TARGET = appBuilder
TEMPLATE = app


SOURCES +=\
    AppGUI.cpp \
    models/Person.cpp \
    models/Mail.cpp \
    models/Headers.cpp \
    models/Email.cpp \
    views/settingswindow.cpp \
    views/mainwindow.cpp \
    views/loadfilewindow.cpp \
    views/helpwindow.cpp \
    utils/GenericException.cpp \
    models/Graph.cpp \
    views/graphspace.cpp \
    views/GraphTest.cpp




HEADERS  += \
    models/Containers.hpp \
    utils/Exceptions.hpp \
    views/settingswindow.h \
    views/mainwindow.h \
    views/loadfilewindow.h \
    views/helpwindow.h \
    models/Graph.h \
    views/graphspace.h




FORMS    += \
    views/settingswindow.ui \
    views/mainwindow.ui \
    views/loadfilewindow.ui \
    views/helpwindow.ui



OTHER_FILES +=

RESOURCES += \
    assets/images.qrc




