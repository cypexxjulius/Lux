
#include "RendererAPI.h"

#include "System/OpenGL/RendererAPI.h"
#include "System/SystemController.h"

#include "Memory/CoreMemory.h"

#include "Utils/Assert.h"

namespace Lux
{

RendererAPI* RendererAPI::s_Instance = nullptr;

RendererAPI* RendererAPI::create()
{
    Assert(s_Instance == nullptr, "Only one RendererAPI instance can exists!");
    s_Instance =  new OpenGL::RendererAPI();
    return s_Instance;
}

}
