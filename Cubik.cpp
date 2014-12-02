#include <QGLViewer/manipulatedCameraFrame.h>
#include "Cubik.h"
#include "solveCube.cpp"

using namespace qglviewer;
using namespace std;

/**************************************************************/
/*************************** Viewer ***************************/
/**************************************************************/
void Viewer::initSpotLight() {
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
    glLoadIdentity();
}

void Viewer::init() {
    restoreStateFromFile();

    glLineWidth(3.0);

    setSceneBoundingBox(Vec( 3.0f, 3.0f, 3.0f), Vec(-3.0f,-3.0f,-3.0f));
    camera()->showEntireScene();

    // Make camera the default manipulated frame.
    setManipulatedFrame(camera()->frame());
    
    setShortcut(EXIT_VIEWER, Qt::CTRL+Qt::Key_Q);

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
    showIndicatorAxis = true;
    hintMode = false;
}

void Viewer::draw() {
    cubik.draw();
    
    // // Draw the intersection line
    // glBegin(GL_LINES);
    // glVertex3fv(orig);
    // glVertex3fv(orig + 100.0*dir);
    // glEnd();
}

void Viewer::drawIndicatorCube() {
    int viewport[4];
    int scissor[4];

    // The viewport and the scissor are changed to fit the lower left
    // corner. Original values are saved.
    glGetIntegerv(GL_VIEWPORT,viewport);
    glGetIntegerv(GL_SCISSOR_BOX,scissor);

    glViewport(0,0,width()/4,height()/4);
    glScissor(0,0,width()/4,height()/4);

    // The Z-buffer is cleared to make the thumbnail appear over the
    // original image.
    glClear(GL_DEPTH_BUFFER_BIT);

    cubik.drawIndicatorCube();
    
    // display text
    qreal threshold = -0.2;
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    qglColor(QColor(255, 239, 191));
    if (-camera()->viewDirection()*Vec( 0.0f, 1.0f, 0.0f) > threshold)
        renderText( 0.0f, 2.0f, 0.0f, "U");
    if (-camera()->viewDirection()*Vec( 0.0f,-1.0f, 0.0f) > threshold)
        renderText( 0.0f,-2.0f, 0.0f, "D");
    if (-camera()->viewDirection()*Vec( 0.0f, 0.0f, 1.0f) > threshold)
        renderText( 0.0f, 0.0f, 2.0f, "F");
    if (-camera()->viewDirection()*Vec( 0.0f, 0.0f,-1.0f) > threshold)
        renderText( 0.0f, 0.0f,-2.0f, "B");
    if (-camera()->viewDirection()*Vec(-1.0f, 0.0f, 0.0f) > threshold)
        renderText(-2.0f, 0.0f, 0.0f, "L");
    if (-camera()->viewDirection()*Vec( 1.0f, 0.0f, 0.0f) > threshold)
        renderText( 2.0f, 0.0f, 0.0f, "R");
    qglColor(QColor( 255, 255, 255));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    // The viewport and the scissor are restored.
    glScissor(scissor[0],scissor[1],scissor[2],scissor[3]);
    glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
}

void Viewer::drawHint() {
    drawText(10, 20, QString::fromStdString("Next Move: " + cubik.getNextMove()));
    drawText(10, 40, QString("%1 Steps Left").arg(cubik.getNumRemainingMoves()));
}

void Viewer::drawIndicatorAxis() {
	int viewport[4];
	int scissor[4];

	// The viewport and the scissor are changed to fit the lower left
	// corner. Original values are saved.
	glGetIntegerv(GL_VIEWPORT, viewport);
	glGetIntegerv(GL_SCISSOR_BOX, scissor);

	// Axis viewport size, in pixels
	const int size = 150;
	glViewport(0,0,size,size);
	glScissor(0,0,size,size);

	// The Z-buffer is cleared to make the axis appear over the
	// original image.
	glClear(GL_DEPTH_BUFFER_BIT);

	// Tune for best line rendering
        glDisable(GL_LIGHTING);
	glLineWidth(3.0);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(camera()->orientation().inverse().matrix());

	glBegin(GL_LINES);
        
        // U: yellow
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.5, 0.0);
        
        // D: white
	glColor3f(1.0, 1.0, 1.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0,-0.5, 0.0);
	
        // F: blue
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.5);
        
        // B: green
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0,-0.5);
        
        // L: pink
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f(-0.5, 0.0, 0.0);

        // R: red
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.5, 0.0, 0.0);
        
        glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable(GL_LIGHTING);

	// The viewport and the scissor are restored.
	glScissor(scissor[0],scissor[1],scissor[2],scissor[3]);
	glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);
}

void Viewer::postDraw() {
    QGLViewer::postDraw();
    if (showIndicatorAxis)
        drawIndicatorCube();
        // drawIndicatorAxis();
    if (hintMode)
        drawHint();
}

void Viewer::drawWithNames() {
    cubik.draw();
}

