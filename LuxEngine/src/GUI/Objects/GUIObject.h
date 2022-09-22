#pragma once

#include "GUI/ECS/Components.h"

#include "GUI/ECS/Registry.h"

#include "Assets/Font.h"

namespace Lux::GUI
{

class Context;

class GUIObject
{
public: 

	virtual ~GUIObject();

	friend class Context;

	

	virtual void refresh(v2 position, float width, float height, float depth) = 0;

public:

	GUIObject(TypeComponent type); 
	
	void shutdown()
	{
		delete this;
	}

	template<typename T>
	inline T& attach_component()
	{
		return s_Registry->add_component<T>(m_ID);
	}

	template<typename T>
	inline T& get()
	{
		return s_Registry->get_component<T>(m_ID);
	}

	inline UUID get_id()
	{
		return m_ID;
	}

	inline void update_transform(v2 position, float width, float height, float depth)
	{
		auto& transform = get<TransformComponent>();

		transform.position = { position.x, position.y, depth};
		transform.scale = { width, height, 1.0f};
	}

	inline void set_parent(UUID id)
	{
		get<LayoutComponent>().parent = id;
	}

	template<typename T>
	inline bool contains()
	{
		s_Registry.has_component<T>(m_ID);
	}

	void set_root();

public:

	static float GetWidth()
	{
		return s_Width;
	}

	static float GetHeight()
	{
		return s_Height;
	}

	
	static const Ref<Font>& GetFont();


	template<typename T>
	static T& Get(UUID id)
	{
		return s_Registry->get_component<T>(id);
	}

	static GUIObject& GetObject(UUID id);

	template<typename T>
	requires std::derived_from<T, GUIObject>
	static T& GetObject(UUID id)
	{
		return *static_cast<T*>(&GetObject(id));
	}

	template<typename T, typename...Args>
	requires std::derived_from<T, GUIObject>
	static UUID CreateObject(Args&&... args)
	{
		T* temp = new T(std::forward<Args>(args)...);
		return temp->get_id();
	}
	static void DestroyPlain(UUID id)
	{
		s_Registry->destroy(id);
	}

	template<typename T>
	static T& AttachComponent(UUID id, T&& comp = {})
	{
		return s_Registry->add_component<T>(id, std::forward<T>(comp));
	}

private:

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

	

	static void SetDimension(float width, float height)
	{
		s_Width = width;
		s_Height = height;
	}


private:

	UUID m_ID;

	static float s_Width, s_Height;

	static ECS::Registry* s_Registry;

	static Context* s_Context;
};


}