//
// Created by 85339 on 2020/12/6.
//

#include "workpiece.h"
#include <GL\glut.h>

static const double PI = 3.14159265358979323846;
static const int step = 50;

static GLfloat default_emission[] = {0.2,0.2,0.2,0.0};

static GLfloat wood_ambient[] = {0.6, 0.6, 0.6, 1.0};
static GLfloat wood_diffuse[] = {0.6, 0.6, 0.6 , 1.0};
static GLfloat wood_specular[] = {0.4, 0.4, 0.4, 1.0};
static GLfloat wood_rough_shininess[] = {0.4};
static GLfloat wood_smooth_shininess[] = {0.4};

static GLfloat metal_ambient[] = {0.19225, 0.19225, 0.19225, 1.0};
static GLfloat metal_diffuse[] = {0.50754, 0.50754, 0.50754, 1.0};
static GLfloat metal_specular[] = {0.508273, 0.508273, 0.508273, 1.0};
static GLfloat metal_rough_shininess[] = {0.4};
static GLfloat metal_smooth_shininess[] = {80};

int workpiece::length_to_block(double length) {
    return (int) (length * step);
}

double workpiece::block_to_length(int block) {
    return (double) block / step;
}

workpiece::workpiece(double length, double r, mat material, unsigned int wood_texture, unsigned int metal_texture) :
        material(material), wood_texture(wood_texture), metal_texture(metal_texture),max_r(r) {
    int size = length_to_block(length);
    printf("size: %d\n", size);
    rs.resize(size + 1, r);
}

workpiece::~workpiece() = default;

void workpiece::render() {
    int q = 360; //画圆时切成的份数
    int size = rs.size();

    if(material == METAL)  {
        glBindTexture(GL_TEXTURE_2D, metal_texture);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, metal_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, metal_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, metal_specular);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, wood_texture);

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wood_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wood_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wood_specular);
    }
    //glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_POLYGON);
    glNormal3d(-1, 0, 0);
    for (int j = 0; j < q; j++) {
        double r = rs[0];
        double angle = 2 * PI * j / q;
        glTexCoord2f(cos(angle)*0.5*r/max_r+0.5, sin(angle)*0.5*r/max_r+0.5);
        glVertex3d(block_to_length(0), cos(angle) * r, sin(angle) * r);
    }
    glEnd();

    //glColor3d(0.0, 1.0, 0.0);
    for (int i = 0; i < size - 1; i++) {
        glBegin(GL_QUADS);
        double r1 = rs[i];
        double r2 = rs[i + 1];
        if(r1 == r2 && r1 == max_r){
            if(material == METAL)
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, metal_rough_shininess);
            else{
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, wood_rough_shininess);
            }
        }
        else{
            if(material == METAL)
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, metal_smooth_shininess);
            else{
                glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, wood_smooth_shininess);
            }

        }
        for (int j = 0; j < q; j++) {
            double angle = 2 * PI * j / q;
            double angle2 = 2 * PI * (j + 1) / q;
            double midAngle = (angle + angle2) / 2;
            double normal[3] = {0, 0, 0};
            normal[0] = r1 - r2;
            normal[1] = block_to_length(1) * cos(midAngle);
            normal[2] = block_to_length(1) * sin(midAngle);
            double normal_sqr = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
            normal[0] /= normal_sqr;
            normal[1] /= normal_sqr;
            normal[2] /= normal_sqr;
            glNormal3d(normal[0], normal[1], normal[2]);
            glTexCoord2f(i*1.0/size,angle/(2*PI));
            glVertex3d(block_to_length(i), cos(angle) * r1, sin(angle) * r1);
            glTexCoord2f((i+1)*1.0/size,angle/(2*PI));
            glVertex3d(block_to_length(i + 1), cos(angle) * r2, sin(angle) * r2);
            glTexCoord2f((i+1)*1.0/size,angle2/(2*PI));
            glVertex3d(block_to_length(i + 1), cos(angle2) * r2, sin(angle2) * r2);
            glTexCoord2f(i*1.0/size,angle2/(2*PI));
            glVertex3d(block_to_length(i), cos(angle2) * r1, sin(angle2) * r1);
        }
        glEnd();

    }

    glBegin(GL_POLYGON);
    glNormal3d(0, 0, 1);
    for (int j = 0; j < q; j++) {
        double r = rs[size - 1];
        double angle = 2 * PI * j / q;
        glTexCoord2f(cos(angle)*0.5*r/max_r+0.5, sin(angle)*0.5*r/max_r+0.5);
        glVertex3d(block_to_length(size - 1), cos(angle) * r, sin(angle) * r);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}

void workpiece::cut(double pos, double r) {

}

void workpiece::cut(double pos1, double pos2, double r1, double r2) {
    int block_pos1 = length_to_block(pos1);
    int block_pos2 = length_to_block(pos2);
    if (block_pos1 > block_pos2) {
        auto tmp = block_pos1;
        block_pos1 = block_pos2;
        block_pos2 = tmp;
        tmp = r1;
        r1 = r2;
        r2 = tmp;
    }
    if (block_pos1 >= rs.size()) return;
    if (block_pos2 >= rs.size()) block_pos2 = rs.size() - 1;
    for (int pos = block_pos1; pos <= block_pos2; pos++) {
        double r = r1 + (r2 - r1) * (pos - block_pos1) / (block_pos2 - block_pos1);
        double old_r = rs[pos];
        if (old_r < r) continue;
        else rs[pos] = r;
    }
}

void workpiece::cut(particleSystem &p, const knife &k, double pos, double depth) {
    int block_pos = length_to_block(pos);
    int size = k.ds.size();
    double cut_amount = 0;
    for (int i = 0; i < size; i++) {
        double new_r = depth + k.depth - k.ds[i];
        double old_r = rs[block_pos + i];
        if (old_r < new_r - 0.1) continue;
        else if (old_r < new_r && old_r > new_r - 0.1)
            cut_amount += (old_r - new_r + 0.1);
        else {
            rs[block_pos + i] = new_r;
            cut_amount += (old_r - new_r + 0.1);
        }
    }
    int particle_amount = (int) (cut_amount / 0.1);
    particle_group g(particle_amount, pos, pos + block_to_length(size), -(depth + k.depth), 0, 0, 0, 0.2,material);
    p.create_particle(g);
    //printf("cut amount:%f\n", cut_amount);
}

void workpiece::change_mat() {
    if (material == WOOD) {
        material = METAL;
    } else
        material = WOOD;
}

void workpiece::reset(double length, double r) {
    int size = length_to_block(length);
    rs.resize(size + 1, r);
    rs.assign(size + 1, r);
}






