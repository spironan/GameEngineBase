#include "pch.h"
#include "General.h"

namespace engine
{
    float SquareDistPointAABB(vec2 const& point, AABB2D const& aabb)
    {
        float sqDist = 0.f;
        for (int i = 0; i < 2; ++i)
        {
            float v = point[i];
            if (v < aabb.min[i]) sqDist += (aabb.min[i] - v) * (aabb.min[i] - v);
            if (v > aabb.max[i]) sqDist += (v - aabb.max[i]) * (v - aabb.max[i]);
        }
        return sqDist;
    }

    //// if value = 0, 3 points are collinear
    //// Assumming Triangle ABC is oriented counterclockwise
    //// if value > 0, point C lies to the left of the directed Line AB.
    //// if value < 0, point C lies to the right of the directed Line AB.
    //// the value also corresponds to twice the signed area of the triangle ABC.
    //float Orient2D(vec2 a, vec2 b, vec2 c)
    //{
    //    return (a.x - c.x) * (b.y - c.y) - (b.x - c.x) * (a.y - c.y);
    //}

    //// if value = 0, all 4 points are coplaner
    //// Assuming Triangle ABC appears in counterclockwise order when viewed from D
    //// if value < 0, Point D lies above the support plane of Triangle ABC
    //// if value > 0 , Point D lies below the support plane of Triangle ABC
    //// the value also corresponds to six times the signed volume of the tetrahedron formed by the four points ABCD.
    //float Orient3D(vec3 a, vec3 b, vec3 c, vec3 d)
    //{
    //    return glm::dot(a - d, glm::cross(b - d, c - d));
    //    //return (a - d).dot((b - d).cross(c - d));
    //}

    ////float InCircle2D(vec2 a, vec2 b, vec2 c, vec2 d)
    ////{
    ////    //a.
    ////    Mat3 mat;
    ////}


    //// Compute barycentric coordinates(u,v,w) for
    //// point p with respect to triangle (a,b,c)
    //void Barycentric(vec3 a, vec3 b, vec3 c, vec3 p, float& u, float& v, float& w)
    //{
    //    // unnormalized triangle normal
    //    vec3 m = glm::cross(b - a, c - a);
    //    // nominators and one-over-denominator for u and v ratios
    //    float nu, nv, ood;
    //    // absolute components for determining projection plane
    //    float x = glm::abs(m.x), y = glm::abs(m.y), z = glm::abs(m.z);


    //    if (x >= y && x >= z)
    //    {
    //        //x is largest, project to the yz plane
    //        nu = TriArea2D(p.y, p.z, b.y, b.z, c.y, c.z);   // Area of PBC in yz plane
    //        nv = TriArea2D(p.y, p.z, c.y, c.z, a.y, a.z);   // Area of PCA in yz plane
    //        ood = 1.f / m.x;                                // 1/(2* area of ABC in yz plane)
    //    }
    //    else if (y >= x && y >= z)
    //    {
    //        // y is largest, project to the xz plane
    //        nu = TriArea2D(p.x, p.z, b.x, b.z, c.x, c.z);   // Area of PBC in xz plane
    //        nv = TriArea2D(p.x, p.z, c.x, c.z, a.x, a.z);   // Area of PCA in xz plane
    //        ood = 1.f / -m.y;                                // 1/(2* area of ABC in xz plane)
    //    }
    //    else
    //    {
    //        // z is largest, project to the xy plane
    //        nu = TriArea2D(p.x, p.y, b.x, b.y, c.x, c.y);   // Area of PBC in xy plane
    //        nv = TriArea2D(p.x, p.y, c.x, c.y, a.x, a.y);   // Area of PCA in xy plane
    //        ood = 1.f / m.z;                                // 1/(2* area of ABC in xy plane)
    //    }
    //    u = nu * ood;
    //    v = nv * ood;
    //    w = 1.f - u - w;

    //    /*vec3 v0 = b - a, v1 = c - a, v2 = p - a;
    //    float d00 = glm::dot(v0, v0);
    //    float d01 = glm::dot(v0, v1);
    //    float d11 = glm::dot(v1, v1);
    //    float d20 = glm::dot(v2, v0);
    //    float d21 = glm::dot(v2, v1);

    //    float denom = d00 * d11 - d01 * d01;
    //    v = (d11 * d20 - d01 * d21) / denom;
    //    w = (d00 * d21 - d01 * d20) / denom;*/
    //    u = 1.0f - v - w;
    //}

    //float TriArea2D(vec2 a, vec2 b, vec2 c)
    //{
    //    return (a.x - b.x) * (b.y - c.y) - (b.x - c.x) * (a.y - b.y);
    //}

