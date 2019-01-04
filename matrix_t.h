#ifndef _MATRIX_T_H_
#define _MATRIX_T_H_

#include "vector_t.h"

class matrix_t
{
public:
    float **m;
    void initialize()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = 0.0f;
            }
        }
    }

    matrix_t() { initialize(); }
    matrix_t(const matrix_t &a) :m(a.m) {}

    // 重载赋值运算符并返回引用
    matrix_t& operator=(const matrix_t &a)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = a.m[i][j];
            }
        }
        return *this;
    }

    matrix_t operator+(const matrix_t &a) const
    {
        matrix_t n;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                n.m[i][j] = m[i][j] + a.m[i][j];
            }
        }
        return n;
    }

    matrix_t operator-(const matrix_t &a) const
    {
        matrix_t z;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                z.m[i][j] = m[i][j] - a.m[i][j];
            }
        }
        return z;
    }

    // 矩阵相乘，重载"*"运算符
    matrix_t operator*(const matrix_t &a) const
    {
        matrix_t z;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                z.m[j][i] = (m[j][0] * a.m[0][i]) +
                    (m[j][1] * a.m[1][i]) +
                    (m[j][2] * a.m[2][i]) +
                    (m[j][3] * a.m[3][i]);
            }
        }
        return z;
    }

    // 矩阵与标量相乘，重载"*"运算符
    matrix_t operator*(float f) const
    {
        matrix_t z;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                z.m[i][j] = m[i][j] * f;
            }
        }
        return z;
    }


    // 设为单位矩阵
    void setIdentity()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                {
                    m[i][j] = 1.0f;
                }
                else
                {
                    m[i][j] = 0.0f;
                }
            }
        }
    }

    // 设为零矩阵
    void setZero()
    {
        initialize();
    }

    // 平移变换
    void setTranslate(float x, float y, float z)
    {
        setIdentity();
        m[3][0] = x;
        m[3][1] = y;
        m[3][2] = z;
    }

    // 缩放变换
    void setScale(float x, float y, float z)
    {
        setIdentity();
        m[0][0] = x;
        m[1][1] = y;
        m[2][2] = z;
    }

    // 旋转变换
    void setRotate(float x, float y, float z, float theta)
    {
        float qsin = (float)sin(theta * 0.5f);
        float qcos = (float)cos(theta * 0.5f);
        //vector_t vec = { x, y, z, 1.0f };
        vector_t vec = vector_t(x, y, z);

        float w = qcos;
        vec.normalize();
        x = vec.x * qsin;
        y = vec.y * qsin;
        z = vec.z * qsin;
        m[0][0] = 1 - 2 * y * y - 2 * z * z;
        m[1][0] = 2 * x * y - 2 * w * z;
        m[2][0] = 2 * x * z + 2 * w * y;
        m[0][1] = 2 * x * y + 2 * w * z;
        m[1][1] = 1 - 2 * x * x - 2 * z * z;
        m[2][1] = 2 * y * z - 2 * w * x;
        m[0][2] = 2 * x * z - 2 * w * y;
        m[1][2] = 2 * y * z + 2 * w * x;
        m[2][2] = 1 - 2 * x * x - 2 * y * y;
        m[0][3] = m[1][3] = m[2][3] = 0.0f;
        m[3][0] = m[3][1] = m[3][2] = 0.0f;
        m[3][3] = 1.0f;
    }
};

#endif // !_MATRIX_T_H_
