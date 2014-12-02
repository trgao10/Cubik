Cubik
=====

Cubik: a visualization project for Rubik's Cube

## Basic Operations

#### View Mode: view cube with mouse
+ press down and hold left button for roration
+ press down and hold right button for translation
+ forward/backward rotate middle wheel for zoom in/out
+ press F to toggle on/off the face color indicator on bottom left corner
+ press Ctrl+Q to exit

#### Interactive Mode: select and rotate faces
+ double click on a face to activate it, then rotate
+ double click at any point not on the cube to deactivate face selection and back to view mode
+ when a face is activated, Ctrl+(left|right|wheel) mouse buttons enables temporary view mode without exiting the interactive mode

#### Hint Mode: guide to a solution
+ press H to toggle hint mode (next move and number of moves left shown on top left)

## Dependency and Compilation
+ **Qt 5**: Cubik is built upon the Qt framework. You need at least Qt 5.2.1 to compile it. If your default Ubuntu repository does not contain Qt5, check out this [link](http://askubuntu.com/questions/279421/how-can-i-install-qt-5-x-on-12-04-lts).
+ **GLU**: Starting from Qt 4.8, the GLU dependecy has been removed from standard Qt versions. Since the library we use still depends on GLU and GL, you may have to install these OpenGL pacakges if they are not already installed. Generally this can be down by (the second apt-get is necessary only if "-lGL is missing" after attempting to compile Cubik after executing the first apt-get; see [source link](http://qt-project.org/forums/viewthread/23855/#110514))
```
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev -y
```
+ **QGLViewer**: Cubik replies heavily on the library [QGLViewer](http://www.libqglviewer.com/), which is a C++ library based on Qt that eases the creation of OpenGL 3D viewers. We have already included the header files and compiled libraries (under Ubuntu 14.04 LTS) to Cubik; in case the compilation still doesn't run through, simply apt-get this library by
```
sudo apt-get install libqglviewer-dev
libqglviewer2
```

## Notation and Algorithm for Solving Rubik's Cube
The solution integrated into this version of Cubik is based on [Mark Jeay's algorithm](http://www.jeays.net/rubiks.htm).

The notation is adapted from a earlier [Rubik's Cube Contest](http://tomas.rokicki.com/cubecontest/). The 6 faces of the cube are denoted as U (top), D(bottom), F(front), B(back), L(left), and R(right), respectively. Each solution step provided in the hint mode indicates one of the following three operations on the corresponding face:

1. 90-degree turn clockwise: U, D, F, B, L, R.

2. 180-degree turn clockwise: U2, D2, F2, B2, L2, R2;

3. 90-degree turn counter-clockwise: U', D', F', B', L', R'.

Note that clockwise and counter-clockwise turns are viewed with respect to the center of the cube.

The solution algorithm consists of 6 phases in general:

1. Up layer edge pieces solved
 
2. Up layer corner pieces solved

3. Middle layer solved

4. Down layer edge pieces solved

5. Down layer corner pieces solved

The hint mode specifies which phase of the algorithm the user is currently at.
