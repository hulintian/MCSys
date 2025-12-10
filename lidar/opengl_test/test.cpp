#include <GL/freeglut.h>
#include <iostream>

void display() {
    const GLubyte* v = glGetString(GL_VERSION);
    std::cout << "OpenGL Version: " << v << std::endl;
    exit(0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(400, 300);
    glutCreateWindow("OpenGL Test");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
