TEMPLATE = app
QT -= gui

DESTDIR = ../Release/

DEPENDPATH +=                   \
    ../ClamityCore/             \

INCLUDEPATH +=                  \
    ../ClamityCore/             \

LIBS +=                         \
    -L../Release/               \
    -lClamityCore               \

SOURCES +=                      \
    Main.cc                     \
