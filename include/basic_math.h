#ifndef BASIC_3D_MATH_H
#define BASIC_3D_MATH_H

#include <math.h>
#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include "basic_util.h"

#define powi(base, exp) (int)powf((float)(base), (float)(exp));

#define toRadian(x) (float)(x * M_PI / 180.f)
#define toDegree(x) (float)(x * 180.f / M_PI)

float randomFloat();
float randomFloatRange(float start, float end);

struct Vector2i
{
    int x;
    int y;
};

struct Vector3i
{
    union
    {
        float x;
        float r;
    };

    union
    {
        float y;
        float g;
    };

    union
    {
        float z;
        float b;
    };
};

struct Vector2f
{
    union
    {
        float x;
        float u;
    };

    union
    {
        float y;
        float v;
    };

    Vector2f() {}

    Vector2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void print(bool endl = true) const
    {
        printf("(%f, %f)", x, y);

        if (endl)
        {
            printf("\n");
        }
    }

    float length() const
    {
        float len = sqrtf(x * x + y * y);
        return len;
    }

    void normalize()
    {
        float len = length();

        assert(len != 0);

        x /= len;
        y /= len;
    }
};

struct Vector3f
{
    union
    {
        float x;
        float r;
    };

    union
    {
        float y;
        float g;
    };

    union
    {
        float z;
        float b;
    };

    Vector3f() {}

    void initRandom(const Vector3f &minVal, const Vector3f &maxVal);

    void initBySphericalCoords(float radius, float pitch, float heading)
    {
        x = radius * cosf(toRadian(pitch)) * sinf(toRadian(heading));
        y = radius * cosf(toRadian(pitch));
        z = radius * sinf(toRadian(pitch)) * sinf(toRadian(heading));
    }

    Vector3f(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    void print(bool endl = true) const
    {
        printf("(%f, %f, %f)", x, y, z);

        if (endl)
        {
            printf("\n");
        }
    }

    float length() const
    {
        float len = sqrtf(x * x + y * y + z * z);
        return len;
    }

    void normalize()
    {
        float len = length();

        assert(len != 0);

        x /= len;
        y /= len;
        z /= len;
    }

    Vector3f cross(const Vector3f &v) const
    {
        Vector3f result;

        result.x = y * v.z - z * v.y;
        result.y = z * v.x - x * v.z;
        result.z = x * v.y - y * v.x;

        return result;
    }

    float dot(const Vector3f &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3f &operator+=(const Vector3f &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    Vector3f &operator-=(const Vector3f &v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    Vector3f &operator*=(float f)
    {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    Vector3f &operator/=(float f)
    {
        x /= f;
        y /= f;
        z /= f;

        return *this;
    }

    bool operator==(const Vector3f &v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vector3f &r)
    {
        return !(*this == r);
    }

    operator const float *() const
    {
        return &(x);
    }

    // distance
    float distance(const Vector3f &v) const
    {
        float dx = x - v.x;
        float dy = y - v.y;
        float dz = z - v.z;

        return sqrtf(dx * dx + dy * dy + dz * dz);
    }

    // Check addition is zero
    bool isZero() const
    {
        return x == 0 && y == 0 && z == 0;
    }

    // Rotate
    void rotate(float angle, const Vector3f &axis);

    Vector3f negate();

    float *data()
    {
        return &x;
    }
};

struct Vector4f
{
    union
    {
        float x;
        float r;
    };

    union
    {
        float y;
        float g;
    };

    union
    {
        float z;
        float b;
    };

    union
    {
        float w;
        float a;
    };

    Vector4f() {}

    Vector4f(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vector4f(const Vector3f &v, float _w)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = _w;
    }

    Vector3f to3f() const
    {
        Vector3f v(x, y, z);
        return v;
    }

    void print(bool endl = true) const
    {
        printf("(%f, %f, %f, %f)", x, y, z, w);

        if (endl)
        {
            printf("\n");
        }
    }

    float length() const
    {
        float len = sqrtf(x * x + y * y + z * z + w * w);
        return len;
    }

    void normalize()
    {
        float len = length();

        assert(len != 0);

        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }

    float dot(const Vector4f &v) const
    {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }

    //== operator
    bool operator==(const Vector4f &v) const
    {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }
};

inline Vector3f operator+(const Vector3f &l, const Vector3f &r)
{
    Vector3f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z);

    return Ret;
}

inline Vector3f operator-(const Vector3f &l, const Vector3f &r)
{
    Vector3f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z);

    return Ret;
}

inline Vector3f operator*(const Vector3f &l, float f)
{
    Vector3f Ret(l.x * f,
                 l.y * f,
                 l.z * f);

    return Ret;
}

inline Vector3f operator/(const Vector3f &l, float f)
{
    Vector3f Ret(l.x / f,
                 l.y / f,
                 l.z / f);

    return Ret;
}

inline Vector4f operator+(const Vector4f &l, const Vector4f &r)
{
    Vector4f Ret(l.x + r.x,
                 l.y + r.y,
                 l.z + r.z,
                 l.w + r.w);

    return Ret;
}

inline Vector4f operator-(const Vector4f &l, const Vector4f &r)
{
    Vector4f Ret(l.x - r.x,
                 l.y - r.y,
                 l.z - r.z,
                 l.w - r.w);

    return Ret;
}

inline Vector4f operator/(const Vector4f &l, float f)
{
    Vector4f Ret(l.x / f,
                 l.y / f,
                 l.z / f,
                 l.w / f);

    return Ret;
}

inline Vector4f operator*(const Vector4f &l, float f)
{
    Vector4f Ret(l.x * f,
                 l.y * f,
                 l.z * f,
                 l.w * f);

    return Ret;
}

inline Vector4f operator*(float f, const Vector4f &l)
{
    Vector4f Ret(l.x * f,
                 l.y * f,
                 l.z * f,
                 l.w * f);

    return Ret;
}

struct PersProjInfo
{
    float fov;
    float width;
    float height;
    float zNear;
    float zFar;
};

struct OrthoProjInfo
{
    float r;
    float l;
    float b;
    float t;
    float n;
    float f;

    float width;
    float height;

    void print()
    {
        printf("Left %f   Right %f\n", l, r);
        printf("Bottom %f Top %f\n", b, t);
        printf("Near %f   Far %f\n", n, f);
    }
};

struct Quaternion
{
    float x, y, z, w;

    Quaternion(float angle, const Vector3f &v);

    Quaternion(float _x, float _y, float _z, float _w);

    void normalize();

    Quaternion conjugate() const;

    Vector3f toDegrees();
};

Quaternion operator*(const Quaternion &l, const Quaternion &r);

Vector3f operator*(const Quaternion &q, const Vector3f &v);

class Matrix4f
{
public:
    float m[4][4];

    Matrix4f() {}

    Matrix4f(float m00, float m01, float m02, float m03,
             float m10, float m11, float m12, float m13,
             float m20, float m21, float m22, float m23,
             float m30, float m31, float m32, float m33)
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }
};

#endif