TEMPLATE = app
CONFIG  += thread warn_on release
QT -= core gui

HEADERS += \
    clamity.h \

SOURCES += \
    main.cpp \
    tools.cpp \
    info.cpp \
    alloc.cpp \
    basic.cpp \
    membasic.cpp \

LIBS += \
    -lOpenCL

QMAKE_CXXFLAGS += -DCLAMITY_DEBUG
