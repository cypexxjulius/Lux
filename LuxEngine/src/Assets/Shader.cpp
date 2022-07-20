

#include <vector>
#include <sstream>
#include <string>

#include "Assets/Shader.h"

namespace Lux
{

static std::string empty = "";

Shader::Shader(const std::string& file_path)
{

    std::fstream file(file_path);

    Verify(file.is_open());

    if (!file.is_open())
        ERROR("Failed to open shader asset file {}", file_path);

    std::vector<RawShader> shaders;

    for(std::string line; std::getline(file, line, '\n');)
    {
        if(line.contains("#type"))
        {
            if(line.contains("vertex"))
                shaders.emplace_back(ShaderType::Vertex);

            else if (line.contains("fragment"))
                shaders.emplace_back(ShaderType::Fragment);

            else 
            {
                m_errorID = ShaderError::InvalidShaderType;
                return;
            }
        } 
        else 
        {
            if(shaders.empty())
                continue;

            line.append("\n");
            shaders.back().source.append(line);
        }
    }
    
    m_program = ShaderProgram::Create(shaders);
}

}