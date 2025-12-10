#include "sdk/LakiBeamUDP.h"
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <algorithm>
#include <boost/asio.hpp>
#include <iostream>
#include <mutex>
// #include <test/opengl_test.h>
#include <thread>
#include <vector>

#include <cmath>
#include <GL/freeglut.h>

using boost::asio::ip::udp;

// shared data
std::mutex lidar_mutex;
// repark_t point_data;
repark_t  pack;
// thread ctrl label
std::atomic<bool> g_running(true);
// the udp connection
std::unique_ptr<LakiBeamUDP> ld_udp_ptr;


//--------------------------------
// reading dot cloud through udp
//--------------------------------

void init_lidar_conn() {
    ld_udp_ptr = std::make_unique<LakiBeamUDP>("192.168.8.1", "2368", "192.168.8.2", "34513");
}

void receiver_thread() {
    while(g_running.load()) {
        ld_udp_ptr->sync_get_repackedpack(pack);
        ld_udp_ptr->get_repackedpack(pack);
    }
}

//--------------------------------
// opengl draw
//--------------------------------

// 雷达最大量程（米）——用于投影范围
constexpr float MAX_RANGE_M = 30.0f;
void draw_axes() {
    glBegin(GL_LINES);
    glColor3f(0.3f, 0.3f, 0.3f);

    // X 轴
    glVertex2f(-MAX_RANGE_M, 0.0f);
    glVertex2f( MAX_RANGE_M, 0.0f);
    // Y 轴
    glVertex2f(0.0f, -MAX_RANGE_M);
    glVertex2f(0.0f,  MAX_RANGE_M);

    glEnd();
}

std::pair<double, double> ploar2coor(u16_t angle, u16_t dist) {
    double ag = ((double)(angle) / (double)100 ) * M_PI / 180.0f;
    double r = (double)dist / (double)100;

    auto res = std::pair<double, double>(r * cosf(ag) / 60.0f, r * sinf(ag)/ 60.0f);
    return res;
}

void display_dots() {
    // std::cerr << "Drawing dots.\n"; 
    draw_axes();
    static repark_t park_copy;
    glClear(GL_COLOR_BUFFER_BIT);
    draw_axes();

    glPointSize(3.0f);

    glBegin(GL_POINTS);
    glColor3f(0, 1, 0);
    {
        std::lock_guard<std::mutex> lock(lidar_mutex);
        park_copy = pack;
    }

    for(int i = 0; i < pack.maxdots; i++) { 
        auto [x, y] = ploar2coor(pack.dotcloud[i].angle, pack.dotcloud[i].distance); 
        glVertex2d(-y,-x);
    }

    glEnd();

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    init_lidar_conn();

    std::thread udp_thread(receiver_thread);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("OpenGL Test");
    glutDisplayFunc(display_dots);
    glutMainLoop();

    return 0;
}
