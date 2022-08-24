#pragma once

#include "Core/UUID.h"

#include "Core.h"

namespace Lux::GUI::ECS
{


class Registry
{

public:

	UUID create()
	{
		UUID id;
		m_Elements.insert({ id, Signature {} });
		return id;
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
		std::initializer_list<u32> hash_codes = {static_cast<u32>(typeid(Components).hash_code())...};
		
		Signature signature;
		for(u32 hash : hash_codes)
			signature.set(m_Components.at(hash));

		if(m_ViewContainer.contains(signature))
			return m_ViewContainer.at(signature);
		
		m_ViewContainer.insert({signature, signature}).second;
		auto& view = m_ViewContainer.at(signature);
		
		for(auto& [id, element_signature]: m_Elements)
		{
			if((signature & element_signature) == element_signature)
				view.insert(id);
		}

		return view;
	}

	template<typename T>
	inline T& add_component(UUID id)
	{			
		Verify(is_contained<T>());
		Verify(is_contained(id));
		
		m_Elements.at(id).set(m_Components.at(get_hash<T>()));

		for(auto& [signature, view] : m_ViewContainer)
		{
			auto& element_signature = m_Elements.at(id);
			if((signature &element_signature)  == element_signature)
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
		
		auto old_signature = m_Elements.at(id);

		m_Elements.at(id).flip(m_Components.at(get_hash<T>()));
		
		for(auto& [signature, view] : m_ViewContainer)
		{
			if(view.test(old_signature))
			{
				if(!view.test(m_Elements.at(id)))
					view.erase(id);
			}
		}

		auto m_Manager = get_manager<T>();
		m_Manager->remove_component(id);
	}

	template<typename T>
	inline void register_component()
	{
		Verify(!is_contained<T>());
		Verify(m_ComponentCount + 1 < MAX_COMPONENTS);

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

	Container<Signature, View> m_ViewContainer;
	Container<u32, IComponentArray*> m_ComponentArray;
};

}