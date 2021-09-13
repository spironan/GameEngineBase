/************************************************************************************//*!
\file           General.h
\project        <PROJECT_NAME>
\author         Chua Teck Lee, c.tecklee, 390008420
\par            email: c.tecklee\@digipen.edu
\date           September 2, 2021
\brief          Contains General Mathematical Functions between various geometrical
                data.

Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*//*************************************************************************************/
#pragma once

#include "Engine/PhysicsCollision/Shapes.h"

namespace engine
{
    //static float    Orient2D                (vec2 a, vec2 b, vec2 c);
    //static float    Signed2DTriArea         (vec2 a, vec2 b, vec2 c);
    ////static float    InCircle2D              (vec2 a, vec2 b, vec2 c, vec2 d);
    float SquareDistPointAABB(vec2 const& point, AABB2D const& aabb);

    //// Given three noncollinear points( ordered ccw), compute plane equation
    //Plane ComputePlane(vec3 a, vec3 b, vec3 c)
    //{
    //    Plane p;
    //    p.normal = oom::normalize(oom::cross(b - a, c - a));
    //    p.dist = oom::dot(p.normal, a);
    //    return p;
    //}

    //static float    Orient3D                (vec3 a, vec3 b, vec3 c, vec3 d);
    //static bool     IsConvexQuad            (vec3 a, vec3 b, vec3 c, vec3 d);
    //static float    SquaredDistPointSegment (vec3 start, vec3 end, vec3 point);
    //
    //static vec3     ClosestPtPointPlane     (vec3 point, Plane plane);
    //static float    DistPointPlane          (vec3 point, Plane plane);
    //static void     ClosestPtPointSegment   (vec3 c, vec3 a, vec3 b, float& t, vec3& d);
    //static float    SquaredDistPointSegment (vec3 a, vec3 b, vec3 c);
    //static float    SquaredDistPointOBB     (vec3 p, OBB3D b);
    //static void     ClosestPtPointRect      (vec3 p, AABB3D r, vec3& q);
    //static void     ClosestPtPointRect      (vec3 p, vec3 a, vec3 b, vec3 c, vec3& q);
    // Distance of Point to AABB
    //static void     ClosestPtPointAABB      (vec3 point, AABB3D aabb, vec3& output);
    //static float    SquareDistPointAABB     (vec3 point, AABB3D aabb);
    //static void     ClosestPtPointOBB       (vec3 point, OBB3D obb, vec3& output);
    //static vec3     ClosestPtPointTriangle  (vec3 point, Triangle3D triangle);

}
