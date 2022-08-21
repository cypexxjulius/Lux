#pragma once

#include "GUI/ECS/Registry.h"

namespace Lux::GUI
{

class Manager
{
public:

	virtual ~Manager() {}


	virtual void on_shutdown() = 0; 

	void shutdown()
	{
		on_shutdown();
	}

	void init(ECS::Registry& registry)
	{
		m_Registry = registry;
	}

	UUID create()
	{
		return m_Registry.create();
	}

	template<typename T>
	inline T& add_component(UUID id)
	{
		return m_Registry.add_component<T>(id);
	}



private:

	ECS::Registry& m_Registry;
};

}