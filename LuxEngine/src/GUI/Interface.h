#pragma once 

#include "GUI/Types.h"

namespace Lux::GUI
{

BoxHandle CreateBox(const std::string& title, v2 position);


void DestroyBox(BoxHandle& handle);

}