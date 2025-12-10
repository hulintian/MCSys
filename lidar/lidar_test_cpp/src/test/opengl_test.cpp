#include <GL/freeglut.h>
#include <iostream>

// void display() {
//     const GLubyte* v = glGetString(GL_VERSION);
//     std::cout << "OpenGL Version: " << v << std::endl;
//     exit(0);
// }
void display() {
    const GLubyte* v = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << v << std::endl;
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0);   // 红色
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0.0, 1.0, 0.0);   // 绿色
        glVertex2f(0.5f, -0.5f);

        glColor3f(0.0, 0.0, 1.0);   // 蓝色
        glVertex2f(0.0f, 0.5f);
    glEnd();

    glutSwapBuffers();
}

void opengl_test(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("OpenGL Test");
    glutDisplayFunc(display);
    glutMainLoop();
}
