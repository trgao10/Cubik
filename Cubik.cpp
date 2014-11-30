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

    // setMouseBinding(Qt::NoModifier, Qt::LeftButton, SELECT, true);
    
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
    glDisable(GL_CULL_FACE);
    cubik.draw();
}

void Viewer::drawWithNames() {
    glDisable(GL_CULL_FACE);
    cubik.draw();
}

void Viewer::endSelection(const QPoint&) {
    glFlush();

    // Get the number of objects that were seen through the pick matrix frustum. Reset GL_RENDER mode.
    GLint nbHits = glRenderMode(GL_RENDER);

    if (nbHits <= 0)
        setSelectedName(-1);
    else {
        GLuint zMin = (selectBuffer())[1];
        setSelectedName((selectBuffer())[3]);
        for (int i=1; i<nbHits; ++i)
            if ((selectBuffer())[4*i+1] < zMin) {
                zMin = (selectBuffer())[4*i+1];
                setSelectedName((selectBuffer())[4*i+3]);
            }
    }
}

void Viewer::postSelection(const QPoint&) {
    if (manipulatedFrame()->isSpinning())
        return;
    if (selectedName() == -1) {
        setManipulatedFrame(camera()->frame());
        cubik.setSelectedFrameNumber(100); // dummy value meaning camera
    }
    else if ((selectedName() >= 0) && (selectedName() < NumFaces)) {
        setManipulatedFrame(cubik.faceCenterCube(selectedName())->getCubeFrame());
        cubik.setSelectedFrameNumber(selectedName());
    } else if ((selectedName() >= NumFaces) && (selectedName() < NumFaces+NumEdges+NumCorners)) {
        Cube * ParentCube = cubik.edgeCornerCube(selectedName()-NumFaces)->parentCube;
        setManipulatedFrame(ParentCube->getCubeFrame());
        cubik.setSelectedFrameNumber(cubik.faceToIdx[(ParentCube->getCubeType())[0]]);
    }
    // in any case, need to update the parent-child cube relations here
    int selected = cubik.getSelectedFrameNumber();
    if ((selected >= 0) && (selected < NumFaces)) {
        Cube * ParentCube = cubik.faceCenterCube(selected);
        // std::cout << "ParentCube location: " << ParentCube->getCubeFrame()->position() << std::endl;
        // std::cout << "ParentCube expects child locations: " << std::endl;
        std::vector<qglviewer::Vec> expectedChildLocations;
        for (int j = 0; j < NumFaces; j++) {
            if (((2*cubik.faceNormal(j)-ParentCube->getCubeFrame()->position()).norm() < 1e-3) || ((2*cubik.faceNormal(j)+ParentCube->getCubeFrame()->position()).norm() < 1e-3))
                continue;
            expectedChildLocations.push_back(ParentCube->getCubeFrame()->position()+2*cubik.faceNormal(j));
            // std::cout << ParentCube->getCubeFrame()->position()+2*cubik.faceNormal(j) << std::endl;
        }
        // std::cout << "Current cubes at these locations: " << std::endl;
        for (auto iter = expectedChildLocations.begin(); iter != expectedChildLocations.end(); ++iter) {
            Cube * ChildCube = cubik.getEdgeCornerCubeAtPosition(*iter);
            qglviewer::Vec newTranslation = cubik.relativePositionByPosition(ChildCube, ParentCube);
            // std::cout << "At position (" << *iter << "), CubeType = " << ChildCube->getCubeType() << ", parentCube = " << ChildCube->parentCube->getCubeType() << std::endl;
            // std::cout << "ChildCube location: " << ChildCube->getCubeFrame()->position() << std::endl;
            // std::cout << "After resetting translation, ChildCube location: " << ChildCube->getCubeFrame()->position() << std::endl;
            ChildCube->parentCube = ParentCube;
            ChildCube->getCubeFrame()->setReferenceFrame(ParentCube->getCubeFrame());
            // std::cout << "After setting reference frame, ChildCube->position() = "  << ChildCube->getCubeFrame()->position() << std::endl;
            ChildCube->getCubeFrame()->setTranslation(newTranslation);
            // std::cout << "After setting translation, ChildCube->position() = "  << ChildCube->getCubeFrame()->position() << std::endl;
            // std::cout << "***************************************" << std::endl;
        }
        // for (int j = 0; j < NumEdges+NumCorners; j++)
        //     for (std::vector<qglviewer::Vec>::size_type k = 0; k < expectedChildLocations.size(); k++) {
        //         if ((cubik.edgeCornerCube(j)->getCubeFrame()->position()-expectedChildLocations[k]).norm() < 1e-3)
        //             std::cout << "(" << cubik.edgeCornerCube(j)->getCubeFrame()->position() << "), CubeType = " << cubik.edgeCornerCube(j)->getCubeType() << ", current parent = " << cubik.edgeCornerCube(j)->parentCube->getCubeType()  << std::endl;
        //     }
    }
}

