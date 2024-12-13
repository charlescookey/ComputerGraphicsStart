#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <memory>
#include <iostream>

#include "Vec2.h"

#define SQ(X) (X)*(X)

#define M_PI       3.14159265358979323846

template<class T>
const T& maxx(const T& a, const T& b)
{
	return (a < b) ? b : a;
}

template<class T>
const T& minn(const T& a, const T& b)
{
	return (a < b) ? a : b;
}


template<typename T>
static T lerp(const T a, const T b, float t)
{
	return a * (1.0f - t) + (b * t);
}

class Vec3
{
public:
	union
	{
		float vec[3];
		struct { float x, y, z; };
	};

	Vec3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Vec3(const Vec3& copy) {
		x = copy.x;
		y = copy.y;
		z = copy.z;
	}

	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {
	}

	Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
	Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
	Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }	

	Vec3& operator+=(const Vec3& v) {x += v.x; y += v.y; z += v.z; return *this;}
	Vec3& operator-=(const Vec3& v) {x -= v.x; y -= v.y; z -= v.z; return *this;}
	Vec3& operator*=(const Vec3& v) {x *= v.x; y *= v.y; z *= v.z; return *this;}
	Vec3& operator/=(const Vec3& v) {x /= v.x; y /= v.y; z /= v.z; return *this;}

	Vec3 operator*(const float v) const { return Vec3(x * v, y * v, z * v); }
	Vec3 operator/(const float v) const { float iv = 1.f / v; return Vec3(x * iv, y * iv, z * iv);}//???????

	Vec3 operator*=(const float v) { x *= v; y *= v; z *= v; return *this; }
	Vec3 operator/=(const float v) { float iv = 1.f / v; x *= iv; y *= iv; z *= iv; return *this; }

	float lenght() const{
		return sqrtf(SQ(x) + SQ(y) + SQ(z));
	}

	float lenghtSquare() {
		return SQ(x) + SQ(y) + SQ(z);
	}

	Vec3 normalize(void)
	{
		float len = 1.0f / lenght();
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength()
	{
		float length = lenght();
		float len = 1.0f / length;
		x *= len; y *= len; z *= len;
		return length;
	}

	float Dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

	Vec3 Cross(const Vec3& v1) { return Vec3(v1.y * z - v1.z * y, v1.z * x - v1.x * z, v1.x * y - v1.y * x); }

	float Max() const { return maxx(x, maxx(y, z)); }
	
	float Min() const { return minn(x, minn(y, z)); }

	void print() { std::cout << "X: " << x << " Y: " << y << " Z: " << z << "\n";}
};

float Dot(const Vec3& v1, const Vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
Vec3 Cross(const Vec3& v1, const Vec3& v2) { return Vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x); }
Vec3 Max(const Vec3& v1, const Vec3& v2) { return Vec3(maxx(v1.x, v2.x), maxx(v1.y, v2.y), maxx(v1.z, v2.z)); }
Vec3 Min(const Vec3& v1, const Vec3& v2) { return Vec3(minn(v1.x, v2.x), minn(v1.y, v2.y), minn(v1.z, v2.z)); }


class Vec4
{
public:
	union
	{
		float vec[4];
		struct { float x, y, z , w; };
	};

	Vec4() {
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	
	Vec4(const Vec3& v, float _w) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = _w;
	}

	Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {
	}

