//
// Created by 85339 on 2020/12/18.
//

#ifndef CG_PROJECT_PARTICLESYSTEM_H
#define CG_PROJECT_PARTICLESYSTEM_H
#include <vector>

using namespace std;
class particle {
public:
    double position[3];
    double velocity[3];
    particle(double x,double y,double z,double vx,double vy,double vz);
    void next_frame();
};

class particle_group {
private:
    int size;
    int lifetime;
    bool alive;
    unsigned int mat;
    vector<particle> particles;
public:
    particle_group(int size, double x0, double x1,double y,double z,double vx,double vy,double vz,unsigned int mat);
    void next_frame();
    bool is_alive() const;
    void render();
};

class particleSystem {
private:
    vector<particle_group> groups;

public:
    particleSystem();
    void create_particle(particle_group &pg);
    void next_frame();
    void render();




};


#endif //CG_PROJECT_PARTICLESYSTEM_H
