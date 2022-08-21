#pragma once

#include "Core/UUID.h"

#include "Utils/Types.h"

#include "Utils/Assert.h"

namespace Lux::GUI::ECS
{

class IComponentArray
{
public:

	virtual ~IComponentArray() {}

	virtual void on_element_destruction(UUID element) = 0;
};

template<typename T, int START_CONTAINER_SIZE = 50>
class ComponentArray final : public IComponentArray
{
private:

	using hash_value = u64;

public:

	ComponentArray()
		:	m_Container(START_CONTAINER_SIZE)
	{
	}

	T& register_element(UUID id)
	{
		Verify(!map_contains(m_Lookup, id));
		
		m_Container.emplace_back();
		u32 index = static_cast<u32>(m_Container.size() - 1);

		m_Lookup.insert({id, index});
		m_RevLookup.insert({index, id});

		return m_Container.back();
	}

	void remove_component(UUID id)
	{
		if(!map_contains(m_Lookup, id))
			return;

		u32 index = m_Lookup.at(id);
		u32 last_index = static_cast<u32>(m_Container.size() - 1);

		// Switch the contents of the last component with the "deleted" component
		std::swap(m_Container[index], m_Container[last_index]);
		m_Container.pop_back();

		// Remove the deleted element from the lookup and update the last element's position in the container
		m_Lookup.erase(id);
		m_Lookup.at(m_RevLookup.at(last_index)) = index; 

		// Update the reverse lookup map
		m_RevLookup.at(index) = id;
		m_RevLookup.erase(last_index);
	}

	virtual void on_element_destruction(UUID element) override 
	{
		remove_component(element);
	}

	T& get_component(UUID id)
	{
		return m_Container[m_Lookup.at(id)];
	}

	T* try_get_element(UUID id)
	{
		if(map_contains(m_Lookup, id))
			return &m_Container[m_Lookup.at(id)];
		
		return nullptr;
	}

	bool is_contained(UUID id)
	{
		return map_contains(m_Lookup, id);
	}

private:


private:
	List<T> m_Container;
	Container<UUID, u32> m_Lookup;
	Container<u32, UUID> m_RevLookup;
};


}