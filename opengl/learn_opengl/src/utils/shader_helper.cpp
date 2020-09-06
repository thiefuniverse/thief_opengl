#include "shader_helper.hpp"
#include "utils.hpp"
#include <memory>

const std::map< SHADER_TYPE, GLenum > ShaderHelper::ShaderType2GLType = {{VERTEX_SHADER, GL_VERTEX_SHADER}, {FRAGMENT_SHADER, GL_FRAGMENT_SHADER}};

ShaderHelper::ShaderHelper() {
    ID = -1;
}
ShaderHelper::~ShaderHelper() {}

bool ShaderHelper::addShaderByPath(const char *shader_path, SHADER_TYPE type) {
    std::unique_ptr< char[] > shader_content;
    int len = 0;
    if (!ReadFile(shader_path, shader_content, len)) {
        std::cout << "read shader path failed: " << shader_path << std::endl;
        return false;
    }
    return addShaderByContent(shader_content.get(), type);
}

bool ShaderHelper::addShaderByContent(const char *shader_content, SHADER_TYPE type) {
    int shader_id = 0;
    bool ret = CompileShader(shader_content, type, shader_id);
    if (!ret) {
        std::cout << "compile shader error" << std::endl;
        return false;
    }
    shader_ids_buffer.push_back(shader_id);
    return false;
}

bool ShaderHelper::CompileShader(const char *shader_content, SHADER_TYPE type, int &shader_id) {
    // compile shader
    int success;
    char infoLog[512];

    if (ShaderType2GLType.find(type) == ShaderType2GLType.end()) {
        return false;
    }
    shader_id = glCreateShader(ShaderType2GLType.at(type));
    glShaderSource(shader_id, 1, &shader_content, NULL);
    glCompileShader(shader_id);

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
        std::cout << "shader compile failed: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool ShaderHelper::linkShader() {
    int success;
    char infoLog[512];
    ID = glCreateProgram();
    for (auto &id : shader_ids_buffer) {
        glAttachShader(ID, id);
    }
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "link shader error: " << infoLog << std::endl;
        return false;
    }
    for (auto &id : shader_ids_buffer) {
        glDeleteShader(id);
    }
    shader_ids_buffer.clear();
    return true;
}

void ShaderHelper::use() {
    glUseProgram(ID);
}

unsigned int ShaderHelper::getID() {
    return ID;
}
