#pragma once 

#include <fstream>
#include <string>
#include <functional>
#include <string_view>

#include "Utils/Types.h"

namespace Lux 
{

struct ShaderDataType
{
    u32 id;
    u32 size;
    DataType type;

    std::string name;
};


enum class ShaderType
{
    Vertex = 0,
    Fragment = 1,
    Compute,
};


enum class ShaderError
{
    NoError,
    InvalidShaderType,
    CompilationFailed,
    LinkingFailed,
};

struct RawShader
{
    ShaderType type;
    std::string source;

    u32 id;
};

class ShaderProgram
{
protected:

    ShaderProgram() = default;

public:

    virtual ~ShaderProgram() {}

    virtual void bind() = 0;

    virtual ShaderError get_error_code() = 0;

    virtual std::string& get_error_message() = 0;

    virtual void get_shader_info(std::unordered_map<std::string, ShaderDataType>& uniforms, std::unordered_map<std::string, ShaderDataType>& attributes) = 0;

    virtual void upload_mat4(u32 uniform_id, mat4 matrix) = 0;

    virtual void upload_float(u32 uniform_id, float fvalue) = 0;

    virtual void upload_int(u32 uniform_id, int ivalue) = 0;

    virtual void upload_v4(u32 uniform_id, v4 vec4) = 0;

    virtual void upload_int_array(u32 uniform_id, const std::vector<int>& array) = 0;

    static Scope<ShaderProgram> Create(std::vector<RawShader> raw_shaders);

};


}