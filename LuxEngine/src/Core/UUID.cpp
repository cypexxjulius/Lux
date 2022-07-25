#include "UUID.h"

namespace Lux
{

std::uniform_int_distribution<u64>	UUID::s_UniformDistribution;
std::random_device					UUID::s_RandomDevice;
std::mt19937_64						UUID::s_Engine(s_RandomDevice());

}