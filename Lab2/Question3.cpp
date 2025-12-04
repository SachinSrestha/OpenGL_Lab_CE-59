#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Circle parameters
int centerX = 400;
int centerY = 300;
int radius = 150;

// Function to plot 8 symmetric points
void plotCirclePoints(int xc, int yc, int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y); // Octant 1
    glVertex2i(xc - x, yc + y); // Octant 2
    glVertex2i(xc + x, yc - y); // Octant 3
    glVertex2i(xc - x, yc - y); // Octant 4
    glVertex2i(xc + y, yc + x); // Octant 5
    glVertex2i(xc - y, yc + x); // Octant 6
    glVertex2i(xc + y, yc - x); // Octant 7
    glVertex2i(xc - y, yc - x); // Octant 8
    glEnd();
}

// Midpoint Circle Drawing Algorithm
void drawCircleMidpoint(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int d = 1 - r; // Initial decision parameter

    std::cout << "\nMidpoint Circle Algorithm: " << std::endl;
    std::cout << "Center: (" << xc << ", " << yc << ")" << std::endl;
    std::cout << "Radius: " << r << std::endl;
    std::cout << "Initial decision parameter d = " << d << std::endl;

    int step = 0;

    // Plot initial point
    plotCirclePoints(xc, yc, x, y);

    // Generate points for first octant and use symmetry for others
    while (x < y)
    {
        x++;

        if (d < 0)
        {
            d = d + 2 * x + 1;
        }
        else
        {
            y--;
            d = d + 2 * (x - y) + 1;
        }

        plotCirclePoints(xc, yc, x, y);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2.0);
    glColor3f(1.0, 1.0, 1.0);

    drawCircleMidpoint(centerX, centerY, radius);

    glPointSize(5.0); // Draw center point in red
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_POINTS);
    glVertex2i(centerX, centerY);
    glEnd();

    glColor3f(0.0, 1.0, 0.0); // Draw radius line in green
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glVertex2i(centerX, centerY);
    glVertex2i(centerX + radius, centerY);
    glEnd();

    glFlush();
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
    { // ESC key
        exit(0);
    }
}

int main(int argc, char **argv)
{
    std::cout << "Midpoint Circle Drawing Algorithm - OpenGL" << std::endl;
    std::cout << "Press ESC in the window to exit." << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Midpoint Circle Drawing Algorithm");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}