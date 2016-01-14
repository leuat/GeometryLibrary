#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T10:08:11
#
#-------------------------------------------------

QT       -= quick widgets

TARGET = GeometryLibrary
TEMPLATE = lib

DEFINES += GEOMETRY_LIBRARY

SOURCES += geometrylibrary.cpp \
    perlin.cpp \
    noise.cpp \
    simplexnoise.cpp \
    simplex.cpp \
    parameters.cpp \
    noiseparameters.cpp

HEADERS += geometrylibrary.h\
        geometrylibrary_global.h \
    simplex.h \
    perlin.h \
    noise.h \
    simplexnoise.h \
    parameters.h \
    noiseparameters.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
