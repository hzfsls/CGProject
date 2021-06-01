//
// Created by 85339 on 2020/12/10.
//

#ifndef CG_PROJECT_KNIFE_H
#define CG_PROJECT_KNIFE_H

#include <vector>


using namespace std;

class knife {
public:
    vector<double> ds;
    double depth;

    knife(vector<double> ds);
    void render();
};


#endif //CG_PROJECT_KNIFE_H
