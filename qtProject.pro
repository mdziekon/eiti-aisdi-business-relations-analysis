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
    src/models/Date.cpp \
    src/models/Email.cpp \
    src/models/Graph.cpp \
    src/utils/GenericException.cpp \
    src/controllers/parser.cpp \
    src/views/settingswindow.cpp \
    src/views/personinfo.cpp \
    src/views/mainwindow.cpp \
    src/views/mailwindow.cpp \
    src/views/mailsinfo.cpp \
    src/views/loadfilewindow.cpp \
    src/views/graphspace2.cpp \
    src/Filter.cpp




HEADERS  += \
    src/models/Containers.hpp \
    src/utils/Exceptions.hpp \
    src/models/Graph.h \
    src/utils/sortComparators.h \
    src/utils/quicksort.h \
    src/views/settingswindow.h \
    src/views/personinfo.h \
    src/views/mainwindow.h \
    src/views/mailwindow.h \
    src/views/mailsinfo.h \
    src/views/loadfilewindow.h \
    src/views/graphspace2.h \
    src/Filter.h \
    src/controllers/parser.hpp




FORMS    += \
    src/views/settingswindow.ui \
    src/views/personinfo.ui \
    src/views/mainwindow.ui \
    src/views/mailwindow.ui \
    src/views/mailsinfo.ui \
    src/views/loadfilewindow.ui



OTHER_FILES +=

RESOURCES += \
    src/assets/images.qrc




