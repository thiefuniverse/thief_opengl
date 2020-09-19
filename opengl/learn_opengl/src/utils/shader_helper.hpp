#ifndef __SHADER_HELPER_H_
#define __SHADER_HELPER_H_

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "light.hpp"
#include "material.hpp"
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

enum SHADER_TYPE { VERTEX_SHADER, FRAGMENT_SHADER };
class ShaderHelper {
public:
    ShaderHelper();
    ~ShaderHelper();
    static const std::map< SHADER_TYPE, GLenum > ShaderType2GLType;
    bool addShaderByPath(const char *shader_path, SHADER_TYPE type);
    bool addShaderByContent(const char *shader_content, SHADER_TYPE type);
    bool linkShader();
    void use();

    template < typename T >
    void setValue(const std::string &name, T value) const {}
    template <>
    void setValue(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }
    template <>
    void setValue(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }
    template <>
    void setValue(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), ( int )value);
    }
    template <>
    void setValue(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }
    template <>
    void setValue(const std::string &name, glm::vec3 value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }
    void setMaterial(const std::string flag, material ma);
    void setLight(const std::string flag, light l);
    unsigned int getID();

private:
    bool CompileShader(const char *shader_content, SHADER_TYPE type, int &shader_id);
    std::vector< int > shader_ids_buffer;
    unsigned int ID;
};

#endif  // __SHADER_HELPER_H_