void Viewer::mouseReleaseEvent(QMouseEvent * e) {
    int selected = cubik.getSelectedFrameNumber();
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::NoButton)) {
        if ((selected >= 0) && (selected < NumFaces)) {
            if (cubik.faceCenterCube(selected)->getCubeFrame()->isSpinning())
                cubik.faceCenterCube(selected)->getCubeFrame()->stopSpinning();
            cubik.faceCenterCube(selected)->getCubeFrame()->startSpinning(0.5);
        }
    }
    QGLViewer::mouseReleaseEvent(e);
}

/**************************************************************/
/*************************** Cubik ****************************/
/**************************************************************/
Cubik::Cubik() {
    faceToIdx['U'] = 0;
    faceToIdx['D'] = 1;
    faceToIdx['F'] = 2;
    faceToIdx['B'] = 3;
    faceToIdx['L'] = 4;
    faceToIdx['R'] = 5;
    
    centerCube = new Cube("UDFBLR");
    faceCenterCubes[0] = new Cube("U");
    faceCenterCubes[1] = new Cube("D");
    faceCenterCubes[2] = new Cube("F");
    faceCenterCubes[3] = new Cube("B");
    faceCenterCubes[4] = new Cube("L");
    faceCenterCubes[5] = new Cube("R");
    
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

    // Initialize faceCenterCubes
    for (int j = 0; j < NumFaces; j++) {
        WorldConstraint * faceConstraint = new WorldConstraint();
        faceConstraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        faceConstraint->setRotationConstraint(AxisPlaneConstraint::AXIS, faceNormal(j));
        
        faceCenterCube(j)->getCubeFrame()->setReferenceFrame(centerCube->getCubeFrame());
        faceCenterCube(j)->getCubeFrame()->setTranslation(2*faceNormal(j));
        faceCenterCube(j)->getCubeFrame()->setConstraint(faceConstraint);
    }
    
    // centerCube->childCubes.push_back(faceCenterCubes[0]);
    // centerCube->childCubes.push_back(faceCenterCubes[1]);
    // centerCube->childCubes.push_back(faceCenterCubes[2]);
    // centerCube->childCubes.push_back(faceCenterCubes[3]);
    // centerCube->childCubes.push_back(faceCenterCubes[4]);
    // centerCube->childCubes.push_back(faceCenterCubes[5]);
    
    for (auto iter = solvedCube.begin(); iter != solvedCube.end(); ++iter) {
        Cube * TmpCube = new Cube(*iter);
        LocalConstraint * TmpConstraint = new LocalConstraint();
        TmpConstraint->setTranslationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        TmpConstraint->setRotationConstraint(AxisPlaneConstraint::FORBIDDEN, Vec(0.0,0.0,0.0));
        Cube * ParentCube = faceCenterCube(TmpCube->getCubeType().substr(0,1));
        TmpCube->parentCube = ParentCube;
        TmpCube->getCubeFrame()->setReferenceFrame(ParentCube->getCubeFrame());
        TmpCube->getCubeFrame()->setTranslation(2*relativePositionByCubeType(TmpCube->getCubeType(), ParentCube->getCubeType()));
        TmpCube->getCubeFrame()->setConstraint(TmpConstraint);
        // ParentCube->childCubes.push_back(TmpCube);
        edgeCornerCubes[iter-solvedCube.begin()] = TmpCube;
    }

    selected = 100; // camera is selected by default
}

