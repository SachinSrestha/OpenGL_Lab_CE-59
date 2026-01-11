#include <GL/glut.h>
#include <cmath>
#include <vector>

int winWidth = 800, winHeight = 600;
int xc = 400, yc = 300;
int radius = 150;

std::vector<float> dataValues = {150, 200, 120, 180, 100};

void putPixel(int x, int y)
{
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void circlePoints(int xc, int yc, int x, int y)
{
    putPixel(xc + x, yc + y);
    putPixel(xc - x, yc + y);
    putPixel(xc + x, yc - y);
    putPixel(xc - x, yc - y);
    putPixel(xc + y, yc + x);
    putPixel(xc - y, yc + x);
    putPixel(xc + y, yc - x);
    putPixel(xc - y, yc - x);
}

void midpointCircle(int xc, int yc, int r)
{
    int x = 0;
    int y = r;
    int p = 1 - r;
    circlePoints(xc, yc, x, y);
    while (x < y)
    {
        x++;
        if (p < 0)
        {
            p += 2 * x + 1;
        }
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
        circlePoints(xc, yc, x, y);
    }
}

void drawLineDDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float steps = std::fabs(dx) > std::fabs(dy) ? std::fabs(dx) : std::fabs(dy);
    if (steps == 0.0f)
    {
        glBegin(GL_POINTS);
        glVertex2f(x1, y1);
        glEnd();
        return;
    }
    float xInc = dx / steps;
    float yInc = dy / steps;
    float x = x1;
    float y = y1;
    glBegin(GL_POINTS);
    for (int i = 0; i <= (int)steps; ++i)
    {
        glVertex2f(x, y);
        x += xInc;
        y += yInc;
    }
    glEnd();
}

void fillPieSlice(float startAngleDeg, float endAngleDeg, float r, float cx, float cy)
{
    int steps = 200;
    float pi = 3.14159265f;
    float startRad = startAngleDeg * pi / 180.0f;
    float endRad = endAngleDeg * pi / 180.0f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= steps; ++i)
    {
        float t = startRad + (endRad - startRad) * i / steps;
        float x = cx + r * std::cos(t);
        float y = cy + r * std::sin(t);
        glVertex2f(x, y);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPointSize(2.5f);
    glColor3f(1.0, 1.0, 1.0);
    midpointCircle(xc, yc, radius);

    float total = 0.0f;
    for (float v : dataValues)
        total += v;
    if (total == 0.0f)
        total = 1.0f;

    float currentAngle = 0.0f;
    float colors[][3] = {
        {1.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 1.0f}};

    for (int i = 0; i < (int)dataValues.size(); ++i)
    {
        float sliceAngle = 360.0f * (dataValues[i] / total);
        float startAngle = currentAngle;
        float endAngle = currentAngle + sliceAngle;

        glColor3f(colors[i % 5][0], colors[i % 5][1], colors[i % 5][2]);
        fillPieSlice(startAngle, endAngle, (float)radius, (float)xc, (float)yc);

        float pi = 3.14159265f;
        float sx = xc + radius * std::cos(startAngle * pi / 180.0f);
        float sy = yc + radius * std::sin(startAngle * pi / 180.0f);
        float ex = xc + radius * std::cos(endAngle * pi / 180.0f);
        float ey = yc + radius * std::sin(endAngle * pi / 180.0f);

        glPointSize(3.0f);
        glColor3f(1.0, 1.0, 1.0);
        drawLineDDA((float)xc, (float)yc, sx, sy);
        drawLineDDA((float)xc, (float)yc, ex, ey);

        currentAngle = endAngle;
    }

    glFlush();
}

void reshape(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Pie Chart using Midpoint Circle Algorithm");

    glClearColor(1.0, 1.0, 1.0, 1.0f);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}