TEMPLATE = lib
CONFIG  += plugin
QT -= gui

DESTDIR = ../Release/Plugins/

DEPENDPATH +=                   \
    ../ClamityCore/             \

INCLUDEPATH +=                  \
    ../ClamityCore/             \

LIBS +=                         \
    -L../Release/               \
    -lClamityCore               \

HEADERS +=                      \
    ClamityMemory.hh            \

SOURCES +=                      \
    Alloc.cc                    \
    ClamityMemory.cc            \
    MemAnd.cc                   \
    MemBasic.cc                 \
