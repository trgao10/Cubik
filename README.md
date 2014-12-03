Cubik
=====

[*Cubik*](https://github.com/trgao10/Cubik): a visualization project for Rubik's Cube

![Cubik Screenshot](/images/Cubik.png)

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

#### Dependency
+ **Qt 5**: Cubik is built upon the Qt framework. You need at least Qt 5.2.1 to compile it. If your default Ubuntu repository does not contain Qt5, check out this [link](http://askubuntu.com/questions/279421/how-can-i-install-qt-5-x-on-12-04-lts).
+ **GLU**: Starting from Qt 4.8, the GLU dependecy has been removed from standard Qt versions. Since the library we use still depends on GLU and GL, you may have to install these OpenGL pacakges if they are not already installed. Generally this can be done by (the second apt-get is necessary only if "-lGL is missing" after attempting to compile Cubik after executing the first apt-get; see [source link](http://qt-project.org/forums/viewthread/23855/#110514))
```
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev -y
```
+ **libQGLViewer**: Cubik replies heavily on the library [libQGLViewer](http://www.libqglviewer.com/), which is a C++ library based on Qt that eases the creation of OpenGL 3D viewers. We have already included the header files and compiled libraries (under Ubuntu 14.04 LTS) to Cubik. As long as you have Qt and GLU installed, you should be fine.

#### Compilation
This compilation procedure has been tested on multiple independent machines running Ubuntu 14.04 LTS (Trusty Tahr).

Follow these steps to copy this repository to your local directory and build:
```
git clone https://github.com/trgao10/Cubik
cd Cubik/
qmake
make
```
After the compilation, you need to specify the path to load shared libarary "libQGLViewer.so":
```
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH
```
That's all. Happy Cubik-ing!
```
./Cubik
```

## Notation and Algorithm for Solving Rubik's Cube

The solution integrated into this version of Cubik is based on [Mark Jeay's algorithm](http://www.jeays.net/rubiks.htm). This algorithm is classic for beginner-level users since it is solved ituitively layer by layer. However, it is definetely not a fast algorithm which involves more steps than some other solutions.

The notation is adapted from an earlier [Rubik's Cube Contest](http://tomas.rokicki.com/cubecontest/). The 6 faces of the cube are denoted as U (top/yellow), D(bottom/white), F(front/blue), B(back/green), L(left/orange), and R(right/red), respectively. Each solution step provided in the hint mode indicates one of the following three operations on the corresponding face:

1. 90-degree turn clockwise: U, D, F, B, L, R;

2. 180-degree turn clockwise: U2, D2, F2, B2, L2, R2;

3. 90-degree turn counter-clockwise: U', D', F', B', L', R'.

Note that clockwise and counter-clockwise turns are viewed with respect to the center of the cube.

The solution algorithm consists of 7 phases in general:

1. solve *up layer edge pieces*;

2. solve *up layer corner pieces*;

3. solve *middle layer*;

4. put *down layer edge pieces* in correct positions;

5. put *down layer edge pieces* in correct directions;

6. put *down layer corner pieces* in correct positions;

7. put *down layer corner pieces* in correct directions.

The hint mode specifies which phase of the algorithm the user is currently at.

## GNU GPL License
This code is released under the terms of the GNU-GPL license Version 3. Any software you create that uses any part of this source code (or its compiled version) will have to be open sourced under the same conditions.

If you would like to use the libQGLViewer library in your applications, you should consult its [license page](http://www.libqglviewer.com/download.html).

## Contributors

Tingran Gao (<trgao10@math.duke.edu>)

Wenyuan Jiao (<wyjiaonju@gmail.com>)

Jincheng Li (<lijincheng8801@gmail.com>)

## Acknowledgement

The contributors thank Shiwen Zhao (<shiwen.zhao@duke.edu>) for compilation testing this project. The first contributor also thanks Gilles Debunne (<contact@libqglviewer.com>) for many useful discussions.

---------------------------------:
Duke University, Dec 2014
