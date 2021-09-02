/************************************************************************************//*!
\file          Shapes.h
\project       <PROJECT_NAME>
\author        Chua Teck Lee, c.tecklee, 390008420
\par           email: c.tecklee\@digipen.edu
\date          September 2, 2021
\brief         

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "PhysicsFwd.h"

namespace engine
{
    //struct Triangle2D
    //{
    //    vec2 a, b, c;
    //};

    //struct LineSegment
    //{
    //    vec2 begin, end;
    //};

    //struct Ray
    //{
    //    vec2 origin, dir;
    //};

    struct Plane2D
    {
        vec2 normal;
        float dist;
    };

    struct AABB2D
    {
        vec2 min, max;
    };

    //struct OBB2D
    //{
    //    vec2 center;
    //    mat2 orientation;
    //    vec2 halfExtents;
    //};

    struct Circle
    {
        vec2 center;
        float radius;
    };

    //struct Capsule2D
    //{
    //    vec2 start, end;
    //    float radius;
    //};

    //struct Lozenges2D
    //{
    //    vec2 origin;
    //    vec2 width, height;
    //    float radius;
    //};


    //struct Triangle3D
    //{
    //    vec3 a, b, c;
    //};

    //struct AABB3D
    //{
    //    vec3 min, max;
    //};

    //struct OBB3D
    //{
    //    vec3 center;
    //    oom::mat3 orientation;
    //    vec3 halfExtents;
    //};

    //struct Sphere
    //{
    //    vec3 center;
    //    float radius;
    //};

    //struct Plane
    //{
    //    vec3 normal;
    //    float dist;
    //};

    //struct Capsule3D
    //{
    //    vec3 start, end;
    //    float radius;
    //};

    //struct Lozenges3D
    //{
    //    vec3 origin;
    //    vec3 width, height;
    //    float radius;
    //};

    //struct Slab 
    //{
    //    vec3 normal;
    //    float dNear;    // signed distance from origin for near plane
    //    float dFar;     // signed distance from origin for far plane
    //};

    //template<size_t size>
    //struct KDOP 
    //{
    //    float min[size];   // maximum distance from origin along axes 0 to 3
    //    float max[size];   // minimum distance from origin along axes 0 to 3
    //};


}
