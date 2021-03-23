#include "math/Math.h"

namespace sem = se::math;

const float* se::math::Vector3::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&x);
}

auto se::math::Vector3::setVector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

auto se::math::Vector3::lengthSq() const
{
	return (x * x + y * y + z * z);
}

auto se::math::Vector3::getLength() const
{
	return (std::sqrt(lengthSq()));
}

auto se::math::Vector3::normalize()
{
	float length = getLength();
	x /= length;
	y /= length;
	z /= length;
}

auto se::math::Vector3::normalize(const Vector3& vec)
{
	Vector3 temp = vec;
	temp.normalize();
	return temp;
}

auto se::math::Vector3::dotProduct(const Vector3& a, const Vector3& b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

auto se::math::Vector3::crossProduct(const Vector3& a, const Vector3& b)
{
	Vector3 temp;
	temp.x = a.y * b.z - a.z * b.y;
	temp.y = a.z * b.x - a.x * b.z;
	temp.z = a.x * b.y - a.y * b.x;
	return temp;
}

auto se::math::Vector3::lerp(const Vector3& a, const Vector3& b, float f)
{
	return Vector3(a + f * (b - a));
}

auto se::math::Vector3::reflect(const Vector3& v, const Vector3& n)
{
	return v - 2.0f * Vector3::dotProduct(v, n) * n;
}

// Quaternion =============================================================================

auto sem::Quaternion::set(float x, float y, float z, float w)->void
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

auto sem::Quaternion::conjugate() -> void
{
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

auto sem::Quaternion::getLengthSq() const -> float
{
	return (x * x + y * y + z * z + w * w);
}

auto sem::Quaternion::getLength() const-> float
{
	return std::sqrtf(getLengthSq());
}

auto sem::Quaternion::normalize()->void
{
	float length = getLength();
	x /= length;
	y /= length;
	z /= length;
	w /= length;
}

auto sem::Quaternion::normalize(const Quaternion& q)->Quaternion
{
	Quaternion retVal = q;
	retVal.normalize();
	return retVal;
}

// Linear interpolation
auto sem::Quaternion::lerp(const Quaternion& a, const Quaternion& b, float f)->Quaternion
{
	Quaternion retVal;
	retVal.x = sem::lerp(a.x, b.x, f);
	retVal.y = sem::lerp(a.y, b.y, f);
	retVal.z = sem::lerp(a.z, b.z, f);
	retVal.w = sem::lerp(a.w, b.w, f);
	retVal.normalize();
	return retVal;
}

static auto dotProduct(const sem::Quaternion& a, const sem::Quaternion& b)->float
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// Spherical Linear Interpolation
auto sem::Quaternion::slerp(const Quaternion& a, const Quaternion& b, float f)->Quaternion
{
	float rawCosm = Quaternion::dotProduct(a, b);

	float cosom = -rawCosm;
	if (rawCosm >= 0.0f)
	{
		cosom = rawCosm;
	}

	float scale0, scale1;

	if (cosom < 0.9999f)
	{
		const float omega = std::acosf(cosom);
		const float invSin = 1.f / std::sinf(omega);
		scale0 = std::sinf((1.f - f) * omega) * invSin;
		scale1 = std::sinf(f * omega) * invSin;
	}
	else
	{
		scale0 = 1.0f - f;
		scale1 = f;
	}

	if (rawCosm < 0.0f)
	{
		scale1 = -scale1;
	}

	Quaternion retVal;
	retVal.x = scale0 * a.x + scale1 * b.x;
	retVal.y = scale0 * a.y + scale1 * b.y;
	retVal.z = scale0 * a.z + scale1 * b.z;
	retVal.w = scale0 * a.w + scale1 * b.w;
	retVal.normalize();
	return retVal;
}

auto sem::Quaternion::concatenate(const Quaternion& q, const Quaternion& p)->Quaternion
{
	Quaternion retVal;

	Vector3 qv(q.x, q.y, q.z);
	Vector3 pv(p.x, p.y, p.z);
	Vector3 newVec = p.w * qv + q.w * pv + Vector3::crossProduct(pv, qv);
	retVal.x = newVec.x;
	retVal.y = newVec.y;
	retVal.z = newVec.z;

	retVal.w = p.w * q.w - Vector3::dotProduct(pv, qv);

	return retVal;
}

const sem::Quaternion sem::Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

//Matrix 4x4 ===================================================================================================

const float* sem::Matrix4::getAsFloatPtr() const
{
	return reinterpret_cast<const float*>(&mat[0][0]);
}

sem::Matrix4 se::math::operator*(const Matrix4& a, const Matrix4& b)
{
	Matrix4 retVal;
	// row 0
	retVal.mat[0][0] =
		a.mat[0][0] * b.mat[0][0] +
		a.mat[0][1] * b.mat[1][0] +
		a.mat[0][2] * b.mat[2][0] +
		a.mat[0][3] * b.mat[3][0];

	retVal.mat[0][1] =
		a.mat[0][0] * b.mat[0][1] +
		a.mat[0][1] * b.mat[1][1] +
		a.mat[0][2] * b.mat[2][1] +
		a.mat[0][3] * b.mat[3][1];

	retVal.mat[0][2] =
		a.mat[0][0] * b.mat[0][2] +
		a.mat[0][1] * b.mat[1][2] +
		a.mat[0][2] * b.mat[2][2] +
		a.mat[0][3] * b.mat[3][2];

	retVal.mat[0][3] =
		a.mat[0][0] * b.mat[0][3] +
		a.mat[0][1] * b.mat[1][3] +
		a.mat[0][2] * b.mat[2][3] +
		a.mat[0][3] * b.mat[3][3];

	// row 1
	retVal.mat[1][0] =
		a.mat[1][0] * b.mat[0][0] +
		a.mat[1][1] * b.mat[1][0] +
		a.mat[1][2] * b.mat[2][0] +
		a.mat[1][3] * b.mat[3][0];

	retVal.mat[1][1] =
		a.mat[1][0] * b.mat[0][1] +
		a.mat[1][1] * b.mat[1][1] +
		a.mat[1][2] * b.mat[2][1] +
		a.mat[1][3] * b.mat[3][1];

	retVal.mat[1][2] =
		a.mat[1][0] * b.mat[0][2] +
		a.mat[1][1] * b.mat[1][2] +
		a.mat[1][2] * b.mat[2][2] +
		a.mat[1][3] * b.mat[3][2];

	retVal.mat[1][3] =
		a.mat[1][0] * b.mat[0][3] +
		a.mat[1][1] * b.mat[1][3] +
		a.mat[1][2] * b.mat[2][3] +
		a.mat[1][3] * b.mat[3][3];

	// row 2
	retVal.mat[2][0] =
		a.mat[2][0] * b.mat[0][0] +
		a.mat[2][1] * b.mat[1][0] +
		a.mat[2][2] * b.mat[2][0] +
		a.mat[2][3] * b.mat[3][0];

	retVal.mat[2][1] =
		a.mat[2][0] * b.mat[0][1] +
		a.mat[2][1] * b.mat[1][1] +
		a.mat[2][2] * b.mat[2][1] +
		a.mat[2][3] * b.mat[3][1];

	retVal.mat[2][2] =
		a.mat[2][0] * b.mat[0][2] +
		a.mat[2][1] * b.mat[1][2] +
		a.mat[2][2] * b.mat[2][2] +
		a.mat[2][3] * b.mat[3][2];

	retVal.mat[2][3] =
		a.mat[2][0] * b.mat[0][3] +
		a.mat[2][1] * b.mat[1][3] +
		a.mat[2][2] * b.mat[2][3] +
		a.mat[2][3] * b.mat[3][3];

	// row 3
	retVal.mat[3][0] =
		a.mat[3][0] * b.mat[0][0] +
		a.mat[3][1] * b.mat[1][0] +
		a.mat[3][2] * b.mat[2][0] +
		a.mat[3][3] * b.mat[3][0];

	retVal.mat[3][1] =
		a.mat[3][0] * b.mat[0][1] +
		a.mat[3][1] * b.mat[1][1] +
		a.mat[3][2] * b.mat[2][1] +
		a.mat[3][3] * b.mat[3][1];

	retVal.mat[3][2] =
		a.mat[3][0] * b.mat[0][2] +
		a.mat[3][1] * b.mat[1][2] +
		a.mat[3][2] * b.mat[2][2] +
		a.mat[3][3] * b.mat[3][2];

	retVal.mat[3][3] =
		a.mat[3][0] * b.mat[0][3] +
		a.mat[3][1] * b.mat[1][3] +
		a.mat[3][2] * b.mat[2][3] +
		a.mat[3][3] * b.mat[3][3];

	return retVal;
}

auto sem::Matrix4::invert()->void
{
	float tmp[12];
	float src[16];
	float dst[16];
	float det;

	src[0] = mat[0][0];
	src[4] = mat[0][1];
	src[8] = mat[0][2];
	src[12] = mat[0][3];

	src[1] = mat[1][0];
	src[5] = mat[1][1];
	src[9] = mat[1][2];
	src[13] = mat[1][3];

	src[2] = mat[2][0];
	src[6] = mat[2][1];
	src[10] = mat[2][2];
	src[14] = mat[2][3];

	src[3] = mat[3][0];
	src[7] = mat[3][1];
	src[11] = mat[3][2];
	src[15] = mat[3][3];

	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];

	dst[0] = tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7];
	dst[0] -= tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7];
	dst[1] = tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7];
	dst[1] -= tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7];
	dst[2] = tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7];
	dst[2] -= tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7];
	dst[3] = tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6];
	dst[3] -= tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6];
	dst[4] = tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3];
	dst[4] -= tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3];
	dst[5] = tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3];
	dst[5] -= tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3];
	dst[6] = tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3];
	dst[6] -= tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3];
	dst[7] = tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2];
	dst[7] -= tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2];

	tmp[0] = src[2] * src[7];
	tmp[1] = src[3] * src[6];
	tmp[2] = src[1] * src[7];
	tmp[3] = src[3] * src[5];
	tmp[4] = src[1] * src[6];
	tmp[5] = src[2] * src[5];
	tmp[6] = src[0] * src[7];
	tmp[7] = src[3] * src[4];
	tmp[8] = src[0] * src[6];
	tmp[9] = src[2] * src[4];
	tmp[10] = src[0] * src[5];
	tmp[11] = src[1] * src[4];

	dst[8] = tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15];
	dst[8] -= tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15];
	dst[9] = tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15];
	dst[9] -= tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15];
	dst[10] = tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15];
	dst[10] -= tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15];
	dst[11] = tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14];
	dst[11] -= tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14];
	dst[12] = tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9];
	dst[12] -= tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10];
	dst[13] = tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10];
	dst[13] -= tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8];
	dst[14] = tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8];
	dst[14] -= tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9];
	dst[15] = tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9];
	dst[15] -= tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8];

	det = src[0] * dst[0] + src[1] * dst[1] + src[2] * dst[2] + src[3] * dst[3];

	det = 1 / det;
	for (int j = 0; j < 16; j++)
	{
		dst[j] *= det;
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			mat[i][j] = dst[i * 4 + j];
		}
	}
}

