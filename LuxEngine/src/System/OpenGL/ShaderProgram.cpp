

/*static std::variant<std::array<RawShader, 2>, ShaderError> pre_process_shader_file(std::fstream& shader_file, bool& has_failed)
{

}*/

#include "ShaderProgram.h"

#include <initializer_list>
#include <glad/glad.h>

#include "OpenGL.h"

namespace Lux::OpenGL
{

constexpr GLenum TO_GL_TYPE[] = {
    GL_VERTEX_SHADER,
    GL_FRAGMENT_SHADER,
    GL_COMPUTE_SHADER,
};

void ShaderProgram::get_shader_info(std::unordered_map<std::string, ShaderDataType>& uniforms, std::unordered_map<std::string, ShaderDataType>& attributes)
{
    int size;     // size of the variable
    GLenum type;    // type of the variable (float, vec3 or mat4, etc)

    constexpr int BUFFER_SIZE = 4096; // maximum name length
    char name[BUFFER_SIZE];       // variable name in GLSL
    int length;             // name length
    int count = 0;
    
    // Uniforms
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &count);

    for (u32 i = 0; i < static_cast<u32>(count); i++)
    {
        glGetActiveUniform(m_id, (GLuint)i, BUFFER_SIZE, &length, &size, &type, name);

        std::string uniform_name = name;
        if(uniform_name.ends_with("[0]"))
            uniform_name = uniform_name.substr(0, uniform_name.size() - 3);

        u32 uniform_location = glGetUniformLocation(m_id, uniform_name.c_str());
        uniforms.insert({uniform_name, ShaderDataType { uniform_location, static_cast<u32>(size), gl_to_lux_datatype(type), uniform_name}});

    
    }

    // Attributes
    glGetProgramiv(m_id, GL_ACTIVE_ATTRIBUTES, &count);
    
    for(u32 i = 0; i < static_cast<u32>(count); i++)
    {
        glGetActiveAttrib(m_id, (GLuint)i, BUFFER_SIZE, &length, &size, &type, name);

        std::string attribute_name = name;
        if(attribute_name.ends_with("[0]"))
            attribute_name = attribute_name.substr(0, attribute_name.size() - 3);

        attributes.insert({ attribute_name, ShaderDataType { i, static_cast<u32>(size), gl_to_lux_datatype(type), attribute_name}});
    }

}

ShaderProgram::ShaderProgram(std::vector<RawShader> shaders)
{
    m_id = glCreateProgram();

    for(RawShader& shader : shaders)
    {

        GLenum glType = TO_GL_TYPE[static_cast<int>(shader.type)];
        shader.id = glCreateShader(glType);
        
        const char* source = shader.source.c_str();
        int length = (int)shader.source.length();

        
        glShaderSource(shader.id, 1, &source, &length);
        glCompileShader(shader.id);

        int result = 0;
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

        if(result == GL_FALSE)
        {
            int error_message_length = 0;
            glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &error_message_length);

            char* errorMessage = new char[error_message_length + 1];
            glGetShaderInfoLog(shader.id, error_message_length, &error_message_length, errorMessage);


            errorMessage[error_message_length] = '\0';
            std::string error_message = errorMessage;
        
            delete[] errorMessage;

            set_error(ShaderError::CompilationFailed, std::move(error_message));
            return;
        }

        glAttachShader(m_id, shader.id);  
    }

    glLinkProgram(m_id);

    for(RawShader& shader : shaders)
    {
        glDetachShader(m_id, shader.id);
        glDeleteShader(shader.id);
    }
    // Error Checking
    int isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);


    if(isLinked)
    {
        glValidateProgram(m_id);
        return;
    }

    // An Error accured
    int errorMessageLength = 0;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &errorMessageLength);

    char *errorMessage = new char[errorMessageLength];

    glGetProgramInfoLog(m_id, errorMessageLength, &errorMessageLength, errorMessage);

    std::string error_message = errorMessage;

    
    delete[] errorMessage;

    
    set_error(ShaderError::LinkingFailed, std::move(error_message));
}

    
    

}