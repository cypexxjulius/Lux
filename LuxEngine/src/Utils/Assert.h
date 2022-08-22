#pragma once 
#include <assert.h>

#ifdef LX_WIN
#define Verify(boolean) if(!(boolean)) __debugbreak();

#define TODO() __debugbreak();

#elif LX_UNIX
#include <signal>
#define Verify(boolean) if(!(boolean)) raise(SIGTRAP);

#define TODO() raise(SIGTRAP);
#endif 