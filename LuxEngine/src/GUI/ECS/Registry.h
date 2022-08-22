#pragma once

#include "Core/UUID.h"

#include "ComponentArray.h"

namespace Lux::GUI::ECS
{

class Registry
{

public:

	Registry() = default;

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

	template<typename T>
	inline T& add_component(UUID id)
	{	
		Verify(!is_contained<T>());
		
		auto m_Manager = get_manager<T>();
		return m_Manager->register_element(id);
	}

	template<typename T, typename ...Tail>
	requires (sizeof...(Tail) == 0)
	void add_components(UUID id) {}


	template<typename T, typename ...Tail>
	void add_components(UUID id)
	{
		Verify(is_contained<T>());
		
		auto m_Manager = get_manager<T>();
		m_Manager->register_element(id);

		add_components<Tail...>(id);
	}

	template<typename T>
	inline T& remove_component(UUID id)
	{
		Verify(is_contained<T>());
		
		
		auto m_Manager = get_manager<T>();
		m_Manager->remove_component(id);
	}

	template<typename T, typename... VAR_ARGS>
	requires (sizeof...(VAR_ARGS) == 0)
	inline void register_component(){ }

	template<typename T, typename... VAR_ARGS>
	requires (sizeof...(VAR_ARGS) != 0)
	inline void register_component()
	{
		m_ComponentArray.insert({get_hash<T>(), new ComponentArray<T>() });
		

		register_component<VAR_ARGS...>();
	}

	template<typename T>
	inline T& get_component(UUID id)
	{
		Verify(is_contained<T>());
		
		
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
		auto hash_value = get_hash<T>();
		return m_ComponentArray.find(hash_value) != m_ComponentArray.end();
	}


private:

	Container<u32, IComponentArray*> m_ComponentArray { };
};

}