#include "pch.h"
#include "Collision.h"

#include "General.h"

namespace engine
{
    //bool Collision::TestSpherePlane(Sphere sphere, Plane plane)
    //{
    //    float dist = glm::dot(sphere.center, plane.normal) - plane.dist;
    //    return abs(dist) <= sphere.radius;
    //}

    //bool Collision::TestSphereBehindPlane(Sphere sphere, Plane plane)
    //{
    //    float dist = glm::dot(sphere.center, plane.normal) - plane.dist;
    //    return dist < -sphere.radius;
    //}

    //bool Collision::TestSphereHalfSpace(Sphere sphere, Plane plane)
    //{
    //    float dist = glm::dot(sphere.center, plane.normal) - plane.dist;
    //    return dist <= sphere.radius;
    //}

    //bool Collision::TestOBBPlane(OBB3D obb, Plane plane)
    //{
    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = obb.halfExtents[0] * abs(glm::dot(plane.normal, obb.orientation[0])) +
    //              obb.halfExtents[1] * abs(glm::dot(plane.normal, obb.orientation[1])) +
    //              obb.halfExtents[2] * abs(glm::dot(plane.normal, obb.orientation[2]));

    //    // Compute signed distance of box center from plane
    //    float s = glm::dot(plane.normal, obb.center) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    // Note : It is not neccesary to normalize the plane's normal as both s and r will both be in the units of |n|
    //    return abs(s) <= r;
    //}

    //bool Collision::TestOBBBehindPlane(OBB3D obb, Plane plane)
    //{
    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = obb.halfExtents[0] * abs(glm::dot(plane.normal, obb.orientation[0])) +
    //        obb.halfExtents[1] * abs(glm::dot(plane.normal, obb.orientation[1])) +
    //        obb.halfExtents[2] * abs(glm::dot(plane.normal, obb.orientation[2]));

    //    // Compute signed distance of box center from plane
    //    float s = glm::dot(plane.normal, obb.center) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    // Note : It is not neccesary to normalize the plane's normal as both s and r will both be in the units of |n|
    //    return s < -r;
    //}

    //bool Collision::TestOBBInsidePlane(OBB3D obb, Plane plane)
    //{
    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = obb.halfExtents[0] * abs(glm::dot(plane.normal, obb.orientation[0])) +
    //        obb.halfExtents[1] * abs(glm::dot(plane.normal, obb.orientation[1])) +
    //        obb.halfExtents[2] * abs(glm::dot(plane.normal, obb.orientation[2]));

    //    // Compute signed distance of box center from plane
    //    float s = glm::dot(plane.normal, obb.center) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    // Note : It is not neccesary to normalize the plane's normal as both s and r will both be in the units of |n|
    //    return s <= r;
    //}

    //bool Collision::TestAABBPlane(AABB3D aabb, Plane plane)
    //{
    //    // These two lines not necessary with a (center, extents) AABB representation
    //    vec3 c = (aabb.max + aabb.min) * 0.5f; // Compute AABB center
    //    vec3 e = aabb.max - c; // Compute positive extents

    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = e[0] * abs(plane.normal[0]) + e[1] * abs(plane.normal[1]) + e[2] * abs(plane.normal[2]);
    //    // Compute distance of box center from plane
    //    float s = glm::dot(plane.normal, c) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    return abs(s) <= r;
    //}

    //bool Collision::TestAABBInsidePlane(AABB3D aabb, Plane plane)
    //{
    //    // These two lines not necessary with a (center, extents) AABB representation
    //    vec3 c = (aabb.max + aabb.min) * 0.5f; // Compute AABB center
    //    vec3 e = aabb.max - c; // Compute positive extents

    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = e[0] * abs(plane.normal[0]) + e[1] * abs(plane.normal[1]) + e[2] * abs(plane.normal[2]);
    //    // Compute distance of box center from plane
    //    float s = glm::dot(plane.normal, c) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    // Note : It is not neccesary to normalize the plane's normal as both s and r will both be in the units of |n|
    //    return s < -r;
    //}

    //bool Collision::TestAABBInsidePlane(AABB3D aabb, Plane plane)
    //{
    //    // These two lines not necessary with a (center, extents) AABB representation
    //    vec3 c = (aabb.max + aabb.min) * 0.5f; // Compute AABB center
    //    vec3 e = aabb.max - c; // Compute positive extents

    //    // Compute the projection interval radius of b onto L(t) = b.c + t * p.n
    //    float r = e[0] * abs(plane.normal[0]) + e[1] * abs(plane.normal[1]) + e[2] * abs(plane.normal[2]);
    //    // Compute distance of box center from plane
    //    float s = glm::dot(plane.normal, c) - plane.dist;

