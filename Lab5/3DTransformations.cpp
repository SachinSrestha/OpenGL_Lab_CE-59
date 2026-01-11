#include <GL/glut.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Pre-defined transformation constants
const float TRANSLATE_X = 2.0f;
const float TRANSLATE_Y = 1.5f;
const float TRANSLATE_Z = -1.0f;

const float ROTATE_ANGLE = 45.0f;
const float ROTATE_X_AXIS = 1.0f;
const float ROTATE_Y_AXIS = 1.0f;
const float ROTATE_Z_AXIS = 0.0f;

const float SCALE_X = 1.5f;
const float SCALE_Y = 0.8f;
const float SCALE_Z = 1.2f;

const float SHEAR_XY = 0.5f;
const float SHEAR_XZ = 0.3f;

// Current display mode
int displayMode = 0; // 0=All, 1=Original, 2=Translation, 3=Rotation, 4=Scaling, 5=Shearing

// Camera parameters
float cameraAngleX = 20.0f;
float cameraAngleY = 30.0f;
float cameraDistance = 16.0f;

void drawText(float x, float y, float z, const char *text)
{
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos3f(x, y, z);
    for (const char *c = text; *c != '\0'; c++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    glEnable(GL_LIGHTING);
}

void drawCube()
{
    glutSolidCube(1.5);
}

void drawAxes()
{
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    // X-axis (Red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-12.0f, 0.0f, 0.0f);
    glVertex3f(12.0f, 0.0f, 0.0f);

    // Y-axis (Green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -4.0f, 0.0f);
    glVertex3f(0.0f, 8.0f, 0.0f);

    // Z-axis (Blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -8.0f);
    glVertex3f(0.0f, 0.0f, 8.0f);
    glEnd();
    glLineWidth(1.0f);
}

void drawOriginal(float offsetX, float offsetZ)
{
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, offsetZ);
    glColor3f(0.7f, 0.7f, 0.7f);
    drawCube();
    drawText(0.0f, -1.5f, 0.0f, "ORIGINAL");
    glPopMatrix();
}

void drawTranslation(float offsetX, float offsetZ)
{
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, offsetZ);
    glTranslatef(TRANSLATE_X, TRANSLATE_Y, TRANSLATE_Z);
    glColor3f(1.0f, 0.3f, 0.3f);
    drawCube();
    drawText(0.0f, -1.5f, 0.0f, "TRANSLATION");
    glPopMatrix();
}

void drawRotation(float offsetX, float offsetZ)
{
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, offsetZ);
    glRotatef(ROTATE_ANGLE, ROTATE_X_AXIS, ROTATE_Y_AXIS, ROTATE_Z_AXIS);
    glColor3f(0.3f, 1.0f, 0.3f);
    drawCube();
    glPopMatrix();
    drawText(offsetX, -1.5f, offsetZ, "ROTATION");
}

void drawScaling(float offsetX, float offsetZ)
{
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, offsetZ);
    glScalef(SCALE_X, SCALE_Y, SCALE_Z);
    glColor3f(0.3f, 0.3f, 1.0f);
    drawCube();
    glPopMatrix();
    drawText(offsetX, -1.5f, offsetZ, "SCALING");
}

