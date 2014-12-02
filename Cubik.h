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
    Cube * indicatorCube;
    Cube * centerCube;
    Cube * faceCenterCubes[NumFaces];
    Cube * edgeCornerCubes[NumEdges+NumCorners];

    std::vector<std::string> solvedCube = { "UF", "UR", "UB", "UL",
                                            "DF", "DR", "DB", "DL",
                                            "FR", "FL", "BR", "BL",
                                            "UFR", "URB", "UBL", "ULF",
                                            "DRF", "DFL", "DLB", "DBR" };
    std::vector<qglviewer::Vec> solvedCubeLocations = { qglviewer::Vec( 0.0f, 2.0f, 2.0f), // UF
                                                        qglviewer::Vec( 2.0f, 2.0f, 0.0f), // UR
                                                        qglviewer::Vec( 0.0f, 2.0f,-2.0f), // UB
                                                        qglviewer::Vec(-2.0f, 2.0f, 0.0f), // UL
                                                        qglviewer::Vec( 0.0f,-2.0f, 2.0f), // DF
                                                        qglviewer::Vec( 2.0f,-2.0f, 0.0f), // DR
                                                        qglviewer::Vec( 0.0f,-2.0f,-2.0f), // DB
                                                        qglviewer::Vec(-2.0f,-2.0f, 0.0f), // DL
                                                        qglviewer::Vec( 2.0f, 0.0f, 2.0f), // FR
                                                        qglviewer::Vec(-2.0f, 0.0f, 2.0f), // FL
                                                        qglviewer::Vec( 2.0f, 0.0f,-2.0f), // BR
                                                        qglviewer::Vec(-2.0f, 0.0f,-2.0f), // BL
                                                        qglviewer::Vec( 2.0f, 2.0f, 2.0f), // UFR
                                                        qglviewer::Vec( 2.0f, 2.0f,-2.0f), // URB
                                                        qglviewer::Vec(-2.0f, 2.0f,-2.0f), // UBL
                                                        qglviewer::Vec(-2.0f, 2.0f, 2.0f), // ULF
                                                        qglviewer::Vec( 2.0f,-2.0f, 2.0f), // DRF
                                                        qglviewer::Vec(-2.0f,-2.0f, 2.0f), // DFL
                                                        qglviewer::Vec(-2.0f,-2.0f,-2.0f), // DLB
                                                        qglviewer::Vec( 2.0f,-2.0f,-2.0f), // DBR
    };
    std::vector<std::string> currentStatus;
    std::vector<std::string> solutionToCurrentStatus;
    int nSteps;
    int selected;
    bool resumeSpinning;
    qglviewer::Vec faceNormals[NumFaces]; // U, D, F, B, L, R
    std::map<Cube *, qglviewer::Vec> edgeCornerPosition;
public:
    Cubik();
    ~Cubik();
    void draw();
    void drawIndicatorCube() { indicatorCube->draw(); }

    bool checkResumeSpinning() { return resumeSpinning; }
    void setResumeSpinning(bool ifResume) { resumeSpinning = ifResume; }
    bool isSpinning();
    std::string getNextMove() {
        if (getNumRemainingMoves() > 0)
            return *solutionToCurrentStatus.begin();
        else
            return "";
    }
    int getNumRemainingMoves() {
        return solutionToCurrentStatus.size();
    }
    Cube * getIndicatorCube () {
        return indicatorCube;
    }
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
    void increase_nSteps() {
        nSteps++;
    }
    void updateEdgeCornerPosition();
    Cube * getEdgeCornerCubeAtPosition(qglviewer::Vec pos);
    qglviewer::Vec relativePositionByCubeType(std::string childCubeType, std::string parentCubeType);
    qglviewer::Vec relativePositionByPosition(Cube * ChildCube, Cube * ParentCube) {
        return (ChildCube->getCubeFrame()->position() - ParentCube->getCubeFrame()->position());
    }
    std::vector<std::string> solveCube();
};

class Viewer : public QGLViewer {
protected :
    virtual void init();
    virtual void draw();
    virtual void postDraw();

    void initSpotLight();
    
    virtual void drawWithNames();
    virtual void endSelection(const QPoint&);
    virtual void postSelection(const QPoint& point);
    
    virtual void keyPressEvent(QKeyEvent * e);
    virtual void mousePressEvent(QMouseEvent * e);
    virtual void mouseReleaseEvent(QMouseEvent * e);

private:
  Cubik cubik;
  qglviewer::Vec orig, dir, selectedPoint;
  bool showIndicatorAxis;
  bool hintMode;
  
  void drawIndicatorAxis();
  void drawIndicatorCube();
  void drawHint();
};

#endif
