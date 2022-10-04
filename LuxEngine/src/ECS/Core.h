#pragma once

namespace Lux::GUI
{

#define MAX_COMPONENTS (128)

using Signature = Bitset<MAX_COMPONENTS>;


}

#include "View.h"
#include "ComponentArray.h"
#include "Registry.h"