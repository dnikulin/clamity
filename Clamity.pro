TEMPLATE = app
CONFIG  += thread warn_on release
QT -= core gui

HEADERS += \
    Clamity.hh \
    TestPipe.hh \
    TestTemplate.hh \
    Logger.hh

SOURCES += \
    Main.cc \
    Tools.cc \
    Info.cc \
    Alloc.cc \
    Basic.cc \
    MemBasic.cc \
    MemAnd.cc \
    Arithmetic.cc \
    TestPipe.cc \
    Logger.cc

LIBS += \
    -lOpenCL

QMAKE_CXXFLAGS += -DCLAMITY_DEBUG
