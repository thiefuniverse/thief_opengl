#ifndef __SHADER_HELPER_H_
#define __SHADER_HELPER_H_

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
    static const std::map< SHADER_TYPE, GLenum > ShaderType2GLType;
    bool addShaderByPath(const char *shader_path);
    bool addShaderByContent(const char *shader_content);
    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    bool CompileShader(const char *shader_content, SHADER_TYPE type, int &shader_id);
    vector< int > shader_ids_buffer;
    unsigned int ID;
};

#endif  // __SHADER_HELPER_H_
