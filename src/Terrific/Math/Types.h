#pragma once

#include "xs_Float.h"


#if 0
typedef int int8;
typedef unsigned int uint8;
typedef int int16;
typedef unsigned int uint16;
typedef long int32;
typedef unsigned long uint32;
typedef float real32;
typedef double real64;
#endif

#include <Magnum/Math/Vector.h>

typedef Magnum::Math::Rad<double> Rad;
typedef Magnum::Math::Rad<float> Radf;

typedef Magnum::Math::Vector2<float> Vector2f;
typedef Magnum::Math::Vector3<float> Vector3f;
typedef Magnum::Math::Vector4<float> Vector4f;
typedef Magnum::Math::Matrix4<float> Matrix4f;

typedef Magnum::Math::Vector2<double> Vector2d;
typedef Magnum::Math::Vector3<double> Vector3d;
typedef Magnum::Math::Vector4<double> Vector4d;
typedef Magnum::Math::Matrix4<double> Matrix4d;


