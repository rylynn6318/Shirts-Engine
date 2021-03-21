#pragma once
#include <cmath>

namespace se::math
{
	namespace sem = se::math;
	//linear interporation
	inline float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}
	const float PI = 3.1415926535f;
	const float fINFINITY = std::numeric_limits<float>::infinity();

	class Vector3
	{
	public:
		float x;
		float y;
		float z;

		Vector3() : x(x), y(y), z(z) {}
		explicit Vector3(float x, float y, float z) {}

		// float pointer로 캐스팅
		const float* getAsFloatPtr() const;

		// setter
		auto setVector(float x, float y, float z);

		friend Vector3 operator+(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		friend Vector3 operator-(const Vector3& a, const Vector3& b)
		{
			return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		friend Vector3 operator*(const Vector3& left, const Vector3& right)
		{
			return Vector3(left.x * right.x, left.y * right.y, left.z * right.z);
		}

		friend Vector3 operator*(const Vector3& vec, float scalar)
		{
			return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
		}

		friend Vector3 operator*(float scalar, const Vector3& vec)
		{
			return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
		}

		Vector3& operator*=(float scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		Vector3& operator+=(const Vector3& right)
		{
			x += right.x;
			y += right.y;
			z += right.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& right)
		{
			x -= right.x;
			y -= right.y;
			z -= right.z;
			return *this;
		}

		auto lengthSq() const;
		auto getLength() const;
		auto normalize();
		static auto normalize(const Vector3& vec);
		static auto dotProduct(const Vector3& a, const Vector3& b);
		static auto crossProduct(const Vector3& a, const Vector3& b);
		static auto lerp(const Vector3& a, const Vector3& b, float f);
		static auto reflect(const Vector3& v, const Vector3& n);
		static auto transform(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
		static auto transformWithPerspDiv(const Vector3& vec, const class Matrix4& mat, float w = 1.0f);
		static auto transform(const Vector3& v, const class Quaternion& q);

		static const Vector3 zero;
		static const Vector3 unitX;
		static const Vector3 unitY;
		static const Vector3 unitZ;
		static const Vector3 infinity;
	};

	//Quaternion
	class Quaternion
	{
	public:
		float x;
		float y;
		float z;
		float w;

		Quaternion()
		{
			*this = Quaternion::identity;
		}

		explicit Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		explicit Quaternion(const Vector3& axis, float angle)
		{
			float scalar = std::sinf(angle / 2.0f);
			x = axis.x * scalar;
			y = axis.y * scalar;
			z = axis.z * scalar;
			w = std::cosf(angle / 2.0f);
		}

		auto set(float x, float y, float z, float w);
		auto conjugate();
		auto getLengthSq() const;
		auto getLength() const;
		auto normalize();
		static auto normalize(const Quaternion& q);
		// Linear interpolation
		static auto lerp(const Quaternion& a, const Quaternion& b, float f);
		static float dotProduct(const Quaternion& a, const Quaternion& b);
		// Spherical Linear Interpolation
		static auto slerp(const Quaternion& a, const Quaternion& b, float f);
		static auto concatenate(const Quaternion& q, const Quaternion& p);

		static const Quaternion identity;
	};

	//Matrix 4x4
	class Matrix4
	{
	public:
		float mat[4][4];

		Matrix4()
		{
			*this = Matrix4::identity;
		}

		explicit Matrix4(float mat[4][4])
		{
			std::memmove(this->mat, mat, 16 * sizeof(float));
		}

		// Cast to a const float pointer
		const float* getAsFloatPtr() const;
		friend Matrix4 operator*(const Matrix4& a, const Matrix4& b);
		Matrix4& operator*=(const Matrix4& right);
		auto Invert();

		Vector3 getTranslation() const;
		Vector3 getXAxis() const;
		Vector3 getYAxis() const;
		Vector3 getZAxis() const;
		Vector3 getScale() const;

		static Matrix4 createScale(float xScale, float yScale, float zScale);
		static Matrix4 createScale(const Vector3& scaleVector);
		static Matrix4 createScale(float scale);
		static Matrix4 createRotationX(float theta);
		static Matrix4 createRotationY(float theta);
		static Matrix4 createRotationZ(float theta);

		static Matrix4 createFromQuaternion(const class Quaternion& q);
		static Matrix4 createTranslation(const Vector3& trans);
		static Matrix4 createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
		static Matrix4 createOrtho(float width, float height, float near, float far);
		static Matrix4 createPerspectiveFOV(float fovY, float width, float height, float near, float far);
		static Matrix4 createSimpleViewProj(float width, float height);

		static const Matrix4 identity;
	};
}
