#pragma once 

#include <memory>
#include <fstream>

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/Core/ShaderProgram.h"

namespace Lux
{


class Shader 
{

private:

    Scope<ShaderProgram> m_program = nullptr;

    ShaderError m_errorID = ShaderError::NoError;

    std::unordered_map<std::string, ShaderDataType> m_uniforms;
    std::unordered_map<std::string, ShaderDataType> m_attributes;

    inline bool check_uniform(const std::unordered_map<std::string, ShaderDataType>& map, std::string name)
    { return map.find(name) != map.end(); }


public:
    
    Shader(std::vector<RawShader> shaders)
        : m_program(ShaderProgram::Create(shaders))
    {
    }



    Shader(const std::string& file);

    inline ShaderError verify(std::string_view& error_message) const
    {
        if(m_errorID != ShaderError::NoError)
            return m_errorID;

        error_message = m_program->get_error_message();
        auto error = m_program->get_error_code();

        if(error != ShaderError::NoError)
            return error;

        return ShaderError::NoError;
    }

    inline void load_shader_info() 
    {
        m_program->get_shader_info(m_uniforms, m_attributes);
    }

    void inline bind()
    { m_program->bind(); } 

    void inline upload_mat4(const std::string& uniform, mat4 matrix)
    {
        Verify(check_uniform(m_uniforms, uniform));
        m_program->upload_mat4(m_uniforms[uniform].id, matrix);
    }

    void inline upload_float(const std::string& uniform, float fvalue)
    {
        Verify(check_uniform(m_uniforms, uniform));
        m_program->upload_float(m_uniforms[uniform].id, fvalue);
    }

    void inline upload_int(const std::string& uniform, int ivalue)
    {
        Verify(check_uniform(m_uniforms, uniform));
        m_program->upload_int(m_uniforms[uniform].id, ivalue);
    }

    void inline upload_v4(const std::string& uniform, v4 vec4)
    {
        Verify(check_uniform(m_uniforms, uniform));
        m_program->upload_v4(m_uniforms[uniform].id, vec4);
    }

    void inline upload_int_array(const std::string& uniform, const std::vector<int>& array)
    {
        Verify(check_uniform(m_uniforms, uniform));
        m_program->upload_int_array(m_uniforms[uniform].id, array);
    }



}; 


}