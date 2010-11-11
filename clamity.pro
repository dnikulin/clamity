TEMPLATE = app
CONFIG  += thread warn_on release
QT -= core gui

HEADERS += \
    clamity.h

SOURCES += \
    main.cpp

LIBS += \
    -lOpenCL
