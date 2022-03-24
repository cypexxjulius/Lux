#pragma once 

#include <string_view>
#include <functional>
#include <fstream>

#include "Utils/Types.h"
#include "Resources/Manager.h"

namespace Lux 
{


enum class ShaderType : u8
{
    Vertex = 0,
    Fragment = 1,
    Compute,
};


enum class ShaderError
{
    NoError,
    InvalidShaderType,
    InvalidAmountOfShaders,
    CompilationFailed,
    LinkingFailed,
};


struct ShaderFunctions
{
    std::function<std::pair<ShaderError, std::string>(Shader&, std::fstream&)> Create;

    std::function<void(Shader&)> Destroy;

    std::function<void(Shader&)> Bind;

    std::function<void(u32, mat4)> UploadMat4;
    std::function<void(u32, float)> UploadFloat;
    std::function<void(u32, int)> UploadInt;
    std::function<void(u32, v4)> UploadV4;
    std::function<void(u32, std::vector<int>)> UploadIntArray;

};



class Shader 
{
private:
    friend ResourceManager;

    static ShaderFunctions functions;

    u32 id;
    std::unordered_map<std::string_view, i32> uniforms;

    Shader() {};

};


}