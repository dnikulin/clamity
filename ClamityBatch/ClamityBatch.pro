TEMPLATE = app
QT += core
QT -= gui
CONFIG  += thread warn_on release

DESTDIR = ../Release/

DEPENDPATH +=                   \
    ../ClamityCore/             \
    ../ClamityMath/             \
    ../ClamityMemory/           \

INCLUDEPATH +=                  \
    ../ClamityCore/             \

LIBS +=                         \
    -L../Release/               \
    -L../Release/Plugins/       \
    -lClamityCore               \
    -lClamityMath               \
    -lClamityMemory             \

SOURCES +=                      \
    Main.cc                     \
