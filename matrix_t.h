#ifndef MATRIX_T_H
#define MATRIX_T_H

#include "vector_t.h"

class matrix_t
{
public:
    //float **m;
    float m[4][4] = { 0.0f };

    matrix_t() {}
    matrix_t(const matrix_t &a)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = a.m[i][j];
            }
        }
    }

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
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                m[i][j] = 0.0f;
            }
        }
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

// 向量与矩阵相乘，重载"*"运算符
inline vector_t operator*(const vector_t &a, const matrix_t &m)
{
    vector_t v;
    v.x = a.x * m.m[0][0] + a.y * m.m[1][0] + a.z * m.m[2][0] + a.w * m.m[3][0];
    v.y = a.x * m.m[0][1] + a.y * m.m[1][1] + a.z * m.m[2][1] + a.w * m.m[3][1];
    v.z = a.x * m.m[0][2] + a.y * m.m[1][2] + a.z * m.m[2][2] + a.w * m.m[3][2];
    v.w = a.x * m.m[0][3] + a.y * m.m[1][3] + a.z * m.m[2][3] + a.w * m.m[3][3];
    return v;
}

matrix_t lookAt(const vector_t &eye, const vector_t &at, const vector_t &up)
{
    vector_t xaxis, yaxis, zaxis;
       
    zaxis = at - eye;
    zaxis.normalize();

    xaxis = crossProduct(up, zaxis);
    xaxis.normalize();

    yaxis = crossProduct(zaxis, xaxis);

    matrix_t m;

    m.m[0][0] = xaxis.x;
    m.m[1][0] = xaxis.y;
    m.m[2][0] = xaxis.z;
    m.m[3][0] = -(xaxis * eye);

    m.m[0][1] = yaxis.x;
    m.m[1][1] = yaxis.y;
    m.m[2][1] = yaxis.z;
    m.m[3][1] = -(yaxis * eye);

    m.m[0][2] = zaxis.x;
    m.m[1][2] = zaxis.y;
    m.m[2][2] = zaxis.z;
    m.m[3][2] = -(zaxis*eye);

    m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;

    return m;
}

matrix_t perspective(float fovy, float aspect, float zn, float zf)
{
    matrix_t m;

    float fax = 1.0f / (float)tan(fovy * 0.5f);
    m.setZero();
    m.m[0][0] = (float)(fax / aspect);
    m.m[1][1] = (float)(fax);
    m.m[2][2] = zf / (zf - zn);
    m.m[3][2] = -zn * zf / (zf - zn);
    m.m[2][3] = 1;

    return m;
}

#endif // !MATRIX_T_H
