#pragma once 
#include "Graphics/Shader.h"

#include <vector>
#include <fstream>
#include <optional>

#include "Utils/Types.h"
#include "Utils/Assert.h"



#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Lux::OpenGL
{

std::pair<ShaderError, std::string> CreateShader(Shader& shader, std::fstream& file);

void DestroyShader(Shader& shader);

void ShaderBind(Shader& shader);


void ShaderUploadMat4(u32 uniform_id, mat4 matrix)
{
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderUploadFloat(u32 uniform_id, float float_val)
{
    glUniform1f(uniform_id, float_val);
}

void ShaderUploadInt(u32 uniform_id, int number)
{
    glUniform1i(uniform_id, number);
}

void ShaderUploadV4(u32 uniform_id, v4 vec4)
{
    glUniform4f(uniform_id, vec4.x, vec4.y, vec4.z, vec4.w);
}

void ShaderUploadIntArray(u32 uniform_id, std::vector<int> array) 
{
    glUniform1iv(uniform_id, array.size(), array.data());
}

Lux::ShaderFunctions ShaderFunctions
{
    .Create = CreateShader,
    .Destroy = DestroyShader,

    .Bind = ShaderBind,
    .UploadMat4 = ShaderUploadMat4,
    .UploadFloat = ShaderUploadFloat,
    .UploadInt = ShaderUploadInt,
    .UploadV4 = ShaderUploadV4,
    .UploadIntArray = ShaderUploadIntArray,

};

}