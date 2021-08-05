#pragma once
#include "vec.h"
#include "matrix.h"

namespace oom
{
    static constexpr std::size_t TWO_DIMENSION      = 2;
    static constexpr std::size_t THREE_DIMENSION    = 3;

    template<std::size_t Dim, typename = std::enable_if_t<(Dim == TWO_DIMENSION) || (Dim == THREE_DIMENSION)>>
    struct Circle 
    {
        vec<Dim>    center;
        float       radius;
    };
    using Circle2D = Circle<2>;
    using Circle3D = Circle<3>;

    template<std::size_t Dim, typename = std::enable_if_t<(Dim == TWO_DIMENSION) || (Dim == THREE_DIMENSION)>>
    struct Rect
    {
        vec<Dim> center;                     // center point of rectangle
        mat<Dim> axesVectors[Dim - 1];       // unit vectors determining local x and y axes for the rectangle
        vec<Dim> extents[Dim - 1];           // the halfwidth extents of the rectangle along the axes.
    };
    using Rect2D = Rect<2>;
    using Rect3D = Rect<3>;

    // cheapest way to store a plane : 4 components.
    template<std::size_t Dim, typename = std::enable_if_t<(Dim == TWO_DIMENSION) || (Dim == THREE_DIMENSION)>>
    struct Plane
    {
        vec<Dim>    normal;     // the normal of this plane
        float       dist;       // distance to the plane.
    };
    using Plane2D = Plane<2>;
    using Plane3D = Plane<3>;

    template<std::size_t Dim, typename = std::enable_if_t<(Dim == TWO_DIMENSION) || (Dim == THREE_DIMENSION)>>
    struct Capsule 
    {
        vec<Dim>    start, end;
        float       radius;
    };
    using Capsule2D = Capsule<2>;
    using Capsule3D = Capsule<3>;

    struct Lozenge 
    {
        vec3 origin;
        vec3 width, height;
        float radius;
    };

    struct Slab 
    {
        vec3 normal;
        float dNear;    // signed distance from origin for near plane
        float dFar;     // signed distance from origin for far plane
    };

    struct KDOP 
    {
        float min[4];   // maximum distance from origin along axes 0 to 3
        float max[4];   // minimum distance from origin along axes 0 to 3
    };


    class Arithmetic
    {
    public:

        // if value = 0, 3 points are coplaner
        // Assumming Triangle ABC is oriented counterclockwise
        // if value > 0, point C lies to the left of the directed Line AB.
        // if value < 0, point C lies to the right of the directed Line AB.
        // the value also corresponds to twice the signed area of the triangle ABC.
        static float Orient2D(point2 const& a, point2 const& b, point2 const& c)
        {
            return (a.x() - c.x()) * (b.y() - c.y()) - (b.x() - c.x()) * (a.y() - c.y());
        }

        // if value = 0, all 4 points are coplaner
        // Assuming Triangle ABC appears in counterclockwise order when viewed from D
        // if value < 0, Point D lies above the support plane of Triangle ABC
        // if value > 0 , Point D lies below the support plane of Triangle ABC
        // the value also corresponds to six times the signed volume of the tetrahedron formed by the four points.
        static float Orient3D(point3 const& a, point3 const& b, point3 const& c, point3 const& d)
        {
            return (a - d).Dot((b - d).Cross(c - d));
        }


    };

}