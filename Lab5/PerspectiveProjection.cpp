#include <GL/glut.h>
#include <iostream>
#include <cmath>

// Class to handle Perspective Projection
class Perspective
{
private:
    // Perspective projection parameters
    float fovy;      // Field of view in y direction (degrees)
    float aspect;    // Aspect ratio (width/height)
    float nearPlane; // Near clipping plane
    float farPlane;  // Far clipping plane

    // View parameters
    float eyeX, eyeY, eyeZ;
    float centerX, centerY, centerZ;
    float upX, upY, upZ;

    // Viewport dimensions
    int windowWidth, windowHeight;

public:
    // Constructor
    Perspective()
    {
        windowWidth = 800;
        windowHeight = 600;
        reset();
    }

    // Setup projection
    void setupPerspective()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, aspect, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

    void setupViewport(int width, int height)
    {
        windowWidth = width;
        windowHeight = height;

        // Prevent division by zero
        if (height == 0)
            height = 1;

        aspect = (float)width / (float)height;

        glViewport(0, 0, width, height);
        setupPerspective();
    }

    // Set projection parameters
    void setPerspective(float fov, float asp, float n, float f)
    {
        fovy = fov;
        aspect = asp;
        nearPlane = n;
        farPlane = f;
    }

    void setView(float ex, float ey, float ez,
                 float cx, float cy, float cz,
                 float ux, float uy, float uz)
    {
        eyeX = ex;
        eyeY = ey;
        eyeZ = ez;
        centerX = cx;
        centerY = cy;
        centerZ = cz;
        upX = ux;
        upY = uy;
        upZ = uz;
    }

    // Apply projection and view
    void applyProjection()
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(fovy, aspect, nearPlane, farPlane);
        glMatrixMode(GL_MODELVIEW);
    }

    void applyView()
    {
        gluLookAt(eyeX, eyeY, eyeZ,
                  centerX, centerY, centerZ,
                  upX, upY, upZ);
    }

    // Modify projection parameters
    void adjustFOV(float delta)
    {
        fovy += delta;

        // Clamp FOV to reasonable range (1-179 degrees)
        if (fovy < 1.0f)
            fovy = 1.0f;
        if (fovy > 179.0f)
            fovy = 179.0f;

        std::cout << "FOV: " << fovy << " degrees" << std::endl;
    }

    void adjustNearFar(float delta)
    {
        nearPlane += delta;
        farPlane += delta;

        // Ensure near plane is positive and less than far plane
        if (nearPlane <= 0.1f)
            nearPlane = 0.1f;

        if (nearPlane >= farPlane - 1.0f)
            nearPlane = farPlane - 1.0f;

        std::cout << "Near: " << nearPlane << ", Far: " << farPlane << std::endl;
    }

    void moveEye(float dx, float dy, float dz)
    {
        eyeX += dx;
        eyeY += dy;
        eyeZ += dz;

        std::cout << "Eye: (" << eyeX << ", " << eyeY << ", " << eyeZ << ")" << std::endl;
    }

    void moveCenter(float dx, float dy, float dz)
    {
        centerX += dx;
        centerY += dy;
        centerZ += dz;

        std::cout << "Center: (" << centerX << ", " << centerY << ", " << centerZ << ")" << std::endl;
    }

    // Handle input
    void handleKeyboard(unsigned char key)
    {
        switch (key)
        {
        case '+':
        case '=':
            adjustFOV(-5.0f); // Decrease FOV (zoom in)
            setupPerspective();
            break;
        case '-':
        case '_':
            adjustFOV(5.0f); // Increase FOV (zoom out)
            setupPerspective();
            break;
        case 'n':
        case 'N':
            adjustNearFar(0.5f);
            setupPerspective();
            break;
        case 'f':
        case 'F':
            adjustNearFar(-0.5f);
            setupPerspective();
            break;
        case 'w':
        case 'W':
            moveEye(0.0f, 0.0f, -0.5f); // Move eye forward
            break;
        case 's':
        case 'S':
            moveEye(0.0f, 0.0f, 0.5f); // Move eye backward
            break;
        case 'a':
        case 'A':
            moveEye(-0.5f, 0.0f, 0.0f); // Move eye left
            break;
        case 'd':
        case 'D':
            moveEye(0.5f, 0.0f, 0.0f); // Move eye right
            break;
        case 'q':
        case 'Q':
            moveEye(0.0f, 0.5f, 0.0f); // Move eye up
            break;
        case 'e':
        case 'E':
            moveEye(0.0f, -0.5f, 0.0f); // Move eye down
            break;
        case 'i':
        case 'I':
            moveCenter(0.0f, 0.0f, -0.5f); // Move center forward
            break;
        case 'k':
        case 'K':
            moveCenter(0.0f, 0.0f, 0.5f); // Move center backward
            break;
        case 'j':
        case 'J':
            moveCenter(-0.5f, 0.0f, 0.0f); // Move center left
            break;
        case 'l':
        case 'L':
            moveCenter(0.5f, 0.0f, 0.0f); // Move center right
            break;
        case 'u':
        case 'U':
            moveCenter(0.0f, 0.5f, 0.0f); // Move center up
            break;
        case 'o':
        case 'O':
            moveCenter(0.0f, -0.5f, 0.0f); // Move center down
            break;
        case 'p':
        case 'P':
            printParameters();
            break;
        case 'r':
        case 'R':
            reset();
            setupPerspective();
            std::cout << "Projection reset" << std::endl;
            break;
        }
    }

    void handleSpecialKeys(int key)
    {
        switch (key)
        {
        case GLUT_KEY_LEFT:
            moveEye(-0.5f, 0.0f, 0.0f);
            break;
        case GLUT_KEY_RIGHT:
            moveEye(0.5f, 0.0f, 0.0f);
            break;
        case GLUT_KEY_UP:
            moveEye(0.0f, 0.5f, 0.0f);
            break;
        case GLUT_KEY_DOWN:
            moveEye(0.0f, -0.5f, 0.0f);
            break;
        case GLUT_KEY_PAGE_UP:
            moveEye(0.0f, 0.0f, -0.5f);
            break;
        case GLUT_KEY_PAGE_DOWN:
            moveEye(0.0f, 0.0f, 0.5f);
            break;
        }
    }

    // Reset to default
    void reset()
    {
        // Default perspective parameters
        fovy = 45.0f; // 45 degree field of view
        aspect = (float)windowWidth / (float)windowHeight;
        nearPlane = 0.1f;  // Near clipping plane (must be > 0)
        farPlane = 100.0f; // Far clipping plane

        // Default view parameters
        eyeX = 5.0f;
        eyeY = 5.0f;
        eyeZ = 10.0f;
        centerX = 0.0f;
        centerY = 0.0f;
        centerZ = 0.0f;
        upX = 0.0f;
        upY = 1.0f;
        upZ = 0.0f;
    }

    // Get parameters
    void printParameters() const
    {
        std::cout << "\n=== Perspective Projection Parameters ===" << std::endl;
        std::cout << "Field of View: " << fovy << " degrees" << std::endl;
        std::cout << "Aspect Ratio: " << aspect << std::endl;
        std::cout << "Near Plane: " << nearPlane << std::endl;
        std::cout << "Far Plane: " << farPlane << std::endl;
        std::cout << "Eye: (" << eyeX << ", " << eyeY << ", " << eyeZ << ")" << std::endl;
        std::cout << "Center: (" << centerX << ", " << centerY << ", " << centerZ << ")" << std::endl;
        std::cout << "Up: (" << upX << ", " << upY << ", " << upZ << ")" << std::endl;
        std::cout << "=========================================\n"
                  << std::endl;
    }
};

