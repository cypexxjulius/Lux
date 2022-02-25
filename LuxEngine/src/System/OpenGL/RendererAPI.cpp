

#include "System/OpenGL/RendererAPI.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <stdio.h>

namespace Lux::OpenGL
{

RendererAPI::RendererAPI()
{

}

RendererAPI::~RendererAPI()
{}

void RendererAPI::init()
{
    glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
        {
            if(severity <= GL_DEBUG_SEVERITY_MEDIUM)
                return;


            (void)source;
            (void)length;
            (void)userParam;
            (void)id;
            
            fprintf(stderr, "[OPENGL ERROR]:\n" 
                    "Source     : 0x%x\n"
                    "Type       : 0x%x\n"
                    "Severity   : 0x%x\n"
                    "ERROR:\n%s\n", source, type, severity, message);
        }, NULL);
		
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
}

void RendererAPI::shutdown()
{

}

void RendererAPI::clear(const v4& color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void RendererAPI::draw_indexed(VertexArray* va, u32 indexCount)
{
    u32 count = (indexCount) ? indexCount : va->indexbuffer()->count();
    
    // Draw Elements
    va->bind();
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
} 

void RendererAPI::set_viewport(u32 width, u32 height) 
{
    glViewport(0, 0, width, height);
}

}