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
    if (cubik.checkSolved())
        cubik.setCurrPhase(7);
    // else {
    //     std::vector<std::vector<std::string> > tmpSoln = cubik.solveCube();
    //     int tmpPhase = cubik.locatePhase(tmpSoln);
    //     if ((tmpPhase >= cubik.getCurrPhase()) && (cubik.getPhaseLength(tmpSoln, tmpPhase) < cubik.getPhaseLength()))
    //         cubik.updateSolutionToCurrentStatus();
    // }
    
    //if ((cubik.getNumSteps() > 0) && (cubik.getCurrPhase() == 7))
    if (cubik.getCurrPhase() == 7)
        drawText(10, 20, QString::fromStdString("Rubik's Cube Solved!"));
    else if ((cubik.getCurrPhase() >= 0) && (cubik.getCurrPhase() <= 6)) {
        std::vector<std::string> remCurrPhase = cubik.getRemStepsInCurrPhase();
        // std::vector<std::string> simpRemCurrPhase = cubik.iterSimplify(remCurrPhase);
        drawText(10, 20, QString::fromStdString("Phase " + std::to_string(cubik.getCurrPhase()+1) + ": " + accumulate(remCurrPhase.begin(), remCurrPhase.end(), string(""))));
        drawText(10, 40, QString::fromStdString("Last Move: " + cubik.getLastMove()));
        // std::vector<std::vector<std::string> > soln = cubik.getSolutionToCurrentStatus();
        // int userPhase = cubik.locatePhase(soln);
        // std::vector<std::string> remCurrPhase = soln[userPhase];
        // std::vector<std::string> simpRemCurrPhase = cubik.iterSimplify(remCurrPhase);
        // int numStepsInCurrPhase = cubik.getNumSteps() - cubik.getCurrPhaseStartStep();
        // drawText(10, 20, QString::fromStdString("Phase " + std::to_string(userPhase+1) + ": " + accumulate(remCurrPhase.begin()+numStepsInCurrPhase, remCurrPhase.end(), string(""))));
        // drawText(10, 20, QString::fromStdString("Phase " + std::to_string(userPhase+1) + ": " + accumulate(remCurrPhase.begin()+numStepsInCurrPhase, remCurrPhase.end(), string(""))));
        // drawText(10, 40, QString::fromStdString("Last Move: " + cubik.getLastMove()));
        // drawText(10, 40, QString::fromStdString("Simplified Phase " + std::to_string(userPhase+1) + ": " + accumulate(simpRemCurrPhase.begin(), simpRemCurrPhase.end(), string(""))));
        // std::vector<std::vector<std::string> > tmpSoln = cubik.solveCube();
        // int tmpPhase = cubik.locatePhase(tmpSoln);
        // drawText(10, 40, QString::fromStdString("Tmp Phase " + std::to_string(tmpPhase+1) + ":" + accumulate(tmpSoln[tmpPhase].begin(), tmpSoln[tmpPhase].end(), string(""))));
        // std::vector<std::string> simplified = cubik.iterSimplify(tmpSoln[tmpPhase]);
        // drawText(10, 60, QString::fromStdString("Tmp Phase Simplified" + std::to_string(tmpPhase+1) + ":" + accumulate(simplified.begin(), simplified.end(), string(""))));
        // drawText(10, 20, QString::fromStdString("Remaining Steps in Current Phase: " + accumulate(soln[userPhase].begin(), soln[userPhase].end(), string(""))));
        // drawText(10, 40, QString::fromStdString("Phase " + std::to_string(userPhase+1)));
    }
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
    if ((manipulatedFrame()->isSpinning()) || cubik.checkResumeSpinning())
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
        else if (!hintMode) {
            // std::cout << "solving cubes..." << std::endl;            
            cubik.solveCubeStoreSolution();
            // std::cout << "solved!" << std::endl;
            
            hintMode = true;
        }
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
    
    std::vector<qglviewer::Vec> tmp;
    tmp.push_back(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
    tmp.push_back(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
    tmp.push_back(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
    for (int j = 0; j < NumFaces; j++) {
        faceCenterCubeOrientation.push_back(tmp);
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
    
    updateEdgeCornerPosition();
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

std::string Cubik::getOrientation(Cube * cube, std::string solvedStatus) {
    std::string result;
    for (auto iter = solvedStatus.begin(); iter != solvedStatus.end(); ++iter) {
        // qglviewer::Vec currNormal = faceNormal(faceToIdx[*iter]);
        qglviewer::Vec currNormalInFrame = cube->getCubeFrame()->transformOf(faceNormal(faceToIdx[*iter]));
        for (auto jter = cube->getCubeType().begin(); jter != cube->getCubeType().end(); ++jter) {
            if ((faceNormal(faceToIdx[*jter])-currNormalInFrame).norm() < 1e-3) {
                result.push_back(*jter);
                break;
            }
        }
    }
    return result;
}

void Cubik::updateEdgeCornerPosition() {
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        edgeCornerPosition[edgeCornerCube(j)] = edgeCornerCube(j)->getCubeFrame()->position();
    }
    
    bool changedFlag = false;
    for (int j = 0; j < NumEdges+NumCorners; j++) {
        for (std::vector<qglviewer::Vec>::size_type k = 0; k < currentStatus.size(); k++) {
            if ((solvedCubeLocations[k]-edgeCornerPosition[edgeCornerCube(j)]).norm() < 1e-3) {
                if ((currentStatus[k] != "") && (currentStatus[k] != getOrientation(edgeCornerCube(j), solvedCube[k]))) {
                    changedFlag = true;
                }
                currentStatus[k] = getOrientation(edgeCornerCube(j), solvedCube[k]);
            }
        }
    }
    if (changedFlag) {
        increase_nSteps();
        std::string updatedFace = "";
        for (int j = 0; j < NumFaces; j++) {
            std::string moveType = updateFaceCenerCubeOrientation(j);
            if ((moveType == "\'") || (moveType == "2") || (moveType == ""))
                updatedFace = faceCenterCube(j)->getCubeType() + moveType;
        }
        if (updatedFace != "")
            setLastMove(updatedFace);
   
        if (!getRemStepsInCurrPhase().empty()) {
            if (getLastMove() == *(getRemStepsInCurrPhase().begin()))
                deleteHeadRemStep();
            else
                addHeadRemStep(inverseMove(getLastMove()));
            if (getRemStepsInCurrPhase().empty()) {
                solveCubeStoreSolution();        
            }
        }
    }
    // solveCubeStoreSolution();
}

std::string Cubik::inverseMove(std::string move) {
    if (move.size() == 1)
        return (move+"\'");
    else if (move.size() == 2) {
        if (move[1] == '\'')
            return move.substr(0,1);
        else if (move[1] == '2')
            return move;
    }
    return "";
}

std::string Cubik::updateFaceCenerCubeOrientation(int j) {
    if (j == 0) { // U, check x,z
        qglviewer::Vec oldX = faceCenterCubeOrientation[j][0];
        qglviewer::Vec newX = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldX + newX).norm() < 1e-3) {
            return "2";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 1.0f, 0.0f) > 1e-3) {
            return "\'";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 1.0f, 0.0f) < -1e-3) {
            return "";
        } else
            return "None";
    } else if (j == 1) { // D, check x,z
        qglviewer::Vec oldX = faceCenterCubeOrientation[j][0];
        qglviewer::Vec newX = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldX + newX).norm() < 1e-3) {
            return "2";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 1.0f, 0.0f) < -1e-3) {
            return "\'";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 1.0f, 0.0f) > 1e-3) {
            return "";
        } else
            return "None";
    } else if (j == 2 ) { // F, check x,y
        qglviewer::Vec oldX = faceCenterCubeOrientation[j][0];
        qglviewer::Vec newX = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldX + newX).norm() < 1e-3) {
            return "2";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 0.0f, 1.0f) > 1e-3) {
            return "\'";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 0.0f, 1.0f) < -1e-3) {
            return "";
        } else
            return "None";
    } else if (j == 3) { // B, check x,y
        qglviewer::Vec oldX = faceCenterCubeOrientation[j][0];
        qglviewer::Vec newX = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldX + newX).norm() < 1e-3) {
            return "2";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 0.0f, 1.0f) < -1e-3) {
            return "\'";
        } else if ((oldX^newX)*qglviewer::Vec( 0.0f, 0.0f, 1.0f) > 1e-3) {
            return "";
        } else
            return "None";
    } else if (j == 4) { // L, check y,z
        qglviewer::Vec oldY = faceCenterCubeOrientation[j][1];
        qglviewer::Vec newY = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldY + newY).norm() < 1e-3) {
            return "2";
        } else if ((oldY^newY)*qglviewer::Vec( 1.0f, 0.0f, 0.0f) < -1e-3) {
            return "\'";
        } else if ((oldY^newY)*qglviewer::Vec( 1.0f, 0.0f, 0.0f) > 1e-3) {
            return "";
        } else
            return "None";
    } else if (j == 5) { // R, check y,z
        qglviewer::Vec oldY = faceCenterCubeOrientation[j][1];
        qglviewer::Vec newY = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][0] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 1.0f, 0.0f, 0.0f));
        faceCenterCubeOrientation[j][1] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 1.0f, 0.0f));
        faceCenterCubeOrientation[j][2] = faceCenterCube(j)->getCubeFrame()->inverseTransformOf(qglviewer::Vec( 0.0f, 0.0f, 1.0f));
        if ((oldY + newY).norm() < 1e-3) {
            return "2";
        } else if ((oldY^newY)*qglviewer::Vec( 1.0f, 0.0f, 0.0f) > 1e-3) {
            return "\'";
        } else if ((oldY^newY)*qglviewer::Vec( 1.0f, 0.0f, 0.0f) < -1e-3) {
            return "";
        } else
            return "None";
    } else
        return "None";
}

