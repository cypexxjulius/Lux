
#include "RendererAPI.h"

#include "System/OpenGL/RendererAPI.h"
#include "System/SystemController.h"

#include "Utils/Assert.h"

namespace Lux
{

RendererAPI* RendererAPI::s_Instance = nullptr;

RendererAPI* RendererAPI::create()
{
    assert(s_Instance == nullptr);
    s_Instance =  new OpenGL::RendererAPI();
    return s_Instance;
}

}