// Get the translation component of the matrix
sem::Vector3 sem::Matrix4::getTranslation() const
{
	return Vector3(mat[3][0], mat[3][1], mat[3][2]);
}

// Get the X axis of the matrix (forward)
sem::Vector3 sem::Matrix4::getXAxis() const
{
	return Vector3::normalize(Vector3(mat[0][0], mat[0][1], mat[0][2]));
}

// Get the Y axis of the matrix (left)
sem::Vector3 sem::Matrix4::getYAxis() const
{
	return Vector3::normalize(Vector3(mat[1][0], mat[1][1], mat[1][2]));
}

// Get the Z axis of the matrix (up)
sem::Vector3 sem::Matrix4::getZAxis() const
{
	return Vector3::normalize(Vector3(mat[2][0], mat[2][1], mat[2][2]));
}

// Extract the scale component from the matrix
sem::Vector3 sem::Matrix4::getScale() const
{
	Vector3 retVal;
	retVal.x = Vector3(mat[0][0], mat[0][1], mat[0][2]).getLength();
	retVal.y = Vector3(mat[1][0], mat[1][1], mat[1][2]).getLength();
	retVal.z = Vector3(mat[2][0], mat[2][1], mat[2][2]).getLength();
	return retVal;
}

// Create a scale matrix with x, y, and z scales
sem::Matrix4 sem::Matrix4::createScale(float xScale, float yScale, float zScale)
{
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, zScale, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};
	return Matrix4(temp);
}

