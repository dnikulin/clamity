TEMPLATE = lib
QT += core
QT -= gui
CONFIG  += thread warn_on release plugin

DESTDIR = ../Release/Plugins/

DEPENDPATH +=                   \
    ../ClamityCore/             \

INCLUDEPATH +=                  \
    ../ClamityCore/             \

LIBS +=                         \
    -L../Release/               \
    -lClamityCore               \

SOURCES +=                      \
    Arithmetic.cc               \
    Basic.cc                    \
