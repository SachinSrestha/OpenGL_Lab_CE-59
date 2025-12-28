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

// Matrix multiplication for composite transformations
void multiplyMatrix(float result[3][3], float m1[3][3], float m2[3][3])
{
    float temp[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                temp[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            result[i][j] = temp[i][j];
}

// Calculate centroid of triangle
Point getCentroid(Point p1, Point p2, Point p3)
{
    Point centroid;
    centroid.x = (p1.x + p2.x + p3.x) / 3.0f;
    centroid.y = (p1.y + p2.y + p3.y) / 3.0f;
    return centroid;
}

// Draw coordinate axes with labels
void drawAxes()
{
    glLineWidth(3.0f);
    glColor3f(0.5f, 0.5f, 0.5f);

    // X-axis
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);
    glEnd();

    // X-axis arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(1.0f, 0.0f);
    glVertex2f(0.95f, 0.02f);
    glVertex2f(0.95f, -0.02f);
    glEnd();

    // Y-axis
    glBegin(GL_LINES);
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);
    glEnd();

    // Y-axis arrow
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, 1.0f);
    glVertex2f(0.02f, 0.95f);
    glVertex2f(-0.02f, 0.95f);
    glEnd();

    glLineWidth(1.0f);
}

// Draw axis labels
void drawAxisLabels()
{
    glColor3f(1.0f, 1.0f, 1.0f);

    // X label
    glRasterPos2f(0.92f, -0.08f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');

    // Y label
    glRasterPos2f(0.03f, 0.92f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');

    // Origin label
    glRasterPos2f(0.02f, -0.05f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'O');
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

// Draw a triangle (filled) - now using NDC coordinates directly
void drawTriangle(Point p1, Point p2, Point p3)
{
    glBegin(GL_TRIANGLES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glEnd();
}

// Draw triangle outline
void drawTriangleOutline(Point p1, Point p2, Point p3)
{
    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
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

// Scaling transformation around centroid with offset
void scale(Point p1, Point p2, Point p3, float scaleX, float scaleY)
{
    Point centroid = getCentroid(p1, p2, p3);

    // Translate to origin
    float T1[3][3] = {
        {1, 0, -centroid.x},
        {0, 1, -centroid.y},
        {0, 0, 1}};

    // Scale
    float S[3][3] = {
        {scaleX, 0, 0},
        {0, scaleY, 0},
        {0, 0, 1}};

    // Translate back and offset to avoid overlap
    float T2[3][3] = {
        {1, 0, centroid.x + 0.6f},
        {0, 1, centroid.y},
        {0, 0, 1}};

    // Composite transformation: T2 * S * T1
    float temp[3][3];
    multiplyMatrix(temp, S, T1);
    float M[3][3];
    multiplyMatrix(M, T2, temp);

    Point tp1 = transformPoint(p1, M);
    Point tp2 = transformPoint(p2, M);
    Point tp3 = transformPoint(p3, M);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Rotation transformation around centroid with offset
void rotate(Point p1, Point p2, Point p3, float theta)
{
    Point centroid = getCentroid(p1, p2, p3);

    // Translate to origin
    float T1[3][3] = {
        {1, 0, -centroid.x},
        {0, 1, -centroid.y},
        {0, 0, 1}};

    // Rotate
    float R[3][3] = {
        {cos(theta), -sin(theta), 0},
        {sin(theta), cos(theta), 0},
        {0, 0, 1}};

    // Translate back and offset to avoid overlap
    float T2[3][3] = {
        {1, 0, centroid.x + 0.6f},
        {0, 1, centroid.y + 0.3f},
        {0, 0, 1}};

    // Composite transformation: T2 * R * T1
    float temp[3][3];
    multiplyMatrix(temp, R, T1);
    float M[3][3];
    multiplyMatrix(M, T2, temp);

    Point tp1 = transformPoint(p1, M);
    Point tp2 = transformPoint(p2, M);
    Point tp3 = transformPoint(p3, M);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Shearing transformation with offset
void shear(Point p1, Point p2, Point p3, float shx, float shy)
{
    // First translate to avoid overlap
    float T[3][3] = {
        {1, 0, 0.6f},
        {0, 1, -0.3f},
        {0, 0, 1}};

    // Then shear
    float SH[3][3] = {
        {1, shx, 0},
        {shy, 1, 0},
        {0, 0, 1}};

    // Composite: SH * T
    float M[3][3];
    multiplyMatrix(M, SH, T);

    Point tp1 = transformPoint(p1, M);
    Point tp2 = transformPoint(p2, M);
    Point tp3 = transformPoint(p3, M);

    glColor3f(0.9f, 0.3f, 0.3f);
    drawTriangle(tp1, tp2, tp3);
    glColor3f(0.0f, 1.0f, 1.0f);
    drawTriangleOutline(tp1, tp2, tp3);
}

// Reflection across X-axis (X-axis at y=0 between triangles)
void reflectX(Point p1, Point p2, Point p3)
{
    // Simply reflect across X-axis (y = 0)
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

// Reflection across Y-axis (Y-axis at x=0 between triangles)
void reflectY(Point p1, Point p2, Point p3)
{
    // Simply reflect across Y-axis (x = 0)
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

    // Draw grid first (background)
    drawGrid();

    // Draw axes (foreground, more visible)
    drawAxes();

    // Draw axis labels
    drawAxisLabels();

    // Define original triangle vertices in NDC coordinates
    // Positioned so reflections work naturally with axes
    Point p1 = {-0.5f, 0.1f}; // Top-left
    Point p2 = {-0.5f, 0.4f}; // Bottom-left (above X-axis)
    Point p3 = {-0.2f, 0.1f}; // Top-right

    // Draw original triangle (blue)
    glColor3f(0.2f, 0.6f, 0.9f);
    drawTriangle(p1, p2, p3);
    glColor3f(1.0f, 1.0f, 1.0f);
    drawTriangleOutline(p1, p2, p3);

    // Apply transformation based on current mode
    switch (currentMode)
    {
    case TRANSLATE:
        translate(p1, p2, p3, 0.5f, 0.3f);
        break;
    case SCALE:
        scale(p1, p2, p3, 1.5f, 1.5f);
        break;
    case ROTATE:
        rotate(p1, p2, p3, M_PI / 3); // 60 degrees
        break;
    case SHEAR:
        shear(p1, p2, p3, 0.4f, 0.2f);
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
        std::cout << "Mode: ROTATE\n";
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
    std::cout << "  2 - Scaling (around center)\n";
    std::cout << "  3 - Rotation (45° around center)\n";
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

    // Enter main loop
    glutMainLoop();

    return 0;
}