#pragma once 

#include <cstdlib>

#include "Utils/Types.h"

namespace Lux
{



class Random
{
public:
    static inline u64 U64()  
    {
        int number = rand();
        return abs(number);
    }



};



}