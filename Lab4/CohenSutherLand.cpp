#include <GL/glut.h>
#include <iostream>
#include <vector>

// Region codes for Cohen-Sutherland algorithm
const int INSIDE = 0; // 0000
const int LEFT = 1;   // 0001
const int RIGHT = 2;  // 0010
const int BOTTOM = 4; // 0100
const int TOP = 8;    // 1000

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

// Compute region code for a point
int computeCode(float x, float y)
{
    int code = INSIDE;

    if (x < xMin)
        code |= LEFT;
    else if (x > xMax)
        code |= RIGHT;
    if (y < yMin)
        code |= BOTTOM;
    else if (y > yMax)
        code |= TOP;

    return code;
}

// Cohen-Sutherland line clipping algorithm
bool cohenSutherlandClip(float &x1, float &y1, float &x2, float &y2)
{
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;

    while (true)
    {
        if ((code1 == 0) && (code2 == 0))
        {
            // Both points inside window
            accept = true;
            break;
        }
        else if (code1 & code2)
        {
            // Both points share an outside region - trivially reject
            break;
        }
        else
        {
            // Line needs clipping
            int codeOut;
            float x, y;

            // Pick a point outside the window
            codeOut = (code1 != 0) ? code1 : code2;

            // Find intersection point using line equation
            // y = y1 + slope * (x - x1), x = x1 + (1/slope) * (y - y1)
            if (codeOut & TOP)
            {
                x = x1 + (x2 - x1) * (yMax - y1) / (y2 - y1);
                y = yMax;
            }
            else if (codeOut & BOTTOM)
            {
                x = x1 + (x2 - x1) * (yMin - y1) / (y2 - y1);
                y = yMin;
            }
            else if (codeOut & RIGHT)
            {
                y = y1 + (y2 - y1) * (xMax - x1) / (x2 - x1);
                x = xMax;
            }
            else if (codeOut & LEFT)
            {
                y = y1 + (y2 - y1) * (xMin - x1) / (x2 - x1);
                x = xMin;
            }

            // Replace point outside window with intersection point
            if (codeOut == code1)
            {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else
            {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }

    return accept;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping window in white with thicker line
    glLineWidth(3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xMin, yMin);
    glVertex2f(xMax, yMin);
    glVertex2f(xMax, yMax);
    glVertex2f(xMin, yMax);
    glEnd();

    // Draw original lines in red (solid, not dotted)
    glLineWidth(2.0f);
    glColor3f(1.0f, 0.3f, 0.3f);
    for (const auto &line : lines)
    {
        glBegin(GL_LINES);
        glVertex2f(line.p1.x, line.p1.y);
        glVertex2f(line.p2.x, line.p2.y);
        glEnd();
    }

    // Draw clipped lines in green with same width
    glColor3f(0.2f, 1.0f, 0.4f);
    glLineWidth(2.0f);
    for (const auto &line : lines)
    {
        float x1 = line.p1.x, y1 = line.p1.y;
        float x2 = line.p2.x, y2 = line.p2.y;

        if (cohenSutherlandClip(x1, y1, x2, y2))
        {
            glBegin(GL_LINES);
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

    // Test cases: Inside, Crosses, Partially outside, Completely outside
    lines.push_back(Line(Point(-0.2f, -0.2f), Point(0.3f, 0.3f))); // Completely inside
    lines.push_back(Line(Point(-0.9f, -0.7f), Point(0.9f, 0.7f))); // Crosses window
    lines.push_back(Line(Point(0.7f, -0.9f), Point(0.9f, 0.6f)));  // Partially outside
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
    glutCreateWindow("Cohen-Sutherland Line Clipping Algorithm");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    std::cout << "Cohen-Sutherland Line Clipping Algorithm\n";
    std::cout << "========================================\n";
    std::cout << "White rectangle: Clipping window\n";
    std::cout << "Red lines: Original lines\n";
    std::cout << "Green lines: Clipped result\n";
    std::cout << "\nTest Cases:\n";
    std::cout << "  1. Line completely inside window\n";
    std::cout << "  2. Line partially inside/outside (crosses window)\n";
    std::cout << "  3. Line completely outside window\n";
    std::cout << "\nPress ESC to exit\n";

    glutMainLoop();
    return 0;
}