#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

int centerX = 400;
int centerY = 300;
int radiusX = 200;
int radiusY = 120;

void plot4Points(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glEnd();
}

void midpointEllipse(int xc, int yc, int rx, int ry)
{
    float rx2 = rx * rx;
    float ry2 = ry * ry;
    float twoRx2 = 2 * rx2;
    float twoRy2 = 2 * ry2;

    int x = 0;
    int y = ry;

    float dx = 0;
    float dy = twoRx2 * y;

    plot4Points(xc, yc, x, y);

    float p1 = ry2 - (rx2 * ry) + (0.25 * rx2);

    while (dx < dy)
    {
        x++;
        dx += twoRy2;

        if (p1 < 0)
        {
            p1 += ry2 + dx;
        }
        else
        {
            y--;
            dy -= twoRx2;
            p1 += ry2 + dx - dy;
        }

        plot4Points(xc, yc, x, y);
    }

    float p2 = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;

    while (y > 0)
    {
        y--;
        dy -= twoRx2;

        if (p2 > 0)
        {
            p2 += rx2 - dy;
        }
        else
        {
            x++;
            dx += twoRy2;
            p2 += rx2 - dy + dx;
        }

        plot4Points(xc, yc, x, y);
    }
}

void drawAxes()
{
    glColor3f(0.5, 0.5, 0.5);
    glLineWidth(1.0);

    glBegin(GL_LINES);
    glVertex2i(0, HEIGHT / 2);
    glVertex2i(WIDTH, HEIGHT / 2);
    glVertex2i(WIDTH / 2, 0);
    glVertex2i(WIDTH / 2, HEIGHT);
    glEnd();
}

void drawCenterPoint()
{
    glColor3f(0.0, 1.0, 0.0);
    glPointSize(6.0);
    glBegin(GL_POINTS);
    glVertex2i(centerX, centerY);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxes();

    glColor3f(1.0, 1.0, 1.0);
    glPointSize(2.0);
    midpointEllipse(centerX, centerY, radiusX, radiusY);

    drawCenterPoint();

    glColor3f(1.0, 1.0, 0.0);
    glRasterPos2i(WIDTH / 2 - 120, HEIGHT - 30);

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    std::cout << "  Midpoint Ellipse Algorithm - OpenGL" << std::endl;
    std::cout << "Algorithm Details:" << std::endl;
    std::cout << "- Uses decision parameters to draw ellipse" << std::endl;
    std::cout << "- Divides ellipse into two regions" << std::endl;
    std::cout << "- Region 1: dx < dy (slope < -1)" << std::endl;
    std::cout << "- Region 2: dx >= dy (slope >= -1)" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "- Press ESC to exit" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "Ellipse:" << std::endl;
    std::cout << "Center: (" << centerX << ", " << centerY << ")" << std::endl;
    std::cout << "Radius X: " << radiusX << ", Radius Y: " << radiusY << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Ellipse Algorithm");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}