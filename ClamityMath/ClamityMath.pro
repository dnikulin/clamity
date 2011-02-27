TEMPLATE = lib
CONFIG += plugin
QT -= gui

DESTDIR = ../Release/Plugins/

DEPENDPATH +=                   \
    ../ClamityCore/             \

INCLUDEPATH +=                  \
    ../ClamityCore/             \

LIBS +=                         \
    -L../Release/               \
    -lClamityCore               \

HEADERS +=                      \
    ClamityMath.hh              \

SOURCES +=                      \
    Arithmetic.cc               \
    Basic.cc                    \
    ClamityMath.cc              \
