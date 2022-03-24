#pragma once 

#include <unordered_map>

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/Shader.h"

#include "Assets/Font.h"
#include "Assets/Texture2D.h"

namespace Lux
{

class ResourceManager
{
private:

    template<typename T>
    static bool is_contained(std::unordered_map<std::string_view, Ref<T>> map, std::string_view name)
    { return map.find(name) != map.end(); }


public:
    static void Init();

    static void Shutdown();

    template<typename T>
    static Ref<T> Create(std::string_view name, T&& object);

    template<>
    static Ref<Shader> Create<Shader>(std::string_view name, Shader&& shader);

    static Ref<Texture2D> Create(std::string_view name, Texture2D&& texture);

    static Ref<Font> Create(std::string_view name, Font&& font);

    static Ref<Texture2D> GetTexture2D(std::string_view name);
    
    static Ref<Font> GetFont(std::string_view name);

    static Ref<Shader> GetShader(std::string_view name);    

    static void DeleteTexture2D(std::string_view name);

    static void DeleteShader(std::string_view name);

    static void DeleteFont(std::string_view name);

};


}