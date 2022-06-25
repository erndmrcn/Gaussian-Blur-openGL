#include <iostream>
#include <string>
#include <stb_image.h>
#include <shader.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
using namespace glm;


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

// configuration about window settings
void configure(int major, int minor, GLboolean forward_compat, GLenum profile)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, forward_compat);
    glfwWindowHint(GLFW_OPENGL_PROFILE, profile);
}

// does all the initializations necessary
void initialize(int width, int height, const char *windowName)
{
    if (!glfwInit())
    {
        std::cout<<"Failed to initialize GLFW\n"<<std::endl;
        exit(-1);
    }

    configure(3, 3, GL_TRUE, GLFW_OPENGL_CORE_PROFILE);
    glfwSetErrorCallback(errorCallback);
    
    win = glfwCreateWindow(1024, 768, windowName, NULL, NULL);
    if (!win)
    {
        std::cerr << "Failed to open GLFW window.\n" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(win);
    glewExperimental= true;
    
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cerr << "GLEW initialization failed.\n" << std::endl;
        exit(-1);
    }

    glfwSetInputMode(win, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetKeyCallback(win, keyCallback);
}


Shader loadShaders(const char *vertexShaderName, const char *fragmentShaderName)
{
    Shader ourShader = Shader(vertexShaderName, fragmentShaderName);
    return ourShader;
}

void loadTexture(const char *fileName, GLuint& texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loading Texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);

    // generating textures
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cerr << "Failed to load texture image: " << stbi_failure_reason() << std::endl;
    }
    stbi_image_free(data);
}

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::cerr << "Wrong usage. Correct usage as follows: ./blur <image_to_be_blurred>." << std::endl;
        exit(-1);
    }
    else if (argc > 2)
    {
        std::cerr << "Wrong usage. You have provided extra input." << std::endl;
        std::cerr << "Correct usage as follows: ./blur <image_to_be_blurred>." << std::endl;
        exit(-1);
    }

    initialize(1024, 768, "Gaussian Blur");

    // colored and texture vertices
    static const GLfloat vertices[] = {
        // positions            // colors         // texture coordinates
        -1.0f,  1.0f, 1.0f,   0.1f, 0.0f, 0.0f,     0.0f, 1.0f,             // top left 
         1.0f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,     1.0f, 1.0f,             // top right
         1.0f, -1.0f, 1.0f,   0.0f, 0.0f, 0.1f,     1.0f, 0.0f,             // bottom right
        -1.0f, -1.0f, 1.0f,   1.0f, 0.0f, 0.0f,     0.0f, 0.0f              // bottom left
    };

    static const GLuint indices[] = {
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    Shader ourShader = loadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
    
    GLuint texture;
    loadTexture(argv[1], texture);

    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    while(!glfwWindowShouldClose(win))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glUniform1i(loc, 0);
        glBindTexture(GL_TEXTURE_2D, texture);

        ourShader.use();       
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    // Cleanup VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
