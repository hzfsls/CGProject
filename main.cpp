#include <GL\glut.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <vector>
#include "workpiece.h"
#include "knifeState.h"
#include "particleSystem.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

static const int step = 50;
unsigned int background_texture;
unsigned int wood_texture;
unsigned int metal_texture;
GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_diffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat low_shininess[] = {5.0};
GLfloat mat_emission[] = {0.2, 0.2, 0.2, 0.0};
GLfloat light0_position[] = {10, 10, 10, 1};
GLfloat light1_position[] = {-10, -10, 10, 1};
GLfloat light_color[] = {1,1,1,1};
GLfloat light_diffuse[] = {1,1,1,1};
GLfloat light_specular[] = {1,1,1,1};
double rot_angle = 0;

int state = 0;

vector<double> boundary;
workpiece *w = nullptr;
knifeState *k = nullptr;
bezierBoundary b;
particleSystem p;

void FlushState() {
    if (state == 0) {
        glEnable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, 1, 0.1, 100);
    } else if (state == 1) {
        //glDisable(GL_LIGHTING);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-12, 12, -12, 12, -10, 10);
    }
}

void ChangeState() {
    if (state == 0) {
        state = 1;
    } else if (state == 1) {
        state = 0;
    }
    FlushState();
}

void Reshape(GLint w, GLint h) {

    glViewport(0, 0, w, h);
    FlushState();


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void KeyProcess(unsigned char key, int xx, int yy) {
    const double move_step = 0.05;
    switch (key) {
        case 27:
            exit(0);
            break;
        case 'q': // 切换到上一个刀具
            k->changeKnife(-1);
            break;
        case 'e': // 切换到下一个刀具
            k->changeKnife(1);
            break;
        case 'w': // 锁定水平方向，只能进退刀
            k->changeLock(1);
            break;
        case 's': // 锁定竖直方向，只能左右移动刀
            k->changeLock(-1);
            break;
        case 'r': // 恢复工件形状至初始
            w->reset(18.4, 5);
            break;
        case 'f': // 改变工件材料
            w->change_mat();
            break;
        case 'b': // 进入曲线绘制模式
            ChangeState();
            break;
        case 'n': // 重置曲线
            if(state == 1){
                b.reset();
            }
            break;
        case 'v': // 提交一个已经画好的曲线
            if(state == 1){
                b.commit();
                //ChangeState();
            }
            break;
        default:
            break;
    }
}

void MouseProcess(int button, int mouse_state, int xx, int yy) {
    if (state == 0) {

    }
    if (state == 1) {
        double x = (xx - 250 / 6.0) / 250.0 * 12.0;
        double y = -(yy - 250) / 250.0 * 12.0;
        if (button == GLUT_LEFT_BUTTON && mouse_state == GLUT_DOWN) {
            if( b.num < 4 ) b.addPoint(x, y);
            else if (!b.move_state) b.startMove(x,y);
            else b.endMove();
        }
    }
}

void MouseMotionProcess(int xx, int yy) {
    if (state == 0) {
        double x = xx / 25.0;
        double y = -(yy - 250) / 25.0;
        k->moveKnife(b, x, y);
        k->cutWorkpiece(p,w);
    }
    if(state == 1) {
        double x = (xx - 250 / 6.0) / 250.0 * 12.0;
        double y = -(yy - 250) / 250.0 * 12.0;
    }
}


void MousePassiveMotionProcess(int xx, int yy) {
    if (state == 0) {
        double x = xx / 25.0;
        double y = -(yy - 250) / 25.0;
        k->moveKnife(b, x, y);
    }
    if(state == 1) {
        double x = (xx - 250 / 6.0) / 250.0 * 12.0;
        double y = -(yy - 250) / 250.0 * 12.0;
        if(b.move_state) b.moveTo(x,y);
    }
}

unsigned int loadTexture(char const * path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = GL_RGBA;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void MyInit() {

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_COLOR, light_color);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    background_texture = loadTexture("../images/room.jpg");
    wood_texture = loadTexture("../textures/wood_albedo.jpg");
    metal_texture = loadTexture("../textures/metal_albedo.jpg");

    w = new workpiece(18.4, 5, workpiece::WOOD,wood_texture,metal_texture);
    k = new knifeState();
    k->setTexture(metal_texture,wood_texture);
    /*
    w->cut(0, 9.2, 2.3, 3.7);
    w->cut(9.2, 18.4, 3.7, 1.6);
     */

    glEnable(GL_TEXTURE_2D);
}

void Display() {
    rot_angle += 3;
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (state == 0)
        gluLookAt(0, 0, 30, 0, 0, 0, 0, 1, 0);

    /*
    glBegin(GL_POLYGON);
    printf("draw r: %f\n",0.2);
    for (int j = 0; j < 20; j++) {
        double r = 0.2;
        double angle = 2 * 3.1415 * j / 20;
        glVertex3d(0.5, cos(angle) * r, sin(angle) * r);
    }
    glEnd();*/
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glTranslated(-10, 0, 0);

    if (state == 0) {
        glPushMatrix();
        glBindTexture(GL_TEXTURE_2D, background_texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(-15, 25, -20);
        glTexCoord2f(0, 1);
        glVertex3f(-15, -25, -20);
        glTexCoord2f(1, 1);
        glVertex3f(35, -25, -20);
        glTexCoord2f(1, 0);
        glVertex3f(35, 25, -20);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }


    glPushMatrix();
    glRotated(rot_angle, 20, 0, 0);
    w->render();
    glPopMatrix();

    if (state == 0) {
        glPushMatrix();
        k->render();
        glPopMatrix();
        glPushMatrix();
        b.render(0);
        glPopMatrix();
        glPushMatrix();
        p.render();
        p.next_frame();
        glPopMatrix();
    }
    if (state == 1) {
        glPushMatrix();
        b.render(1);
        glPopMatrix();
    }

    glutSwapBuffers();
    //Sleep(1000);
}
void Timer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(16, Timer, 1);
}
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(400, 100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("模拟车床加工");

    MyInit();

    glutReshapeFunc(Reshape);
    glutKeyboardFunc(KeyProcess);
    glutMouseFunc(MouseProcess);

    glutMotionFunc(MouseMotionProcess);
    glutPassiveMotionFunc(MousePassiveMotionProcess);
    glutIdleFunc(Display);
    glutDisplayFunc(Display);

    glutTimerFunc(16, Timer, 1);

    glutMainLoop();
    return 0;
}

