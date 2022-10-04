#pragma once

#include "Core/UUID.h"

#include "Assets/Font.h"

namespace Lux::GUI
{

enum GUIType
{
	CHILD,
	GROUP,
	PARENT,
	RECT,
	GLYPH
};

class Context;

class GUIObject
{
public: 

	virtual ~GUIObject();

	friend class Context;

public:

	GUIObject(GUIType type); 
	
	inline GUIType get_type()
	{
		return m_Type;
	}

	inline UUID get_id()
	{
		return m_ID;
	}
	
	void register_glyph(Transform* transform, Glyph* rect);

	void register_rect(Transform* transform, Rect* rect);

	void unregister_glyph();

	void unregister_rect();

public:

	static float GetWidth()
	{
		return s_Width;
	}

	static float GetHeight()
	{
		return s_Height;
	}

	static void SetRoot(UUID id);

	template<typename T>
	requires std::derived_from<T, GUIObject>
	inline T& GetObject(UUID id)
	{
		return *std::dynamic_pointer_cast<T>(&GetObject(id));
	}

public:

	inline GUIObject& GetObject(UUID id);

	static const Ref<Font>& GetFont();


private:

	static void Init(Context& ctx)
	{
		s_Context = &ctx;
	}


	static void Shutdown()
	{
		s_Context = nullptr;
	}

	

	static void SetDimension(float width, float height)
	{
		s_Width = width;
		s_Height = height;
	}


private:

	UUID m_ID;
	GUIType m_Type;

private:

	static float s_Width, s_Height;

	static Context* s_Context;
};


}