Cube * Cubik::getEdgeCornerCubeAtPosition(qglviewer::Vec pos) {
    for (auto iter = edgeCornerPosition.begin(); iter != edgeCornerPosition.end(); ++iter) {
        if ((iter->second - pos).norm() < 1e-3)
            return iter->first;
    }
    return NULL;
}

void Cubik::solveCubeStoreSolution() {
    // std::cout << "currentStatus: ";
    // for (auto iter = currentStatus.begin(); iter != currentStatus.end(); ++iter)
    //     std::cout << *iter << " ";
    // std::cout << std::endl;
    solutionToCurrentStatus.erase(solutionToCurrentStatus.begin(), solutionToCurrentStatus.end());
    solutionToCurrentStatus = solveCube();
    currPhase = locateCurrPhase();    
    resetNumSteps();
    setCurrPhaseStartStep(getNumSteps());
    setRemStepsInCurrPhase();
}

void Cubik::setRemStepsInCurrPhase() {
    if ((currPhase >= 0) && (currPhase < 7))
        remStepsInCurrPhase = solutionToCurrentStatus[currPhase];
}

int Cubik::getPhaseLength() {
    if (currPhase == 7)
        return 0;
    else if ((currPhase >= 0) && (currPhase < 7))
        return ((int) solutionToCurrentStatus[currPhase].size());
    else
        return -1;
}

