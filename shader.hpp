#ifndef SHADER_HPP
#define SHADER_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char* vertex_file_path, const char* frag_file_path)
        {
            std::string vertexCode;
            std::string fragCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                vShaderFile.open(vertex_file_path);
                fShaderFile.open(frag_file_path);

                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
            
                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragCode = fShaderStream.str();
            }
            catch (std::ifstream::failure &e)
            {
                std::cerr << "ERROR: Shader file cannot be read successfully: " << e.what() << std::endl;
            }

            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragCode.c_str();

            // compile shaders
            unsigned int vertex, fragment;

            // vertex shader
            vertex = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertex, 1, &vShaderCode, NULL);
            glCompileShader(vertex);
            checkCompileErrors(vertex, "VERTEX");

            // fragment shader
            fragment = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragment, 1, &fShaderCode, NULL);
            glCompileShader(fragment);
            checkCompileErrors(fragment, "FRAGMENT");

            // shader program
            ID = glCreateProgram();
            glAttachShader(ID, vertex);
            glAttachShader(ID, fragment);
            glLinkProgram(ID);
            checkCompileErrors(ID, "PROGRAM");

            glDeleteShader(vertex);
            glDeleteShader(fragment);
        }

        void use()
        {
        	glUseProgram(ID);

        }

        // sets a boolean value to a bool uniform variable
        void setBool(const std::string &name, bool value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        // sets a int value to a int uniform variable
        void setInt(const std::string &name, int value)
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

        // sets a float value to a float uniform variable
        void setFloat(const std::string &name, float value)
        {
	        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        }

    private:
        void checkCompileErrors(unsigned int shader, std::string type)
        {
            int success;
            char infoLog[1024];

            if (type == "PROGRAM")
            {
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if (!success)
                {
                    glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR: Shader compilation error of type: " << type << "\n" << infoLog << std::endl; 
                }
            }
            else
            {
                glGetProgramiv(shader, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                    std::cerr << "ERROR: Shader linking error of type: " << type << "\n" << infoLog << std::endl; 
                }
            }
        }
};


// GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif