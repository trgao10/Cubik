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
    std::vector<std::vector<std::string> > solutionToCurrentStatus;
    
    int currPhase;
    int currPhaseStartStep;
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

    int getCurrPhase() { return currPhase; }
    void setCurrPhase(int cp) { currPhase = cp; }
    std::string getOrientation(Cube * cube, std::string solvedStatus);
    std::vector<std::vector<std::string> > getSolutionToCurrentStatus() {
        return solutionToCurrentStatus;
    }
    int getCurrPhaseStartStep() {
        return currPhaseStartStep;
    }
    void setCurrPhaseStartStep(int ns) {
        currPhaseStartStep = ns;
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
    int getPhaseLength() {
        if (currPhase == 7)
            return 0;
        else if ((currPhase >= 0) && (currPhase < 7))
            return ((int) solutionToCurrentStatus[currPhase].size());
        else
            return -1;
    }
    int getPhaseLength(std::vector<std::vector<std::string> > tmpSoln, int tmpPhase) {
        if (tmpPhase == 7)
            return 0;
        else if ((tmpPhase >= 0) && (tmpPhase < 7))
            return ((int) tmpSoln[currPhase].size());
        else
            return -1;
    }
    void increase_nSteps() {
        nSteps++;
    }
    int getNumSteps() {
        return nSteps;
    }
    bool checkSolved() {
        for (auto iter = currentStatus.begin(), jter = solvedCube.begin();
             iter != currentStatus.end(), jter != solvedCube.end();
             ++iter, ++jter) {
            if ((*iter) != (*jter))
                return false;
        }
        return true;
    }
    void updateSolutionToCurrentStatus() {
        solveCubeStoreSolution();
    }
    void updateEdgeCornerPosition();
    Cube * getEdgeCornerCubeAtPosition(qglviewer::Vec pos);
    qglviewer::Vec relativePositionByCubeType(std::string childCubeType, std::string parentCubeType);
    qglviewer::Vec relativePositionByPosition(Cube * ChildCube, Cube * ParentCube) {
        return (ChildCube->getCubeFrame()->position() - ParentCube->getCubeFrame()->position());
    }
    int locatePhase(std::vector<std::vector<std::string> > tmpSoln) {
        int userPhase = 0;
        for (auto iter = tmpSoln.begin(); iter != tmpSoln.end(); ++iter) {
        if (!(*iter).empty())
            break;
        userPhase++;
        }
        return userPhase;
    }
    int locateCurrPhase() {
        int userPhase = 0;
        for (auto iter = solutionToCurrentStatus.begin(); iter != solutionToCurrentStatus.end(); ++iter) {
        if (!(*iter).empty())
            break;
        userPhase++;
        }
        return userPhase;
    }
    std::vector<std::vector<std::string> > solveCube();
    void solveCubeStoreSolution();
    std::vector<std::string> localSimplify(std::vector<std::string> original) {
        std::vector<std::string> simpResult;
        for (auto iter = original.begin(); iter != original.end(); ++iter) {
            if (((*iter).size() == 2) && ((*iter)[1] == '2')) {
                simpResult.push_back((*iter).substr(0,1));
                simpResult.push_back((*iter).substr(0,1));
            } else
                simpResult.push_back(*iter);
        }
        original = simpResult;

        for (auto iter = original.begin(); iter != original.end(); ++iter) {
            if (iter+1 >= original.end())
                break;
            if (((*iter).size() != (*(iter+1)).size()) && ((*iter)[0] == (*(iter+1))[0])) {
                iter = original.erase(iter, iter+2);
            }
        }
        for (auto iter = original.begin(); iter != original.end(); ++iter) {
            if ((original.end() - iter) < 4)
                break;
            if (((*iter) == (*(iter+1))) && ((*iter) == (*(iter+2))) && ((*iter) == (*(iter+3))))
                original.erase(iter, iter+4);
        }
        return original;
    }
    bool vecStringEqual(std::vector<std::string> original, std::vector<std::string> simplified) {
        for (auto iter = original.begin(), jter = simplified.begin();
             iter != original.end(), jter != simplified.end();
             ++iter, ++jter) {
            if ((*iter) != (*jter))
                return false;
        }
        return true;
    }
    std::vector<std::string> iterSimplify(std::vector<std::string> original) {
        /* std::cout << "before iterSimplify: "; */
        /* for (auto iter = original.begin(); iter != original.end(); ++iter) */
        /*     std::cout << *iter << " "; */
        /* std::cout << std::endl; */
        
        while (!vecStringEqual(original, localSimplify(original)))
            original = localSimplify(original);
        
        /* std::cout << "after iterSimplify: "; */
        /* for (auto iter = original.begin(); iter != original.end(); ++iter) */
        /*     std::cout << *iter << " "; */
        /* std::cout << std::endl; */
        return original;
    }
    std::vector<std::vector<std::string> > iterSimplify(std::vector<std::vector<std::string> > result) {
        for (std::vector<std::vector<std::string> >::iterator iter = result.begin(); iter != result.end(); ++iter) {
            *iter = iterSimplify(*iter);
        }
        return result;
    }
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
