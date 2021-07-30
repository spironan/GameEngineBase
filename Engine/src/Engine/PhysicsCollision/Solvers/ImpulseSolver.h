#pragma once

#include "Engine/PhysicsCollision/Manifold.h"
#include "Engine/PhysicsCollision/Rigidbody.h"

#include <vector>
#include <glm/glm.hpp>

namespace engine
{
    class ImpulseSolver
    {
    public:
        void ImpulseSolver::Solve(
            std::vector<Manifold2D>& manifolds,
            float dt)
        {
            for (Manifold2D& manifold : manifolds) 
            {
                // Replaces non dynamic objects with default values.

                Rigidbody2D* aBody = !manifold.ObjA.IsStatic ? &manifold.ObjA : nullptr;
                Rigidbody2D* bBody = !manifold.ObjA.IsStatic ? &manifold.ObjB : nullptr;
                
                glm::vec2 aVel = aBody ? aBody->GetVelocity() : glm::vec2{0.0f};
                glm::vec2 bVel = bBody ? bBody->GetVelocity() : glm::vec2{0.0f};
                glm::vec2 rVel = bVel - aVel;
                float nSpd = glm::dot(rVel, manifold.Normal);

                float aInvMass = aBody ? aBody->GetInverseMass() : 1.0f;
                float bInvMass = bBody ? bBody->GetInverseMass() : 1.0f;

                // Impluse

                // This is important for convergence
                // a negitive impulse would drive the objects closer together
                if (nSpd >= 0)
                    continue;

                float e = (aBody ? aBody->Restitution : 1.0f) * (bBody ? bBody->Restitution : 1.0f);

                float j = -(1.0f + e) * nSpd / (aInvMass + bInvMass);

                glm::vec2 impluse = j * manifold.Normal;

                if (aBody ? /*aBody->IsKinematic*/ !aBody->IsStatic : false) {
                    aVel -= impluse * aInvMass;
                }

                if (bBody ? /*bBody->IsKinematic*/ !aBody->IsStatic : false) {
                    bVel += impluse * bInvMass;
                }

                // Friction

                rVel = bVel - aVel;
                nSpd = glm::dot(rVel, manifold.Normal);

                glm::vec2 tangent = rVel - nSpd * manifold.Normal;

                if (glm::length(tangent) > 0.0001f) { // safe normalize
                    tangent = glm::normalize(tangent);
                }

                float fVel = glm::dot(rVel, tangent);

                float aSF = aBody ? aBody->StaticFriction : 0.0f;
                float bSF = bBody ? bBody->StaticFriction : 0.0f;
                float aDF = aBody ? aBody->DynamicFriction : 0.0f;
                float bDF = bBody ? bBody->DynamicFriction : 0.0f;
                float mu = glm::vec2(aSF, bSF).length();

                float f = -fVel / (aInvMass + bInvMass);

                glm::vec2 friction;
                if (abs(f) < j * mu) {
                    friction = f * tangent;
                }

                else {
                    mu = glm::length(glm::vec2(aDF, bDF));
                    friction = -j * tangent * mu;
                }

                if (aBody ? /*aBody->IsKinematic*/ !aBody->IsStatic : false) {
                    aBody->SetVelocity(aVel - friction * aInvMass);
                }

                if (bBody ? /*bBody->IsKinematic */!bBody->IsStatic : false) {
                    bBody->SetVelocity(bVel + friction * bInvMass);
                }
            }
        }
    };
}
