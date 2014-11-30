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
#define NumCorners 8

// overload manipulatedFrame to automatically align frames
class CubeFrame : public qglviewer::ManipulatedFrame {
public:
    bool checkAlignedWithFrame (const CubeFrame * const frame);
};

class Cube {
private: 
    std::string CubeType;
    CubeFrame * cubeFrame;
    
    qglviewer::Vec faceToNormal(char fId);
    int faceToFaceIdx(char fIdx);
public:
    Cube * parentCube;
    // std::vector<Cube *> childCubes;
    
    Cube();
    Cube(std::string cubetype);
    Cube(std::string cubetype, CubeFrame * cubeFrame_);
    ~Cube();
    
    void draw();
    
    bool isFaceCenterCube() { return (CubeType.size() == 1); }
    std::string getCubeType() { return CubeType; }
    CubeFrame * getCubeFrame() { return cubeFrame; }
    qglviewer::Vec getTranslationVector();
    int getFaceCenter() { return faceToFaceIdx(CubeType[0]); }
};

#endif
