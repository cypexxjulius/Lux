#pragma once

#include "GUI/ECS/Registry.h"

#include "Managers/Manager.h"

#include "Managers/SectionManager.h"

#include "Interface/Interface.h"

#include <type_traits>

namespace Lux::GUI
{

template <typename T>
concept IsManager = std::is_base_of_v<Manager, T>;

template<typename T>
concept IsInterface = requires(T, void* m)
{
	T::SetManager(m);
};

class Context
{
private:

	using type_hash = u64;

	


public:

	Context();
	~Context();

	template<typename INTERFACE, typename MANAGER>
	requires IsInterface<INTERFACE> && IsManager<MANAGER> 
	void connect()
	{
		type_hash hash = static_cast<type_hash>(typeid(INTERFACE).hash_code());

		Manager* manager = new MANAGER();
		m_Managers.insert({hash, manager});
		
		manager->init(m_Registry);
		INTERFACE::SetManager(manager);
	}

private:

	ECS::Registry m_Registry;
	
	Container<type_hash, Manager*> m_Managers;
};


}