TEMPLATE = app
TARGET   = Cubik

HEADERS  = Cubik.h cube.h
SOURCES  = Cubik.cpp solveCube.cpp cube.cpp main.cpp

QT *= xml opengl widgets gui

CONFIG += qt opengl warn_on thread rtti console c++11 ggdb3

QMAKE_LIBS_OPENGL *= -lGLU

INCLUDEPATH *= ./include
LIBS *= -L./lib/QGLViewer/ -lQGLViewer


