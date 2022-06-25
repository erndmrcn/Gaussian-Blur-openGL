#include <iostream>
#include <chrono>
#include <string>
#include <cmath>
// #include "jpeghelper.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;

#include <shader.hpp>

static GLFWwindow* win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;

int widthWindow = 1024, heightWindow = 768;
int *t_width, *t_height;

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
    glClearColor(1.0, 0.4, 0.05, 1);
    
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    Shader shader = Shader("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
    GLuint programID = shader.ID;

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f, // bottom left
         1.0f,  1.0f, 0.0f, // top right
         1.0f, -1.0f, 0.0f, // bottom right
        -1.0f,  1.0f, 0.0f  // top left
    };

    static const GLuint indices[] = {
        0, 2, 1, // first triangle
        0, 1, 3  // second triangle
    };


    static const GLfloat colored_vertices[] = {
         0.0,  0.5, 1.0, 0.7,  0.05, 0.05, // reddish
        -0.5, -0.5, 1.0, 0.05, 0.7,  0.05, // greenish
         0.5, -0.5, 1.0, 0.05, 0.05, 0.7  // blueish
    };

    GLuint EBO, clr;
    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &clr);


    // glUniform1i(glGetUniformLocation(shader.ID, "greenColor"), (0.0, greenValue, 0.0));

    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, clr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(colored_vertices), colored_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // initTexture(idJpegTexture, argv[1], t_width, t_height);

    while(!glfwWindowShouldClose(win))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        // glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glUseProgram(programID);

        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3*sizeof(float),
            (void*)0
        );
        glEnableVertexAttribArray(0);

        float timeValue = glfwGetTime();
        float greenValue = sin(timeValue) / 2.0f + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
        glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
        // glDisableVertexAttribArray(0);

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