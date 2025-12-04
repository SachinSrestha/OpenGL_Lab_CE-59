#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct Point
{
    int x, y;
};

Point p1 = {100, 100};
Point p2 = {700, 500};

// Bresenham Line Drawing Algorithm for |m| < 1
void bresenhamLow(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    int d = 2 * dy - dx; // Initial decision parameter
    int y = y1;

    glBegin(GL_POINTS);
    for (int x = x1; x <= x2; x++)
    {
        glVertex2i(x, y);

        if (d > 0)
        {
            y += yi;
            d += 2 * (dy - dx);
        }
        else
        {
            d += 2 * dy;
        }
    }
    glEnd();
}

// Bresenham Line Drawing Algorithm for |m| >= 1
void bresenhamHigh(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int d = 2 * dx - dy; // Initial decision parameter
    int x = x1;

    glBegin(GL_POINTS);
    for (int y = y1; y <= y2; y++)
    {
        glVertex2i(x, y);

        if (d > 0)
        {
            x += xi;
            d += 2 * (dx - dy);
        }
        else
        {
            d += 2 * dx;
        }
    }
    glEnd();
}

// Main Bresenham function
void drawLineBresenham(int x1, int y1, int x2, int y2)
{
    // Display slope information
    float slope = (x2 - x1) != 0 ? (float)(y2 - y1) / (x2 - x1) : INFINITY;
    std::cout << "Drawing line from (" << x1 << ", " << y1 << ") to ("
              << x2 << ", " << y2 << ")" << std::endl;
    std::cout << "Slope: " << slope << " -> Using ";

    if (abs(y2 - y1) < abs(x2 - x1))
    {
        std::cout << "Bresenham Low (|m| < 1)" << std::endl;
        if (x1 > x2)
        {
            bresenhamLow(x2, y2, x1, y1);
        }
        else
        {
            bresenhamLow(x1, y1, x2, y2);
        }
    }
    else
    {
        std::cout << "Bresenham High (|m| >= 1)" << std::endl;
        if (y1 > y2)
        {
            bresenhamHigh(x2, y2, x1, y1);
        }
        else
        {
            bresenhamHigh(x1, y1, x2, y2);
        }
    }
}

void display()
{

    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(2.0);
    glColor3f(1.0, 1.0, 1.0);

    // Draw line using Bresenham algorithm
    drawLineBresenham(p1.x, p1.y, p2.x, p2.y);

    // Draw endpoints for reference
    glPointSize(6.0);
    glColor3f(1.0, 0.0, 0.0); // Red color for start point
    glBegin(GL_POINTS);
    glVertex2i(p1.x, p1.y);
    glEnd();

    glColor3f(0.0, 0.0, 1.0); // Blue color for end point
    glBegin(GL_POINTS);
    glVertex2i(p2.x, p2.y);
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

void setPredefinedLine(int option)
{
    switch (option)
    {
    case 1: // Shallow slope (|m| < 1)
        p1.x = 100;
        p1.y = 200;
        p2.x = 700;
        p2.y = 400;
        break;
    case 2: // Steep slope (|m| >= 1)
        p1.x = 200;
        p1.y = 100;
        p2.x = 400;
        p2.y = 500;
        break;
    default:
        std::cout << "Invalid option! Using default line." << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::cout << "Bresenham Line Drawing Algorithm - OpenGL" << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Algorithm Details:" << std::endl;
    std::cout << "- For |m| < 1: Uses bresenhamLow (increments x)" << std::endl;
    std::cout << "- For |m| >= 1: Uses bresenhamHigh (increments y)" << std::endl;
    std::cout << "\nPress ESC in the window to exit." << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "\nChoose a predefined line to draw:" << std::endl;
    std::cout << "1. Shallow Slope (m ~ 0.33, |m| < 1)" << std::endl;
    std::cout << "2. Steep Slope (m ~ 2, |m| >= 1)" << std::endl;
    std::cout << "\nEnter your choice (1-5): ";

    int choice;
    std::cin >> choice;

    if (choice >= 1 && choice <= 5)
    {
        setPredefinedLine(choice);
        std::cout << "\nPredefined line set!" << std::endl;
    }
    else
    {
        std::cout << "\nInvalid choice! Using default line." << std::endl;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham Line Drawing Algorithm");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