// Global perspective object
Perspective perspective;

// Function to draw a simple 3D scene
void drawScene()
{
    // Draw a colored cube at origin
    glBegin(GL_QUADS);

    // Front face (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back face (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Top face (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Bottom face (yellow)
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (magenta)
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Left face (cyan)
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glEnd();

    // Draw coordinate axes
    glBegin(GL_LINES);
    // X axis (red)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 0.0f);

    // Y axis (green)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 3.0f, 0.0f);

    // Z axis (blue)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 3.0f);
    glEnd();

    // Draw a ground plane
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex3f(-5.0f, -1.5f, -5.0f);
    glVertex3f(5.0f, -1.5f, -5.0f);
    glVertex3f(5.0f, -1.5f, 5.0f);
    glVertex3f(-5.0f, -1.5f, 5.0f);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply the camera view
    perspective.applyView();

    // Draw the scene
    drawScene();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
    perspective.setupViewport(width, height);
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // ESC key
    {
        exit(0);
    }

    perspective.handleKeyboard(key);
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
    perspective.handleSpecialKeys(key);
    glutPostRedisplay();
}

void printInstructions()
{
    std::cout << "\n========== PERSPECTIVE PROJECTION CONTROLS ==========" << std::endl;
    std::cout << "Field of View (Zoom):" << std::endl;
    std::cout << "  +/= : Decrease FOV (zoom in)" << std::endl;
    std::cout << "  -/_ : Increase FOV (zoom out)" << std::endl;
    std::cout << "\nClipping Planes:" << std::endl;
    std::cout << "  N   : Move near/far planes away" << std::endl;
    std::cout << "  F   : Move near/far planes closer" << std::endl;
    std::cout << "\nCamera Position (Eye):" << std::endl;
    std::cout << "  W/S : Move forward/backward" << std::endl;
    std::cout << "  A/D : Move left/right" << std::endl;
    std::cout << "  Q/E : Move up/down" << std::endl;
    std::cout << "  Arrow Keys : Move eye position" << std::endl;
    std::cout << "  PgUp/PgDn  : Move eye forward/backward" << std::endl;
    std::cout << "\nLook-At Point (Center):" << std::endl;
    std::cout << "  I/K : Move center forward/backward" << std::endl;
    std::cout << "  J/L : Move center left/right" << std::endl;
    std::cout << "  U/O : Move center up/down" << std::endl;
    std::cout << "\nOther:" << std::endl;
    std::cout << "  P   : Print current parameters" << std::endl;
    std::cout << "  R   : Reset to default view" << std::endl;
    std::cout << "  ESC : Exit" << std::endl;
    std::cout << "====================================================\n"
              << std::endl;
}

int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Perspective Projection Example");

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Setup initial viewport and projection
    perspective.setupViewport(800, 600);

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Print instructions
    printInstructions();
    perspective.printParameters();

    // Enter main loop
    glutMainLoop();

    return 0;
}