#include <QGLViewer/manipulatedCameraFrame.h>
#include "Cubik.h"

using namespace qglviewer;
using namespace std;

/**************************************************************/
/*************************** Viewer ***************************/
/**************************************************************/
void Viewer::initSpotLight() {
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHT1);
    glLoadIdentity();
}

void Viewer::init() {
    restoreStateFromFile();

    setSceneBoundingBox(Vec( 3.0f, 3.0f, 3.0f), Vec(-3.0f,-3.0f,-3.0f));
    camera()->showEntireScene();

    // Make camera the default manipulated frame.
    setManipulatedFrame(camera()->frame());

    setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT, true);
    
    setMouseBinding(Qt::ControlModifier, Qt::LeftButton, QGLViewer::CAMERA, QGLViewer::ROTATE);
    setMouseBinding(Qt::ControlModifier, Qt::RightButton, QGLViewer::CAMERA, QGLViewer::TRANSLATE);
    setMouseBinding(Qt::ControlModifier, Qt::MidButton, QGLViewer::CAMERA, QGLViewer::ZOOM);
    setWheelBinding(Qt::ControlModifier, QGLViewer::CAMERA, QGLViewer::ZOOM);

    setMouseBinding(Qt::NoModifier, Qt::LeftButton, QGLViewer::FRAME, QGLViewer::ROTATE);
    setMouseBinding(Qt::NoModifier, Qt::RightButton, QGLViewer::FRAME, QGLViewer::TRANSLATE);
    setMouseBinding(Qt::NoModifier, Qt::MidButton, QGLViewer::FRAME, QGLViewer::ZOOM);
    setWheelBinding(Qt::NoModifier, QGLViewer::FRAME, QGLViewer::ZOOM);

    initSpotLight();
}

void Viewer::draw() {
    drawAxis();
    cubik.draw();
}

void Viewer::drawWithNames() {
    drawAxis();
    cubik.draw(true);
}

void Viewer::postSelection(const QPoint&) {
    if (selectedName() == -1) {
        setManipulatedFrame(camera()->frame());
        cubik.setSelectedFrameNumber(100); // dummy value meaning camera
    }
    else {
        setManipulatedFrame(cubik.faceCenterCube(selectedName())->getCubeFrame());
        cubik.setSelectedFrameNumber(selectedName());
    }
}

void Viewer::mouseReleaseEvent(QMouseEvent * e) {
    QGLViewer::mouseReleaseEvent(e);
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::NoButton)) {
        if ((selectedName() >= 0) && (selectedName() <= 5)) {
            if (cubik.faceCenterCube(selectedName())->getCubeFrame()->isSpinning())
                cubik.faceCenterCube(selectedName())->getCubeFrame()->stopSpinning();
            cubik.faceCenterCube(selectedName())->getCubeFrame()->startSpinning(0.5);
        }
    }
}

/**************************************************************/
/*************************** Cubik ****************************/
/**************************************************************/
Cubik::Cubik() {
    centerCube = new Cube("UDFBLR");
    faceCenterCubes[0] = new Cube("U");
    faceCenterCubes[1] = new Cube("D");
    faceCenterCubes[2] = new Cube("F");
    faceCenterCubes[3] = new Cube("B");
    faceCenterCubes[4] = new Cube("L");
    faceCenterCubes[5] = new Cube("R");
    
    edgeCubes[0] = new Cube("UF");
    edgeCubes[1] = new Cube("UB");
    edgeCubes[2] = new Cube("UL");
    edgeCubes[3] = new Cube("UR");
    edgeCubes[4] = new Cube("FL");
    edgeCubes[5] = new Cube("FR");
    edgeCubes[6] = new Cube("BL");
    edgeCubes[7] = new Cube("BR");
    edgeCubes[8] = new Cube("DF");
    edgeCubes[9] = new Cube("DB");
    edgeCubes[10] = new Cube("DL");
    edgeCubes[11] = new Cube("DR");
    
    faceNormals[0] = Vec( 0.0f, 1.0f, 0.0f); // U
    faceNormals[1] = Vec( 0.0f,-1.0f, 0.0f); // D
    faceNormals[2] = Vec( 0.0f, 0.0f, 1.0f); // F
    faceNormals[3] = Vec( 0.0f, 0.0f,-1.0f); // B
    faceNormals[4] = Vec(-1.0f, 0.0f, 0.0f); // L
    faceNormals[5] = Vec( 1.0f, 0.0f, 0.0f); // R
    
    // Initialize centerCube
    WorldConstraint * centerConstraint = new WorldConstraint();
    centerConstraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
    centerConstraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
    centerCube->getCubeFrame()->setConstraint(centerConstraint);
    // centerCube->getCubeFrameConstraint()->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
    // centerCube->getCubeFrameConstraint()->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
    // centerCube->setConstraint();
    
    // Initialize faceCenterCubes
    for (int j = 0; j < NumFaces; j++) {
        WorldConstraint * faceConstraint = new WorldConstraint();
        faceConstraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        faceConstraint->setRotationConstraint(AxisPlaneConstraint::AXIS, faceNormal(j));
        
        faceCenterCube(j)->getCubeFrame()->setReferenceFrame(centerCube->getCubeFrame());
        faceCenterCube(j)->getCubeFrame()->setTranslation(2*faceNormal(j));
        faceCenterCube(j)->getCubeFrame()->setConstraint(faceConstraint);
    }

    // Initialize edgeCubes
    for (int j = 0; j < NumFaces; j++) {
        WorldConstraint * edgeConstraint = new WorldConstraint();
        edgeConstraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        edgeConstraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        edgeCube(j)->getCubeFrame()->setReferenceFrame(centerCube->getCubeFrame());
        edgeCube(j)->getCubeFrame()->setConstraint(edgeConstraint);
    }

    selected = 8; // camera is selected by default
}

void Cubik::draw(bool names) {
    for (int j = 0; j < NumFaces; j++) {
        if (faceCenterCube(j)->getCubeFrame()->isSpinning()) {
            // check if the faceCenterCube has been aligned to the centerCube
            if (faceCenterCube(j)->getCubeFrame()->checkAlignedWithFrame(centerCube->getCubeFrame()))
                faceCenterCube(j)->getCubeFrame()->stopSpinning();
        }
    }
    
    glPushMatrix();
    glMultMatrixd(centerCube->getCubeFrame()->matrix());
    // glScalef( 0.5f, 0.5f, 0.5f);
    drawCenter();
    glPopMatrix();
    
    for (int j = 0; j < NumFaces; j++) {
        if (names)
            glPushName(j);
        glPushMatrix();
        glMultMatrixd(faceCenterCube(j)->getCubeFrame()->matrix());
        // glScalef( 0.5f, 0.5f, 0.5f);
        QGLViewer::drawAxis(2.0);
        drawFaceCenter(j);
        glPopMatrix();
        if (names)
            glPopName();
    }
}

void Cubik::drawCenter() {
    centerCube->draw();
}

// void Cubik::drawFace(unsigned short i) {
//     faceCenterCube(i)->draw();
// }

void Cubik::drawFaceCenter(unsigned short i) {
    faceCenterCube(i)->draw();
}