    //bool TestPointTriangle(vec3 p, vec3 a, vec3 b, vec3 c)
    //{
    //    float u, v, w;
    //    Barycentric(a, b, c, p, u, v, w);
    //    return v >= 0.f && w >= 0.f && (v + w) <= 1.0f;
    //}

    //// take note this is not fullproof, there are edge cases
    //// refer to real-time collision book page 61
    //bool IsConvexQuad(vec3 a, vec3 b, vec3 c, vec3 d)
    //{
    //    vec3 bda = glm::cross(d - b, a - b); //(d - b).cross(a - b);
    //    vec3 bdc = glm::cross(d - b, c - b); //(d - b).cross(c - b);
    //    if (/*bda.dot(bdc) */ glm::dot(bda, bdc) >= 0.f) return false;  // Quad is not convex if dot product of resulting 2 cross products are >= 0

    //    // Quad is now convex if Dot product is lesser then 0
    //    vec3 acd = glm::cross(c - a, d - a); //(c - a).cross(d - a);
    //    vec3 acb = glm::cross(c - a, b - a); //(c - a).cross(b - a);

    //    return glm::dot(acd, acb) < 0.f;
    //    //return acd.dot(acb) < 0.f;
    //}

    //float SquaredDistPointSegment(vec2 start, vec2 end, vec2 point)
    //{
    //    vec2 dirVec = end - start;
    //    vec2 normal{ -dirVec.y, dirVec.x };
    //    //normal.normalize();
    //    normal = glm::normalize(normal);
    //    float dist = glm::dot(start - point, normal) ;
    //    return dist * dist;
    //}


    //// This function is presumably more optimzed then the code above, must be tested though.
    //// it postpone's the division operation unlike the above which performs it all the time.
    //void ClosestPtPointSegment(vec3 c, vec3 a, vec3 b, float& t, vec3& d)
    //{
    //    vec3 ab = b - a;
    //    // Project c onto ab, but deferring divide by Dot(ab,ab)
    //    
    //    t = glm::dot(c - d, ab); //(c - a).dot(ab);
    //    if (t <= 0.f)
    //    {
    //        // c projects outside the [a,b] interval, on the a side; clamp to a
    //        t = 0.f;
    //        d = a;
    //    }
    //    else
    //    {
    //        float denom = glm::dot(ab, ab); // ab.dot(ab);   // always positive since denom ||ab||^2
    //        if (t >= denom)
    //        {
    //            // c projects outside the [a,b] interval, on the b side; clamp to b
    //            t = 1.f;
    //            d = b;
    //        }
    //        else
    //        {
    //            // c projects outside the [a,b] interval, on the b side; clamp to b
    //            t = t / denom;
    //            d = a + t * ab;
    //        }
    //    }
    //}

    //// returns the squared distance between point c and segment ab
    //float SqDistancePointSegment(vec3 a, vec3 b, vec3 c)
    //{
    //    vec3 ab = b - a, ac = c - a, bc = c - b;
    //    float e = glm::dot(ac, ab); // ac.dot(ab);
    //    // handle cases where c projects outside ab
    //    if (e <= 0.f) return glm::dot(ac, ac); // ac.dot(ac);
    //    float f = glm::dot(ab, ab); // ab.dot(ab);
    //    if (e >= f) return glm::dot(bc, bc);// bc.dot(bc);
    //    // handle cases where c projects onto ab
    //    return /*ac.dot(ac) */ glm::dot(ac, ac) - e * e / f;
    //}

    //// Given point p, return the point q on or in AABB b that is closest to p
    //void ClosestPtPointAABB(vec3 p, AABB3D b, vec3& q)
    //{
    //    // for each coordinate axis, if the point coordinate value is
    //    // outside box, clamp it to the box, else keep it as is
    //    for (int i = 0; i < 3; ++i) {
    //        float v = p[i];
    //        if (v < b.min[i]) v = b.min[i]; // v = max(v, b.min[i])
    //        if (v > b.max[i]) v = b.max[i]; // v = min(v, b.max[i])
    //        q[i] = v;
    //    }
    //}

    //// Distance of Point to AABB
    //float SquaredDistPointAABB(vec3 p, AABB3D b)
    //{
    //    float sqDist = 0.f;
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        float v = p[i];
    //        if (v < b.min[i]) sqDist += (b.min[i] - v) * (b.min[i] - v);
    //        if (v > b.max[i]) sqDist += (v - b.max[i]) * (v - b.max[i]);
    //    }
    //    return sqDist;
    //}

