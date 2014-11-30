#ifndef __CUBIK_H__
#define __CUBIK_H__

/**************************************************************/
/*********** define basic viewer in this header file **********/
/**************************************************************/
#include <QGLViewer/qglviewer.h>
#include <QMouseEvent>
#include <map>
#include "cube.h"

class Cubik {
private:
    Cube * centerCube;
    Cube * faceCenterCubes[NumFaces];
    Cube * edgeCornerCubes[NumEdges+NumCorners];

    std::vector<std::string> solvedCube = {"UF", "UR", "UB", "UL", "DF", "DR", "DB", "DL", "FR", "FL", "BR", "BL", "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR"};
    int selected;
    qglviewer::Vec faceNormals[NumFaces]; // U, D, F, B, L, R
    std::map<Cube *, qglviewer::Vec> edgeCornerPosition;
public:
    Cubik();
    ~Cubik();
    void draw();

    bool isSpinning();
    Cube * getCenterCube() {
        return centerCube;
    }
    Cube * faceCenterCube(int i) {
        return faceCenterCubes[i];
    }
    Cube * faceCenterCube(std::string c) {
        return faceCenterCubes[faceToIdx[c[0]]];
    }
    Cube * edgeCornerCube(int i) {
        return edgeCornerCubes[i];
    }
    qglviewer::Vec faceNormal(int i) {
        return faceNormals[i];
    }
    std::map<char, int> faceToIdx;
    int getSelectedFrameNumber() {
        return selected;
    }
    void setSelectedFrameNumber(int nb) {
        selected = nb;
    }
    void updateEdgeCornerPosition();
    Cube * getEdgeCornerCubeAtPosition(qglviewer::Vec pos);
    qglviewer::Vec relativePositionByCubeType(std::string childCubeType, std::string parentCubeType);
    qglviewer::Vec relativePositionByPosition(Cube * ChildCube, Cube * ParentCube) {
        return (ChildCube->getCubeFrame()->position() - ParentCube->getCubeFrame()->position());
    }
};

class Viewer : public QGLViewer {
protected :
    virtual void init();
    virtual void draw();
    void initSpotLight();
    
    virtual void drawWithNames();
    virtual void endSelection(const QPoint&);
    virtual void postSelection(const QPoint& point);
    
    virtual void mouseReleaseEvent(QMouseEvent * e);

private:
  Cubik cubik;
  qglviewer::Vec orig, dir, selectedPoint;
};

#endif
