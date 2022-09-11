#include "GUILayer.h"

#include "Graphics/Renderer2D/Renderer2D.h"

#include "Space.h"

namespace Lux::GUI
{
GUILayer* GUILayer::s_Instance = nullptr;

static Ref<Bitmap> standard_color = nullptr;

void GUILayer::on_attach()
{
	Verify(!s_Instance);
	s_Instance = this;

    m_Context.update_dimensions(m_Width, m_Height);

    BitmapSpec spec;

    spec.type = ImageType::ALPHA;


    standard_color = Bitmap::Create(spec, 4, 2);

    m_RectTexture = ResourceManager::CreateTexture("Logo", { "res/textures/logo.png"});

    u32* pure_color = new u32[2];

    *pure_color = 1;

    standard_color->set_data(pure_color, sizeof(u32) * 2);

    delete[] pure_color;
}

void GUILayer::on_detach()
{
	s_Instance = nullptr;
}
bool GUILayer::on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event)
{
	return false;
}
bool GUILayer::on_key_press(const Event<EventType::KeyPressed>& event)
{
	if (event.action != KeyState::Pressed)
        return false;


    if (event.key == Key::W)
    {
        INFO("[GUILayer] switched to Camera3D");
        m_MainCamera = &m_Camera3D;
    }
    else if (event.key == Key::A)
    {
        m_MainCamera = &m_Camera;
        INFO("[GUILayer] switched to Camera2D");
    }
    else if(event.key == Key::R)
    {
        m_Context.refresh();
    }
    else
        return false;

    return true;
}
bool GUILayer::on_mouse_move(const Event<EventType::MouseMoved>& event)
{
    return m_Camera3D.on_mouse_move(event);
}

void GUILayer::on_resize(const Event<EventType::WindowResize>& event)
{
    m_Width = event.width;
    m_Height = event.height;
    m_Camera3D.on_resize(event);
    m_Camera.on_resize(event);
    m_Context.update_dimensions(m_Width, m_Height);
}
bool GUILayer::on_scroll(const Event<EventType::Scrolled>& event)
{
    return m_Camera3D.on_scroll(event);
}
void GUILayer::on_update()
{
	Renderer2D::BeginScene(m_MainCamera->projection());

    float AspectRatio = m_Width / m_Height;

    v3 test_scale = {1.0f, 1.0f, 1.0f};
	v3 test_position = { 0.0f, 0.0f, 0.9f};
		

	mat4 test_rect_transform =   glm::translate(mat4{ 1.0f }, test_position) *
                            glm::scale(mat4{ 1.0f }, test_scale);


    Renderer2D::DrawTexturedRect(test_rect_transform, {1.0f, 1.0f, 1.0f, 1.0f}, m_RectTexture, 1.0f);

    
	m_Context.render_rects([&](const TransformComponent& transform, const RectComponent& rect){

        v3 scale = GUISpace::ToRenderSpaceDelta(AspectRatio, m_Width, m_Height, transform.scale);
	    v3 position = GUISpace::ToRenderSpace(AspectRatio, m_Width, m_Height, transform.position);
		

		mat4 rect_transform =   glm::translate(mat4{ 1.0f }, position) *
                                glm::toMat4(glm::quat(transform.rotation)) *
                                glm::scale(mat4{ 1.0f }, scale); 

		Renderer2D::DrawTexturedRect(rect_transform, rect.color, rect.texture, rect.tiling);
	});

    m_Context.render_glyphs([&](const TransformComponent& transform, const GlyphComponent& glyph){
       
        v3 scale = GUISpace::ToRenderSpaceDelta(AspectRatio, m_Width, m_Height, transform.scale);
	    v3 position = GUISpace::ToRenderSpace(AspectRatio, m_Width, m_Height, transform.position);
		
        mat4 glyph_transform =  glm::translate(mat4{ 1.0f }, position) *
                                glm::toMat4(glm::quat(transform.rotation)) *
                                glm::scale(mat4{ 1.0f }, scale); 

        glyph_transform *= glyph.char_transform;

        Renderer2D::DrawGlyph(glyph_transform, glyph.color, glyph.tex_coords, glyph.font);
    });
    

	Renderer2D::EndScene();
}

}