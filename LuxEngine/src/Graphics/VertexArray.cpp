
#include "VertexArray.h"

#include "System/SystemController.h"
#include "System/OpenGL/VertexArray.h"

#include "Utils/Types.h"

namespace Lux 
{

Ref<VertexArray> VertexArray::Create()
{
    return std::make_shared<OpenGL::VertexArray>();
}   

}