	Vec4 operator+(const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);}
	Vec4 operator-(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vec4 operator*(const Vec4& v) const { return Vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vec4 operator/(const Vec4& v) const { return Vec4(x / v.x, y / v.y, z / v.z, w / v.w); }

	Vec4& operator+=(const Vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	Vec4& operator-=(const Vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	Vec4& operator*=(const Vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	Vec4& operator/=(const Vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

	Vec4 operator*(const float v) const { return Vec4(x * v, y * v, z * v, w * v); }
	Vec4 operator/(const float v) const { float iv = 1.f / v; return Vec4(x * iv, y * iv, z * iv, w * iv); }//???????

	Vec4 operator*=(const float v) { x *= v; y *= v; z *= v; w *= v;  return* this; }
	Vec4 operator/=(const float v) { float iv = 1.f / v; x *= iv; y *= iv; z *= iv; w *= iv; return *this; }


	float lenght() {
		return sqrtf(SQ(x) + SQ(y) + SQ(z) + SQ(w));
	}

	float lenghtSquare() {
		return SQ(x) + SQ(y) + SQ(z) + SQ(w);
	}

	Vec4 normalize(void)
	{
		float len = 1.0f / lenght();
		return Vec4(x * len, y * len, z * len, w * len);
	}

	float normalize_GetLength()
	{
		float length = lenght();
		float len = 1.0f / length;
		x *= len; y *= len; z *= len; w *= len; 
		return length;
	}

	void print() { std::cout << "X: " << x << " Y: " << y << " Z: " << z << " W: " << w << "\n"; }
	
	Vec4 divivdeByW() {
		float iw = 1.f / w;
		return Vec4(x * iw, y * iw, z * iw, 1 * iw);
	}

	Vec3 ToCartesian() const {
		if (w != 0.0f) {
			return Vec3(x / w, y / w, z / w);
		}
		else {
			return Vec3(x, y, z);
		}
	}
};





class Matrix
{
public:
	union
	{
		float a[4][4];
		float m[16];
	};

	void identity() {
		memset(m, 0, 16 * sizeof(float));
		a[0][0] = 1.0f;
		a[1][1] = 1.0f;
		a[2][2] = 1.0f;
		a[3][3] = 1.0f;
	}

	void zeros() {
		memset(m, 0, 16 * sizeof(float));
	}

	Matrix() {
		identity();
	}

	Matrix(float* otherMatrix) {
		memcpy(&m, otherMatrix, 16 * sizeof(float));
	}

	Vec3 mulPoint(Vec3& v) const
	{
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		float w;
		w = (m[12] * v.x) + (m[13] * v.y) + (m[14] * v.z) + m[15];
		w = 1.0f / w;
		return (v1 * w);
	}
	
	Vec4 mulPoint( Vec4& v) const
	{
		Vec4 v1 = Vec4(
			(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11]),
			(v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15])
		);
		return v1 ;
	}

	Vec3 mulPointNoProj(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	static Matrix translation(const Vec3& v) {
		Matrix mat;
		mat.a[0][3] = v.x;
		mat.a[1][3] = v.y;
		mat.a[2][3] = v.z;
		return mat;
	}

	static Matrix scaling(const Vec3& v) {
		Matrix mat;
		mat.a[0][0] = v.x;
		mat.a[1][1] = v.y;
		mat.a[2][2] = v.z;
		return mat;
	}

	static Matrix rotateX(float theta) {
		Matrix mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.a[1][1] = ct;
		mat.a[2][2] = ct;
		mat.a[2][1] = st;
		mat.a[1][2] = -st;
		return mat;
	}
	static Matrix rotateY(float theta) {
		Matrix mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.a[0][0] = ct;
		mat.a[2][2] = ct;
		mat.a[2][0] = -st;
		mat.a[0][2] = st;
		return mat;
	}
	
	static Matrix rotateZ(float theta) {
		Matrix mat;
		float ct = cosf(theta);
		float st = sinf(theta);
		mat.a[0][0] = ct;
		mat.a[1][1] = ct;
		mat.a[1][0] = st;
		mat.a[0][1] = -st;
		return mat;
	}

	static Matrix ProjectionMatrix(float _fov , float aspectRatio , float zfar , float znear) {
		float fov = _fov * M_PI / 180;
		Matrix mat;
		mat.a[0][0] = 1 / (aspectRatio * tan(fov/2));
		mat.a[1][1] = 1 /  tan(fov / 2);
		mat.a[2][2] = ( - zfar) / (zfar - znear);
		mat.a[2][3] = -zfar * znear / (zfar - znear);
		mat.a[3][2] = -1;
		mat.a[3][3] = 0;
		return mat;
	}

	static Matrix LookAtMatrix(Vec3 from, Vec3 to, Vec3 up) {
		Vec3 dir = from - to;
		dir.normalize();

		Vec3 right = up.Cross(dir);
		Vec3 _up = dir.Cross(right);

		Matrix mat;
		mat.a[0][0] = right.x;
		mat.a[0][1] = right.y;
		mat.a[0][2] = right.z;
		mat.a[0][3] = 0;

		mat.a[1][0] = _up.x;
		mat.a[1][1] = _up.y;
		mat.a[1][2] = _up.z;
		mat.a[1][3] = 0;

		mat.a[2][0] = dir.x;
		mat.a[2][1] = dir.y;
		mat.a[2][2] = dir.z;
		mat.a[2][3] = 0;

		return mat;
	}

	static Matrix LookAtMatrixRot(Vec3 from, Vec3 to, Vec3 up) {
		Vec3 dir = from - to;
		dir = dir.normalize();

		//Vec3 right = Cross(up ,dir).normalize();
		Vec3 right = Cross(up, dir);
		right = right.normalize();
		Vec3 _up = Cross(dir ,right);
;

		Matrix mat;
		mat.a[0][0] = right.x;
		mat.a[0][1] = right.y;
		mat.a[0][2] = right.z;

		mat.a[1][0] = _up.x;
		mat.a[1][1] = _up.y;
		mat.a[1][2] = _up.z;

		mat.a[2][0] = dir.x;
		mat.a[2][1] = dir.y;
		mat.a[2][2] = dir.z;

		Vec3 _from = from * -1;

		//mat.a[0][3] = ((from*-1).Dot(right));
		mat.a[0][3] = Dot(_from, right);
		//mat.a[1][3] = ((from*-1).Dot(_up));
		mat.a[1][3] = Dot(_from, _up);
		//mat.a[2][3] = ((from*-1).Dot(dir));
		mat.a[2][3] = Dot(_from, dir);

		return mat;
	}

	float& operator[](int index) {
		return m[index];
	}

	Matrix invert()
	{
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
		inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
		inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
		inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
		inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
		inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
		inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
		inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
		inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
		inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
		inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
		inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
		inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
		inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
		inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
		inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];
		float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
			// Handle this case
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}


	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[4] * matrix.m[1] + m[8] * matrix.m[2] + m[12] * matrix.m[3];
		ret.m[1] = m[1] * matrix.m[0] + m[5] * matrix.m[1] + m[9] * matrix.m[2] + m[13] * matrix.m[3];
		ret.m[2] = m[2] * matrix.m[0] + m[6] * matrix.m[1] + m[10] * matrix.m[2] + m[14] * matrix.m[3];
		ret.m[3] = m[3] * matrix.m[0] + m[7] * matrix.m[1] + m[11] * matrix.m[2] + m[15] * matrix.m[3];
		ret.m[4] = m[0] * matrix.m[4] + m[4] * matrix.m[5] + m[8] * matrix.m[6] + m[12] * matrix.m[7];
		ret.m[5] = m[1] * matrix.m[4] + m[5] * matrix.m[5] + m[9] * matrix.m[6] + m[13] * matrix.m[7];
		ret.m[6] = m[2] * matrix.m[4] + m[6] * matrix.m[5] + m[10] * matrix.m[6] + m[14] * matrix.m[7];
		ret.m[7] = m[3] * matrix.m[4] + m[7] * matrix.m[5] + m[11] * matrix.m[6] + m[15] * matrix.m[7];
		ret.m[8] = m[0] * matrix.m[8] + m[4] * matrix.m[9] + m[8] * matrix.m[10] + m[12] * matrix.m[11];
		ret.m[9] = m[1] * matrix.m[8] + m[5] * matrix.m[9] + m[9] * matrix.m[10] + m[13] * matrix.m[11];
		ret.m[10] = m[2] * matrix.m[8] + m[6] * matrix.m[9] + m[10] * matrix.m[10] + m[14] * matrix.m[11];
		ret.m[11] = m[3] * matrix.m[8] + m[7] * matrix.m[9] + m[11] * matrix.m[10] + m[15] * matrix.m[11];
		ret.m[12] = m[0] * matrix.m[12] + m[4] * matrix.m[13] + m[8] * matrix.m[14] + m[12] * matrix.m[15];
		ret.m[13] = m[1] * matrix.m[12] + m[5] * matrix.m[13] + m[9] * matrix.m[14] + m[13] * matrix.m[15];
		ret.m[14] = m[2] * matrix.m[12] + m[6] * matrix.m[13] + m[10] * matrix.m[14] + m[14] * matrix.m[15];
		ret.m[15] = m[3] * matrix.m[12] + m[7] * matrix.m[13] + m[11] * matrix.m[14] + m[15] * matrix.m[15];
		return ret;
	}

	Matrix mull(const Matrix& matrix) const 
	{
		Matrix ret;
		ret.zeros();
		int N = 4;

		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < N; ++j) {
				for (int k = 0; k < N; ++k) {
					ret.a[i][j] += matrix.a[i][k] * a[k][j];
				}
			}
		}

		return ret;

	}

	Matrix mul2(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
		ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
		ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
		ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
		ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
		ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
		ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
		ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
		ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
		ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
		ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
		ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
		ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
		ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
		ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
		ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
		return ret;
	}


	Matrix Transpose() const
	{
		Matrix ret;
		for (int i = 0; i < 4; ++i) {
			for (int j = i + 1; j < 4; ++j) {
				ret.a[i][j] = a[j][i];
			}
		}
		return ret;
	}

	Matrix operator*(const Matrix& matrix)
	{
		return mul2(matrix);
	}

	Vec3 getZDirection() {
		// The third column represents the Z direction in column-major order
		return Vec3(a[0][2], a[1][2], a[2][2]);
	}

	Vec3 getXDirection() {
		// The first column represents the X direction in column-major order
		return Vec3(a[0][0], a[1][0], a[2][0]);
	}
	
	Vec3 getYDirection() {
		// The first column represents the X direction in column-major order
		return Vec3(a[0][1], a[1][1], a[2][1]);
	}

	Vec3 getTranslation() {
		// The fourth column represents the translation in column-major order
		return Vec3(a[0][3], a[1][3], a[2][3]);
	}

	Vec3 getZDirection_c() {
		// The third row represents the Z direction in row-major order
		return Vec3(a[2][0], a[2][1], a[2][2]);
	}

	Vec3 getXDirection_c() {
		// The first row represents the X direction in row-major order
		return Vec3(a[0][0], a[0][1], a[0][2]);
	}

	Vec3 getTranslation_c() {
		// The fourth row represents the translation in row-major order
		return Vec3(a[3][0], a[3][1], a[3][2]);
	}
	
	void setTranslation(const Vec3& translation)
	{
		a[0][3] = translation.x;
		a[1][3] = translation.y;
		a[2][3] = translation.z;
	}

	static Matrix reflection(const Vec3& normal, float d) {
		Matrix reflectionMatrix;

		// Normalize the normal vector
		Vec3 n = normal;
		n = n.normalize();

		// Calculate coefficients for the reflection matrix
		float nx = n.x;
		float ny = n.y;
		float nz = n.z;

		reflectionMatrix.a[0][0] = 1.0f - 2.0f * nx * nx;
		reflectionMatrix.a[0][1] = -2.0f * nx * ny;
		reflectionMatrix.a[0][2] = -2.0f * nx * nz;
		reflectionMatrix.a[0][3] = -2.0f * nx * d;

		reflectionMatrix.a[1][0] = -2.0f * ny * nx;
		reflectionMatrix.a[1][1] = 1.0f - 2.0f * ny * ny;
		reflectionMatrix.a[1][2] = -2.0f * ny * nz;
		reflectionMatrix.a[1][3] = -2.0f * ny * d;

		reflectionMatrix.a[2][0] = -2.0f * nz * nx;
		reflectionMatrix.a[2][1] = -2.0f * nz * ny;
		reflectionMatrix.a[2][2] = 1.0f - 2.0f * nz * nz;
		reflectionMatrix.a[2][3] = -2.0f * nz * d;

		reflectionMatrix.a[3][0] = 0.0f;
		reflectionMatrix.a[3][1] = 0.0f;
		reflectionMatrix.a[3][2] = 0.0f;
		reflectionMatrix.a[3][3] = 1.0f;

		return reflectionMatrix;
	}
};


