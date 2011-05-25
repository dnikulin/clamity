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
    ReportService.hh \
    ClamityConfig.hh

SOURCES +=                      \
    Info.cc                     \
    Logger.cc                   \
    Tools.cc                    \
    ReportService.cc \
    Clamity.cc \
    ClamityConfig.cc

LIBS +=                         \
    -lOpenCL                  \
    -L/opt/local/lib          \
    -lboost_program_options     \
    -lboost_system              \
    -lboost_serialization


RESOURCES += \
    ErrorStrings.qrc
