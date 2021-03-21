#pragma once
#include "Component.h"
#include "math/Math.h"

namespace sem = se::math;

namespace se
{
	//선분
	struct LineSegment
	{
		LineSegment(const sem::Vector3& start, const sem::Vector3& end);
		//선분상의 한점 반환 0 <= t <= 1
		sem::Vector3 pointOnSegment(float t) const;
		//return : 점과 최소거리
		auto minDistSq(const sem::Vector3& point) const;
		// Get MinDistSq between two line segments
		static auto minDistSq(const LineSegment& s1, const LineSegment& s2);

		sem::Vector3 start;
		sem::Vector3 end;
	};

	//평면
	struct Plane
	{
		Plane(const sem::Vector3& normal, float d);
		
		Plane(const sem::Vector3& a, const sem::Vector3& b, const sem::Vector3& c);
		auto signedDist(const sem::Vector3& point) const;

		sem::Vector3 normal;
		float d;
	};

	//구
	struct Sphere
	{
		Sphere(const sem::Vector3& center, float radius);
		bool contains(const sem::Vector3& point) const;

		sem::Vector3 center;
		float radius;
	};

	//AABB
	struct AABB
	{
		AABB(const sem::Vector3& min, const sem::Vector3& max);
		void updateMinMax(const sem::Vector3& point);
		void rotate(const sem::Quaternion& q);
		bool contains(const sem::Vector3& point) const;
		float minDistSq(const sem::Vector3& point) const;

		sem::Vector3 min;
		sem::Vector3 max;
	};

	struct CollisionComponent// : Component<CollisionComponent>
	{

	};
}