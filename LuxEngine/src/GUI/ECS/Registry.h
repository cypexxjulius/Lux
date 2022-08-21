#pragma once

#include "Core/UUID.h"

#include "ComponentArray.h"

namespace Lux::GUI::ECS
{

class Registry
{

public:
	UUID create()
	{
		return {};
	}

	void destroy(UUID id)
	{
		for(auto manager : m_ComponentArray)
		{
			manager.second->on_element_destruction(id);
		}
	}

	template<typename T>
	inline T& add_component(UUID id)
	{	
		Verify(!is_contained<T>());
		
		auto m_Manager = get_manager<T>();
		return m_Manager.register_element(id);
	}

	template<typename T>
	inline T& remove_component(UUID id)
	{

		Verify(!is_contained<T>());
		
		auto m_Manager = get_manager<T>();
		m_Manager.remove_component(id);
	}

	inline void register_component();

	template<typename T, typename... VAR_ARGS>
	inline void register_component()
	{
		m_ComponentArray.insert(get_hash<T>(), new ComponentArray<T>());
		
		m_ComponentIDs.insert({get_hash<T>(), m_IDCounter});
		m_IDCounter++;

		register_component<VAR_ARGS...>();
	}
	
	~Registry()
	{
		for(auto managers : m_ComponentArray)
			delete managers.second;
	}

private:

	template<typename T>
	constexpr u64 get_hash()
	{
		return static_cast<u64>(typeid(T).hash_code());
	}

	template<typename T>
	inline ComponentArray<T>& get_manager()
	{
		return *static_cast<ComponentArray<T>*>(m_ComponentArray.at(get_hash<T>()));
	}

	template<typename T>
	inline bool is_contained()
	{
		return map_contains(m_ComponentArray, get_hash());
	}


private:

	u32 m_IDCounter = 0;

	Container<u64, u32> m_ComponentIDs;
	Container<u64, IComponentArray*> m_ComponentArray;
};

}