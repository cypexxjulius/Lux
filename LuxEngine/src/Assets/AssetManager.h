#pragma once 

#include <string_view>
#include <unordered_map>

#include "Utils/Types.h"
#include "Utils/Assert.h"
#include "Utils/Logger.h"

#include "Assets/Font.h"
#include "Assets/Shader.h"
#include "Assets/Texture.h"


namespace Lux
{

struct AssetContainers
{

    Container<String, Ref<Font>>         FontContainer;
    Container<String, Ref<Shader>>       ShaderContainer;
    Container<String, Ref<Texture>>      TextureContainer;

};

class ResourceManager
{

    static Scope<AssetContainers> s_Container;

    template<typename T>
    static inline bool is_contained(const Container<String, Ref<T>>& map, const String& name)
    { return map.find(name) != map.end(); }
    
public:
    static inline void Init()
    { 
        s_Container.reset(new AssetContainers());

        Font::Init();
    }

    static inline void Shutdown()
    { 
        Font::Shutdown();
    
        delete s_Container.release();
        s_Container = nullptr;
    }

    static Ref<Shader> CreateShader(const String& name, Shader&& shader)
    {
        Verify(!is_contained(s_Container->ShaderContainer, name));

        s_Container->ShaderContainer.insert({ name, create_ref<Shader>(std::forward<Shader>(shader))});

        auto& new_shader = s_Container->ShaderContainer.at(name);

        StringView error_message;
        auto error_id = new_shader->verify(error_message);

        if(error_id == ShaderError::NoError)
        {
            new_shader->load_shader_info();
            return s_Container->ShaderContainer.at(name);
        }

        ERROR("ErrorCode {} in shader \"{}\"\n{}\n", static_cast<int>(error_id), name, error_message);
        TODO();
        return nullptr;
    }

    static Ref<Texture> CreateTexture(const String& name, Texture&& texture)
    {
        Verify(!is_contained(s_Container->TextureContainer, name));

        s_Container->TextureContainer.insert({name, create_ref<Texture>(std::forward<Texture>(texture))});
        return s_Container->TextureContainer.at(name);
    }

    static Ref<Font> CreateFont(const String& name, Font&& font)
    {
        Verify(!is_contained(s_Container->FontContainer, name));

        s_Container->FontContainer.insert({ name, create_ref<Font>(std::forward<Font>(font))});
        return s_Container->FontContainer.at(name);
    }

    static Ref<Texture> GetTexture(const String& name)
    {
        Verify(is_contained(s_Container->TextureContainer, name));

        return s_Container->TextureContainer.at(name);
    }
    
    static Ref<Font> GetFont(const String& name)
    {
        Verify(is_contained(s_Container->FontContainer, name));

        return s_Container->FontContainer.at(name);
    }

    static Ref<Shader> GetShader(const String& name)
    {
        Verify(is_contained(s_Container->ShaderContainer, name));

        return s_Container->ShaderContainer.at(name);
    }

    
    static void DeleteTexture(const String& name)
    {
        Verify(is_contained(s_Container->TextureContainer, name));

        s_Container->TextureContainer.erase(name);
    }

    static void DeleteShader(const String& name)
    {
        Verify(is_contained(s_Container->ShaderContainer, name));

        s_Container->ShaderContainer.erase(name);
    }

    static void DeleteFont(const String& name)
    {
        Verify(is_contained(s_Container->FontContainer, name));

        s_Container->FontContainer.erase(name);
    }

};


}