int Cubik::getPhaseLength(std::vector<std::vector<std::string> > tmpSoln, int tmpPhase) {
    if (tmpPhase == 7)
        return 0;
    else if ((tmpPhase >= 0) && (tmpPhase < 7))
        return ((int) tmpSoln[currPhase].size());
    else
        return -1;
}

bool Cubik::checkSolved() {
    for (auto iter = currentStatus.begin(), jter = solvedCube.begin();
         iter != currentStatus.end(), jter != solvedCube.end();
         ++iter, ++jter) {
        if ((*iter) != (*jter))
            return false;
    }
    return true;
}

qglviewer::Vec Cubik::relativePositionByPosition(Cube * ChildCube, Cube * ParentCube) {
    return (ChildCube->getCubeFrame()->position() - ParentCube->getCubeFrame()->position());
}

int Cubik::locateCurrPhase() {
    int userPhase = 0;
    for (auto iter = solutionToCurrentStatus.begin(); iter != solutionToCurrentStatus.end(); ++iter) {
        if (!(*iter).empty())
            break;
        userPhase++;
    }
    return userPhase;
}

int Cubik::locatePhase(std::vector<std::vector<std::string> > tmpSoln) {
    int userPhase = 0;
    for (auto iter = tmpSoln.begin(); iter != tmpSoln.end(); ++iter) {
        if (!(*iter).empty())
            break;
        userPhase++;
    }
    return userPhase;
}

std::vector<std::string> Cubik::localSimplify(std::vector<std::string> original) {
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

bool Cubik::vecStringEqual(std::vector<std::string> original, std::vector<std::string> simplified) {
    for (auto iter = original.begin(), jter = simplified.begin();
         iter != original.end(), jter != simplified.end();
         ++iter, ++jter) {
        if ((*iter) != (*jter))
            return false;
    }
    return true;
}

std::vector<std::string> Cubik::iterSimplify(std::vector<std::string> original) {
    while (!vecStringEqual(original, localSimplify(original)))
        original = localSimplify(original);
    return original;
}

std::vector<std::vector<std::string> > Cubik::iterSimplify(std::vector<std::vector<std::string> > result) {
    for (std::vector<std::vector<std::string> >::iterator iter = result.begin(); iter != result.end(); ++iter) {
        *iter = iterSimplify(*iter);
    }
    return result;
}

void Cubik::addHeadRemStep(std::string move) {
    if ((move.size() == 2) && (move[1] == '2')) {
        remStepsInCurrPhase.insert(remStepsInCurrPhase.begin(), move.substr(0,1));
        remStepsInCurrPhase.insert(remStepsInCurrPhase.begin(), move.substr(0,1));
    } else
        remStepsInCurrPhase.insert(remStepsInCurrPhase.begin(), move);
    remStepsInCurrPhase = iterSimplify(remStepsInCurrPhase);
}