    //    // Intersection occurs when distance s falls within [-r, +r] interval
    //    // Note : It is not neccesary to normalize the plane's normal as both s and r will both be in the units of |n|
    //    return s <= r;
    //}

    bool Collision::Test2DCircleCircle(Circle circle, Circle circle2)
    {
        float sqDist = glm::dot((circle.center - circle2.center), (circle.center - circle2.center));
        return sqDist <= (circle.radius + circle2.radius) * (circle.radius + circle2.radius);
    }

    bool Collision::Test2DCircleAABB(Circle circle, AABB2D aabb)
    {
        // Compute squared distance between sphere center and AABB
        float sqDist = SquareDistPointAABB(circle.center, aabb);
        // Sphere and AABB intersect if the (squared) distance
        // between them is less than the (squared) sphere radius
        return sqDist <= circle.radius * circle.radius;
    }

    //bool Collision::Test2DCircleOBB(Circle circle, OBB2D obb)
    //{
    //    // Find point p on AABB closest to sphere center
    //    ClosestPtPointOBB(sphere.center, obb, closestPoint);

    //    // Sphere and AABB intersect if the (squared) distance from sphere
    //    // center to point p is less than the (squared) sphere radius
    //    vec3 v = closestPoint - sphere.center;
    //    return glm::dot(v, v) <= sphere.radius * sphere.radius;
    //}

    bool Collision::Test2DAABBAABB(AABB2D aabb, AABB2D aabb2)
    {
        return (aabb.min.x <= aabb2.max.x && aabb.max.x >= aabb2.min.x) 
            && (aabb.min.y <= aabb2.max.y && aabb.max.y >= aabb2.min.y);
    }

    bool Collision::Test2DPlaneCircle(Plane2D plane2d, Circle circle)
    {
        // For a normalized plane ( plane2d.normal = 1 ), evaluating the plane equation
        float dist = glm::dot(circle.center, plane2d.normal) - plane2d.dist;
        // If sphere center within +/-radius from plane, plane intersects sphere
        return glm::abs(dist) <= circle.radius;
    }

    bool Collision::Test2DPlaneAABB(Plane2D plane2d, AABB2D aabb)
    {
        // these two lines not necessary wit ha (center, extents) AABB representation
        vec2 center = aabb.max + aabb.min * 0.5f;   // compute AABB center
        vec2 extents = aabb.max - center;   // compute positive extents

        // Compute the projection interval radius of aabb onto L(t) = aabb.c + t * plane2d.n
        float r = extents[0] * glm::abs(plane2d.normal[0]) + extents[1] * glm::abs(plane2d.normal[1]);
        // Compute distance of box center from plane
        float s = glm::dot(plane2d.normal, center) - plane2d.dist;

        // Intersection occurs when distance s falls within [-r, +r] interval
        return glm::abs(s) <= r;
    }

    /*bool Collision::Test2DAABBOBB(AABB2D aabb, OBB2D obb)
    {
        return false;
    }*/

    //bool Collision::TestSphereAABB(Sphere sphere, AABB3D aabb)
    //{
    //    // Compute squared distance between sphere center and AABB
    //    float sqDist = SquareDistPointAABB(sphere.center, aabb);
    //    // Sphere and AABB intersect if the (squared) distance
    //    // between them is less than the (squared) sphere radius
    //    return sqDist <= sphere.radius * sphere.radius;
    //}

    //bool Collision::TestSphereAABB(Sphere sphere, AABB3D aabb, vec3& closestPoint)
    //{
    //    // Find point p on AABB closest to sphere center
    //    ClosestPtPointAABB(sphere.center, aabb, closestPoint);

    //    // Sphere and AABB intersect if the (squared) distance from sphere
    //    // center to point p is less than the (squared) sphere radius
    //    vec3 v = closestPoint - sphere.center;
    //    return glm::dot(v,v) <= sphere.radius * sphere.radius;
    //}

    //bool Collision::TestSphereOBB(Sphere sphere, OBB3D obb, vec3& closestPoint)
    //{
    //    // Find point p on AABB closest to sphere center
    //    ClosestPtPointOBB(sphere.center, obb, closestPoint);

    //    // Sphere and AABB intersect if the (squared) distance from sphere
    //    // center to point p is less than the (squared) sphere radius
    //    vec3 v = closestPoint - sphere.center;
    //    return glm::dot(v,v) <= sphere.radius * sphere.radius;
    //}

    //bool Collision::TestSphereTriangle(Sphere sphere, Triangle3D triangle, vec3& closestPoint)
    //{
    //    closestPoint = ClosestPtPointTriangle(sphere.center, triangle);

    //    // Sphere and triangle intersect if the (squared) distance from sphere
    //    // center to point p is less than the (squared) sphere radius
    //    vec3 v = closestPoint - sphere.center;
    //    return glm::dot(v, v) <= sphere.radius * sphere.radius;
    //}
}
