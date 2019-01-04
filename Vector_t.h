#include <math.h>

class vector_t
{
public:
	float x, y, z, w;
	vector_t() :x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}
	vector_t(const vector_t &a) :x(a.x), y(a.y), z(a.z), w(a.w) {}
	vector_t(float nx, float ny, float nz) :x(nx), y(ny), z(nz), w(1.0f) {}

	// 重载赋值运算符并返回引用
	vector_t& operator=(const vector_t &a) 
	{
		x = a.x; y = a.y; z = a.z; w = a.w;
		return *this;
	}

	bool operator==(const vector_t &a) const
	{
		return x == a.x && y == a.y && z == a.z && w == a.w;
	}

	bool operator!=(const vector_t &a) const
	{
		return x != a.x || y != a.y || z != a.z || w != a.w;
	}

	// 置为零向量
	void zero() { x = y = z = 0.0f; }

	vector_t operator-() const 
	{
		return vector_t(-x, -y, -z);
	}

	vector_t operator+(const vector_t &a) const
	{
		return vector_t(x + a.x, y + a.y, z + a.z);
	}

	vector_t operator-(const vector_t &a) const
	{
		return vector_t(x - a.x, y - a.y, z - a.z);
	}

	vector_t operator*(float a)const
	{
		return vector_t(x*a, y*a, z*a);
	}

	vector_t operator/(float a)const
	{
		float oneOverA = 1.0f / a;
		return vector_t(x*oneOverA, y*oneOverA, z*oneOverA);
	}

	vector_t& operator+=(const vector_t &a) 
	{
		x += a.x; y += a.y; z += a.z;
		return *this;
	}

	vector_t& operator-=(const vector_t &a)
	{
		x -= a.x; y -= a.y; z -= a.z;
		return *this;
	}

	vector_t& operator*=(float a)
	{
		x *= a; y *= a; z *= a;
		return *this;
	}

	vector_t& operator/=(float a)
	{
		float oneOverA = 1.0f / a;
		x *= oneOverA; y *= oneOverA; z *= oneOverA;
		return *this;
	}

	friend inline float vectorMag(const vector_t &a);

	// 向量标准化
	void normalize()
	{
		float length = vectorMag(*this);
		if (length != 0.0f) {
			float inv = 1.0f / length;
			x *= inv;
			y *= inv;
			z *= inv;
		}
	}

	// 向量点乘,重载"*"运算符
	float operator*(const vector_t &a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}


};

// 向量模长
inline float vectorMag(const vector_t &a)
{
	float sq = a.x * a.x + a.y * a.y + a.z * a.z;
	return (float)sqrt(sq);
}

// 向量叉乘
inline vector_t crossProduct(const vector_t &a, const vector_t &b)
{
	return vector_t(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

// 标量左乘，重载"*"运算符
inline vector_t operator*(float k, const vector_t &v)
{
	return vector_t(k*v.x, k*v.y, k*v.z);
}

// 向量距离
inline float distance(const vector_t &a, const vector_t &b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - a.z;
	return (float)sqrt(dx*dx + dy * dy + dz * dz);
}

// 计算插值：t 为 [0, 1] 之间的数值
float interp(float x1, float x2, float t);

// 向量插值，t取值 [0, 1]
inline vector_t vectorInterp(const vector_t &a, const vector_t &b, float t)
{
	return vector_t(
		interp(a.x, b.x, t),
		interp(a.y, b.y, t),
		interp(a.z, b.z, t)
	);
}

