#pragma once 

#include "Utils/Types.h"

#include <string>

namespace Lux
{

class LGA 
{
    
public:

    static void Begin(const std::string& title, v2 position = { 0.0f, 0.0f }, float width = 1.0f, float height = 1.0f);

    static void End();
};

}