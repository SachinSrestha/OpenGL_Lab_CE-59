#include <GL/glut.h>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Point
{
    float x;
    float y;
};

typedef float Matrix3x3[3][3];

// Initialize identity matrix
void Identity(Matrix3x3 M)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            M[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

// Draw coordinate axes with grid
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

// Draw a filled triangle
void drawTriangle(Point p1, Point p2, Point p3)
{
    glBegin(GL_TRIANGLES);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glEnd();
}

// Draw triangle outline with white/cyan border
void drawTriangleOutline(Point p1, Point p2, Point p3, bool isCyan = false)
{
    glLineWidth(2.0f);
    if (isCyan)
        glColor3f(0.0f, 1.0f, 1.0f); // Cyan outline
    else
        glColor3f(1.0f, 1.0f, 1.0f); // White outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(p1.x, p1.y);
    glVertex2f(p2.x, p2.y);
    glVertex2f(p3.x, p3.y);
    glEnd();
    glLineWidth(1.0f);
}

// Transform a point using homogeneous coordinates
Point transformPoint(Point p, Matrix3x3 M)
{
    Point result;
    result.x = M[0][0] * p.x + M[0][1] * p.y + M[0][2];
    result.y = M[1][0] * p.x + M[1][1] * p.y + M[1][2];
    return result;
}

// Translation matrix
void translate(Matrix3x3 T, float tx, float ty)
{
    Identity(T);
    T[0][2] = tx;
    T[1][2] = ty;
}

// Scaling matrix
void scale(Matrix3x3 S, float scaleX, float scaleY)
{
    Identity(S);
    S[0][0] = scaleX;
    S[1][1] = scaleY;
}

// Rotation matrix
void rotate(Matrix3x3 R, float theta)
{
    Identity(R);
    R[0][0] = cos(theta);
    R[0][1] = -sin(theta);
    R[1][0] = sin(theta);
    R[1][1] = cos(theta);
}

// Shearing matrix
void shear(Matrix3x3 Sh, float shx, float shy)
{
    Identity(Sh);
    Sh[0][1] = shx;
    Sh[1][0] = shy;
}

// Matrix multiplication
void matrixMultiply(Matrix3x3 A, Matrix3x3 B, Matrix3x3 result)
{
    Matrix3x3 temp;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            temp[i][j] = 0;
            for (int k = 0; k < 3; k++)
            {
                temp[i][j] += A[i][k] * B[k][j];
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

// Display callback function
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw grid first (background)
    drawGrid();

    // Draw axes (foreground)
    drawAxes();

    // Draw axis labels
    drawAxisLabels();

    // Define original triangle in NDC coordinates - CENTER
    Point t1 = {-0.08f, -0.1f}; // Bottom-left
    Point t2 = {-0.08f, 0.05f}; // Top-left
    Point t3 = {0.05f, -0.1f};  // Bottom-right

    // Draw original triangle (BLUE with WHITE outline) - CENTER
    glColor3f(0.2f, 0.6f, 0.9f); // Blue fill
    drawTriangle(t1, t2, t3);
    drawTriangleOutline(t1, t2, t3, false); // White outline

    // ===== STEP 1: TRANSLATION (Left side) =====
    Matrix3x3 T1;
    translate(T1, -0.65f, 0.0f);

    Point t1_step1 = transformPoint(t1, T1);
    Point t2_step1 = transformPoint(t2, T1);
    Point t3_step1 = transformPoint(t3, T1);

    glColor3f(0.9f, 0.3f, 0.3f); // Red fill
    drawTriangle(t1_step1, t2_step1, t3_step1);
    drawTriangleOutline(t1_step1, t2_step1, t3_step1, true); // Cyan outline

    // ===== STEP 2: TRANSLATION + ROTATION (Top side) =====
    Matrix3x3 T_to_origin, R, T_back, T2, temp1, temp2, temp3, result2;

    Point center_step1 = getCentroid(t1_step1, t2_step1, t3_step1);

    // Translate to origin
    translate(T_to_origin, -center_step1.x, -center_step1.y);

    // Rotate 45 degrees
    rotate(R, 45.0f * M_PI / 180.0f);

    // Translate back
    translate(T_back, center_step1.x, center_step1.y);

    // Additional translation to top
    translate(T2, 0.65f, 0.62f);

    // Compose: T2 * T_back * R * T_to_origin * T1
    matrixMultiply(T_to_origin, T1, temp1);
    matrixMultiply(R, temp1, temp2);
    matrixMultiply(T_back, temp2, temp3);
    matrixMultiply(T2, temp3, result2);

    Point t1_step2 = transformPoint(t1, result2);
    Point t2_step2 = transformPoint(t2, result2);
    Point t3_step2 = transformPoint(t3, result2);

    glColor3f(1.0f, 0.8f, 0.0f); // Yellow fill
    drawTriangle(t1_step2, t2_step2, t3_step2);
    drawTriangleOutline(t1_step2, t2_step2, t3_step2, true); // Cyan outline

    // ===== STEP 3: TRANSLATION + ROTATION + SCALE (Right side) =====
    Matrix3x3 S, T_to_origin2, T_back2, T3, temp4, temp5, temp6, temp7, result3;

    Point center_step2 = getCentroid(t1_step2, t2_step2, t3_step2);

    // Translate to origin
    translate(T_to_origin2, -center_step2.x, -center_step2.y);

    // Scale 1.3x
    scale(S, 1.3f, 1.3f);

    // Translate back
    translate(T_back2, center_step2.x, center_step2.y);

    // Additional translation to right
    translate(T3, 0.18f, -0.62f);

    // Compose: T3 * T_back2 * S * T_to_origin2 * result2
    matrixMultiply(T_to_origin2, result2, temp4);
    matrixMultiply(S, temp4, temp5);
    matrixMultiply(T_back2, temp5, temp6);
    matrixMultiply(T3, temp6, result3);

    Point t1_step3 = transformPoint(t1, result3);
    Point t2_step3 = transformPoint(t2, result3);
    Point t3_step3 = transformPoint(t3, result3);

    glColor3f(0.3f, 1.0f, 0.3f); // Green fill
    drawTriangle(t1_step3, t2_step3, t3_step3);
    drawTriangleOutline(t1_step3, t2_step3, t3_step3, true); // Cyan outline

    // ===== STEP 4: TRANSLATION + ROTATION + SCALE + SHEAR (Bottom side) =====
    Matrix3x3 Sh, T4, temp8, result4;

    // Shear
    shear(Sh, 0.8f, 0.0f);

    // Additional translation to bottom
    translate(T4, -0.18f, -0.65f);

    // Compose: T4 * Sh * result3
    matrixMultiply(Sh, result3, temp8);
    matrixMultiply(T4, temp8, result4);

    Point t1_step4 = transformPoint(t1, result4);
    Point t2_step4 = transformPoint(t2, result4);
    Point t3_step4 = transformPoint(t3, result4);

    glColor3f(1.0f, 0.3f, 0.8f); // Magenta fill
    drawTriangle(t1_step4, t2_step4, t3_step4);
    drawTriangleOutline(t1_step4, t2_step4, t3_step4, true); // Cyan outline

    glutSwapBuffers();
}

// Reshape callback
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Display transformation info
void displayTransformInfo()
{
    std::cout << "\n========================================================\n";
    std::cout << "  COMPOSITE TRANSFORMATION WITH INTERMEDIATE STEPS\n";
    std::cout << "  Using Homogeneous Coordinates (3x3 Matrices)\n";
    std::cout << "========================================================\n\n";

    std::cout << "Final Composite Transformation = Shear * Scale * Rotate * Translate\n\n";

    std::cout << "STEP 0: ORIGINAL (BLUE with WHITE outline)\n";
    std::cout << "  Position: CENTER of screen\n";
    std::cout << "  Starting triangle at (0, 0)\n";
    std::cout << "  Matrix: I (Identity)\n\n";

    std::cout << "STEP 1: TRANSLATE (RED with CYAN outline)\n";
    std::cout << "  Position: LEFT side (around center)\n";
    std::cout << "  Operation: Translate left by (-0.55, 0.0)\n";
    std::cout << "  Matrix: T1\n";
    std::cout << "  Result: T1 applied to original\n\n";

    std::cout << "STEP 2: TRANSLATE + ROTATE (YELLOW with CYAN outline)\n";
    std::cout << "  Position: TOP side (around center)\n";
    std::cout << "  Operation: Rotate 45° + Translate up\n";
    std::cout << "  Matrix: T2 * T_back * R * T_origin * T1\n";
    std::cout << "  Result: Rotation applied to Step 1\n\n";

    std::cout << "STEP 3: TRANSLATE + ROTATE + SCALE (GREEN with CYAN outline)\n";
    std::cout << "  Position: RIGHT side (around center)\n";
    std::cout << "  Operation: Scale 1.3x + Translate right\n";
    std::cout << "  Matrix: T3 * T_back * S * T_origin * [Step 2 Matrix]\n";
    std::cout << "  Result: Scaling applied to Step 2\n\n";

    std::cout << "STEP 4: TRANSLATE + ROTATE + SCALE + SHEAR (MAGENTA with CYAN outline)\n";
    std::cout << "  Position: BOTTOM side (around center)\n";
    std::cout << "  Operation: Shear (0.8, 0.0) + Translate down\n";
    std::cout << "  Matrix: T4 * Sh * [Step 3 Matrix]\n";
    std::cout << "  Result: Final composite transformation\n\n";

    std::cout << "========================================================\n";
    std::cout << "Total: 5 triangles displayed (NO OVERLAP)\n";
    std::cout << "  - Original (Blue) - CENTER\n";
    std::cout << "  - After Step 1 (Red) - LEFT side\n";
    std::cout << "  - After Step 2 (Yellow) - TOP side\n";
    std::cout << "  - After Step 3 (Green) - RIGHT side\n";
    std::cout << "  - After Step 4 - FINAL (Magenta) - BOTTOM side\n";
    std::cout << "\n  Layout: Transformed triangles surround the center original\n";
    std::cout << "Press ESC to exit\n";
    std::cout << "========================================================\n\n";
}

// Keyboard callback
void keyboard(unsigned char key, int x, int y)
{
    if (key == 27) // ESC key
    {
        exit(0);
    }
}

// Main function
int main(int argc, char **argv)
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Composite Transformation with Steps");

    // Set clear color - dark background
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    // Enable blending for smooth rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable line smoothing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    // Display transformation information
    displayTransformInfo();

    // Register callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Enter main loop
    glutMainLoop();

    return 0;
}