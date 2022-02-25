
#include "VertexArray.h"

#include "System/SystemController.h"
#include "System/OpenGL/VertexArray.h"

#include "Utils/Types.h"

namespace Lux 
{

VertexArray* VertexArray::create()
{
    return new OpenGL::VertexArray();
}   

}