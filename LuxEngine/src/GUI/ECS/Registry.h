#pragma once

#include "Core/UUID.h"

#include "ComponentArray.h"

#include "View.h"

namespace Lux::GUI::ECS
{

#define MAX_COMPONENTS 512

class Registry
{

public:

	UUID create()
	{
		return {};
	}

	void destroy(UUID id)
	{
		for(auto& manager : m_ComponentArray)
		{
			manager.second->on_element_destruction(id);
		}
	}


	template<typename ... Components>
	inline const View& view()
	{
		std::initializer_list<size_t> hash_codes = {typeid(Components).hash_code()...};
		
		u32 hash_value = View::Hash(hash_codes);
		if(map_contains(m_ViewContainer, hash_value))
			return m_ViewContainer.at(hash_value);
		
		auto& view = m_ViewContainer.insert({hash_value, hash_codes}).second;
		
		for(auto [hash, manager]: m_ComponentArray)
		{
			if(!view.contains_hash(hash))
				continue;

			view.insert
		}
	}

	template<typename T>
	inline T& add_component(UUID id)
	{			
		Verify(is_contained<T>());
		Verify(is_contained(id));
		
		m_Elements.at(id).set(m_Components.at(get_hash<T>()));

		for(auto& [view_id, view] : m_ViewContainer)
		{
			if(m_Elements.at(id).test(view_id))
				view.insert(id);
		}

		auto m_Manager = get_manager<T>();
		return m_Manager->register_element(id);
	}

	template<typename T, typename ...Tail>
	requires (sizeof...(Tail) == 0)
	void add_components(UUID id) 
	{
		add_component<T>(id);
	}

	template<typename T, typename ...Tail>
	void add_components(UUID id)
	{
		add_component<T>(id);

		add_components<Tail...>(id);
	}

	template<typename T>
	inline T& remove_component(UUID id)
	{
		Verify(is_contained<T>());
		Verify(is_contained(id));

		Verify(element_contains_component<T>(id));
		
		
		auto m_Manager = get_manager<T>();
		m_Manager->remove_component(id);
	}

	template<typename T>
	inline void register_component()
	{
		Verify(!is_contained<T>());

		INFO("Registering component {}", typeid(T).name());
		m_ComponentArray.insert({get_hash<T>(), new ComponentArray<T>() });

		m_Components.insert({ get_hash<T>(), m_ComponentCount });
		m_ComponentCount++;
	}

	template<typename T, typename... VAR_ARGS>
	requires (sizeof...(VAR_ARGS) == 0)
	inline void register_components()
	{ 
		register_component<T>();
	}

	template<typename T, typename... VAR_ARGS>
	inline void register_components()
	{
		register_component<T>();

		register_components<VAR_ARGS...>();
	}

	template<typename T>
	inline T& get_component(UUID id)
	{
		Verify(is_contained<T>());
		Verify(is_contained(id));

		Verify(element_contains_component<T>(id));
		
		auto m_Manager = get_manager<T>();
		return m_Manager->get_component(id);
	}
	
	~Registry()
	{
		for(auto& managers : m_ComponentArray)
			delete managers.second;
	}

private:


	template<typename T>
	constexpr u32 get_hash()
	{
		return static_cast<u32>(typeid(T).hash_code());
	}

	template<typename T>
	inline ComponentArray<T>* get_manager()
	{
		return dynamic_cast<ComponentArray<T>*>(m_ComponentArray.at(get_hash<T>()));
	}

	template<typename T>
	inline bool is_contained()
	{
		return map_contains(m_ComponentArray, get_hash<T>());
	}

	inline bool is_contained(UUID id)
	{
		return map_contains(m_Elements, id);
	}

	template<typename T>
	inline bool element_contains_component(UUID id)
	{
		return m_Elements.at(id).test(m_Components.at(get_hash<T>()));
	}


private:

	u32	m_ComponentCount = 0;
	Container<UUID, Bitset<MAX_COMPONENTS>> m_Elements;
	Container<u32, u32> m_Components;

	Container<u32, View> m_ViewContainer;
	Container<u32, IComponentArray*> m_ComponentArray;
};

}