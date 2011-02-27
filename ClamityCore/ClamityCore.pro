TEMPLATE = lib
QT += core
QT -= gui
CONFIG  += thread warn_on release
DEFINES += CLAMITY_CORE_LIBRARY

DESTDIR = ../Release/

HEADERS +=                      \
    Clamity.hh                  \
    Common.hh                   \
    Logger.hh                   \
    TestSuite.hh                \

SOURCES +=                      \
    Info.cc                     \
    Logger.cc                   \
    Tools.cc                    \

LIBS +=                         \
    -lOpenCL                    \
