#pragma once 

#include <iostream>
#include <fmt/core.h>

namespace Lux
{

template<class... Arguments>
void ERROR(const std::string& fmt, Arguments&&... args)
{
    std::cout << "[ERROR] " << fmt::vformat(fmt, std::make_format_args(args...)) << "\n";
}

template<class... Arguments>
void INFO(const std::string&fmt, Arguments&&... args)
{
    std::cout << "[INFO] " << fmt::vformat(fmt, std::make_format_args(args...)) << "\n";
}

}