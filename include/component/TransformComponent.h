#include "Component.h"

namespace se
{
	struct Position
	{
		Position() :x(0), y(0), z(0) {}
		float x, y, z;
	};
	struct Rotation
	{
		Rotation() :yaw(0), pitch(0), roll(0) {}
		float yaw, pitch, roll;
	};
	struct Scale
	{
		Scale() :x(1), y(1), z(1) {}
		float x, y, z;
	};

	struct TransformComponent : Component<TransformComponent>
	{
		Position position;
		Rotation rotation;
		Scale scale;
	};
}