class Matrix3x3
{
public:
	union
	{
		float a[3][3];
		float m[9];
	};

	void identity() {
		memset(m, 0, 16 * sizeof(float));
		a[0][0] = 1.0f;
		a[1][1] = 1.0f;
		a[2][2] = 1.0f;
	}

	Matrix3x3() {
		identity();
	}

	Matrix3x3(float* otherMatrix) {
		memcpy(&m, otherMatrix, 9 * sizeof(float));
	}

	Vec3 mulVec(const Vec3& v)
	{
		return Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) ,
			(v.x * m[3] + v.y * m[4] + v.z * m[5]) ,
			(v.x * m[6] + v.y * m[7] + v.z * m[8]));
	}


	float& operator[](int index) {
		return m[index];
	}


	Matrix Transpose() const
	{
		Matrix ret;
		for (int i = 0; i < 3; ++i) {
			for (int j = i + 1; j < 3; ++j) {
				ret.a[i][j] = a[j][i];
			}
		}
		return ret;
	}
};

class SphericalCoordinates {
public:
	float theta;
	float rho;
	float r;

	SphericalCoordinates(float _theta, float _rho, float _r) : theta(_theta), rho(_rho), r(_r) {}

	void CalculateTheta_Zup(const Vec3& v ){
		theta = acos(v.z / r);
	}

