#pragma once 


#include "Utils/Types.h"
#include "Utils/Assert.h"

#include <vector>
#include <glad/glad.h>

#pragma warning(disable: 4201)
#include <glm/gtc/type_ptr.hpp>
#pragma warning(default: 4201)

#include "Graphics/Core/ShaderProgram.h"


namespace Lux::OpenGL
{

class ShaderProgram final : public ::Lux::ShaderProgram
{
private:

    u32 m_id;
    ShaderError m_errorID = ShaderError::NoError;
    std::string m_errorMessage;

    void set_error(ShaderError error_id, std::string&& ErrorMessage)
    {
        m_errorID = error_id;
        m_errorMessage = ErrorMessage;
    }
    static void bind_shader(u32 id)
    {
        static i32 bound_id = -1;

        if(static_cast<i32>(id) == bound_id)
            return;

        glUseProgram(id);
        bound_id = id;
    }

public: 

    ShaderProgram(std::vector<RawShader> raw_shaders);

    ~ShaderProgram() 
    {
        glDeleteProgram(m_id);
    }

    virtual ShaderError get_error_code() override
    {
        return m_errorID;
    }

    virtual void get_shader_info(std::unordered_map<std::string, ShaderDataType>& uniforms, std::unordered_map<std::string, ShaderDataType>& attributes) override;

    virtual std::string& get_error_message()
    {
        return m_errorMessage;
    }


    virtual void bind() override 
    {
        bind_shader(m_id);
    }

    virtual void upload_mat4(u32 uniform_id, mat4 matrix) override
    {
        glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    virtual void upload_float(u32 uniform_id, float float_val) override
    {
        glUniform1f(uniform_id, float_val);
    }

    virtual void upload_int(u32 uniform_id, int number) override
    {
        glUniform1i(uniform_id, number);
    }

    virtual void upload_v4(u32 uniform_id, v4 vec4) override
    {
        glUniform4f(uniform_id, vec4.x, vec4.y, vec4.z, vec4.w);
    }

    virtual void upload_int_array(u32 uniform_id, const std::vector<int>& array) override
    {
        glUniform1iv(uniform_id, static_cast<int>(array.size()), array.data());
    }

};






}