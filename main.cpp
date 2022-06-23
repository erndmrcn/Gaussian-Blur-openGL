#include <iostream>
#include <chrono>
#include <string>
// #include "jpeghelper.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include <shader.hpp>

static GLFWwindow* win = NULL;

void errorCallback(int error, const char* description)
{
    fprintf(stderr, "Error(%d): %s\n", error, description);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }   
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit())
    {
        std::cout<<"Failed to initialize GLFW\n"<<std::endl;
        exit(-1);
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(1024, 768, "Basic Triangle", NULL, NULL);
    if (!win)
    {
        std::cout<<"Failed to open GLFW window.\n"<<std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(win);

    glewExperimental= true;
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        errorCallback(-1, "GLEW initialization failed");
        exit(-1);
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetKeyCallback(win, keyCallback);
    glClearColor(0.0, 0.4, 0.4, 1);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    while(!glfwWindowShouldClose(win))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glDisableVertexAttribArray(0);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    // Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
    glfwDestroyWindow(win);

    glfwTerminate();
    return 0;
}