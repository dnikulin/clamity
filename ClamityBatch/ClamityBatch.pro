CONFIG   += console
CONFIG   -= app_bundle
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
    -L/opt/local/lib            \
    -lboost_program_options     \
    -lboost_system    

SOURCES +=                      \
    Main.cc                     \
