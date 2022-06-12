#pragma once 


#include "Utils/Types.h"

namespace Lux::GUI
{


struct Box
{

    float width, height;
    v2 position;

};

class BoxHandle
{
private:
    u32 m_ID;

public:

    void hide() const;

    void show() const;

    bool is_hidden() const;

    void lock_position() const;

    void unlock_position() const;

    void is_locked() const;

    


};



}