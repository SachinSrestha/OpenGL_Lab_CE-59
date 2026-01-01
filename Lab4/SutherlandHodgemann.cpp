#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <string>

struct Point
{
    float x, y;
    Point(float x = 0, float y = 0) : x(x), y(y) {}
};

// Global variables
std::vector<Point> subjectPolygon;
std::vector<Point> clipPolygon;
std::vector<Point> clippedPolygon;
bool showClipped = false;

// Check if point is inside edge (left side of the line from p1 to p2)
bool inside(Point p, Point p1, Point p2)
{
    return (p2.x - p1.x) * (p.y - p1.y) > (p2.y - p1.y) * (p.x - p1.x);
}

// Find intersection point of line segment with edge
Point intersection(Point p1, Point p2, Point edgeStart, Point edgeEnd)
{
    float dx1 = p2.x - p1.x;
    float dy1 = p2.y - p1.y;
    float dx2 = edgeEnd.x - edgeStart.x;
    float dy2 = edgeEnd.y - edgeStart.y;

    float denom = dx1 * dy2 - dy1 * dx2;
    if (fabs(denom) < 1e-10)
        return p1;

    float t = ((edgeStart.x - p1.x) * dy2 - (edgeStart.y - p1.y) * dx2) / denom;

    return Point(p1.x + t * dx1, p1.y + t * dy1);
}

// Sutherland-Hodgman algorithm
std::vector<Point> sutherlandHodgman(std::vector<Point> poly, std::vector<Point> clipPoly)
{
    std::vector<Point> output = poly;

    for (size_t i = 0; i < clipPoly.size(); i++)
    {
        if (output.empty())
            break;

        std::vector<Point> input = output;
        output.clear();

        Point edgeStart = clipPoly[i];
        Point edgeEnd = clipPoly[(i + 1) % clipPoly.size()];

        for (size_t j = 0; j < input.size(); j++)
        {
            Point current = input[j];
            Point previous = input[(j + input.size() - 1) % input.size()];

            bool currentInside = inside(current, edgeStart, edgeEnd);
            bool previousInside = inside(previous, edgeStart, edgeEnd);

            if (currentInside)
            {
                if (!previousInside)
                {
                    output.push_back(intersection(previous, current, edgeStart, edgeEnd));
                }
                output.push_back(current);
            }
            else if (previousInside)
            {
                output.push_back(intersection(previous, current, edgeStart, edgeEnd));
            }
        }
    }

    return output;
}

void init()
{
    glClearColor(0.05f, 0.05f, 0.12f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-400, 400, -300, 300);

    // Define subject polygon (to be clipped)
    subjectPolygon.push_back(Point(-200, -100));
    subjectPolygon.push_back(Point(100, -150));
    subjectPolygon.push_back(Point(200, 50));
    subjectPolygon.push_back(Point(50, 200));
    subjectPolygon.push_back(Point(-150, 150));

    // Define clipping polygon (rectangular window)
    clipPolygon.push_back(Point(-180, -160));
    clipPolygon.push_back(Point(180, -160));
    clipPolygon.push_back(Point(180, 160));
    clipPolygon.push_back(Point(-180, 160));
}

void drawPolygon(const std::vector<Point> &poly, float r, float g, float b, float a, float lineWidth, bool filled = false)
{
    if (poly.empty())
        return;

    if (filled)
    {
        glColor4f(r * 0.3f, g * 0.3f, b * 0.3f, a * 0.4f);
        glBegin(GL_POLYGON);
        for (const auto &p : poly)
        {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

    glLineWidth(lineWidth);
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
    for (const auto &p : poly)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // Draw vertices as points
    glPointSize(lineWidth + 3.0f);
    glBegin(GL_POINTS);
    for (const auto &p : poly)
    {
        glVertex2f(p.x, p.y);
    }
    glEnd();
}

void drawText(float x, float y, const std::string &text, float r, float g, float b)
{
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    for (char c : text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
}

void drawInstructions()
{
    if (!showClipped)
    {
        drawText(-380, -270, "ORIGINAL POLYGON", 0.3f, 0.8f, 0.9f);
        drawText(-380, -290, "Press SPACE to clip polygon", 0.7f, 0.7f, 0.7f);
    }
    else
    {
        drawText(-380, -270, "CLIPPED RESULT", 0.3f, 0.9f, 0.5f);
        std::string vertexCount = "Vertices: " + std::to_string(clippedPolygon.size());
        drawText(-380, -290, vertexCount, 0.7f, 0.7f, 0.7f);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw clipping window with glow effect
    drawPolygon(clipPolygon, 1.0f, 0.8f, 0.2f, 1.0f, 3.5f, false);

    if (!showClipped)
    {
        // Draw original subject polygon
        drawPolygon(subjectPolygon, 0.3f, 0.9f, 1.0f, 1.0f, 2.5f, false);
    }
    else
    {
        // Draw clipped polygon
        drawPolygon(clippedPolygon, 0.3f, 1.0f, 0.5f, 1.0f, 3.0f, true);
    }

    drawInstructions();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == ' ')
    {
        showClipped = !showClipped;

        if (showClipped)
        {
            clippedPolygon = sutherlandHodgman(subjectPolygon, clipPolygon);
            std::cout << "\n=== CLIPPING PERFORMED ===\n";
            std::cout << "Original vertices: " << subjectPolygon.size() << "\n";
            std::cout << "Clipped vertices:  " << clippedPolygon.size() << "\n";
        }
        glutPostRedisplay();
    }
    else if (key == 27)
    {
        exit(0);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(850, 650);
    glutInitWindowPosition(250, 100);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping Algorithm");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    std::cout << "SUTHERLAND-HODGMAN POLYGON CLIPPING DEMO \n";
    std::cout << "Controls:\n";
    std::cout << "  [SPACE] - Toggle clipping visualization\n";
    std::cout << "  [ESC]   - Exit program\n\n";
    std::cout << "Algorithm: Clips a polygon against a convex region\n";
    std::cout << "by iteratively clipping against each edge.\n\n";

    glutMainLoop();
    return 0;
}