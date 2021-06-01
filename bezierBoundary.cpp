//
// Created by 85339 on 2020/12/11.
//

#include "bezierBoundary.h"
#include "GL/glut.h"

bezierBoundary::bezierBoundary() {
    num = 0;
    complete = false;
    move_last_x = 0;
    move_last_y = 0;
    move_state = false;

}

void bezierBoundary::addPoint(double x, double y) {
    if (num >= 4) return;
    if (y > -0.1) y = -0.1;
    points.emplace_back(x, y);
    num++;
}

double bezierBoundary::bound(double x) {
    if(!complete)
        return -0.1;
    else{
        auto it = upper_bound(line.begin(),line.end(),make_pair(x,0.0));
        if (it == line.begin()) return -0.1;
        if (it == line.end()) return -0.1;
        double x1 = (it-1)->first;
        double y1 = (it-1)->second;
        double x2 = it->first;
        double y2 = it->second;
        double y = y1 + (x-x1) * (y2-y1) /(x2-x1);
        if(y > -0.1) y = -0.1;
        return y;
    }
}

void bezierBoundary::renderPoints() {
    glDisable(GL_LIGHTING);
    glPointSize(3);
    glColor3d(1,0,0);
    glBegin(GL_POINTS);
    for (int i = 0; i < num; i++) {
        glVertex3d(points[i].first, points[i].second, 5);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void bezierBoundary::renderCurve() {
    glDisable(GL_LIGHTING);
    glPointSize(1);
    glLineWidth(2);
    if(!complete) glColor3d(1,0,0);
    else glColor3d(0,1,0);
    glBegin(GL_LINE_STRIP);
    double t;
    for (int i = 0; i <= 1000; i++) {
        t = i / 1000.0;
        double x = points[0].first * (1 - t) * (1 - t) * (1 - t) +
                   3 * points[1].first * t * (1 - t) * (1 - t) +
                   3 * points[2].first * t * t * (1 - t) +
                   points[3].first * t * t * t;
        double y = points[0].second * (1 - t) * (1 - t) * (1 - t) +
                   3 * points[1].second * t * (1 - t) * (1 - t) +
                   3 * points[2].second * t * t * (1 - t) +
                   points[3].second * t * t * t;
        glVertex3d(x, y, 5);
    }
    glEnd();
    glEnable(GL_LIGHTING);
}

void bezierBoundary::renderCurveSurfave()
{
    if(!complete) return;
    glDisable(GL_LIGHTING);
    glColor3d(1,0,0);
    glBegin(GL_LINE_STRIP);
    glVertex3d(-1, -0.1,0);
    glVertex3d(line[0].first, -0.1,0);

    for (int i = 0; i <= 1000; i++) {
        double x = line[i].first;
        double y = line[i].second;
        glVertex3d(x, y, 0);
    }
    glVertex3d(line[1000].first, -0.1,0);
    glVertex3d(20, -0.1,0);
    glEnd();
    glEnable(GL_LIGHTING);
}

void bezierBoundary::render(int state) {
    if(state == 1) {
        if (num < 4) {
            renderPoints();
            return;
        } else if (num == 4) {
            //renderPoints();
            renderCurve();
        }
    }
    else if (state == 0){
        renderCurveSurfave();
    }

}

void bezierBoundary::reset() {
    complete = false;
    num = 0;
    points.clear();
    line.clear();
}

void bezierBoundary::commit() {
    if (num != 4) reset();
    else{
        double last_x;
        bool upper = false;
        double t;
        for (int i = 0; i <= 1000; i++) {
            t = i / 1000.0;
            double x = points[0].first * (1 - t) * (1 - t) * (1 - t) +
                       3 * points[1].first * t * (1 - t) * (1 - t) +
                       3 * points[2].first * t * t * (1 - t) +
                       points[3].first * t * t * t;
            double y = points[0].second * (1 - t) * (1 - t) * (1 - t) +
                       3 * points[1].second * t * (1 - t) * (1 - t) +
                       3 * points[2].second * t * t * (1 - t) +
                       points[3].second * t * t * t;
            if(i==0){
                last_x = x;
            }
            else if(i==1){
                if(x > last_x) upper = true;
                else upper = false;
                last_x = x;
            }
            else {
                if((upper && x < last_x) ||(!upper && x > last_x)) {
                    printf("invalid curve. commit failed\n");
                    reset();
                    return;
                }
                last_x = x;
            }
            line.emplace_back(x,y);
            sort(line.begin(),line.end());

        }
        complete = true;
    }
}

void bezierBoundary::startMove(double x, double y) {
    if (complete) return;
    move_last_x = x;
    move_last_y = y;
    move_state = true;

}

void bezierBoundary::moveTo(double x, double y) {
    if (complete) return;
    double delta_x = x - move_last_x;
    double delta_y = y - move_last_y;
    move_last_x = x;
    move_last_y = y;
    for (int i = 0; i < num; i++) {
       points[i].first += delta_x;
       points[i].second += delta_y;
    }
}

void bezierBoundary::endMove() {
    move_state = false;
}