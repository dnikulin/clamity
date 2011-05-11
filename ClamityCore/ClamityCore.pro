TEMPLATE = lib
QT -= gui

DEFINES += CLAMITY_CORE_LIBRARY

DESTDIR = ../Release/

INCLUDEPATH +=                  \
    /opt/local/include             \

HEADERS +=                      \
    Clamity.hh                  \
    Common.hh                   \
    Logger.hh                   \
    TestSuite.hh                \
    MemoryTools.hh \
    ReportService.hh

SOURCES +=                      \
    Info.cc                     \
    Logger.cc                   \
    Tools.cc                    \
    ReportService.cc \
    Clamity.cc

LIBS +=                         \
    -lOpenCL

RESOURCES += \
    ErrorStrings.qrc
