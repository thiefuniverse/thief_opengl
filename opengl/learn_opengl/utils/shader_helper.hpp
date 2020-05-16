#ifndef __SHADER_HELPER_H_
#define __SHADER_HELPER_H_

#include <fstream>
#include <glad/glad.h>
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

private:
    bool CompileShader(const char *shader_content, SHADER_TYPE type, int &shader_id);
    std::vector< int > shader_ids_buffer;
    unsigned int ID;
};

#endif  // __SHADER_HELPER_H_
