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
    -lboost_program_options

SOURCES +=                      \
    Main.cc                     \
