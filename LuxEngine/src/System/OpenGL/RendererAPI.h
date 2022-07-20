#include "Graphics/Renderer.h"


#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Utils/Logger.h"
#include "Utils/Assert.h"

namespace Lux::OpenGL
{

    inline GLenum lux_to_gl_rendering_option(RenderingOption option)
    {
        switch (option)
        {
        case RenderingOption::BLEND:        return GL_BLEND;
        case RenderingOption::DEPTH_TEST:   return GL_DEPTH_TEST;
        case RenderingOption::MULTISAMPLE:  return GL_MULTISAMPLE;
        default: return GL_FALSE;
        }
    }

    inline GLenum lux_to_gl_blend_func(BlendFunc option)
    {
        switch (option)
        {
        case BlendFunc::ZERO:                   return GL_ZERO;
        case BlendFunc::ONE:                    return GL_ONE;
        case BlendFunc::DST_ALPHA:              return GL_DST_ALPHA;
        case BlendFunc::DST_COLOR:              return GL_DST_COLOR;
        case BlendFunc::SRC_COLOR:              return GL_SRC_COLOR;
        case BlendFunc::SRC_ALPHA:              return GL_SRC_ALPHA;
        case BlendFunc::ONE_MINUS_SRC_COLOR:    return GL_ONE_MINUS_SRC_COLOR;
        case BlendFunc::ONE_MINUS_DST_COLOR:    return GL_ONE_MINUS_DST_COLOR;
        case BlendFunc::ONE_MINUS_SRC_ALPHA:    return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFunc::ONE_MINUS_DST_ALPHA:    return GL_ONE_MINUS_DST_ALPHA;
        default: return GL_FALSE;
        }
    }

    void OpenGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        (void)source;
        (void)length;
        (void)userParam;
        (void)id;

        ERROR("[OPENGL ERROR]:\n"
            "Source     : {}\n"
            "Type       : {}\n"
            "Severity   : {}\n"
            "Text:     \n"
            "{}\n", source, type, severity, message);
        //TODO(); 
    }

    class RendererAPI final : public ::Lux::RendererAPI
    {
    public:

        virtual void init()
        {
        }

        virtual void shutdown()
        {
        }

        virtual void enable_debug() const override
        {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

            glDebugMessageCallback(OpenGLDebugCallback, NULL);

            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

        }

        virtual void disable_debug() const override
        {
            glDisable(GL_DEBUG_OUTPUT);
        }

        virtual void enable(RenderingOption option) const override
        {
            glEnable(lux_to_gl_rendering_option(option));
        }

        virtual void disable(RenderingOption option) const override
        {
            glDisable(lux_to_gl_rendering_option(option));
        }

        virtual void set_blend_function(BlendFunc src, BlendFunc func) const override
        {
            glBlendFunc(lux_to_gl_blend_func(src), lux_to_gl_blend_func(func));
        }

        virtual void clear() const override
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        virtual void set_clear_color(const v4& color) const override
        {
            glClearColor(color.x, color.y, color.z, color.w);
        }


        virtual void draw_indexed(const Ref<VertexArray>& va, u32 indexCount) const override
        {
            u32 count = (indexCount) ? indexCount : va->indexbuffer()->count();

            // Draw Elements
            va->bind();
            glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
        }

        virtual void set_viewport(u32 width, u32 height) const override
        {
            glViewport(0, 0, width, height);
        }


    };


    inline void RendererInit()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_MULTISAMPLE);
    }
}