Cubik::~Cubik() {
    for (int j = 0; j < NumEdges+NumFaces; j++)
        delete edgeCornerCubes[j];
    for (int j = 0; j < NumFaces; j++)
        delete faceCenterCubes[j];
    delete centerCube;
    // recursiveDeleteCube(centerCube);
}

void Cubik::draw() {
    if(!isSpinning()) {
        updateEdgeCornerPosition();
    }
    for (int j = 0; j < NumFaces; j++) {
        if (faceCenterCube(j)->getCubeFrame()->isSpinning()) {
            // check if the faceCenterCube has been aligned to the centerCube
            if (faceCenterCube(j)->getCubeFrame()->checkAlignedWithFrame(centerCube->getCubeFrame()))
                faceCenterCube(j)->getCubeFrame()->stopSpinning();
        }
    }
    
    glPushMatrix();
    glMultMatrixd(centerCube->getCubeFrame()->matrix());
    centerCube->draw();
    glPopMatrix();
    
    for (int j = 0; j < NumFaces; j++) {
        glPushMatrix();
        glMultMatrixd(faceCenterCube(j)->getCubeFrame()->matrix());
        glPushName(j);
        faceCenterCube(j)->draw();
        glPopName();
        glPopMatrix();
    }
    
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        glPushMatrix();
        glMultMatrixd(edgeCornerCube(j)->parentCube->getCubeFrame()->matrix());
        glMultMatrixd(edgeCornerCube(j)->getCubeFrame()->matrix());
        glPushName(j+NumFaces);
        edgeCornerCube(j)->draw();
        glPopName();
        glPopMatrix();
    }
}

// void Cubik::recursiveDeleteCube(Cube * cube) {
//     if (cube == NULL)
//         return;
//     for (auto iter = cube->childCubes.begin(); iter != cube->childCubes.end(); ++iter)
//         recursiveDeleteCube(*iter);
//     delete cube;
// }

qglviewer::Vec Cubik::relativePositionByCubeType(std::string childCubeType, std::string parentCubeType) {
    // parentCubeType will always contain only one letter!
    qglviewer::Vec rVec( 0.0f, 0.0f, 0.0f);
    for (auto iter = childCubeType.begin(); iter != childCubeType.end(); ++iter) {
        if ((*iter) != parentCubeType[0])
            rVec += faceNormal(faceToIdx[*iter]);
    }
    return rVec;
}

bool Cubik::isSpinning() {
    for (int j = 0; j < NumFaces; j++) {
        if (faceCenterCube(j)->getCubeFrame()->isSpinning())
            return true;
    }
    return false;
}

void Cubik::updateEdgeCornerPosition() {
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        edgeCornerPosition[edgeCornerCube(j)] = edgeCornerCube(j)->getCubeFrame()->position();
    }
}

Cube * Cubik::getEdgeCornerCubeAtPosition(qglviewer::Vec pos) {
    for (auto iter = edgeCornerPosition.begin(); iter != edgeCornerPosition.end(); ++iter) {
        if ((iter->second - pos).norm() < 1e-3)
            return iter->first;
    }
    return NULL;
}

// void Cubik::recursiveDrawCube(Cube * cube) {
//     if (cube == NULL)
//         return;
//     glPushMatrix();
//     glMultMatrixd(cube->getCubeFrame()->matrix());
//     if (cube->isFaceCenterCube())
//         glPushName(faceToIdx[cube->getCubeType()[0]]);
//     cube->draw();
//     if (cube->isFaceCenterCube())
//         glPopName();
//     for (auto iter = cube->childCubes.begin(); iter != cube->childCubes.end(); ++iter)
//         recursiveDrawCube(*iter);
//     glPopMatrix();
// }

