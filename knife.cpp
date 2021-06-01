//
// Created by 85339 on 2020/12/10.
//

#include "knife.h"
#include <GL\glut.h>

#include <utility>

static const int step = 50;

knife::knife(vector<double> ds) {
    int size = ds.size();
    depth = 0;
    for (int i = 0; i < size; i++) {
        if (ds[i] > depth) depth = ds[i];
    }
    this->ds = std::move(ds);

}

void knife::render() {
    glBegin(GL_POLYGON);
    glNormal3d(0, 0, 1);
    int size = ds.size();
    glVertex3d(0, -2.4, 0);
    for (int i = 0; i < size; i++) {
        glVertex3d((double) i / step, ds[i] - depth, 0);
    }
    glVertex3d((double) (size - 1) / step, -2.4, 0);

    glEnd();
}
