TEMPLATE = app
CONFIG  += thread warn_on release
QT -= core gui

HEADERS += \
    clamity.h \
    testpipe.h \
    TestTemplate.h

SOURCES += \
    main.cpp \
    tools.cpp \
    info.cpp \
    alloc.cpp \
    basic.cpp \
    membasic.cpp \
    memand.cpp \
    arithmetictests.cpp \
    testpipe.cpp

LIBS += \
    -lOpenCL

QMAKE_CXXFLAGS += -DCLAMITY_DEBUG
