//
// Created by 85339 on 2020/12/11.
//

#ifndef CG_PROJECT_BEZIERBOUNDARY_H
#define CG_PROJECT_BEZIERBOUNDARY_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class bezierBoundary {

public:
    int num;
    double move_last_x;
    double move_last_y;
    bool complete;
    bool move_state;
    vector<pair<double,double>> points;
    vector<pair<double,double>> line;

    bezierBoundary();

    void reset();
    void commit();

    void addPoint(double x,double y);
    void renderPoints();
    void renderCurve();
    void renderCurveSurfave();
    double bound(double x);

    void startMove(double x,double y);
    void moveTo(double x,double y);
    void endMove();

    void render(int state);


};


#endif //CG_PROJECT_BEZIERBOUNDARY_H
