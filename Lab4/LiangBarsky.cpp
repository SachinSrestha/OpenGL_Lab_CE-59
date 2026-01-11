#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <algorithm>

// Clipping window boundaries
float xMin = -0.5f, yMin = -0.5f;
float xMax = 0.5f, yMax = 0.5f;

struct Point
{
    float x, y;
    Point(float px = 0, float py = 0) : x(px), y(py) {}
};

struct Line
{
    Point p1, p2;
    Line(Point a, Point b) : p1(a), p2(b) {}
};

std::vector<Line> lines;

// Liang-Barsky line clipping algorithm
bool liangBarskyClip(float &x1, float &y1, float &x2, float &y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;

    float p[4], q[4];
    float u1 = 0.0f, u2 = 1.0f;

    // Define p and q arrays for the four boundaries
    p[0] = -dx;
    q[0] = x1 - xMin; // Left
    p[1] = dx;
    q[1] = xMax - x1; // Right
    p[2] = -dy;
    q[2] = y1 - yMin; // Bottom
    p[3] = dy;
    q[3] = yMax - y1; // Top

    // Check each boundary
    for (int i = 0; i < 4; i++)
    {
        if (p[i] == 0)
        {
            // Line is parallel to boundary
            if (q[i] < 0)
            {
                // Line is outside boundary
                return false;
            }
        }
        else
        {
            float r = q[i] / p[i];

            if (p[i] < 0)
            {
                // Line enters the boundary (potentially entering)
                u1 = std::max(u1, r);
            }
            else
            {
                // Line exits the boundary (potentially leaving)
                u2 = std::min(u2, r);
            }
        }
    }

    // Check if line is clipped out
    if (u1 > u2)
    {
        return false;
    }

    // Calculate clipped coordinates
    float clippedX1 = x1 + u1 * dx;
    float clippedY1 = y1 + u1 * dy;
    float clippedX2 = x1 + u2 * dx;
    float clippedY2 = y1 + u2 * dy;

    x1 = clippedX1;
    y1 = clippedY1;
    x2 = clippedX2;
    y2 = clippedY2;

    return true;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw clipping window in white
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xMin, yMin);
    glVertex2f(xMax, yMin);
    glVertex2f(xMax, yMax);
    glVertex2f(xMin, yMax);
    glEnd();
    // Draw original lines in red (solid)
    glLineWidth(2.5f);
    glColor3f(1.0f, 0.3f, 0.3f);
    for (const auto &line : lines)
    {
        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();
    }
    // Draw clipped lines in green
    glColor3f(0.2f, 1.0f, 0.4f);
    glLineWidth(2.5f);
    for (const auto &line : lines)
    {
        float x1 = line.p1.x, y1 = line.p1.y;
        float x2 = line.p2.x, y2 = line.p2.y;

        if (liangBarskyClip(x1, y1, x2, y2))
        {
            glBegin(GL_LINES);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();

            // Draw endpoints
            glPointSize(6.0f);
            glBegin(GL_POINTS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y2);
            glEnd();
        }
    }
    glLineWidth(1.0f);

    glFlush();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Four test cases demonstrating different clipping scenarios

    // Test Case 1: Line completely INSIDE the clipping window
    lines.push_back(Line(Point(-0.3f, -0.2f), Point(0.2f, 0.3f)));

    // Test Case 2: Line PARTIALLY INSIDE (crosses from outside to inside)
    lines.push_back(Line(Point(-0.7f, -0.3f), Point(0.2f, 0.1f)));

    // Test Case 3: Line PARTIALLY INSIDE (crosses through the window)
    lines.push_back(Line(Point(-0.1f, -0.7f), Point(0.3f, 0.7f)));

    // Test Case 4: Line completely OUTSIDE the clipping window
    lines.push_back(Line(Point(0.55f, 0.55f), Point(0.9f, 0.9f)));
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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 600);
    glutInitWindowPosition(250, 100);
    glutCreateWindow("Liang-Barsky Line Clipping Algorithm");

    init();

    std::cout << "Liang-Barsky Line Clipping Algorithm\n";
    std::cout << "====================================\n";
    std::cout << "Clipping Window: (" << xMin << ", " << yMin << ") to ("
              << xMax << ", " << yMax << ")\n\n";

    std::cout << "Test Cases:\n\n";

    for (size_t i = 0; i < lines.size(); i++)
    {
        float x1 = lines[i].p1.x, y1 = lines[i].p1.y;
        float x2 = lines[i].p2.x, y2 = lines[i].p2.y;

        std::cout << "Line " << (i + 1) << ": (" << x1 << ", " << y1 << ") to ("
                  << x2 << ", " << y2 << ")\n";

        if (liangBarskyClip(x1, y1, x2, y2))
        {
            std::cout << "  Result: ACCEPTED (visible)\n";
            std::cout << "  Clipped coordinates: (" << x1 << ", " << y1 << ") to ("
                      << x2 << ", " << y2 << ")\n";
        }
        else
        {
            std::cout << "  Result: REJECTED (completely outside window)\n";
        }
        std::cout << "\n";
    }

    std::cout << "====================================\n";
    std::cout << "Visual Legend:\n";
    std::cout << "  White rectangle: Clipping window\n";
    std::cout << "  Red lines: Original lines\n";
    std::cout << "  Green lines: Clipped result\n";
    std::cout << "  Green dots: Clipped endpoints\n";
    std::cout << "\nPress ESC to exit\n";

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
