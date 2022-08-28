#pragma once


#include <type_traits>

#include "GUI/ECS/Core.h"

#include "Interface/Interface.h"
#include "Managers/Manager.h"

#include <functional>


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

		m_Managers.insert({hash,  std::dynamic_pointer_cast<Manager>(create_ref<MANAGER>())});
	}

	template<typename MANAGER>
	requires std::derived_from<MANAGER, Manager> 
	inline Ref<MANAGER> get_manager()
	{
		type_hash hash = static_cast<type_hash>(typeid(MANAGER).hash_code());

		Verify(map_contains(m_Managers, hash));
		return std::dynamic_pointer_cast<MANAGER>(m_Managers.at(hash));
	}

	UUID create_gui_element(TypeComponent type, const std::string& name);

	void set_root(UUID id);

	void force_refresh();

	void render_rects(std::function<void(const TransformComponent&, const RectComponent&)>);

	void render_glyphs(std::function<void(const TransformComponent&, const GlyphComponent&)>);

	void update_dimensions(float width, float height);

	void refresh_section(UUID id);

	UUID get_root()
	{
		return m_RootElement;
	}

	
private:

	void update(UUID id = 0);

	void update_root();

private:

	UUID m_RootElement = 0;

	float m_Width, m_Height;

	ECS::Registry m_Registry;
	
	Container<type_hash, Ref<Manager>> m_Managers;
};


}