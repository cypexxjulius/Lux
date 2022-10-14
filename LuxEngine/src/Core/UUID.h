#pragma once


#include <random>

#include "Utils/Types.h"

namespace Lux
{

class UUID
{
public:

	UUID()
		:	m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID(u64 id)
		: m_UUID(id)
	{}

	UUID(UUID& id)
		: m_UUID(id)
	{
	}

	UUID(UUID&& id)
		: m_UUID(id)
	{
		id.m_UUID = 0;
	}

	~UUID()
	{

	}

	operator u64() const { return m_UUID; }

	void operator=(UUID other)
	{
		m_UUID = other.m_UUID;
	}

private:

	static std::uniform_int_distribution<u64> s_UniformDistribution;

	static std::random_device s_RandomDevice;

	static std::mt19937_64 s_Engine;

private:

	u64 m_UUID;
};


}

namespace std {
template <typename T> struct hash;

template<>
struct hash<Lux::UUID>
{
	std::size_t operator()(const Lux::UUID& uuid) const
	{
		return (uint64_t)uuid;
	}
};

}