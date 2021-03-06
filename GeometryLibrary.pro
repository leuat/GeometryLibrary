#-------------------------------------------------
#
# Project created by QtCreator 2016-01-14T10:08:11
#
#-------------------------------------------------

QT       -= quick widgets

TARGET = GeometryLibrary
TEMPLATE = lib

CONFIG += c++11
#QMAKE_CXXFLAGS += -std=c++0x-Wno-c++11-extension
DEFINES += GEOMETRY_LIBRARY
#DEFINES += USE_FFTW

SOURCES += geometrylibrary.cpp \
    perlin.cpp \
    noise.cpp \
    simplexnoise.cpp \
    simplex.cpp \
    parameters.cpp \
    models/multifractalparameters.cpp \
    models/noiseparameters.cpp \
    models/regularmodel.cpp \
    models/multifractalmodel.cpp \
    model.cpp \
    likelihood/lgraph.cpp \
    misc/util.cpp \
    likelihood/likelihood.cpp

HEADERS += geometrylibrary.h\
        geometrylibrary_global.h \
    simplex.h \
    perlin.h \
    noise.h \
    simplexnoise.h \
    parameters.h \
    models/multifractalparameters.h \
    models/noiseparameters.h \
    models/regularmodel.h \
    models/multifractalmodel.h \
    model.h \
    likelihood/lgraph.h \
    likelihood/spline.h \
    misc/util.h \
    likelihood/likelihood.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
