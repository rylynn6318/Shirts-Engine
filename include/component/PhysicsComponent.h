#pragma once
#include "Component.h"
#include "math/Math.h"

namespace sem = se::math;

namespace se
{
	struct PhysicsComponent// : Component<PhysicsComponent>
	{
		float mass;
		sem::Vector3 velocity;
		sem::Vector3 impulse;
		float friction;
	};
}