#pragma once

#include "Utils/Types.h"

#include "Core/UUID.h"

#include "GUI/ECS/Registry.h"

namespace Lux::GUI::ECS
{

class View
{
public:
	View(Bitset<MAX_COMPONENTS>& signature)
		:	m_Signature(signature)
	{
	}

	void insert(UUID id)
	{
		m_Elements.insert(id);
	}

	bool test()
	{
		return m_TypeHashes.find(type_hash) != m_TypeHashes.end();
	}

	const Set<UUID>& get_elements() const
	{
		return m_Elements;
	}

	static u32 Hash(const std::initializer_list<size_t>& hash_list)
	{
		u32 seed = static_cast<u32>(hash_list.size());

		for(auto x : hash_list) {
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = ((x >> 16) ^ x) * 0x45d9f3b;
			x = (x >> 16) ^ x;
			seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed;
	}

private:

	Bitset<MAX_COMPONENTS> m_Signature;
	Set<UUID> m_Elements;
};


}