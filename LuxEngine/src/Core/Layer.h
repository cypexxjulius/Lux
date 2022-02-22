#pragma once 


namespace Lux 
{

class Layer
{
protected:
    Layer() = default;
public:
    
    virtual ~Layer() {}

    virtual void on_attach() = 0;

    virtual void on_detach() = 0;

    virtual void on_update() = 0;

    virtual void on_event() = 0;


};


}