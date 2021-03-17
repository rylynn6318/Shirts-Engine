#pragma once
#include "Component.h
#include "math/Math.h"

namespace se
{
	struct MovementComponent : Component<MovementComponent>
	{
		float angularSpeed;
		float forwardSpeed;
	};
}