#include <GL/glut.h>
#include <GL/gl.h>

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void drawS()
{
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex2f(110, 400);
    glVertex2f(180, 400);
    glVertex2f(180, 380);
    glVertex2f(110, 380);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(110, 400);
    glVertex2f(130, 400);
    glVertex2f(130, 340);
    glVertex2f(110, 340);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(110, 340);
    glVertex2f(180, 340);
    glVertex2f(180, 320);
    glVertex2f(110, 320);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(160, 320);
    glVertex2f(180, 320);
    glVertex2f(180, 260);
    glVertex2f(160, 260);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(110, 280);
    glVertex2f(180, 280);
    glVertex2f(180, 260);
    glVertex2f(110, 260);
    glEnd();
}

void drawA()
{
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex2f(200, 260);
    glVertex2f(220, 260);
    glVertex2f(220, 400);
    glVertex2f(200, 400);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(260, 260);
    glVertex2f(280, 260);
    glVertex2f(280, 400);
    glVertex2f(260, 400);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(200, 400);
    glVertex2f(280, 400);
    glVertex2f(280, 380);
    glVertex2f(200, 380);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(200, 340);
    glVertex2f(280, 340);
    glVertex2f(280, 320);
    glVertex2f(200, 320);
    glEnd();
}

void drawC()
{
    glColor3f(0.0, 0.0, 1.0);

    glBegin(GL_POLYGON);
    glVertex2f(300, 400);
    glVertex2f(370, 400);
    glVertex2f(370, 380);
    glVertex2f(300, 380);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(300, 400);
    glVertex2f(320, 400);
    glVertex2f(320, 260);
    glVertex2f(300, 260);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(300, 280);
    glVertex2f(370, 280);
    glVertex2f(370, 260);
    glVertex2f(300, 260);
    glEnd();
}

void drawH()
{
    glColor3f(1.0, 1.0, 0.0);

    glBegin(GL_POLYGON);
    glVertex2f(390, 260);
    glVertex2f(410, 260);
    glVertex2f(410, 400);
    glVertex2f(390, 400);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(450, 260);
    glVertex2f(470, 260);
    glVertex2f(470, 400);
    glVertex2f(450, 400);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(390, 340);
    glVertex2f(470, 340);
    glVertex2f(470, 320);
    glVertex2f(390, 320);
    glEnd();
}

void drawI()
{
    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_POLYGON);
    glVertex2f(500, 260);
    glVertex2f(520, 260);
    glVertex2f(520, 400);
    glVertex2f(500, 400);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(480, 400);
    glVertex2f(540, 400);
    glVertex2f(540, 380);
    glVertex2f(480, 380);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(480, 280);
    glVertex2f(540, 280);
    glVertex2f(540, 260);
    glVertex2f(480, 260);
    glEnd();
}

void drawN()
{
    glColor3f(0.0f, 1.0f, 1.0f);

    glBegin(GL_POLYGON);
    glVertex2f(560.0f, 260.0f);
    glVertex2f(580.0f, 260.0f);
    glVertex2f(580.0f, 400.0f);
    glVertex2f(560.0f, 400.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(620.0f, 260.0f);
    glVertex2f(640.0f, 260.0f);
    glVertex2f(640.0f, 400.0f);
    glVertex2f(620.0f, 400.0f);
    glEnd();

    glBegin(GL_POLYGON);
    glVertex2f(580.0f, 400.0f);
    glVertex2f(595.0f, 400.0f);
    glVertex2f(620.0f, 260.0f);
    glVertex2f(605.0f, 260.0f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawS();
    drawA();
    drawC();
    drawH();
    drawI();
    drawN();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sachin - OpenGL Name Display");

    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}