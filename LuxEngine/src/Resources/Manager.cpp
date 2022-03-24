#include "Manager.h"

#include "Utils/Assert.h"
#include "Utils/Logger.h"

namespace Lux
{

struct Containers
{

    std::unordered_map<std::string_view, Ref<Font>>         FontContainer;
    std::unordered_map<std::string_view, Ref<Shader>>       ShaderContainer;
    std::unordered_map<std::string_view, Ref<Texture2D>>    Texture2DContainer;

};

static Containers* Container;

void ResourceManager::Init()
{ Container = new Containers; }

void ResourceManager::Shutdown()
{ delete Container; }

Ref<Shader> ResourceManager::Create(std::string_view name, Shader&& shader)
{


}

Ref<Texture2D> ResourceManager::Create(std::string_view name, Texture2D&& texture)
{
    if(is_contained(Container->Texture2DContainer, name))
    {
        LOG_ERROR("ResourceManager: Texture {} already exists", name);
        return;
    }
    Container->Texture2DContainer[name] = std::make_shared<Texture2D>(texture);
    if(Container->Texture2DContainer[name]->is_valid() == 0)
        return Container->Texture2DContainer[name];
    
    LOG_ERROR("ResourceManager: failed to load {}", name);

}

Ref<Font> ResourceManager::Create(std::string_view name, Font&& font)
{
    if(is_contained(Container->FontContainer, name))
    {
        LOG_ERROR("ResourceManager: Font {} already exists", name);
        return;
    }

    Container->FontContainer[name] = std::make_shared<Font>(font);
    if(Container->FontContainer[name]->is_valid() == 0)
        return Container->FontContainer[name];

    LOG_ERROR("ResourceManager: failed to load {}", name);
}

Ref<Texture2D> ResourceManager::GetTexture2D(std::string_view name)
{

}

Ref<Font> ResourceManager::GetFont(std::string_view name)
{

}

Ref<Shader> ResourceManager::GetShader(std::string_view name)
{

}

void ResourceManager::DeleteTexture2D(std::string_view name)
{

}

void ResourceManager::DeleteShader(std::string_view name)
{

}

void ResourceManager::DeleteFont(std::string_view name)
{

}

}