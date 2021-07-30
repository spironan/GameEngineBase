#include "pch.h"
//#include "CircleCollider.h"
//
//#include "Engine/PhysicsCollision/Algorithms/Collision.h"
//
//#include "BoxCollider.h"
//
//namespace engine
//{
//    CircleCollider2D::CircleCollider2D(Entity entity, bool active)
//        : Collider2D{ entity, active }
//        , m_bounds{ { 0.f, 0.f }, 1.f }
//        , m_radius{ 0.5f }
//    {};
//    
//    Circle CircleCollider2D::GetGlobalBounds() const
//    {
//        vec2 worldScale = WorldScale();
//        float scalar = worldScale.x >= worldScale.y ? worldScale.x : worldScale.y;
//
//        return Circle
//        { m_bounds.center + WorldPosition() * Offset
//        , m_bounds.radius * m_radius * scalar
//        };
//    }
//
//    bool CircleCollider2D::TestCollision(Collider2D const* collider) const
//    {
//        return collider->TestCollision(this);
//    }
//
//    bool CircleCollider2D::TestCollision(BoxCollider2D const* boxCollider2D) const
//    {
//        return Collision::Test2DCircleAABB(this->GetGlobalBounds(), boxCollider2D->GetGlobalBounds());
//    }
//
//    bool CircleCollider2D::TestCollision(CircleCollider2D const* circleCollider2D) const
//    {
//        return Collision::Test2DCircleCircle(this->GetGlobalBounds(), circleCollider2D->GetGlobalBounds());
//    }
//}
