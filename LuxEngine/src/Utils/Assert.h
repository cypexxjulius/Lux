#pragma once 

#ifdef LX_WIN
#include <assert.h>
#define Verify(boolean) if(!(boolean)) __debugbreak();

#define TODO() __debugbreak();

#elif LX_UNIX
#include <signal>
#define Verify(boolean) if(!(boolean)) raise(SIGTRAP);

#define TODO() raise(SIGTRAP);
#endif 