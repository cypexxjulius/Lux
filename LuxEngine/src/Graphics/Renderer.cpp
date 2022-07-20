#include "Renderer.h"

#include "System/OpenGL/RendererAPI.h"
#include "System/SystemController.h"

namespace Lux
{

Scope<RendererAPI> Renderer::m_API = nullptr;

void Renderer::Init()
{
    Verify(m_API == nullptr);
    m_API = std::make_unique<OpenGL::RendererAPI>();
    m_API->init();
}

void Renderer::Shutdown()
{
    Verify(m_API != nullptr);
    m_API->shutdown();
}

}
