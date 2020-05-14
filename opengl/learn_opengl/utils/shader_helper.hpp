#ifndef __SHADER_HELPER_H_
#define __SHADER_HELPER_H_

#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class ShaderHelper {
public:
    unsigned int ID;

    ShaderHelper(const GLchar *vertexPath, const GLchar *fragmentPath);

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif  // __SHADER_HELPER_H_
