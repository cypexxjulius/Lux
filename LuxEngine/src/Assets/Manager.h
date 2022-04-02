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

    std::unordered_map<std::string, Ref<Font>>         FontContainer;
    std::unordered_map<std::string, Ref<Shader>>       ShaderContainer;
    std::unordered_map<std::string, Ref<Texture>>      TextureContainer;

};

class ResourceManager
{

    static AssetContainers* s_Container;

    template<typename T>
    static inline bool is_contained(const std::unordered_map<std::string, Ref<T>>& map, const std::string& name)
    { return map.find(name) != map.end(); }
    
public:
    static inline void Init()
    { 
        s_Container = new AssetContainers; 
    }

    static inline void Shutdown()
    { delete s_Container; }

    static Ref<Shader> CreateShader(const std::string& name, Shader&& shader)
    {
        Verify(!is_contained(s_Container->ShaderContainer, name));

        s_Container->ShaderContainer.insert({ name, std::make_shared<Shader>(std::forward<Shader>(shader))});

        auto& new_shader = s_Container->ShaderContainer.at(name);

        std::string_view error_message;
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

    static Ref<Texture> CreateTexture(const std::string& name, Texture&& texture)
    {
        Verify(!is_contained(s_Container->TextureContainer, name));

        s_Container->TextureContainer.insert({name, std::make_shared<Texture>(std::forward<Texture>(texture))});
        return s_Container->TextureContainer.at(name);
    }

    static Ref<Font> CreateFont(const std::string& name, Font&& font)
    {
        Verify(!is_contained(s_Container->FontContainer, name));

        s_Container->FontContainer.insert({ name, std::make_shared<Font>(std::forward<Font>(font))});
        return s_Container->FontContainer.at(name);
    }

    static Ref<Texture> GetTexture(const std::string& name)
    {
        Verify(is_contained(s_Container->TextureContainer, name));

        return s_Container->TextureContainer.at(name);
    }
    
    static Ref<Font> GetFont(const std::string& name)
    {
        Verify(is_contained(s_Container->FontContainer, name));

        return s_Container->FontContainer.at(name);
    }

    static Ref<Shader> GetShader(const std::string& name)
    {
        Verify(is_contained(s_Container->ShaderContainer, name));

        return s_Container->ShaderContainer.at(name);
    }

    
    static void DeleteTexture(const std::string& name)
    {
        Verify(is_contained(s_Container->TextureContainer, name));

        s_Container->TextureContainer.erase(name);
    }

    static void DeleteShader(const std::string& name)
    {
        Verify(is_contained(s_Container->ShaderContainer, name));

        s_Container->ShaderContainer.erase(name);
    }

    static void DeleteFont(const std::string& name)
    {
        Verify(is_contained(s_Container->FontContainer, name));

        s_Container->FontContainer.erase(name);
    }

};


}
