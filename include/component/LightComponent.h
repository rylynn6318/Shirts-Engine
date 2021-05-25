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

	struct LightProperty
	{
		float intensity; 
		sem::Vector3 color;
		bool castShadow;
		bool affectWorld;
		LightMobility lightMobility = LightMobility::STATIC;
	};

	struct DirectionalLightComponent final : Component<DirectionalLightComponent>
	{
		LightProperty lightProperty;
		sem::Vector3 directionColor;
		sem::Vector3 diffuseColor;
		sem::Vector3 specularColor;
	};

	struct SpotLightComponent final : Component<SpotLightComponent>
	{
		LightProperty lightProperty;
		float coneAngle;
		float attenuationRadius;
	};

	struct PointLightComponent final : Component<PointLightComponent>
	{
		LightProperty lightProperty;
		float attenuationRadius;
	};
}