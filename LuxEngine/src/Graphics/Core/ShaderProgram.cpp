#include "ShaderProgram.h"

#include "System/OpenGL/ShaderProgram.h"
#include "Graphics/Renderer.h"

namespace Lux
{

std::unique_ptr<ShaderProgram> ShaderProgram::Create(std::vector<RawShader> raw_shaders)
{
    return std::make_unique<OpenGL::ShaderProgram>(raw_shaders);
}

}
