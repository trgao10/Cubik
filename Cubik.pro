TEMPLATE = app
TARGET   = Cubik

HEADERS  = Cubik.h cube.h
SOURCES  = Cubik.cpp cube.cpp main.cpp

QT *= xml opengl widgets gui

CONFIG += qt opengl warn_on thread rtti console c++11

INCLUDEPATH *= ./include
LIBS *= -L./lib/QGLViewer/ -lQGLViewer


