#pragma once 

#include <format>
#include <iostream>

namespace Lux
{

template<class... Arguments>
void ERROR(const std::string& fmt, Arguments&&... args)
{
    std::cout << "[ERROR] " << std::vformat(fmt, std::make_format_args(args...)) << "\n";
}

template<class... Arguments>
void INFO(const std::string&fmt, Arguments&&... args)
{
    std::cout << "[INFO] " << std::vformat(fmt, std::make_format_args(args...)) << "\n";
}

}