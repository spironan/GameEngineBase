#pragma once

#include "Collider.h"

namespace engine
{
    class CircleCollider2D : public Collider2D
    {
    public:
        virtual Manifold2D TestCollision(Collider2D const* collider) const;
        virtual Manifold2D TestCollision(BoxCollider2D const* boxCollider2D) const;
        virtual Manifold2D TestCollision(CircleCollider2D const* circleCollider2D) const;
    
    private:
        Circle m_bounds;
        float m_radius;

    public:
        explicit CircleCollider2D(Entity entity, bool active = true);
        
        /*-----------------------------------------------------------------------------*/
        /* Constructors and Destructors                                                */
        /*-----------------------------------------------------------------------------*/
        CircleCollider2D()                                  = delete;
        CircleCollider2D(CircleCollider2D const&)           = default;
        CircleCollider2D(CircleCollider2D &&)               = default;
        CircleCollider2D& operator=(CircleCollider2D const&)= default;
        CircleCollider2D& operator=(CircleCollider2D &&)    = default;
        virtual ~CircleCollider2D() override                = default;

        Circle GetGlobalBounds() const;
    };
}