	void CalculateRho_Zup(const Vec3& v) {
		rho = atan2(v.y, v.x);
	}
	
	Vec3 SphericalToCartesian_Zup() {
		return Vec3(r * sin(theta) * cos(rho), r * sin(theta) * sin(rho), r * cos(theta));
	}

	void CalculateTheta_Camera(const Vec3& v ){
		theta = acos(v.y / r);
	}

	void CalculateRho_Camera(const Vec3& v) {
		rho = atan2(v.z, v.x);
	}

	Vec3 SphericalToCartesian_Camera() {
		return Vec3(r * sin(theta) * cos(rho) , r * cos(rho) ,r * sin(theta) * sin(rho));
	}
};

class Colour {
public:
	float r;
	float g;
	float b;
	float a;

	Colour() {
		r = 0.f; 
		g = 0.f;
		b = 0.f;
		a = 0.f;
	};

	Colour(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {};

	Colour(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) {
		r = _r / 255.0f;
		g = _g / 255.0f;
		b = _b / 255.0f;
		a = _a / 255.0f;
	}

	Colour operator+(const Colour& c) const { return Colour(r + c.r, g + c.g, b + c.b, a + c.a); };
	Colour operator-(const Colour& c) const { return Colour(r - c.r, g - c.g, b - c.b, a - c.a); };
	Colour operator*(const float c) const { return Colour(r * c, g * c, b * c, a * c); };
	Colour operator/(const float c) const { return Colour(r / c, g / c, b / c, a / c); };


};

class Quaternions {
public:
	union {
		struct {
			float a;
			float b;
			float c;
			float d;
		};
		float q[4];
	};
	Quaternions() {
		a = 0.f;
		b = 0.f;
		c = 0.f;
		d = 0.f;
	}
	Quaternions(float _a, float _b, float _c, float _d) :a(_a), b(_b), c(_c), d(_d) {}

