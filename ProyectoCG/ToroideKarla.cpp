#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glfw3.h>
#include <iostream>

//For Keyboard rgrrwq
float movX = -15.0f;
float movY = 20.0f;
float movZ = -50.0f;
float rotY = 0.0f;

// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

void my_input(GLFWwindow* window);
void resize(GLFWwindow* window, int width, int height);

void dibujaToroide(int Anillos, int Lados, float RadioMayor, float RadioMenor, float Red, float Green, float Blue)
{
    glLoadIdentity();
    glRotatef((float)glfwGetTime() * 20.f, 0.f, 1.f, 0.f);
    glColor4ub(Red, Green, Blue, 255);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0.75, 0);

    glEnd();

    static double PI = 3.1415926535897932384626433832795;

    double majorStep = 2.0 * PI / Anillos;
    double minorStep = 2.0 * PI / Lados;


    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);

    for (int i = 0; i < Anillos; ++i) {
        double a0 = i * majorStep;
        double a1 = a0 + majorStep;
        GLdouble x0 = cos(a0);
        GLdouble y0 = sin(a0);
        GLdouble x1 = cos(a1);
        GLdouble y1 = sin(a1);

        glBegin(GL_TRIANGLE_STRIP);

        for (int j = 0; j <= Lados; ++j) {
            double b = j * minorStep;
            GLdouble c = cos(b);
            GLdouble r = RadioMenor * c + RadioMayor;
            GLdouble z = RadioMenor * sin(b);

            glNormal3d(x0 * c, y0 * c, z / RadioMenor);
            glTexCoord2d(i / (GLdouble)Anillos, j / (GLdouble)Lados);
            glVertex3d(x0 * r, y0 * r, z);

            glNormal3d(x1 * c, y1 * c, z / RadioMenor);
            glTexCoord2d((i + 1) / (GLdouble)Anillos, j / (GLdouble)Lados);
            glVertex3d(x1 * r, y1 * r, z);
        }
        glEnd();
    }
}


int main()
{
    GLFWwindow* window;

    if (!glfwInit()) {
        fprintf(stderr, "Failed to start glfw\n");
        return -1;
    }

    //Set OpenGL Revision
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    //Create Window
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Toroide Karla Najera", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        my_input(window);
        double mx, my, t, dt;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        float ratio;

        glfwGetCursorPos(window, &mx, &my);
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        ratio = (float)fbWidth / (float)winWidth;
        glViewport(0, 0, fbWidth, fbHeight);

        glClearColor(0.3, 0.3, 0.3, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dibujaToroide(25, 80, 0.5, 0.2, 0, 255, 255);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void my_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        movY -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        movY += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.01f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.01f;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}