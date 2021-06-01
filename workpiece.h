//
// Created by 85339 on 2020/12/6.
//

#ifndef CG_PROJECT_WORKPIECE_H
#define CG_PROJECT_WORKPIECE_H

#include <vector>
#include "knife.h"
#include "particleSystem.h"

using namespace std;

class workpiece {
    static int length_to_block(double length);
    static double block_to_length(int block);
private:
    vector<double> rs;
    unsigned int wood_texture;
    unsigned int metal_texture;
    double max_r;


public:
    enum mat {
        METAL,
        WOOD
    } material;

    workpiece(double length, double r, mat material,unsigned int wood_texture,unsigned int metal_texture);

    void reset(double length, double r);

    void change_mat();

    void cut(double pos, double r);

    void cut(double pos1, double pos2, double r1, double r2);

    void cut(particleSystem &p,const knife &k, double pos, double depth);

    void render();

    ~workpiece();



};


#endif //CG_PROJECT_WORKPIECE_H
