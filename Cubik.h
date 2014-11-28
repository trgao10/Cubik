#ifndef __CUBIK_H__
#define __CUBIK_H__

/**************************************************************/
/*********** define basic viewer in this header file **********/
/**************************************************************/
#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include "cube.h"

class Cubik {
public:
    Cubik();
    ~Cubik();
    void draw(bool names=false);

    Cube * getCenterCube() {
        return centerCube;
    }
    Cube * faceCenterCube(unsigned short i) {
        return faceCenterCubes[i];
    }
    Cube * faceCenterCube(std::string c) {
        if (c == "U")
            return faceCenterCubes[0];
        else if (c == "D")
            return faceCenterCubes[1];
        else if (c == "F")
            return faceCenterCubes[2];
        else if (c == "B")
            return faceCenterCubes[3];
        else if (c == "L")
            return faceCenterCubes[4];
        else if (c == "R")
            return faceCenterCubes[5];
        else
            return NULL;
    }
    Cube * edgeCube(unsigned short i) {
        return edgeCubes[i];
    }
    qglviewer::Vec faceNormal(unsigned short i) {
        return faceNormals[i];
    }
    std::string faceColor(unsigned short c) {
        return faceColors[c];
    }
    void setSelectedFrameNumber(unsigned short nb) {
        selected = nb;
    }
private:
    Cube * centerCube;
    Cube * faceCenterCubes[NumFaces];
    Cube * edgeCubes[NumEdges];
    
    unsigned short selected;
    
    qglviewer::Vec faceNormals[NumFaces]; // U, D, F, B, L, R
    const std::vector<std::string> faceColors = {"U", "D", "F", "B", "L", "R"}; // U, D, F, B, L, R
    
    void drawCenter();
    // void drawFace(unsigned short i);
    void drawFaceCenter(unsigned short i);
};

class Viewer : public QGLViewer {
protected :
    virtual void init();
    virtual void draw();
    void initSpotLight();
    
    virtual void drawWithNames();
    virtual void postSelection(const QPoint& point);
    
    virtual void mouseReleaseEvent(QMouseEvent * e);

private:
  Cubik cubik;
};

#endif