	Quaternions operator*(const Quaternions& v) const {
		return Quaternions(
			(a * v.a) - (b * v.b) - (c * v.c) - (d * v.d),
			(a * v.b) - (b * v.a) - (c * v.d) - (d * v.c),
			(a * v.c) - (b * v.d) - (c * v.a) - (d * v.b),
			(a * v.d) - (b * v.c) - (c * v.b) - (d * v.a));
	}

	Quaternions operator*(const float v) const { return Quaternions(a * v, b * v, c * v, d * v); }

	Quaternions operator+(const Quaternions& v) const { return Quaternions(a + v.a, b + v.b, c + v.c, d + v.d); }
	Quaternions operator-(const Quaternions& v) const { return Quaternions(a - v.a, b - v.b, c - v.c, d - v.d); }
	Quaternions operator-() const { return Quaternions{ -a, -b, -c, -d };}

	float magnitude() {
		return sqrtf(SQ(a) + SQ(b) + SQ(c) + SQ(d));
	}

	Quaternions normalize(void)
	{
		float len = 1.0f / magnitude();
		return Quaternions(a * len, b * len, c * len, d * len);
	}

	Quaternions conjugate(void)
	{
		return Quaternions(a, -b, -c, -d);
	}

	Quaternions inverse(void)
	{
		float len = 1.0f / magnitude();
		return Quaternions(a * len, -b * len, -c * len, -d * len);
	}

