#include "pch.h"

#include "ImpulseSolver.h"

#include "Engine/PhysicsCollision/RigidBody.h"

#include "Engine/ECS/WorldManager.h"
#include "Engine/Transform/Transform3D.h"

namespace engine
{
    void ImpulseSolver::Solve(std::vector<Manifold2D>& manifolds, float dt)
    {
        for (Manifold2D& manifold : manifolds)
        {
            //Replaces non dynamic objects with default values.
            
            Rigidbody2D* aBody = manifold.ObjA->IsDynamic() ? manifold.ObjA : nullptr;
            Rigidbody2D* bBody = manifold.ObjB->IsDynamic() ? manifold.ObjB : nullptr;

            oom::vec2 aVel = aBody ? aBody->GetVelocity() : oom::vec2{ 0.0f };
            oom::vec2 bVel = bBody ? bBody->GetVelocity() : oom::vec2{ 0.0f };
            oom::vec2 rVel = bVel - aVel;
            float velAlongNormal = oom::dot(rVel, manifold.Normal);

            float aInvMass = aBody ? aBody->GetInverseMass() : 1.0f;
            float bInvMass = bBody ? bBody->GetInverseMass() : 1.0f;

            
            // Impluse

            // This is important for convergence
            // a negitive impulse would drive the objects closer together
            if (velAlongNormal >= 0)
                continue;

            // restitution
            float e = (aBody ? aBody->GetMaterial().Restitution : 1.0f) * (bBody ? bBody->GetMaterial().Restitution : 1.0f);

            float j = -(1.0f + e) * velAlongNormal / (aInvMass + bInvMass);

            oom::vec2 impluse = j * manifold.Normal;

            if (aBody ? !aBody->IsKinematic() : false) {
                aVel -= impluse * aInvMass;
            }

            if (bBody ? !bBody->IsKinematic() : false) {
                bVel += impluse * bInvMass;
            }

            // Friction
            rVel = bVel - aVel;

            oom::vec2 tangent = rVel - oom::dot(rVel, manifold.Normal) * manifold.Normal;

            if (oom::length(tangent) > 0.0001f) { // safe normalize
                tangent = oom::normalize(tangent);
            }

            // Solve for magnitude to apply along the friction vector
            float fVel = oom::dot(rVel, tangent);

            float aSF = aBody ? aBody->GetMaterial().StaticFriction : 0.0f;
            float bSF = bBody ? bBody->GetMaterial().StaticFriction : 0.0f;
            float aDF = aBody ? aBody->GetMaterial().DynamicFriction : 0.0f;
            float bDF = bBody ? bBody->GetMaterial().DynamicFriction : 0.0f;
            float mu = oom::length(vec2(aSF, bSF));

            float f = -fVel / (aInvMass + bInvMass);

            oom::vec2 friction;
            if (abs(f) < j * mu) {
                friction = f * tangent;
            }
            else {
                mu = oom::length(oom::vec2(aDF, bDF));
                friction = -j * tangent * mu;
            }

            if (aBody ? !aBody->IsKinematic() : false) {
                aBody->SetVelocity(aVel - friction * aInvMass);
            }

            if (bBody ? !bBody->IsKinematic() : false) {
                bBody->SetVelocity(bVel + friction * bInvMass);
            }

            // Naive positional correction.
            {
                const float percent = 0.8f; // usually 20% to 80%
                const float slop = 0.01f; // usually 0.01 to 0.1
                vec2 correction = std::fmaxf(manifold.PenetrationDepth - slop, 0.0f) / (aInvMass + bInvMass)* percent* manifold.Normal;
                if(aBody) aBody->GetComponent<Transform3D>().Position() -= oom::vec3{ aInvMass * correction , 0.f};
                if(bBody) bBody->GetComponent<Transform3D>().Position() += oom::vec3{ bInvMass * correction , 0.f};
            }
        }
    }

}
