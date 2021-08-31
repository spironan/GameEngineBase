#pragma once

#include "Engine/PhysicsCollision/Shapes.h"

namespace engine
{
    class Collision
    {
    public:
        
        static bool Test2DCircleCircle      (Circle circle, Circle circle2);
        static bool Test2DCircleAABB        (Circle circle, AABB2D aabb);
        //static bool Test2DCircleOBB       (Circle circle, OBB2D obb);

        static bool Test2DAABBAABB          (AABB2D aabb, AABB2D aabb2);
        //static bool Test2DAABBOBB         (AABB2D aabb, OBB2D obb);

        static bool Test2DPlaneCircle       (Plane2D plane2d, Circle circle);
        static bool Test2DPlaneAABB         (Plane2D plane2d, AABB2D aabb);
        //static bool Test2DPlaneOBB        (Plane2D plane2d, OBB2D obb);
        //static bool Test2DSegmentSegment  (vec2 a, vec2 b, vec2 c, vec2 d, float& t, vec2& p);


        /*static bool TestSphereAABB          (Sphere sphere, AABB3D aabb);
        static bool TestSphereAABB          (Sphere sphere, AABB3D aabb, vec3& closestPoint);
        static bool TestSphereOBB           (Sphere sphere, OBB3D obb, vec3& closestPoint);
        static bool TestSphereTriangle      (Sphere sphere, Triangle3D triangle, vec3& p);

        static bool TestSpherePlane         (Sphere sphere, Plane plane);
        static bool TestSphereBehindPlane   (Sphere sphere, Plane plane);
        static bool TestSphereHalfSpace     (Sphere sphere, Plane plane);

        static bool TestOBBPlane            (OBB3D obb, Plane plane);
        static bool TestOBBBehindPlane      (OBB3D obb, Plane plane);
        static bool TestOBBInsidePlane      (OBB3D obb, Plane plane);
                                            
        static bool TestAABBPlane           (AABB3D aabb, Plane plane);
        static bool TestAABBInsidePlane     (AABB3D aabb, Plane plane);
        static bool TestAABBInsidePlane     (AABB3D aabb, Plane plane);*/
    };

}
