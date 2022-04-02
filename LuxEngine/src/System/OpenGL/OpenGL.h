#pragma once 

#include "Utils/Types.h"
#include "Utils/Assert.h"

#include <glad/glad.h>

namespace Lux
{

DataType gl_to_lux_datatype(GLenum type)
{   
    switch (type)
    {
        case GL_FLOAT:
            return DataType::FLOAT;
        case GL_INT:
            return DataType::INT;
        case GL_BOOL:
            return DataType::BOOL;
        case GL_FLOAT_VEC2:
            return DataType::V2;
        case GL_FLOAT_VEC3:
            return DataType::V3;
        case GL_FLOAT_VEC4:
            return DataType::V4;
        case GL_FLOAT_MAT2:
            return DataType::MAT2;
        case GL_FLOAT_MAT3:
            return DataType::MAT3;
        case GL_FLOAT_MAT4:
            return DataType::MAT4;
        case GL_SAMPLER_1D:
            return DataType::SAMPLER1D;
        case GL_SAMPLER_2D:
            return DataType::SAMPLER2D;
        case GL_SAMPLER_3D:
            return DataType::SAMPLER3D;

        default:
            TODO();
            return DataType::INVALID;
    }
}

}