//
// Created by 85339 on 2020/12/11.
//

#ifndef CG_PROJECT_KNIFESTATE_H
#define CG_PROJECT_KNIFESTATE_H
#include "knife.h"
#include "workpiece.h"
#include "bezierBoundary.h"
#include "particleSystem.h"

class knifeState {
public:
    vector<knife> knives;
    double knifePosX;
    double knifePosY;
    int currentKnife;
    unsigned int metal_texture;
    unsigned int wood_texture;
    enum lock_state {
        FREE,
        X_LOCKED,
        Y_LOCKED,
    } ls;


    knifeState();
    void setTexture(unsigned int metal_texture,unsigned int wood_texture);
    void render();
    void moveKnife(bezierBoundary& boundary, double x, double y);
    void cutWorkpiece(particleSystem &p,workpiece* w);
    void changeKnife(int i);    // i=1Ϊ��ǰ�л���i=-1Ϊ����л�
    void changeLock(int i);     // i=1Ϊ�л�x�ᣬi=-1λ�л�y��

};


#endif //CG_PROJECT_KNIFESTATE_H
