#pragma once


#include <type_traits>

#include "GUI/ECS/Registry.h"

#include "Interface/Interface.h"
#include "Managers/Manager.h"

namespace Lux::GUI
{

template <typename T>
concept IsManager = std::derived_from<T, Manager>;

class Context;

template<typename T, typename N>
concept IsConnectable = requires(T, Context* ctx)
{
	T::Init(ctx);
} && IsManager<N>;

class Context
{
private:

	using type_hash = u64;

	


public:

	Context();
	~Context();

	template<typename MANAGER>
	requires std::derived_from<MANAGER,Manager> 
	inline void register_manager()
	{	
		type_hash hash = static_cast<type_hash>(typeid(MANAGER).hash_code());
		Verify(!map_contains(m_Managers, hash));

		m_Managers.insert({hash, new MANAGER()});
	}

	template<typename MANAGER>
	requires std::derived_from<MANAGER, Manager> 
	inline MANAGER& get_manager()
	{
		type_hash hash = static_cast<type_hash>(typeid(MANAGER).hash_code());
		return *dynamic_cast<MANAGER*>(m_Managers.at(hash));
	}

	UUID create_gui_element();

	void set_root(UUID id);

private:

	UUID m_RootElement = 0;

	ECS::Registry m_Registry;
	
	Container<type_hash, Manager*> m_Managers;
};


}