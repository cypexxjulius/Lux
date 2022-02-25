#pragma once 

#include "Utils/Types.h"
#include <string_view>

namespace Lux 
{

enum class ShaderType : u8
{
    Vertex = 0,
    Fragment = 1
};

struct RawShader
{
    u32 id;
    ShaderType type;
    std::string data;

    RawShader(u32 a_id, ShaderType a_type, std::string& a_data)
        : id(a_id)
        , type(a_type)
        , data(a_data)
    {
    }

    RawShader(ShaderType a_type, std::string_view a_data)
        : id(0)
        , type(a_type)
        , data(a_data)
    {
    }
    
    RawShader(u32 a_id, ShaderType a_type, std::string_view a_data)
        : id(a_id)
        , type(a_type)
        , data(a_data)
    {
    }
};

class Shader 
{
protected:

    Shader() = default;

public:

    virtual ~Shader() {}

    virtual void bind() const = 0;

    virtual void set_mat4(std::string_view name, mat4 matrix) const = 0;

    virtual void set_float(std::string_view name, float float0) const = 0;

    virtual void set_int(std::string_view name, int number) const = 0;

    virtual void set_float4(std::string_view name, v4 vec4) const = 0;

    virtual void set_int_array(std::string_view name, int* values, u32 count) const = 0;


    static Shader* create(std::string_view shaderName, std::initializer_list<RawShader> init_list);

    static Shader* create(std::string_view shaderName, std::string_view shaderPath);
};

}