    //// Given point p, return poiunt q on (or in) OBB b, closest to p
    //void ClosestPtPointOBB(vec3 p, OBB3D b, vec3& q)
    //{
    //    vec3 d = p - b.center;
    //    // Start result at center of box; make steps from there
    //    q = b.center;
    //    // For each OBB axis : effectively transforming point to local OBB coord system and converting back to world coords
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        // ...project d onto that axis to get the distance
    //        // along the axis of d from the box center
    //        float dist = d.dot(b.orientation[i]);
    //        // if distance farther than the box extents, clamp to the box
    //        if (dist > b.halfExtents[i]) dist = b.e[i];
    //        if (dist < -b.halfExtents[i]) dist = -b.e[i];
    //        // Step that distance along the axis to get world coordinate
    //        q += dist * b.orientation[i];
    //    }
    //}

    //// Compute the square distance between point p and OBB b
    //float SquaredDistPointOBB(vec3 p, OBB3D b)
    //{
    //    vec3 closestPoint;
    //    ClosestPtPointOBB(p, b, closestPoint);
    //    float sqDist = (closestPoint - p).dot(closest - p);
    //    return sqDist;
    //}


    //// return point q on (or in) rect (specified by a, b, and c), closest to given point p
    //void ClosestPtPointRect(vec3 p, vec3 a, vec3 b, vec3 c, vec3& q)
    //{
    //    vec3 ab = b - a;   // vector across rect
    //    vec3 ac = c - a;   // vector down rect
    //    // Start result at top-left corner of rect; make steps from there
    //    q = a;
    //    // Clamp p' (projection of p to plane of r) to rectangle in the across direction
    //    float dist = d.dot(ab);
    //    float maxdist = ab.dot(ab);
    //    if (dist >= maxdist) q += ab;
    //    else if (dist > 0.f) q += (dist / maxdist) * ab;

    //    // Clamp p' (projection of p to plane of r) to rectangle in the down direction
    //    dist = d.dot(ac);
    //    maxdist = ac.dot(ac);
    //    if (dist >= maxdist) q += ac;
    //    else if (dist > 0.f) q += (dist / maxdist) * ac;

    //}


    //// returns 2 times the signed triangle area. the result is positive if
    //// abc is ccw, negative if abc is cw, zero if abc is degenerate.
    //float Signed2DTriArea(vec2 a, vec2 b, vec2 c)
    //{
    //    return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
    //}

    //float SquareDistPointSegment(vec2 start, vec2 end, vec2 point)
    //{
    //    vec2 dirVec = end - start;
    //    vec2 normal{ -dirVec.y, dirVec.x };
    //    normal = glm::normalize(normal);
    //    //normal.normalize();
    //    return glm::dot(start - point, normal)* glm::dot(start - end, normal);
    //    //return (start - point).dot(normal) * (start - point).dot(normal);
    //}


    //bool TestSphereCapsule(Sphere sphere, Capsule3D capsule)
    //{
    //    // Compute the squared distance between sphere center and capsule line segment
    //    float distSq = SquaredDistPointSegment(capsule.start, capsule.end, sphere.center);
    //    // if squared distance smaller than squared sum of radii they collide.
    //    float radius = sphere.radius + capsule.radius;
    //    return distSq <= radius * radius;
    //}

    //bool TestCapsuleCapsule(Capsule3D capsule1, Capsule3D capsule2)
    //{
    //    float s, t;
    //    vec3 c1, c2;
    //    float distSq = ClosestPtSegmentSegment(capsule1.start, capsule1.end, capsule2.start, capsule2.end, s, t, c1, c2);

    //    float radius = capsule1.radius + capsule2.radius;
    //    return distSq <= radius * radius;
    //}

    //// Calculate the closest point on a given plane, given a point that may or may not be on the plane.
    //vec3 ClosestPtPointPlane(vec3 point, Plane plane2)
    //{
    //    // if plane's normal is not normalized
    //    float t = (plane2.normal.dot(point) - plane2.dist) / plane2.normal.dot(plane2.normal);
    //    // if plane's normal is normalized
    //    // float t = plane2.normal.dot(point) - plane2.dist;
    //    return point - t * plane2.normal;
    //}

    //// Calculate the closest point on a given plane, given a point that may or may not be on the plane.
    //vec3 ClosestPtPointPlane(vec3 point, Plane plane)
    //{
    //    // if plane's normal is not normalized
    //    float t = plane.normal.dot(point - plane.point) / plane.normal.dot(plane.normal);
    //    // if plane's normal is normalized
    //    // float t = plane.normal.dot(point - plane.point);
    //    return point - t * plane.normal;
    //}

