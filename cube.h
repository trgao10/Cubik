#ifndef __CUBE_H__
#define __CUBE_H__

/**************************************************************/
/************ define basic cube in this header file ***********/
/**************************************************************/
#include <QGLViewer/qglviewer.h>
#include <QGLViewer/frame.h>
#include <QGLViewer/manipulatedFrame.h>
#include <string>

#define NumFaces 6
#define NumEdges 12

// overload manipulatedFrame to automatically align frames
class CubeFrame : public qglviewer::ManipulatedFrame {
public:
    bool checkAlignedWithFrame (const CubeFrame * const frame);
};

class Cube {
private: 
    std::string CubeType;
    CubeFrame * cubeFrame;
public:
    Cube();
    Cube(std::string cubetype);
    Cube(std::string cubetype, CubeFrame * cubeFrame_);
    void draw();
    std::string getCubeType() { return CubeType; }
    CubeFrame * getCubeFrame() { return cubeFrame; }
};

#endif
