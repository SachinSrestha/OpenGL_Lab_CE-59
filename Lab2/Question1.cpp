#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 700;
const int WINDOW_HEIGHT = 500;

struct Point
{
    float x, y;
};

Point p1 = {100, 100};
Point p2 = {600, 400};

// DDA Line Drawing Algorithm
void drawLineDDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;

    float x = x1;
    float y = y1;

    // Draw the line
    glBegin(GL_POINTS);
    for (int i = 0; i <= steps; i++)
    {
        glVertex2f(round(x), round(y)); // Plot pixel at rounded coordinates
        x += xIncrement;
        y += yIncrement;
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Set point size for better visibility
    glPointSize(2.0);

    // Set line color to white
    glColor3f(1.0, 1.0, 1.0);

    drawLineDDA(p1.x, p1.y, p2.x, p2.y);

    // Draw endpoints for reference
    glPointSize(6.0);
    glColor3f(1.0, 0.0, 0.0); // Red color for start point
    glBegin(GL_POINTS);
    glVertex2f(p1.x, p1.y);
    glEnd();

    glColor3f(0.0, 0.0, 1.0); // Blue color for end point
    glBegin(GL_POINTS);
    glVertex2f(p2.x, p2.y);
    glEnd();

    glFlush();
}

void init()
{
    // Set background color to black
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
    std::cout << "DDA Line Drawing Algorithm - OpenGL" << std::endl;
    std::cout << "- Press ESC to exit" << std::endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("DDA Line Drawing Algorithm");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();

    return 0;
}