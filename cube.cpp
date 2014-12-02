#include "cube.h"
#include <iostream>

/**************************************************************/
/************************* CubeFrame **************************/
/**************************************************************/


/**************************************************************/
/**************************** Cube ****************************/
/**************************************************************/
// cube ////////////////////////////////////////////////////////
//    v6----- v5
//   /|      /|
//  v1------v0|
//  | |     | |
//  | |v7---|-|v4
//  |/      |/
//  v2------v3

static GLubyte indices[36]  = { 0, 1, 2,   2, 3, 0,      // front
                                4, 5, 6,   6, 7, 4,      // right
                                8, 9,10,  10,11, 8,      // top
                                12,13,14,  14,15,12,      // left
                                16,17,18,  18,19,16,      // bottom
                                20,21,22,  22,23,20 };    // back

static GLfloat vertices[] = {  1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3 (front)
                               1, 1, 1,   1,-1, 1,   1,-1,-1,   1, 1,-1,   // v0,v3,v4,v5 (right)
                               1, 1, 1,   1, 1,-1,  -1, 1,-1,  -1, 1, 1,   // v0,v5,v6,v1 (top)
                               -1, 1, 1,  -1, 1,-1,  -1,-1,-1,  -1,-1, 1,   // v1,v6,v7,v2 (left)
                               -1,-1,-1,   1,-1,-1,   1,-1, 1,  -1,-1, 1,   // v7,v4,v3,v2 (bottom)
                               1,-1,-1,  -1,-1,-1,  -1, 1,-1,   1, 1,-1 }; // v4,v7,v6,v5 (back)

static GLfloat normals[]  = {  0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
                               1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
                               0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
                               -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
                               0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
                               0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

Cube::Cube() {
}

Cube::Cube(std::string cubetype) {
    Cube();
    CubeType = cubetype;
    cubeFrame = new CubeFrame();
}

Cube::Cube(std::string cubetype, CubeFrame * cubeFrame_) {
    Cube();
    CubeType = cubetype;
    cubeFrame = cubeFrame_;
}

Cube::~Cube() {
}

void Cube::draw() {
    GLfloat colors[72] = {0};
    if (find(CubeType.begin(), CubeType.end(), 'U') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[24+3*j] = 0.8;
            colors[24+3*j+1] = 0.8;
        }
    }
    if (find(CubeType.begin(), CubeType.end(), 'D') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[48+3*j] = 0.8;
            colors[48+3*j+1] = 0.8;
            colors[48+3*j+2] = 0.8;
        }
    }
    if (find(CubeType.begin(), CubeType.end(), 'F') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[3*j+2] = 0.5;
        }
    }
    if (find(CubeType.begin(), CubeType.end(), 'B') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[60+3*j+1] = 0.6;
        }
    }
    if (find(CubeType.begin(), CubeType.end(), 'L') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[36+3*j] = 1;
            colors[36+3*j+1] = 0.4;
            // colors[36+3*j+2] = 1;
        }
    }
    if (find(CubeType.begin(), CubeType.end(), 'R') != CubeType.end()) {
        for (int j = 0; j < 4; j++) {
            colors[12+3*j] = 0.8;
        }
    }
    // GLfloat colors[]   = {  0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front "B", blue)
    //                         1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right "R", red)
    //                         1, 1, 0,   1, 1, 0,   1, 1, 0,   1, 1, 0,   // v0,v5,v6,v1 (top "U", yellow)
    //                         1,0.5,0,   1,0.5,0,   1,0.5,0,   1,0.5,0,   // v1,v6,v7,v2 (left "L", orange)
    //                         1, 1, 1,   1, 1, 1,   1, 1, 1,   1, 1, 1,   // v7,v4,v3,v2 (bottom "D", white)
    //                         0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0 }; // v4,v7,v6,v5 (back "B", green)

    glEnable(GL_LIGHTING);
    
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    
    glDrawRangeElements(GL_TRIANGLES, 0, 11, 18, GL_UNSIGNED_BYTE, indices);
    glDrawRangeElements(GL_TRIANGLES, 12, 23, 18, GL_UNSIGNED_BYTE, indices+18);

    glDisableClientState(GL_VERTEX_ARRAY);  // disable vertex arrays
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