void Viewer::endSelection(const QPoint& point) {
    glFlush();
    glRenderMode(GL_RENDER);
    
    // Compute orig and dir, used to draw a representation of the intersecting line
    camera()->convertClickToLine(point, orig, dir);

    // Find the selectedPoint coordinates, using camera()->pointUnderPixel().
    bool found;
    selectedPoint = camera()->pointUnderPixel(point, found);
    selectedPoint -= 0.01f*dir; // Small offset to make point clearly visible.
    // Note that "found" is different from (selectedObjectId()>=0) because of the size of the select region.
    
    // We compute the intersection of the line with each face
    // of the bounding box and pick the first object being hit.
    // This is fairly fundamental for ray-tracing.
    qreal tmin = 1000;
    int selectedIdx = -1;
    for (int j = 0; j < NumFaces; j++) {
        qreal t = (cubik.faceCenterCube(j)->getCubeFrame()->position()*3.0/2.0-selectedPoint)*cubik.faceNormal(j)/(dir*cubik.faceNormal(j));
        qglviewer::Vec localCoords = cubik.faceCenterCube(j)->getCubeFrame()->coordinatesOf(selectedPoint+t*dir-cubik.faceCenterCube(j)->getCubeFrame()->position()*3.0/2.0);
        if ((fabs(cubik.faceCenterCube(j)->getCubeFrame()->position().x)>1e-3) && (fabs(localCoords.y) <3) && (fabs(localCoords.z) <3)) {
            if ((t >= 0) && (t < tmin)) {
                tmin = t;
                selectedIdx = j;
            }
        }
        else if ((fabs(cubik.faceCenterCube(j)->getCubeFrame()->position().y)>1e-3) && (fabs(localCoords.x) <3) && (fabs(localCoords.z) <3)) {
            if ((t >= 0) && (t < tmin)) {
                tmin = t;
                selectedIdx = j;
            }
        }
        else if ((fabs(cubik.faceCenterCube(j)->getCubeFrame()->position().z)>1e-3) && (fabs(localCoords.x) <3) && (fabs(localCoords.y) <3)) {
            if ((t >= 0) && (t < tmin)) {
                tmin = t;
                selectedIdx = j;
            }
        }
    }
    setSelectedName(selectedIdx);
}

void Viewer::postSelection(const QPoint&) {
    if (manipulatedFrame()->isSpinning())
        return;
    
    cubik.updateEdgeCornerPosition();
    if (selectedName() == -1) {
        setManipulatedFrame(camera()->frame());
        cubik.setSelectedFrameNumber(100); // dummy value meaning camera
    } else if ((selectedName() >= 0) && (selectedName() < NumFaces)) {
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
        std::vector<qglviewer::Vec> expectedChildLocations;
        for (int j = 0; j < NumFaces; j++) {
            if (((2*cubik.faceNormal(j)-ParentCube->getCubeFrame()->position()).norm() < 1e-3) || ((2*cubik.faceNormal(j)+ParentCube->getCubeFrame()->position()).norm() < 1e-3))
                continue;
            expectedChildLocations.push_back(ParentCube->getCubeFrame()->position()+2*cubik.faceNormal(j));
        }
        std::vector<qglviewer::Vec> expectedCornerLocations;
        for (auto iter = expectedChildLocations.begin(); iter != expectedChildLocations.end(); ++iter)
            for (auto jter = iter+1; jter != expectedChildLocations.end(); ++jter) {
                qglviewer::Vec sum = (*iter)+(*jter)-2*ParentCube->getCubeFrame()->position();
                if (sum.norm() < 1e-3)
                    continue;
                expectedCornerLocations.push_back(sum+ParentCube->getCubeFrame()->position());
            }
        expectedChildLocations.insert(expectedChildLocations.end(), expectedCornerLocations.begin(), expectedCornerLocations.end());
        for (auto iter = expectedChildLocations.begin(); iter != expectedChildLocations.end(); ++iter) {
            // should first get the matrix in world coordinate system, then transform it to the new frame coordinate system
            Cube * ChildCube = cubik.getEdgeCornerCubeAtPosition(*iter);
            if (ChildCube == NULL)
                return;
            qglviewer::Vec newTranslation = ParentCube->getCubeFrame()->coordinatesOf(ChildCube->getCubeFrame()->position());
            qglviewer::Quaternion oldOrientation = ChildCube->getCubeFrame()->orientation();
            qglviewer::Quaternion newParentOrientation = ParentCube->getCubeFrame()->orientation();
            ChildCube->parentCube = ParentCube;
            ChildCube->getCubeFrame()->setReferenceFrame(ParentCube->getCubeFrame());
            ChildCube->getCubeFrame()->setTranslation(newTranslation);
            ChildCube->getCubeFrame()->setRotation(newParentOrientation.inverse()*oldOrientation);
        }
    }
}

