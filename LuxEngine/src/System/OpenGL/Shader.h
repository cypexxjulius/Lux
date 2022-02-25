#pragma once 
#include "Renderer/Shader.h"

#include <string_view>
#include <vector>
#include <unordered_map>
#include "Utils/Types.h"


namespace Lux::OpenGL
{

class Shader final : public ::Lux::Shader
{
private:
    u32 m_ID;
    std::string m_Name;
    std::unordered_map<std::string_view, i32> m_Uniforms;


    int compile(std::vector<RawShader>& shaders);

    int pre_process_shader_file(std::string& File, std::vector<RawShader>& shaders);

    void create_shader(std::vector<RawShader>& shaders);

    i32 get_uniform(std::string_view uniformName);

public:
    Shader(std::string_view shaderName, std::initializer_list<RawShader>& init_list);

    Shader(std::string_view shaderName, std::string_view shaderPath);

    ~Shader();

    virtual void bind() const override;

    virtual void set_mat4(std::string_view name, mat4 matrix) override;

    virtual void set_float(std::string_view name, float float0) override;

    virtual void set_int(std::string_view name, int number) override;

    virtual void set_float4(std::string_view name, v4 vec4) override;

    virtual void set_int_array(std::string_view name, int* values, u32 count) override;



};

}