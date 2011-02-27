TEMPLATE = app
QT += core
QT -= gui
CONFIG  += thread warn_on release

HEADERS +=                      \
    Clamity.hh                  \
    Common.hh                   \
    Logger.hh                   \
    TestSuite.hh                \

SOURCES +=                      \
    Alloc.cc                    \
    Arithmetic.cc               \
    Basic.cc                    \
    Info.cc                     \
    Logger.cc                   \
    Main.cc                     \
    MemAnd.cc                   \
    MemBasic.cc                 \
    Tools.cc                    \

LIBS +=                         \
    -lOpenCL                    \
