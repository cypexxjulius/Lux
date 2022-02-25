#include "System/OpenGL/Shader.h"

#include <string_view>
#include <vector>
#include <fstream>
#include <glad/glad.h>

#pragma warning(disable: 4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)

#include "Utils/Assert.h"
#include "Utils/Logger.h"

#include "Core/IO.h"

namespace Lux::OpenGL
{

int Shader::pre_process_shader_file(std::string& shader_file, std::vector<RawShader>& shaders)
{

    assert(!shader_file.empty());


    int count = 0;
    size_t nPos = shader_file.find("#type"); // first occurrence
    while(nPos != std::string::npos)
    {
        count++;
        nPos = shader_file.find("#type", nPos + 1);
    }

    shaders.reserve(count);

    size_t StartPosition = shader_file.find_first_of("#type") + 5; // Get first #type in file; 5 for strlen("#type")

    if (StartPosition == std::string::npos) // Return Error Code if no #type definition is found
        return 0;

    while (StartPosition != std::string::npos)
    {
        shader_file = shader_file.substr(StartPosition);

        size_t lineLength = shader_file.find_first_of('\n');

        if (lineLength == std::string::npos)
            return 0;

        unsigned int i = 0;
        ShaderType typeOfShader;

        while (shader_file[i] != '\0' || shader_file[i] != '\n') // Iterate through the rest of the line
        {
            if (shader_file[i] == ' ') // dont care about white spaces
            {
                i++;
                continue;
            }
            if(shader_file.substr(i, sizeof("vertex") - 1) == "vertex")
            {
                typeOfShader = ShaderType::Vertex;
                break;
            }

            if (shader_file.substr(i, sizeof("fragment") - 1) == "fragment")
            {
                typeOfShader = ShaderType::Fragment;
                break;
            }

            return 0;
        }
       
        // We have found the ShaderType
        shader_file = shader_file.substr(lineLength + 1); // Set the "Cursor" to the beginning of the Shader

        size_t ShaderStringLength = shader_file.find("#type");

        if (ShaderStringLength == std::string::npos)
        {
            shaders.emplace_back(0, typeOfShader, shader_file);
            break;
        }

        ShaderStringLength--; // Setting the length one char before the '#' of "#type"

        shaders.emplace_back(0, typeOfShader, shader_file.substr(0, ShaderStringLength));

        shader_file = shader_file.substr(ShaderStringLength);

        StartPosition = 1 + sizeof("#type");
    }

    // Success return point
    return 1; 
}

int Shader::compile(std::vector<RawShader>& shaders)
{
    constexpr GLenum TO_GL_TYPE[2] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER
    };

    for(RawShader& shader : shaders)
    {
        GLenum glType = TO_GL_TYPE[static_cast<u8>(shader.type)];
        shader.id = glCreateShader(glType);
        
        const char* source = shader.data.c_str();
        int length = (int)shader.data.length();

        
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
            printf("Failed to compile shader \n%s\n", errorMessage);
        
            delete[] errorMessage;

            shader.id = 0;
        
            TODO();

            return 0;
        }

    }

    return 1;
}

Shader::~Shader()
{
    glDeleteProgram(m_id);
}

void Shader::create_shader(std::vector<RawShader>& raw_shaders)
{
    m_id = glCreateProgram();

    assert(compile(raw_shaders));
        
    for(RawShader& shader : raw_shaders)
        glAttachShader(m_id, shader.id);   

    glLinkProgram(m_id);

    int isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);

    if(!isLinked)
    {
        int errorMessageLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &errorMessageLength);

        char *errorMessage = new char[errorMessageLength];

        glGetProgramInfoLog(m_id, errorMessageLength, &errorMessageLength, errorMessage);

        for(RawShader& shader : raw_shaders)
            glDeleteShader(shader.id);

        LOG("Failed to compile shader", m_name,"\n", errorMessage);
    
        delete[] errorMessage;

        m_id = 0;
    }
    
    for(RawShader& shader : raw_shaders)
    {
        glDetachShader(m_id, shader.id);
        glDeleteShader(shader.id);
    }

    glValidateProgram(m_id);
}

Shader::Shader(std::string_view shaderName, std::string_view shaderPath)
    : m_name(shaderName)
{
    auto file_content = IO::read_file(std::string(shaderPath));
    


    std::vector<RawShader> shaders;
    Verify(pre_process_shader_file(file_content, shaders))
    
    create_shader(shaders);

}

Shader::Shader(std::string_view shaderName, std::initializer_list<RawShader>& shaders)
    : m_name(shaderName)
{
    std::vector<RawShader> raw_shaders = shaders;

    create_shader(raw_shaders);
}

static inline void shader_bind(u32 id)
{
    static u32 bound_shader_id = 0;

    if(id == bound_shader_id)
        return

    glUseProgram(id);
    bound_shader_id = id;
}

void Shader::bind() const
{
    shader_bind(m_id);
}

i32 Shader::get_uniform(std::string_view uniformName)
{
    if(m_uniforms.find(uniformName) != m_uniforms.end())
        return m_uniforms[uniformName];

    i32 location = glGetUniformLocation(m_id, uniformName.data());

    assert(location != -1);

    m_uniforms[uniformName] = location;

    return location;
}

void Shader::set_mat4(std::string_view name, mat4 matrix)
{
    glUniformMatrix4fv(get_uniform(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_float(std::string_view name, float float0)
{
    glUniform1f(get_uniform(name), float0);
}

void Shader::set_int(std::string_view name, int number)
{
    glUniform1i(get_uniform(name), number);
}

void Shader::set_float4(std::string_view name, v4 vec4)
{
    glUniform4f(get_uniform(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::set_int_array(std::string_view name, int* values, u32 count) 
{
    glUniform1iv(get_uniform(name), count, values);
}

}