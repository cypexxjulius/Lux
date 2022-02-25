#include "Shader.h"

#include "System/OpenGL/Shader.h"

namespace Lux
{

Shader* Shader::create(std::string_view shaderName, std::string_view shaderPath)
{
    return new OpenGL::Shader(shaderName, shaderPath);
}
    
Shader* Shader::create(std::string_view shaderName, std::initializer_list<RawShader> init_list)
{
    return new OpenGL::Shader(shaderName, init_list);
}

}