	Matrix3x3 _toMatrix() {
		Matrix3x3 mat;

		mat.a[0][0] = 1 - (2 * SQ(c)) - (2 * SQ(d));
		mat.a[0][1] = (2 * b * c) - (2 * a * d);
		mat.a[0][2] = (2 * b * d) + (2 * a * c);
		mat.a[1][0] = (2 * b * c) + (2 * a * d);
		mat.a[1][0] = (2 * b * c) + (2 * a * d);
		mat.a[1][1] = 1 - (2 * SQ(b)) - (2 * SQ(d));
		mat.a[1][2] = (2 * c * d) - (2 * a * d);
		mat.a[2][0] = (2 * b * d) - (2 * a * c);
		mat.a[2][1] = (2 * c * d) + (2 * a * b);
		mat.a[2][2] = 1 - (2 * SQ(b)) - (2 * SQ(c));

		return mat;
	}

	Matrix toMatrix__() {
		Matrix mat;

		mat.a[0][0] = 1 - (2 * SQ(c)) - (2 * SQ(d));
		mat.a[0][1] = (2 * b * c) - (2 * a * d);
		mat.a[0][2] = (2 * b * d) + (2 * a * c);
		mat.a[1][0] = (2 * b * c) + (2 * a * d);
		mat.a[1][1] = 1 - (2 * SQ(b)) - (2 * SQ(d));
		mat.a[1][2] = (2 * c * d) - (2 * a * b);
		mat.a[2][0] = (2 * b * d) - (2 * a * c);
		mat.a[2][1] = (2 * c * d) + (2 * a * b);
		mat.a[2][2] = 1 - (2 * SQ(b)) - (2 * SQ(c));

		mat.a[0][3] = 0;
		mat.a[1][3] = 0;
		mat.a[2][3] = 0;

		mat.a[3][0] = 0;
		mat.a[3][1] = 0;
		mat.a[3][2] = 0;
		mat.a[3][3] = 1;

		return mat;
	}
	
