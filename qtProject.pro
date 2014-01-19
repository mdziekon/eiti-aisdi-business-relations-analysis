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
    src/utils/MD5.cpp \
    src/controllers/parser.cpp \
    src/controllers/Filter.cpp \
    src/controllers/FiltersParser.cpp \
    src/views/settingswindow.cpp \
    src/views/personinfo.cpp \
    src/views/mainwindow.cpp \
    src/views/mailwindow.cpp \
    src/views/mailsinfo.cpp \
    src/views/loadfilewindow.cpp \
    src/views/helpwindow.cpp \
    src/views/graphspace2.cpp




HEADERS  += \
    src/models/Containers.hpp \
    src/models/Graph.h \
    src/models/Graph.h \
    src/utils/Exceptions.hpp \
    src/utils/sortComparators.h \
    src/utils/quicksort.h \
    src/utils/MD5.h \
    src/utils/sortComparators.h \
    src/utils/quicksort.h \
    src/views/settingswindow.h \
    src/views/personinfo.h \
    src/views/mainwindow.h \
    src/views/mailwindow.h \
    src/views/mailsinfo.h \
    src/views/loadfilewindow.h \
    src/views/helpwindow.h \
    src/views/graphspace2.h \
    src/controllers/Filter.h \
    src/controllers/parser.hpp \
    src/controllers/FiltersParser.hpp




FORMS    += \
    src/views/settingswindow.ui \
    src/views/personinfo.ui \
    src/views/mainwindow.ui \
    src/views/mailwindow.ui \
    src/views/mailsinfo.ui \
    src/views/loadfilewindow.ui \
    src/views/helpwindow.ui



OTHER_FILES +=

RESOURCES += \
    src/assets/images.qrc