void drawShearing(float offsetX, float offsetZ)
{
    glPushMatrix();
    glTranslatef(offsetX, 0.0f, offsetZ);

    // Apply shear matrix
    float shearMatrix[16] = {
        1.0f, SHEAR_XY, SHEAR_XZ, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
    glMultMatrixf(shearMatrix);

    glColor3f(1.0f, 1.0f, 0.3f);
    drawCube();
    glPopMatrix();
    drawText(offsetX, -1.5f, offsetZ, "SHEARING");
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera position
    float camX = cameraDistance * sin(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);
    float camY = cameraDistance * sin(cameraAngleX * M_PI / 180.0f);
    float camZ = cameraDistance * cos(cameraAngleY * M_PI / 180.0f) * cos(cameraAngleX * M_PI / 180.0f);

    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw axes
    drawAxes();

    if (displayMode == 0)
    {
        // Show all transformations in a staggered layout (2-3-2 pattern)
        // Top row
        drawOriginal(-6.0f, 3.0f);
        drawTranslation(0.0f, 3.0f);

        // Middle row (center)
        drawRotation(0.0f, -1.0f);

        // Bottom row
        drawScaling(-6.0f, -5.0f);
        drawShearing(6.0f, -5.0f);
    }
    else if (displayMode == 1)
    {
        // Original only
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawOriginal(0.0f, 0.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawOriginal(0.0f, 0.0f);
    }
    else if (displayMode == 2)
    {
        // Translation
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawOriginal(0.0f, 0.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawTranslation(0.0f, 0.0f);
    }
    else if (displayMode == 3)
    {
        // Rotation
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawOriginal(0.0f, 0.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawRotation(3.5f, 0.0f);
    }
    else if (displayMode == 4)
    {
        // Scaling
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawOriginal(0.0f, 0.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawScaling(3.5f, 0.0f);
    }
    else if (displayMode == 5)
    {
        // Shearing
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        drawOriginal(0.0f, 0.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        drawShearing(3.5f, 0.0f);
    }

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '0':
        displayMode = 0;
        std::cout << "Display: All Transformations\n";
        break;
    case '1':
        displayMode = 1;
        std::cout << "Display: Original\n";
        break;
    case '2':
        displayMode = 2;
        std::cout << "Display: Translation\n";
        break;
    case '3':
        displayMode = 3;
        std::cout << "Display: Rotation\n";
        break;
    case '4':
        displayMode = 4;
        std::cout << "Display: Scaling\n";
        break;
    case '5':
        displayMode = 5;
        std::cout << "Display: Shearing\n";
        break;
    case 27:
        exit(0);
        break; // ESC
    }
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        cameraAngleX += 5.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraAngleX -= 5.0f;
        break;
    case GLUT_KEY_LEFT:
        cameraAngleY -= 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraAngleY += 5.0f;
        break;
    case GLUT_KEY_PAGE_UP:
        cameraDistance -= 0.5f;
        break;
    case GLUT_KEY_PAGE_DOWN:
        cameraDistance += 0.5f;
        break;
    }
    glutPostRedisplay();
}

void init()
{
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = {10.0f, 10.0f, 10.0f, 1.0f};
    GLfloat lightAmbient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat lightDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

void printInstructions()
{
    std::cout << "\n=== 3D TRANSFORMATIONS IN OPENGL ===\n\n";

    std::cout << "TRANSFORMATION CONSTANTS:\n";
    std::cout << "  Translation: X=" << TRANSLATE_X << ", Y=" << TRANSLATE_Y << ", Z=" << TRANSLATE_Z << "\n";
    std::cout << "  Rotation: Angle=" << ROTATE_ANGLE << "°, Axis=(" << ROTATE_X_AXIS << "," << ROTATE_Y_AXIS << "," << ROTATE_Z_AXIS << ")\n";
    std::cout << "  Scaling: X=" << SCALE_X << ", Y=" << SCALE_Y << ", Z=" << SCALE_Z << "\n";
    std::cout << "  Shearing: XY=" << SHEAR_XY << ", XZ=" << SHEAR_XZ << "\n\n";

    std::cout << "DISPLAY MODES:\n";
    std::cout << "  0 - Show all transformations side by side\n";
    std::cout << "  1 - Original shape only\n";
    std::cout << "  2 - Translation (Red)\n";
    std::cout << "  3 - Rotation (Green)\n";
    std::cout << "  4 - Scaling (Blue)\n";
    std::cout << "  5 - Shearing (Yellow)\n\n";

    std::cout << "CAMERA CONTROLS:\n";
    std::cout << "  Arrow Keys - Rotate camera view\n";
    std::cout << "  Page Up/Down - Zoom in/out\n";
    std::cout << "  ESC - Exit\n\n";
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 700);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("3D Transformations with Pre-defined Constants");

    init();
    printInstructions();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}