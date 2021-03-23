#pragma once
#include "component/Component.h"
#include "math/Math.h"

namespace sem = se::math;

namespace se
{
	enum class LightMobility
	{
		STATIC,
		MOVABLE
	};

	struct LightComponent //: Component<LightComponent>
	{
		float intensity; 
		sem::Vector3 color;
		bool castShadow;
		bool affectWorld;
		LightMobility lightMobility = LightMobility::STATIC;
	};

	struct DirectionalLightComponent : LightComponent
	{
		sem::Vector3 directionColor;
		sem::Vector3 diffuseColor;
		sem::Vector3 specularColor;
	};

	struct SpotLightComponent : LightComponent
	{
		float coneAngle;
		float attenuationRadius;
	};

	struct PointLightComponent : LightComponent
	{
		float attenuationRadius;
	};
}