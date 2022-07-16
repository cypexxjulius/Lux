#include "Renderer.h"

#include "System/OpenGL/RendererAPI.h"
#include "System/SystemController.h"

#include "Renderer2D/Renderer2D.h"

#include "Utils/Assert.h"

#include <functional>

namespace Lux
{





RendererFunctions Renderer::s_functions;
bool Renderer::s_initialized = false;
RendererAPI Renderer::m_api = RendererAPI::NA;

void Renderer::Init(RendererAPI api)
{
    m_api = api;

    switch (api)
    {
    case RendererAPI::OpenGL:
        s_functions = OpenGL::RendererAPIFunctions;
        s_functions.Init(); 

        s_initialized = true;
        break;
    
    default:
        TODO();
        break;
    }    
}

void Renderer::Shutdown()
{
    Verify(s_initialized);
    s_functions.Shutdown();
}

}
