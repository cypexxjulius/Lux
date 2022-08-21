#pragma once

#include "GUI/ECS/Registry.h"

namespace Lux::GUI
{

class Context;

class Manager
{
public:

	virtual ~Manager() {}


	virtual void on_shutdown() = 0; 

	void shutdown()
	{
		on_shutdown();
	}

	UUID create();
	
	template<typename T>
	inline T& add_component(UUID id)
	{
		return s_Registry->add_component<T>(id);
	}

	static void Init(Context& ctx, ECS::Registry& registry)
	{
		s_Registry = &registry;
		s_Context = &ctx;
	}

	static void Shutdown()
	{
		s_Registry = nullptr;
		s_Context = nullptr;
	}

private:

	static ECS::Registry* s_Registry;
	static Context* s_Context;
};

}