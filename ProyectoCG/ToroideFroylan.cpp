#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glfw3.h>

/*
r = radio externo
c = radio interno
rSeg, cSeg = resolución externa/interna
A mayor resolución, mayor detalle
*/

void drawTorus(float, float, int, int, int);

void drawTorus(float r = 0.07f, float c = 0.15f, int rSeg = 16, int cSeg = 16, int texture = 0)
{
    glFrontFace(GL_CW);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    const float PI = 3.14159f;
    const float TAU = 2.0f * PI;

    for (int i = 0; i < rSeg; i++) {
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j <= cSeg; j++) {
            for (int k = 0; k <= 1; k++) {
                float s = (i + k) % rSeg + 0.5f;
                float t = j % (cSeg + 1);

                float x = (c + r * cos(s * TAU / rSeg)) * cos(t * TAU / cSeg);
                float y = (c + r * cos(s * TAU / rSeg)) * sin(t * TAU / cSeg);
                float z = r * sin(s * TAU / rSeg);

                float u = (i + k) / (float)rSeg;
                float v = t / (float)cSeg;

                glTexCoord2d(u, v);
                glNormal3f(2 * x, 2 * y, 2 * z);
                glVertex3d(2 * x, 2 * y, 2 * z);
            }
        }
        glEnd();
    }

    glFrontFace(GL_CCW);
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
    window = glfwCreateWindow(800, 600, "Toroide", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to make window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window)) {
        double mx, my, t, dt;
        int winWidth, winHeight;
        int fbWidth, fbHeight;
        float ratio;

        glfwGetCursorPos(window, &mx, &my);
        glfwGetWindowSize(window, &winWidth, &winHeight);
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

        ratio = (float)fbWidth / (float)winWidth;
        glViewport(0, 0, fbWidth, fbHeight);

        glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawTorus();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
