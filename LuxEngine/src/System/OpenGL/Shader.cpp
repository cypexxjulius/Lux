#include "System/OpenGL/Shader.h"

#include <string_view>
#include <vector>
#include <fstream>
#include <sstream>
#include <variant>
#include <optional>

#include "Utils/Assert.h"
#include "Utils/Logger.h"

#include "Core/IO.h"

namespace Lux::OpenGL
{
    
struct RawShader
{
    ShaderType type;
    std::string data;
    std::string file_path;
    u32 id = 0;

    RawShader() {}

    RawShader(ShaderType shader_type)
    {
        type = shader_type;
    }
};

static std::variant<std::array<RawShader, 2>, ShaderError> pre_process_shader_file(std::fstream& shader_file, bool& has_failed)
{
    has_failed = true;
    std::array<RawShader, 2> shaders = { ShaderType::Vertex, ShaderType::Fragment };
    i8 active_shader_slot = -1;

    for(std::string line; std::getline(shader_file, line, '\n');)
    {
        if(line.contains("#type"))
        {
            if(line.contains("vertex"))
                active_shader_slot = static_cast<int>(ShaderType::Vertex);
            else if (line.contains("fragment"))
                active_shader_slot = static_cast<int>(ShaderType::Fragment);
            else 
                return ShaderError::InvalidShaderType;
        } 
        else 
        {
            if(active_shader_slot == -1)
                continue;

            line.append("\n");
            shaders[active_shader_slot].data.append(line);
        }
    }

    if(shaders[0].data.empty() || shaders[1].data.empty())
        return ShaderError::InvalidAmountOfShaders;
    
    has_failed = false;
    return shaders;
}

constexpr GLenum TO_GL_TYPE[] = {
        GL_VERTEX_SHADER,
        GL_FRAGMENT_SHADER,
        GL_COMPUTE_SHADER,
};

static std::pair<ShaderError, std::string> compile_shader(std::array<RawShader, 2>& shaders)
{
    for(RawShader& shader : shaders)
    {
        GLenum glType = TO_GL_TYPE[static_cast<int>(shader.type)];
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
            std::string error_message = errorMessage;
        
            delete[] errorMessage;

            shader.id = 0;
            return  { ShaderError::CompilationFailed, error_message };
        }

    }

    return { ShaderError::NoError, {} };
}

static inline std::pair<ShaderError, std::string> link_shader(Shader& shader, std::array<RawShader, 2> raw_shaders)
{
    for(RawShader& raw_shader : raw_shaders)
        glAttachShader(shader.id, raw_shader.id);  

    glLinkProgram(shader.id);

    for(RawShader& raw_shader : raw_shaders)
    {
        glDetachShader(shader.id, raw_shader.id);
        glDeleteShader(raw_shader.id);
    }

    // Error checking
    int isLinked = 0;
    glGetProgramiv(shader.id, GL_LINK_STATUS, &isLinked);

    if(isLinked)
        return { ShaderError::NoError, {} };

    
    int errorMessageLength = 0;
    glGetProgramiv(shader.id, GL_INFO_LOG_LENGTH, &errorMessageLength);

    char *errorMessage = new char[errorMessageLength];

    glGetProgramInfoLog(shader.id, errorMessageLength, &errorMessageLength, errorMessage);

    for(RawShader& raw_shader : raw_shaders)
        glDeleteShader(raw_shader.id);

    std::string error_message = errorMessage;

    
    delete[] errorMessage;

    
    return { ShaderError::LinkingFailed, error_message };

}

std::pair<ShaderError, std::string> CreateShader(Shader& shader, std::fstream& file)
{
    shader.id = glCreateProgram();
    
    
    bool pre_process_failed;
    std::array<RawShader, 2> shaders;
    auto pre_process_result = pre_process_shader_file(file, pre_process_failed);
    

    // TODO set file.path

    if(pre_process_failed)
        return { std::get<ShaderError>(pre_process_result), {} };
    
    auto& raw_shaders = std::get<std::array<RawShader, 2>>(pre_process_result);

    
    // Compilation
    {
        auto [ compile_result, error_string ] = compile_shader(raw_shaders);
    
        if(compile_result == ShaderError::CompilationFailed)
            return { compile_result, error_string };
    }

    // Linking 
    {
        auto [link_result, error_string] = link_shader(shader, raw_shaders);

        if(link_result == ShaderError::LinkingFailed)
            return { link_result, error_string };

    }

    glValidateProgram(shader.id);
}

void DestroyShader(Shader& shader)
{
    glDeleteProgram(shader.id);
}

void ShaderBind(Shader& shader)
{
    glUseProgram(shader.id);
}

}