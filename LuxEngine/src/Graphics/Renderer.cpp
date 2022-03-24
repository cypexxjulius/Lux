#include "Renderer.h"

#include "System/OpenGL/RendererAPI.h"
#include "System/SystemController.h"

#include "Utils/Assert.h"

#include <functional>

namespace Lux
{





RendererFunctions Renderer::s_functions;
RendererAPI Renderer::s_current_api = RendererAPI::NA;

void Renderer::Init(RendererAPI api)
{
    SetRenderingAPI(api);
}

void Renderer::Shutdown()
{
    if(s_current_api != RendererAPI::NA)
        s_functions.Shutdown();
}

void Renderer::SetRenderingAPI(RendererAPI api)
{
    if(s_current_api != RendererAPI::NA)
        s_functions.Shutdown();

    s_current_api = api;
    
    s_functions = OpenGL::RendererAPIFunctions;
    s_functions.Init();


    
}

}
