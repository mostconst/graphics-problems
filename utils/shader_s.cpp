#include "shader_s.h"

#include "Shader.h"

void checkCompileErrors(GLuint shader, std::string type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
                "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

ShaderProgram createShader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    // open files
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    std::string vertexCode = vShaderStream.str();
    std::string fragmentCode = fShaderStream.str();

    return {vertexCode, fragmentCode};
}

ShaderProgram::ShaderProgram(const std::string& vertexCode, const std::string& fragmentCode)
{
    const char* fShaderCode = fragmentCode.c_str();
    Shader vertex(Shader::Type::Vertex);
    vertex.Compile(vertexCode);
    Shader fragment(Shader::Type::Fragment);
    fragment.Compile(fragmentCode);
    id = glCreateProgram();
    glAttachShader(id, vertex.GetId());
    glAttachShader(id, fragment.GetId());
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
    : id(other.id)
{
    other.id = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
    if (this != &other)
    {
        id = other.id;
        other.id = 0;
    }

    return *this;
}

void ShaderProgram::use() const
{
    glUseProgram(id);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