	Matrix toMatrix()
	{
		float xx = q[0] * q[0];
		float xy = q[0] * q[1];
		float xz = q[0] * q[2];
		float yy = q[1] * q[1];
		float zz = q[2] * q[2];
		float yz = q[1] * q[2];
		float wx = q[3] * q[0];
		float wy = q[3] * q[1];
		float wz = q[3] * q[2];
		Matrix matrix;
		matrix[0] = 1.0f - 2.0f * (yy + zz);
		matrix[1] = 2.0f * (xy - wz);
		matrix[2] = 2.0f * (xz + wy);
		matrix[3] = 0.0;
		matrix[4] = 2.0f * (xy + wz);
		matrix[5] = 1.0f - 2.0f * (xx + zz);
		matrix[6] = 2.0f * (yz - wx);
		matrix[7] = 0.0;
		matrix[8] = 2.0f * (xz - wy);
		matrix[9] = 2.0f * (yz + wx);
		matrix[10] = 1.0f - 2.0f * (xx + yy);
		matrix[11] = 0.0;
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1;
		return matrix;
	}

	float dot(const Quaternions& v) const { return (a * v.a) + (b * v.b) + (c * v.c) + (d * v.d); };

	Quaternions slerp(const Quaternions& v , float t) const { 

		float _dot = dot(v);

		float theta = acos(_dot);

		float s1 = sin(theta * (1 - t)) / sin(theta);
		float s2 = sin(theta * (t)) / sin(theta);
		return (*this * s1) + (v * s2);
	}
	static Quaternions _slerp(const Quaternions& u, const Quaternions& v, float t)  {

		float _dot = u.dot(v);

		float theta = acos(_dot);

		float s1 = sin(theta * (1 - t)) / sin(theta);
		float s2 = sin(theta * (t)) / sin(theta);
		return (u * s1) + (v * s2);
	}
	static Quaternions __slerp(const Quaternions& u, const Quaternions& v, float t) {
		float _dot = u.dot(v);

		Quaternions v2 = v;
		if (_dot < 0.0f) {
			_dot = -_dot;
			v2 = -v2;
		}

		float theta = acos(_dot);

		if (theta < 1e-5f) {
			return (u * (1.0f - t)) + (v2 * t);
		}

		float s1 = sin((1.0f - t) * theta) / sin(theta);
		float s2 = sin(t * theta) / sin(theta);

		return (u * s1) + (v2 * s2);
	}
	static Quaternions slerp(const Quaternions& q1, const Quaternions& q2, float t) {
		float cos;
		Quaternions q1_ = Quaternions(q1.a, q1.b, q1.c, q1.d);

		cos = q1.a * q2.a + q1.b * q2.b + q1.c * q2.c + q1.d * q2.d;
		if (cos < 0) {
			Quaternions q1_ = Quaternions(-q1.a, -q1.b, -q1.c, -q1.d);
			cos = fabs(cos);
		}

		if (cos > 0.95) {
			return Quaternions(
				(1 - t) * q1_.a + t * q2.a,
				(1 - t) * q1_.b + t * q2.b,
				(1 - t) * q1_.c + t * q2.c,
				(1 - t) * q1_.d + t * q2.d
			).normalize();
		}

		float theta = acos(cos);
		float sinTheta = sinf(theta);
		// sin((1-t)theta) * q1_ / sin theta  +  sin(t*theta) * q2 / sin(theta)
		float a_ = sinf((1 - t) * theta) * q1_.a / sinTheta + sinf(t * theta) * q2.a / sinTheta;
		float b_ = sinf((1 - t) * theta) * q1_.b / sinTheta + sinf(t * theta) * q2.b / sinTheta;
		float c_ = sinf((1 - t) * theta) * q1_.c / sinTheta + sinf(t * theta) * q2.c / sinTheta;
		float d_ = sinf((1 - t) * theta) * q1_.d / sinTheta + sinf(t * theta) * q2.d / sinTheta;

		return Quaternions(a_, b_, c_, d_).normalize();
	}
};

Vec3 TransformToScreenSpace(const Vec3& vertex, const Matrix& projectionMatrix, const Matrix& lookAtMatrix, int screenWidth, int screenHeight) {
	Vec4 homogenousVertex(vertex.x, vertex.y, vertex.z, 1.0f);

	Vec4 transformed = lookAtMatrix.mulPoint(homogenousVertex); 
	transformed = projectionMatrix.mulPoint(transformed);

	Vec3 cartesian = transformed.ToCartesian();

	// Map to screen space
	int Vx = static_cast<int>((cartesian.x + 1.0f) * screenWidth / 2);
	int Vy = static_cast<int>((1.0f - cartesian.y) * screenHeight / 2);
	int Vz = static_cast<int>(cartesian.z);

	return Vec3(Vx, Vy, Vz);
};

Vec2 TransformToScreenSpace2(const Vec3& vertex, const Matrix projectionMatrix, int screenWidth, int screenHeight) {
	Vec4 homogenousVertex(vertex.x, vertex.y, vertex.z, 1.0f);

	Vec4 transformed = projectionMatrix.mulPoint(homogenousVertex);

	Vec3 cartesian = transformed.ToCartesian();

	// Map to screen space
	int Vx = static_cast<int>((cartesian.x + 1.0f) * screenWidth / 2);
	int Vy = static_cast<int>((1.0f - cartesian.y) * screenHeight / 2);
	int Vz = static_cast<int>(cartesian.z);

	return Vec2(Vx, Vy);
};

//copied from GameEngBase
class Timer
{
private:
	LARGE_INTEGER freq;   // Frequency of the performance counter
	LARGE_INTEGER start;  // Starting time

public:
	// Constructor that initializes the frequency
	Timer()
	{
		QueryPerformanceFrequency(&freq);
		reset();
	}