    //// Calculates the signed distance of point q from Plane
    //// if dist > 0, q is above the plane(in the normal's direction)
    //// else if dist < 0 q is below the plane (against the normal's direction)
    //// else q lies on the plane
    //// Note : this is Equivalent to calculating the value of t; t = (normal.point)-dist;
    //float DistPointPlane(vec3 point, Plane plane)
    //{
    //    return plane.normal.dot(point) - plane.dist / plane.normal.dot(plane.normal);
    //}


    //// Given point p, return the point q on or in AABB b that is closest to p
    //void ClosestPtPointAABB(vec3 point, AABB3D aabb, vec3& output)
    //{
    //    // for each coordinate axis, if the point coordinate value is
    //    // outside box, clamp it to the box, else keep it as is
    //    for (int i = 0; i < 3; ++i) {
    //        float v = point[i];
    //        if (v < aabb.min[i]) v = aabb.min[i];
    //        if (v > aabb.max[i]) v = aabb.max[i];
    //        output[i] = v;
    //    }
    //}
    // 
    //// Distance of Point to AABB
    //float SquareDistPointAABB(vec3 point, AABB3D aabb)
    //{
    //    float sqDist = 0.f;
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        float v = point[i];
    //        if (v < aabb.min[i]) sqDist += (aabb.min[i] - v) * (aabb.min[i] - v);
    //        if (v > aabb.max[i]) sqDist += (v - aabb.max[i]) * (v - aabb.max[i]);
    //    }
    //    return sqDist;
    //}

    //// Given point p, return poiunt q on (or in) OBB b, closest to p
    //void ClosestPtPointOBB(vec3 point, OBB3D obb, vec3& output)
    //{
    //    vec3 d = point - obb.center;
    //    // Start result at center of box; make steps from there
    //    output = obb.center;
    //    // For each OBB axis : effectively transforming point to local OBB coord system and converting back to world coords
    //    for (int i = 0; i < 3; ++i)
    //    {
    //        // ...project d onto that axis to get the distance
    //        // along the axis of d from the box center
    //        float dist = glm::dot(d, obb.orientation[i]);
    //        // if distance farther than the box extents, clamp to the box
    //        if (dist > obb.halfExtents[i]) dist = obb.halfExtents[i];
    //        if (dist < -obb.halfExtents[i]) dist = -obb.halfExtents[i];
    //        // Step that distance along the axis to get world coordinate
    //        output += dist * obb.orientation[i];
    //    }
    //}

    //vec3 ClosestPtPointTriangle(vec3 point, Triangle3D triangle)
    //{
    //    // Check if P in vertex region outside A
    //    vec3 ab = triangle.b - triangle.a;
    //    vec3 ac = triangle.c - triangle.a;
    //    vec3 ap = point - triangle.a;


    //    float d1 = glm::dot(ab, ap);
    //    float d2 = glm::dot(ac, ap);
    //    if (d1 <= 0.f && d2 <= 0.f) return triangle.a;    // barycentric coordinates (1,0,0)

    //    // Check if P in vertex region outside B
    //    vec3 bp = point - triangle.b;

    //    float d3 = glm::dot(ab, bp);
    //    float d4 = glm::dot(ac, bp);
    //    if (d3 >= 0.f && d4 <= d3) return triangle.b;    // barycentric coordinates (0, 1, 0)

    //    float vc = d1 * d4 - d3 * d2;
    //    if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f)
    //    {
    //        float v = d1 / (d1 - d3);
    //        return triangle.a + v * ab; // barycentric coordinates (1-v, v, 0)
    //    }

    //    // Check if P in vertex region outside C
    //    vec3 cp = point - triangle.c;

    //    float d5 = glm::dot(ab, cp);
    //    float d6 = glm::dot(ac, cp);
    //    if (d6 >= 0.f && d5 <= d6) return triangle.c; // barycentric coordinates (0, 0, 1)

    //    // Check if P in edge region of AC, if so return projection of P onto AC
    //    float vb = d5 * d2 - d1 * d6;
    //    if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f)
    //    {
    //        float w = d2 / (d2 - d6);
    //        return triangle.a + w * ac;  // barycentric coordinates (1-w, 0, w)
    //    }

    //    // check if P in edge region of BC, if so return projection of P onto BC

    //    float va = d3 * d6 - d5 * d4;
    //    if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f)
    //    {
    //        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    //        return triangle.b + w * (triangle.c - triangle.b);   // barycentric coordinates (0, 1-w, w)
    //    }

    //    // P inside face region. Compute Q through its barycentric coordinates (u, v ,w)
    //    float denom = 1.f / (va + vb + vc);
    //    float v = vb * denom;
    //    float w = vc * denom;
    //    return triangle.a + v * ab + w * ac; // = u*a + v*b + w*c, u = va * denom = 1.f - v - w

    //}
}