bool CubeFrame::checkAlignedWithFrame (const CubeFrame * const frame) {
    qglviewer::Vec directions[2][3];
    for (int d=0; d<3; ++d) {
        qglviewer::Vec dir((d==0)? 1.0 : 0.0, (d==1)? 1.0 : 0.0, (d==2)? 1.0 : 0.0);
        if (frame)
            directions[0][d] = frame->inverseTransformOf(dir);
        else
            directions[0][d] = dir;
        directions[1][d] = inverseTransformOf(dir);
    }

    float maxProj = 0.0f;
    float proj;
    unsigned short index[2];
    index[0] = index[1] = 0;
    for (int i=0; i<3; ++i)
        for (int j=0; j<3; ++j)
            if ( (proj=fabs(directions[0][i]*directions[1][j])) >= maxProj ) {
                index[0] = i;
                index[1] = j;
                maxProj  = proj;
            }
    float coef = directions[0][index[0]] * directions[1][index[1]];
    qglviewer::Vec axis = cross(directions[0][index[0]], directions[1][index[1]]);
    float angle = asin(axis.norm());
    if (coef >= 0.0)
        angle = -angle;
    rotate(rotation().inverse() * qglviewer::Quaternion(axis, angle) * orientation());
    // first aligned axis have been determined (by construction, angle must be 0)
    // now we check the alignment of the second axis
    for (int i=0; i<3; ++i) {
        if (i == index[0])
            continue;
        for (int j=0; j<3; ++j) {
            if (j == index[1])
                continue;
            float innerProduct = directions[0][i] * directions[1][j];
            if (fabs(innerProduct) < 5e-2) {
                // directions[0][i] is almost orthogonal to directions[1][j]
                // let's find out which axis it should align with
                int flag = -1;
                for (int k = 0; k < 3; k++) {
                    if ((k == index[1]) || (k == j))
                        continue;
                    flag = k;
                }
                axis = cross(directions[0][i], directions[1][flag]);
                angle = asin(axis.norm());
                if (directions[0][i] * directions[1][flag] >= 0)
                    angle = -angle;
                rotate(rotation().inverse() * qglviewer::Quaternion(axis, angle) * orientation());
                return true;
            }
        }
    }
    
    return false;
}

qglviewer::Vec Cube::getTranslationVector() {
    qglviewer::Vec tVec(0, 0, 0);
    for (std::string::size_type j = 0; j < CubeType.size(); j++) {
        tVec += faceToNormal(CubeType[j]);
    }
    return tVec;
}

qglviewer::Vec Cube::faceToNormal(char fId) {
    if (fId == 'U')
        return qglviewer::Vec( 0.0f, 1.0f, 0.0f);
    else if (fId == 'D')
        return qglviewer::Vec( 0.0f,-1.0f, 0.0f);
    else if (fId == 'F')
        return qglviewer::Vec( 0.0f, 0.0f, 1.0f);
    else if (fId == 'B')
        return qglviewer::Vec( 0.0f, 0.0f,-1.0f);
    else if (fId == 'L')
        return qglviewer::Vec(-1.0f, 0.0f, 0.0f);
    else if (fId == 'R')
        return qglviewer::Vec( 1.0f, 0.0f, 0.0f);
    else
        return qglviewer::Vec( 0.0f, 0.0f, 0.0f);
}

int Cube::faceToFaceIdx(char fId) {
    if (fId == 'U')
        return 0;
    else if (fId == 'D')
        return 1;
    else if (fId == 'F')
        return 2;
    else if (fId == 'B')
        return 3;
    else if (fId == 'L')
        return 4;
    else if (fId == 'R')
        return 5;
    else
        return -1;
}
