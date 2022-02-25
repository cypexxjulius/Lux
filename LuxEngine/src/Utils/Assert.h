#pragma once 
#include <assert.h>

#define Verify(boolean) if(!(boolean)) __debugbreak();

#define TODO() __debugbreak();