	// Resets the timer
	void reset()
	{
		QueryPerformanceCounter(&start);
	}

	// Returns the elapsed time since the last reset in seconds. Note this should only be called once per frame as it resets the timer.
	float dt()
	{
		LARGE_INTEGER cur;
		QueryPerformanceCounter(&cur);
		float value = static_cast<float>(cur.QuadPart - start.QuadPart) / freq.QuadPart;
		reset();
		return value;
	}
};

//Fix later
class ShadingFrame {
public:
	Vec3 tangent;    // u
	Vec3 bitangent;  // v
	Vec3 normal;     // z

	ShadingFrame(){
		tangent = Vec3(1, 0, 0);
		bitangent = Vec3(0, 1, 0);
		normal = Vec3(0, 0, 1);
	}

	ShadingFrame(Vec3& n) {
		fromNormal(n);
	}

	ShadingFrame(Vec3& n, Vec3& t) {
		fromNormalAndTangent(n, t);
	}

	void fromNormal(Vec3& n) {
		normal = n.normalize();

		if (std::abs(normal.x) > std::abs(normal.z)) {
			tangent = Vec3(-normal.y, normal.x, 0).normalize();
		}
		else {
			tangent = Vec3(0, -normal.z, normal.y).normalize();
		}

		bitangent = normal.Cross(tangent);
	}

	void fromNormalAndTangent(Vec3& n, Vec3& t) {
		normal = n.normalize();
		tangent = t.normalize();

		tangent = tangent - normal * normal.Dot(tangent);
		tangent = tangent.normalize();

		bitangent = normal.Cross(tangent);
	}

	Vec3 toWorldSpace(const Vec3& tangentSpaceVec) const {
		return tangent * tangentSpaceVec.x +
			bitangent * tangentSpaceVec.y +
			normal * tangentSpaceVec.z;
	}

	Vec3 toTangentSpace(const Vec3& worldSpaceVec) const {
		return Vec3(
			worldSpaceVec.Dot(tangent),
			worldSpaceVec.Dot(bitangent),
			worldSpaceVec.Dot(normal)
		);
	}
};