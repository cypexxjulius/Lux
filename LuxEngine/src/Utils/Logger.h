#pragma once 
#include <iostream>

namespace Lux
{

template<typename T>
void LOG(T element)
{
    std::cout << element << std::endl;
}



template<typename T,typename... Types>
void LOG(const T& firstElement, const Types&... params)
{
    std::cout << firstElement;
    LOG(params...);
}

}