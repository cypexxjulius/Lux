#include "Graphics/Renderer.h"


#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "Utils/Logger.h"
#include "Utils/Assert.h"

namespace Lux::OpenGL
{

inline void RendererInit()
{
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
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
        }, NULL
    );
		
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);
}

inline void RendererShutdown()
{

}

inline void RendererClear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

inline void RendererSetClearColor(const v4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}


inline void RendererDrawIndexed(const Ref<VertexArray>& va, u32 indexCount)
{
    u32 count = (indexCount) ? indexCount : va->indexbuffer()->count();
    
    // Draw Elements
    va->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
} 

inline void RendererSetViewport(u32 width, u32 height) 
{
    glViewport(0, 0, width, height);
}


RendererFunctions RendererAPIFunctions 
{
    .Init = RendererInit,
    .Shutdown = RendererShutdown,

    .Clear = RendererClear,
    .SetClearColor = RendererSetClearColor,
    .SetViewport = RendererSetViewport,
    .DrawIndexed = RendererDrawIndexed,
};

}