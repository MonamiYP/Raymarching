#include "Shader.hpp"
#include "Renderer.hpp"

void Shader::Bind() const {
    glUseProgram(m_rendererID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

void Shader::CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    m_rendererID = program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Returns pointer to data in std::string
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);

    // Compiler error handling
    int success;
    char infoLog[512];
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER:"<<
        (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT")
        <<":COMPILATION_FAILED\n" << infoLog << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

std::string Shader::ParseShader(const std::string& filepath) {
    std::ifstream file(filepath);
    std::string str;
    std::string content;

    while (std::getline(file, str)) {
        content.append(str + "\n");
    }

    return content;
}

int Shader::GetUniformLocation(const std::string& name) {
    if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
        return m_uniformLocationCache[name];

    int location = glGetUniformLocation(m_rendererID, name.c_str());
    if (location == -1)
        std::cout << "Warning: uniform " << name << " doesn't exist" << std::endl;
    
    m_uniformLocationCache[name] = location;
    return location;
}

void Shader::SetFloat(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetInt(const std::string& name, int value) {
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetVector3(const std::string& name, glm::vec3& vector) {
    GLCall(glUniform3fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetVector3(const std::string& name, float v1, float v2, float v3) {
    GLCall(glUniform3f(GetUniformLocation(name), v1, v2, v3));
}

void Shader::SetVector4(const std::string& name, glm::vec4& vector) {
    GLCall(glUniform4fv(GetUniformLocation(name), 1, &vector[0]));
}

void Shader::SetVector4(const std::string& name, float v1, float v2, float v3, float v4) {
    GLCall(glUniform4f(GetUniformLocation(name), v1, v2, v3, v4));
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix) {
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}