void Viewer::keyPressEvent(QKeyEvent *e) {
    // Get event modifiers key
    const Qt::KeyboardModifiers modifiers = e->modifiers();

    bool handled = false;
    if ((e->key()==Qt::Key_F) && (modifiers==Qt::NoButton)) {
        if (showIndicatorAxis)
            showIndicatorAxis = false;
        else if (!showIndicatorAxis)
            showIndicatorAxis = true;
        handled = true;
        updateGL();
    } else if ((e->key()==Qt::Key_H) && (modifiers==Qt::NoButton)) {
        if (hintMode)
            hintMode = false;
        else if (!hintMode)
            hintMode = true;
        handled = true;
        updateGL();
    }
    
    if (!handled)
        QGLViewer::keyPressEvent(e);
}

void Viewer::mousePressEvent(QMouseEvent * e) {
    if (cubik.isSpinning())
        cubik.setResumeSpinning(true);
    QGLViewer::mousePressEvent(e);
}

void Viewer::mouseReleaseEvent(QMouseEvent * e) {
    int selected = cubik.getSelectedFrameNumber();
    if (cubik.checkResumeSpinning())
        cubik.faceCenterCube(selected)->getCubeFrame()->startSpinning(0.5);
    if ((e->button() == Qt::LeftButton) && (e->modifiers() == Qt::NoButton)) {
        if ((selected >= 0) && (selected < NumFaces)) {
            if (cubik.faceCenterCube(selected)->getCubeFrame()->isSpinning())
                cubik.faceCenterCube(selected)->getCubeFrame()->stopSpinning();
            cubik.faceCenterCube(selected)->getCubeFrame()->startSpinning(0.6);
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
    
    indicatorCube = new Cube("UDFBLR");
    centerCube = new Cube("");
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
        edgeCornerCubes[iter-solvedCube.begin()] = TmpCube;
    }

    for (int j = 0; j < NumEdges+NumCorners; j++)
        currentStatus.push_back("");
    selected = 100; // camera is selected by default
    nSteps = 0;
}

Cubik::~Cubik() {
    for (int j = 0; j < NumEdges+NumFaces; j++)
        delete edgeCornerCubes[j];
    for (int j = 0; j < NumFaces; j++)
        delete faceCenterCubes[j];
    delete centerCube;
    delete indicatorCube;
}

void Cubik::draw() {
    // std::cout << "CurrentStatus: " << std::endl;
    // for (auto iter = currentStatus.begin(); iter != currentStatus.end(); ++iter) {
    //     std::cout << *iter << " ";
    // }
    // std::cout << std::endl;

    for (int j = 0; j < NumFaces; j++) {
        if (faceCenterCube(j)->getCubeFrame()->isSpinning()) {
            // check if the faceCenterCube has been aligned to the centerCube
            if (faceCenterCube(j)->getCubeFrame()->checkAlignedWithFrame(centerCube->getCubeFrame())) {
                faceCenterCube(j)->getCubeFrame()->stopSpinning();
                setResumeSpinning(false);
                updateEdgeCornerPosition();
                // std::cout << nSteps << std::endl;
            }
        }
    }
    
    // if(!isSpinning()) {
    //     updateEdgeCornerPosition();
    // }
    
    glPushMatrix();
    glMultMatrixd(centerCube->getCubeFrame()->matrix());
    glScalef( 0.90f, 0.90f, 0.90f);
    centerCube->draw();
    glPopMatrix();
    
    for (int j = 0; j < NumFaces; j++) {
        glPushMatrix();
        glMultMatrixd(faceCenterCube(j)->getCubeFrame()->matrix());
        glPushName(j);
        glScalef( 0.90f, 0.90f, 0.90f);
        faceCenterCube(j)->draw();
        glPopName();
        glPopMatrix();
    }
    
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        glPushMatrix();
        glMultMatrixd(edgeCornerCube(j)->parentCube->getCubeFrame()->matrix());
        glMultMatrixd(edgeCornerCube(j)->getCubeFrame()->matrix());
        glPushName(j+NumFaces);
        glScalef( 0.90f, 0.90f, 0.90f);
        edgeCornerCube(j)->draw();
        glPopName();
        glPopMatrix();
    }
}

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
    
    bool changedFlag = false;
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        for (std::vector<qglviewer::Vec>::size_type k = 0; k < currentStatus.size(); k++) {
            if ((solvedCubeLocations[k]-edgeCornerPosition[edgeCornerCube(j)]).norm() < 1e-3) {
                if ((currentStatus[k] != "") && (currentStatus[k] != edgeCornerCube(j)->getCubeType())) {
                    changedFlag = true;
                }
                currentStatus[k] = edgeCornerCube(j)->getCubeType();
            }
        }
    }
    if (changedFlag)
        increase_nSteps();
    solutionToCurrentStatus.erase(solutionToCurrentStatus.begin(), solutionToCurrentStatus.end());
    // std::cout << "solving cubes..." << std::endl;
    solutionToCurrentStatus = solveCube();
    // std::cout << "solved!" << std::endl;
}

Cube * Cubik::getEdgeCornerCubeAtPosition(qglviewer::Vec pos) {
    for (auto iter = edgeCornerPosition.begin(); iter != edgeCornerPosition.end(); ++iter) {
        if ((iter->second - pos).norm() < 1e-3)
            return iter->first;
    }
    return NULL;
}

