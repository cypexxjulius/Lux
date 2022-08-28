#pragma once

#include "GUI/ECS/Core.h"


#include "GUI/ECS/Components.h"

namespace Lux::GUI
{

class Context;

class Manager
{

public:

	virtual ~Manager() {}

	
	void shutdown()
	{
		on_shutdown();
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

public:

	virtual void on_shutdown() = 0; 

	UUID create(TypeComponent type, const std::string& name);

	inline UUID create_plain()
	{
		s_Registry->create();
	}
	
	template<typename T>
	inline T& add_component(UUID id)
	{
		return s_Registry->add_component<T>(id);
	}

	template<typename T, typename... Tail>
	inline void add_components(UUID id)
	{
		return s_Registry->add_components<T, Tail...>(id);
	}


	template<typename T>
	inline T& get_component(UUID id)
	{
		return s_Registry->get_component<T>(id);
	}

	void refresh_section(UUID id);
	

private:

	static ECS::Registry* s_Registry;
	static Context* s_Context;
};

}