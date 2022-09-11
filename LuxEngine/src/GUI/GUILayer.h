#pragma once
#pragma once
#pragma once 

#include "Core/Layer.h"
#include "Core/Event.h"

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include "Graphics/Camera/Camera2D.h"
#include "Graphics/Camera/Camera3D.h"

#include "Assets/AssetManager.h"

#include "Context.h"


namespace Lux::GUI
{

class GUILayer final : public Layer
{
public:

    GUILayer()
        :   m_Camera(Application::AspectRatio()),
            m_Camera3D(45.0f, Application::AspectRatio()),
            m_MainCamera(&m_Camera)
    {
        m_Width = Application::Width();
        m_Height = Application::Height();
    };

    virtual void on_attach() override;

    virtual void on_detach() override;

    virtual bool on_mouse_button_press(const Event<EventType::MouseButtonPressed>& event) override;

    virtual bool on_key_press(const Event<EventType::KeyPressed>& event) override;

    virtual bool on_mouse_move(const Event<EventType::MouseMoved>& event) override;

    virtual void on_resize(const Event<EventType::WindowResize>& event) override;

    virtual bool on_scroll(const Event<EventType::Scrolled>& event) override;

    virtual void on_update() override;

private:

    static GUILayer* s_Instance;

    float m_Width, m_Height;
    
    Camera2D m_Camera;
    Camera3D m_Camera3D;

    Camera* m_MainCamera = nullptr;

    Ref<Font> m_UsedFont;
    Ref<Texture> m_RectTexture;
    Context m_Context;


};

}