//
// Created by 85339 on 2020/12/18.
//

#include "particleSystem.h"
#include "GL/glut.h"
#include <random>
#include <ctime>

const double gravity_acceleration = 0.03;
const int particle_lifetime = 300;

particle::particle(double x, double y, double z, double vx, double vy, double vz) {
    position[0] = x;
    position[1] = y;
    position[2] = z;
    velocity[0] = vx;
    velocity[1] = vy;
    velocity[2] = vz;
}

void particle::next_frame() {
    position[0] += velocity[0];
    position[1] += velocity[1];
    position[2] += velocity[2];
    velocity[1] -= gravity_acceleration;
}

particle_group::particle_group(int size, double x0, double x1, double y, double z, double vx, double vy, double vz,
                               unsigned int mat) : mat(mat) {
    default_random_engine r(time(nullptr));
    for (int i = 0; i < size; ++i) {
        double r1 = (r() % 101 - 50) / 500.0;
        double r2 = (r() % 101 - 50) / 500.0;
        double r3 = (r() % 101 - 50) / 500.0;
        double x = (r() % 101 / 100.0) * (x1 - x0) + x0;
        double yr = ((r() % 101) / 100.0) * 0.6 + y;
        particles.emplace_back(x, yr, z, vx + r1, vy + r2, vz + r3);
    }
    this->size = size;
    lifetime = particle_lifetime;
    alive = true;
}

void particle_group::next_frame() {
    for (int i = 0; i < size; ++i)
        particles[i].next_frame();
    if (lifetime <= 0) {
        alive = false;
        return;
    }
    lifetime--;


}

void particle_group::render() {
    default_random_engine r(time(nullptr));
    glBindTexture(GL_TEXTURE_2D, mat);
    glBegin(GL_POINTS);
    for (int i = 0; i < size; ++i) {
        glTexCoord2f((r() % 1024) / 1023.0, (r() % 1024) / 1023.0);
        glVertex3d(particles[i].position[0], particles[i].position[1], particles[i].position[2]);
    }
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

bool particle_group::is_alive() const {
    return alive;
}

particleSystem::particleSystem() = default;


void particleSystem::create_particle(particle_group &pg) {
    groups.push_back(pg);
}

void particleSystem::next_frame() {
    for (auto it = groups.begin(); it < groups.end();) {
        it->next_frame();
        if (!it->is_alive()) it = groups.erase(it);
        else it++;
    }
}

void particleSystem::render() {
    glPointSize(2);
    int size = groups.size();
    for (int i = 0; i < size; i++) groups[i].render();
}
