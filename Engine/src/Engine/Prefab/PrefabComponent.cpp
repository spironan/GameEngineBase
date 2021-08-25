#include "pch.h"

#include "PrefabComponent.h"
#include "Engine/ECS/GameObject.h"
namespace engine
{
PrefabComponent::PrefabComponent(Entity entity, bool active)
	: Component{ entity, active }
{
	static_cast<GameObject>(entity).ActiveSelf() = false;
}




}
