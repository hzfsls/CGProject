//
// Created by 85339 on 2020/12/11.
//

#include "knifeState.h"
#include "GL/glut.h"

static GLfloat metal_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
static GLfloat metal_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
static GLfloat metal_specular[] = {0.508273, 0.508273, 0.508273, 1.0};
static GLfloat metal_smooth_shininess[] = {128};

static const int step = 50;
enum knife_type {
    FLAT,
    LEFT_TILT,
    RIGHT_TILT,
    TRIANGLE,
    ROUND,
};

static vector<double> genKnife(knife_type type, double width, double depth) {
    vector<double> ds;
    int size = step * width;
    ds.reserve(size);
    switch (type) {
        case FLAT:
            for (int i = 0; i < size; i++) ds.push_back(depth);
            break;

        case LEFT_TILT:
            for (int i = 0; i < size; i++) ds.push_back(depth * (size - i) / size);
            break;

        case RIGHT_TILT:
            for (int i = 0; i < size; i++) ds.push_back(depth * (i + 1) / size);
            break;

        case TRIANGLE:
            for (int i = 0; i < size / 2; i++) ds.push_back(2 * depth * (i + 1) / size);
            for (int i = size / 2; i < size; i++) ds.push_back(2 * depth * (size - i) / size);
            break;

        case ROUND:
            for (int i = 0; i < size ; i++) ds.push_back(depth * sin(acos((2.0*i - size) / size)));
            break;
    }
    return ds;
};

knifeState::knifeState() {
    knifePosX = 0;
    knifePosY = -5;
    ls = FREE;
    knives.emplace_back(genKnife(FLAT,0.6,0.6));
    knives.emplace_back(genKnife(ROUND,0.6,0.6));
    knives.emplace_back(genKnife(LEFT_TILT,0.6,0.6));
    knives.emplace_back(genKnife(RIGHT_TILT,0.6,0.6));
    knives.emplace_back(genKnife(TRIANGLE,0.6,0.6));

    currentKnife = 0;
}

void knifeState::render() {
    glBindTexture(GL_TEXTURE_2D, metal_texture);

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, metal_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, metal_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metal_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, metal_smooth_shininess);
    glTranslated(knifePosX, knifePosY, 0);
    knives[currentKnife].render();
    glBindTexture(GL_TEXTURE_2D,0);
}

void knifeState::cutWorkpiece(particleSystem &p,workpiece *w) {
    w->cut(p,knives[currentKnife], knifePosX, -knifePosY);
}

void knifeState::moveKnife(bezierBoundary& boundary,double x, double y) {
    if (x <= 0) x = 0;
    else if (x >= 20) x = 20;
    if (y >= boundary.bound(x)) y = boundary.bound(x);
    else if (y <= -10) y = -10;
    if (ls == FREE || ls == Y_LOCKED) knifePosX = x;
    if (ls == FREE || ls == X_LOCKED) knifePosY = y;
}

void knifeState::changeKnife(int i) {
    if (i == 1) currentKnife = (currentKnife + 1) % knives.size();
    else if (i == -1) currentKnife = (currentKnife + knives.size() - 1) % knives.size();
}

void knifeState::changeLock(int i) {
    if (i == 1) {
        if (ls == FREE || ls == Y_LOCKED) ls = X_LOCKED;
        else ls = FREE;
    } else if (i == -1) {
        if (ls == FREE || ls == X_LOCKED) ls = Y_LOCKED;
        else ls = FREE;
    }
}

void knifeState::setTexture(unsigned int metal_tex, unsigned int wood_tex) {
    metal_texture = metal_tex;
    wood_texture = wood_tex;
}

