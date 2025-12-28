#include <GL/glut.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Structure to represent a 2D point
struct Point
{
    float x;
    float y;
};

// Enumeration for transformation modes
enum TransformMode
{
    NONE,
    TRANSLATE,
    SCALE,
    ROTATE,
    SHEAR,
    REFLECT_X,
    REFLECT_Y
};

// Global variables
TransformMode currentMode = NONE;
float animationTime = 0.0f;
int windowWidth = 800;
int windowHeight = 600;

// Transform a point using a 3x3 transformation matrix
Point transformPoint(Point p, float M[3][3])
{
    Point result;
    result.x = M[0][0] * p.x + M[0][1] * p.y + M[0][2];
    result.y = M[1][0] * p.x + M[1][1] * p.y + M[1][2];
    return result;
}

// Convert pixel coordinates to normalized device coordinates
float toNDCX(float x)
{
    return (2.0f * x / windowWidth) - 1.0f;
}

float toNDCY(float y)
{
    return 1.0f - (2.0f * y / windowHeight);
}

// Draw coordinate axes
void drawAxes()
{
    glLineWidth(2.0f);
    glColor3f(0.3f, 0.3f, 0.3f);

    glBegin(GL_LINES);
    // X axis
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    // Y axis
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    glLineWidth(1.0f);
}

// Draw grid
void drawGrid()
{
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);

    for (float i = -1.0f; i <= 1.0f; i += 0.1f)
    {
        // Vertical lines
        glVertex2f(i, -1.0f);
        glVertex2f(i, 1.0f);
        // Horizontal lines
        glVertex2f(-1.0f, i);
        glVertex2f(1.0f, i);
    }

    glEnd();
}

// Draw a triangle (filled)
void drawTriangle(Point p1, Point p2, Point p3)
{
    float x1 = toNDCX(p1.x);
    float y1 = toNDCY(p1.y);
    float x2 = toNDCX(p2.x);
    float y2 = toNDCY(p2.y);
    float x3 = toNDCX(p3.x);
    float y3 = toNDCY(p3.y);

    glBegin(GL_TRIANGLES);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
}

// Draw triangle outline
void drawTriangleOutline(Point p1, Point p2, Point p3)
{
    float x1 = toNDCX(p1.x);
    float y1 = toNDCY(p1.y);
    float x2 = toNDCX(p2.x);
    float y2 = toNDCY(p2.y);
    float x3 = toNDCX(p3.x);
    float y3 = toNDCY(p3.y);

    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
    glEnd();
    glLineWidth(1.0f);
}

// Translation transformation
void translate(Point p1, Point p2, Point p3, float tx, float ty)
{
    float T[3][3] = {
        {1, 0, tx},
        {0, 1, ty},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, T);
    Point tp2 = transformPoint(p2, T);
    Point tp3 = transformPoint(p3, T);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Scaling transformation
void scale(Point p1, Point p2, Point p3, float scaleX, float scaleY)
{
    float S[3][3] = {
        {scaleX, 0, 0},
        {0, scaleY, 0},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, S);
    Point tp2 = transformPoint(p2, S);
    Point tp3 = transformPoint(p3, S);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Rotation transformation
void rotate(Point p1, Point p2, Point p3, float theta)
{
    float R[3][3] = {
        {cos(theta), -sin(theta), 0},
        {sin(theta), cos(theta), 0},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, R);
    Point tp2 = transformPoint(p2, R);
    Point tp3 = transformPoint(p3, R);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Shearing transformation
void shear(Point p1, Point p2, Point p3, float shx, float shy)
{
    float SH[3][3] = {
        {1, shx, 0},
        {shy, 1, 0},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, SH);
    Point tp2 = transformPoint(p2, SH);
    Point tp3 = transformPoint(p3, SH);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Reflection across X-axis
void reflectX(Point p1, Point p2, Point p3)
{
    float RX[3][3] = {
        {1, 0, 0},
        {0, -1, 0},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, RX);
    Point tp2 = transformPoint(p2, RX);
    Point tp3 = transformPoint(p3, RX);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Reflection across Y-axis
void reflectY(Point p1, Point p2, Point p3)
{
    float RY[3][3] = {
        {-1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}};

    Point tp1 = transformPoint(p1, RY);
    Point tp2 = transformPoint(p2, RY);
    Point tp3 = transformPoint(p3, RY);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Display callback function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid and axes
    drawGrid();
    drawAxes();

    // Define original triangle vertices (in pixel coordinates)
    Point p1 = {250.0f, 400.0f};
    Point p2 = {250.0f, 300.0f};
    Point p3 = {350.0f, 400.0f};

    // Draw original triangle
    glColor3f(0.2f, 0.6f, 0.9f);
    drawTriangle(p1, p2, p3);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTriangleOutline(p1, p2, p3);

    // Apply transformation based on current mode
    switch (currentMode)
    {
    case TRANSLATE:
        translate(p1, p2, p3, 200.0f, -150.0f);
        break;
    case SCALE:
        scale(p1, p2, p3, 1.8f, 1.8f);
        break;
    case ROTATE:
        rotate(p1, p2, p3, animationTime);
        break;
    case SHEAR:
        shear(p1, p2, p3, 0.6f, 0.2f);
        break;
    case REFLECT_X:
        reflectX(p1, p2, p3);
        break;
    case REFLECT_Y:
        reflectY(p1, p2, p3);
        break;
    case NONE:
        break;
    }

    glutSwapBuffers();
}

// Idle callback for animation
void idle()
{
    if (currentMode == ROTATE)
    {
        animationTime += 0.01f;
        glutPostRedisplay();
    }
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        currentMode = TRANSLATE;
        std::cout << "Mode: TRANSLATE\n";
        break;
    case '2':
        currentMode = SCALE;
        std::cout << "Mode: SCALE\n";
        break;
    case '3':
        currentMode = ROTATE;
        animationTime = 0.0f;
        std::cout << "Mode: ROTATE (Animated)\n";
        break;
    case '4':
        currentMode = SHEAR;
        std::cout << "Mode: SHEAR\n";
        break;
    case '5':
        currentMode = REFLECT_X;
        std::cout << "Mode: REFLECT_X\n";
        break;
    case '6':
        currentMode = REFLECT_Y;
        std::cout << "Mode: REFLECT_Y\n";
        break;
    case '0':
        currentMode = NONE;
        animationTime = 0.0f;
        std::cout << "Mode: NONE (Original)\n";
        break;
    case 27: // ESC key
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

// Reshape callback
void reshape(int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Display instructions
void displayInstructions()
{
    std::cout << "\n=================================\n";
    std::cout << "  2D TRANSFORMATIONS - GLUT\n";
    std::cout << "=================================\n";
    std::cout << "Press keys to apply transformations:\n";
    std::cout << "  1 - Translation\n";
    std::cout << "  2 - Scaling\n";
    std::cout << "  3 - Rotation (Animated)\n";
    std::cout << "  4 - Shearing\n";
    std::cout << "  5 - Reflect across X-axis\n";
    std::cout << "  6 - Reflect across Y-axis\n";
    std::cout << "  0 - Reset (show original)\n";
    std::cout << "  ESC - Exit\n";
    std::cout << "=================================\n\n";
}

// Main function
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("2D Transformations - GLUT");

    // Set clear color
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    // Enable blending for smooth rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Display instructions
    displayInstructions();

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    // Enter main loop
    glutMainLoop();

    return 0;
}