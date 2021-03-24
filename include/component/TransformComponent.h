#pragma once
#include "Component.h"
#include "math/Math.h"

namespace sem = se::math;
namespace se
{
	struct TransformComponent : Component<TransformComponent>
	{
		sem::Vector3 position;
		sem::Quaternion rotation;
		float scale;
	};
}