sem::Matrix4 sem::Matrix4::createScale(const Vector3& scaleVector)
{
	return createScale(scaleVector.x, scaleVector.y, scaleVector.z);
}

// Create a scale matrix with a uniform factor
sem::Matrix4 sem::Matrix4::createScale(float scale)
{
	return createScale(scale, scale, scale);
}

// Rotation about x-axis
sem::Matrix4 sem::Matrix4::createRotationX(float theta)
{
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f , 0.0f },
		{ 0.0f, std::cosf(theta), std::sinf(theta), 0.0f },
		{ 0.0f, -std::sinf(theta), std::cosf(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4(temp);
}

// Rotation about y-axis
sem::Matrix4 sem::Matrix4::createRotationY(float theta)
{
	float temp[4][4] =
	{
		{ std::cosf(theta), 0.0f, -std::sinf(theta), 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ std::sinf(theta), 0.0f, std::cosf(theta), 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4(temp);
}

// Rotation about z-axis
sem::Matrix4 sem::Matrix4::createRotationZ(float theta)
{
	float temp[4][4] =
	{
		{ std::cosf(theta), std::sinf(theta), 0.0f, 0.0f },
		{ -std::sinf(theta), std::cosf(theta), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
	};
	return Matrix4(temp);
}

// Create a rotation matrix from a quaternion
sem::Matrix4 sem::Matrix4::createFromQuaternion(const class Quaternion& q)
{
	float mat[4][4];

	mat[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	mat[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	mat[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
	mat[0][3] = 0.0f;

	mat[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	mat[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	mat[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
	mat[1][3] = 0.0f;

	mat[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	mat[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	mat[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
	mat[2][3] = 0.0f;

	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;

	return Matrix4(mat);
}

sem::Matrix4 sem::Matrix4::createTranslation(const Vector3& trans)
{
	float temp[4][4] =
	{
		{ 1.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f }
	};
	return Matrix4(temp);
}

sem::Matrix4 sem::Matrix4::createLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = Vector3::normalize(target - eye);
	Vector3 xaxis = Vector3::normalize(Vector3::crossProduct(up, zaxis));
	Vector3 yaxis = Vector3::normalize(Vector3::crossProduct(zaxis, xaxis));
	Vector3 trans;
	trans.x = -Vector3::dotProduct(xaxis, eye);
	trans.y = -Vector3::dotProduct(yaxis, eye);
	trans.z = -Vector3::dotProduct(zaxis, eye);

	float temp[4][4] =
	{
		{ xaxis.x, yaxis.x, zaxis.x, 0.0f },
		{ xaxis.y, yaxis.y, zaxis.y, 0.0f },
		{ xaxis.z, yaxis.z, zaxis.z, 0.0f },
		{ trans.x, trans.y, trans.z, 1.0f }
	};
	return Matrix4(temp);
}

sem::Matrix4 sem::Matrix4::createOrtho(float width, float height, float near, float far)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f / (far - near), 0.0f },
		{ 0.0f, 0.0f, near / (near - far), 1.0f }
	};
	return Matrix4(temp);
}

sem::Matrix4 sem::Matrix4::createPerspectiveFOV(float fovY, float width, float height, float near, float far)
{
	float yScale = (1.0f / std::tanf(fovY / 2.0f));
	float xScale = yScale * height / width;
	float temp[4][4] =
	{
		{ xScale, 0.0f, 0.0f, 0.0f },
		{ 0.0f, yScale, 0.0f, 0.0f },
		{ 0.0f, 0.0f, far / (far - near), 1.0f },
		{ 0.0f, 0.0f, -near * far / (far - near), 0.0f }
	};
	return Matrix4(temp);
}

// Create "Simple" View-Projection Matrix from Chapter 6
sem::Matrix4 sem::Matrix4::createSimpleViewProj(float width, float height)
{
	float temp[4][4] =
	{
		{ 2.0f / width, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / height, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 1.0f }
	};
	return Matrix4(temp);
}

const sem::Matrix4 identity;