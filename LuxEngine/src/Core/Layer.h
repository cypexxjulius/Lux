#pragma once 

#include "Event.h"

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

    virtual bool on_event(const Event& event) = 0;


};


}