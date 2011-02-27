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
    ClamityMemory.cl            \

SOURCES +=                      \
    Alloc.cc                    \
    ClamityMemory.cc            \
    MemAnd.cc                   \
    MemBasic.cc                 \

RESOURCES +=                    \
    ClamityMemory.qrc           \
