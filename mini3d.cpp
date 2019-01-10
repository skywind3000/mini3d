//=====================================================================
// 
// mini3d.c - Mini Software Render All-In-One
//
// build:
//   mingw: gcc -O3 mini3d.c -o mini3d.exe -lgdi32
//   msvc:  cl -O2 -nologo mini3d.c 
//
// history:
//   2007.7.01  skywind  create this file as a tutorial
//   2007.7.02  skywind  implementate texture and color render
//   2008.3.15  skywind  fixed a trapezoid issue
//   2015.8.09  skywind  rewrite with more comment
//   2015.8.12  skywind  adjust interfaces for clearity 
// 
//=====================================================================
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <windows.h>
#include <tchar.h>

#include "vector_t.h"
#include "matrix_t.h"

typedef unsigned int IUINT32;

//=====================================================================
// 数学库：此部分应该不用详解，熟悉 D3D 矩阵变换即可
//=====================================================================
//typedef struct { float m[4][4]; } matrix_t;
//typedef struct { float x, y, z, w; } vector_t;
typedef vector_t point_t;

int CMID(int x, int min, int max) { return (x < min) ? min : ((x > max) ? max : x); }

// 计算插值：t 为 [0, 1] 之间的数值
float interp(float x1, float x2, float t) { return x1 + (x2 - x1) * t; }

// | v |
// float vector_length(const vector_t *v) {
// 	float sq = v->x * v->x + v->y * v->y + v->z * v->z;
// 	return (float)sqrt(sq);
// }
// 
// z = x + y
// void vector_add(vector_t *z, const vector_t *x, const vector_t *y) {
// 	z->x = x->x + y->x;
// 	z->y = x->y + y->y;
// 	z->z = x->z + y->z;
// 	z->w = 1.0;
// }
// 
// z = x - y
// void vector_sub(vector_t *z, const vector_t *x, const vector_t *y) {
// 	z->x = x->x - y->x;
// 	z->y = x->y - y->y;
// 	z->z = x->z - y->z;
// 	z->w = 1.0;
// }
// 
// 矢量点乘
// float vector_dotproduct(const vector_t *x, const vector_t *y) {
// 	return x->x * y->x + x->y * y->y + x->z * y->z;
// }
// 
// 矢量叉乘
// void vector_crossproduct(vector_t *z, const vector_t *x, const vector_t *y) {
// 	float m1, m2, m3;
// 	m1 = x->y * y->z - x->z * y->y;
// 	m2 = x->z * y->x - x->x * y->z;
// 	m3 = x->x * y->y - x->y * y->x;
// 	z->x = m1;
// 	z->y = m2;
// 	z->z = m3;
// 	z->w = 1.0f;
// }
// 
// 矢量插值，t取值 [0, 1]
// void vector_interp(vector_t *z, const vector_t *x1, const vector_t *x2, float t) {
// 	z->x = interp(x1->x, x2->x, t);
// 	z->y = interp(x1->y, x2->y, t);
// 	z->z = interp(x1->z, x2->z, t);
// 	z->w = 1.0f;
// }
// 
// 矢量归一化
// void vector_normalize(vector_t *v) {
// 	float length = vector_length(v);
// 	if (length != 0.0f) {
// 		float inv = 1.0f / length;
// 		v->x *= inv; 
// 		v->y *= inv;
// 		v->z *= inv;
// 	}
// }

//// c = a + b
//void matrix_add(matrix_t *c, const matrix_t *a, const matrix_t *b) {
//    int i, j;
//    for (i = 0; i < 4; i++) {
//        for (j = 0; j < 4; j++)
//            c->m[i][j] = a->m[i][j] + b->m[i][j];
//    }
//}
//
//// c = a - b
//void matrix_sub(matrix_t *c, const matrix_t *a, const matrix_t *b) {
//    int i, j;
//    for (i = 0; i < 4; i++) {
//        for (j = 0; j < 4; j++)
//            c->m[i][j] = a->m[i][j] - b->m[i][j];
//    }
//}
//
//// c = a * b
//void matrix_mul(matrix_t *c, const matrix_t *a, const matrix_t *b) {
//    matrix_t z;
//    int i, j;
//    for (i = 0; i < 4; i++) {
//        for (j = 0; j < 4; j++) {
//            z.m[j][i] = (a->m[j][0] * b->m[0][i]) +
//                (a->m[j][1] * b->m[1][i]) +
//                (a->m[j][2] * b->m[2][i]) +
//                (a->m[j][3] * b->m[3][i]);
//        }
//    }
//    c[0] = z;
//}
//
//// c = a * f
//void matrix_scale(matrix_t *c, const matrix_t *a, float f) {
//    int i, j;
//    for (i = 0; i < 4; i++) {
//        for (j = 0; j < 4; j++)
//            c->m[i][j] = a->m[i][j] * f;
//    }
//}
//
//// y = x * m
//void matrix_apply(vector_t *y, const vector_t *x, const matrix_t *m) {
//    float X = x->x, Y = x->y, Z = x->z, W = x->w;
//    y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
//    y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
//    y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
//    y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
//}
//
//void matrix_set_identity(matrix_t *m) {
//    m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
//    m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
//    m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
//    m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
//    m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
//}
//
//void matrix_set_zero(matrix_t *m) {
//    m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
//    m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
//    m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
//    m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
//}
//
//// 平移变换
//void matrix_set_translate(matrix_t *m, float x, float y, float z) {
//    matrix_set_identity(m);
//    m->m[3][0] = x;
//    m->m[3][1] = y;
//    m->m[3][2] = z;
//}
//
//// 缩放变换
//void matrix_set_scale(matrix_t *m, float x, float y, float z) {
//    matrix_set_identity(m);
//    m->m[0][0] = x;
//    m->m[1][1] = y;
//    m->m[2][2] = z;
//}
//
//// 旋转矩阵
//void matrix_set_rotate(matrix_t *m, float x, float y, float z, float theta) {
//    float qsin = (float)sin(theta * 0.5f);
//    float qcos = (float)cos(theta * 0.5f);
//    //vector_t vec = { x, y, z, 1.0f };
//    vector_t vec = vector_t(x, y, z);
//
//    float w = qcos;
//    vec.normalize();
//    x = vec.x * qsin;
//    y = vec.y * qsin;
//    z = vec.z * qsin;
//    m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
//    m->m[1][0] = 2 * x * y - 2 * w * z;
//    m->m[2][0] = 2 * x * z + 2 * w * y;
//    m->m[0][1] = 2 * x * y + 2 * w * z;
//    m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
//    m->m[2][1] = 2 * y * z - 2 * w * x;
//    m->m[0][2] = 2 * x * z - 2 * w * y;
//    m->m[1][2] = 2 * y * z + 2 * w * x;
//    m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
//    m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
//    m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
//    m->m[3][3] = 1.0f;
//}
//
//// 设置摄像机
//void matrix_set_lookat(matrix_t *m, const vector_t *eye, const vector_t *at, const vector_t *up) {
//    vector_t xaxis, yaxis, zaxis;
//
//    //vector_sub(&zaxis, at, eye);
//    zaxis = *at - *eye;
//    //vector_normalize(&zaxis);
//    zaxis.normalize();
//    //vector_crossproduct(&xaxis, up, &zaxis);
//    xaxis = crossProduct(*up, zaxis);
//    //vector_normalize(&xaxis);
//    xaxis.normalize();
//    //vector_crossproduct(&yaxis, &zaxis, &xaxis);
//    yaxis = crossProduct(zaxis, xaxis);
//
//    m->m[0][0] = xaxis.x;
//    m->m[1][0] = xaxis.y;
//    m->m[2][0] = xaxis.z;
//    //m->m[3][0] = -vector_dotproduct(&xaxis, eye);
//    m->m[3][0] = -(xaxis * (*eye));
//
//    m->m[0][1] = yaxis.x;
//    m->m[1][1] = yaxis.y;
//    m->m[2][1] = yaxis.z;
//    //m->m[3][1] = -vector_dotproduct(&yaxis, eye);
//    m->m[3][1] = -(yaxis * (*eye));
//
//    m->m[0][2] = zaxis.x;
//    m->m[1][2] = zaxis.y;
//    m->m[2][2] = zaxis.z;
//    //m->m[3][2] = -vector_dotproduct(&zaxis, eye);
//    m->m[3][2] = -(zaxis*(*eye));
//
//    m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
//    m->m[3][3] = 1.0f;
//}
//
//// D3DXMatrixPerspectiveFovLH
//void matrix_set_perspective(matrix_t *m, float fovy, float aspect, float zn, float zf) {
//    float fax = 1.0f / (float)tan(fovy * 0.5f);
//    matrix_set_zero(m);
//    m->m[0][0] = (float)(fax / aspect);
//    m->m[1][1] = (float)(fax);
//    m->m[2][2] = zf / (zf - zn);
//    m->m[3][2] = -zn * zf / (zf - zn);
//    m->m[2][3] = 1;
//}


//=====================================================================
// 坐标变换
//=====================================================================
typedef struct {
    matrix_t world;         // 世界坐标变换
    matrix_t view;          // 摄影机坐标变换
    matrix_t projection;    // 投影变换
    matrix_t transform;     // transform = world * view * projection
    float w, h;             // 屏幕大小
}	transform_t;


// 矩阵更新，计算 transform = world * view * projection
void transform_update(transform_t *ts) {
    //matrix_t m;
    //matrix_mul(&m, &ts->world, &ts->view);
    //m = ts->world * ts->view;
    //matrix_mul(&ts->transform, &m, &ts->projection);
    ts->transform = ts->world * ts->view * ts->projection;
}

// 初始化，设置屏幕长宽
void transform_init(transform_t *ts, int width, int height) {
    float aspect = (float)width / ((float)height);
    //matrix_set_identity(&ts->world);
    ts->world.setIdentity();
    //matrix_set_identity(&ts->view);
    ts->view.setIdentity();
    //matrix_set_perspective(&ts->projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
    ts->projection = perspective(3.1415926f*0.5f, aspect, 1.0f, 500.0f);
    ts->w = (float)width;
    ts->h = (float)height;
    transform_update(ts);
}

// 将矢量 x 进行 project 
void transform_apply(const transform_t *ts, vector_t *y, const vector_t *x) {
    //matrix_apply(y, x, &ts->transform);
    *y = *x * ts->transform;
}

// 检查齐次坐标同 cvv 的边界用于视锥裁剪
int transform_check_cvv(const vector_t *v) {
    float w = v->w;
    int check = 0;
    if (v->z < 0.0f) check |= 1;
    if (v->z > w) check |= 2;
    if (v->x < -w) check |= 4;
    if (v->x > w) check |= 8;
    if (v->y < -w) check |= 16;
    if (v->y > w) check |= 32;
    return check;
}

// 归一化，得到屏幕坐标
void transform_homogenize(const transform_t *ts, vector_t *y, const vector_t *x) {
    float rhw = 1.0f / x->w;
    y->x = (x->x * rhw + 1.0f) * ts->w * 0.5f;
    y->y = (1.0f - x->y * rhw) * ts->h * 0.5f;
    y->z = x->z * rhw;
    y->w = 1.0f;
}


//=====================================================================
// 几何计算：顶点、扫描线、边缘、矩形、步长计算
//=====================================================================
typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
typedef struct { point_t pos; texcoord_t tc; color_t color; float rhw; } vertex_t;

typedef struct { vertex_t v, v1, v2; } edge_t;
typedef struct { float top, bottom; edge_t left, right; } trapezoid_t;
typedef struct { vertex_t v, step; int x, y, w; } scanline_t;

void color_modulated(color_t *y, const color_t *x1, const color_t *x2)
{
    y->r = x1->r*x2->r;
    y->g = x1->g*x2->g;
    y->b = x1->b*x2->b;
}


void vertex_rhw_init(vertex_t *v) {
    float rhw = 1.0f / v->pos.w;
    v->rhw = rhw;
    v->tc.u *= rhw;
    v->tc.v *= rhw;
    v->color.r *= rhw;
    v->color.g *= rhw;
    v->color.b *= rhw;
}

void vertex_interp(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float t) {
    //vector_interp(&y->pos, &x1->pos, &x2->pos, t);
    y->pos = vectorInterp(x1->pos, x2->pos, t);
    y->tc.u = interp(x1->tc.u, x2->tc.u, t);
    y->tc.v = interp(x1->tc.v, x2->tc.v, t);
    y->color.r = interp(x1->color.r, x2->color.r, t);
    y->color.g = interp(x1->color.g, x2->color.g, t);
    y->color.b = interp(x1->color.b, x2->color.b, t);
    y->rhw = interp(x1->rhw, x2->rhw, t);
}

void vertex_division(vertex_t *y, const vertex_t *x1, const vertex_t *x2, float w) {
    float inv = 1.0f / w;
    y->pos.x = (x2->pos.x - x1->pos.x) * inv;
    y->pos.y = (x2->pos.y - x1->pos.y) * inv;
    y->pos.z = (x2->pos.z - x1->pos.z) * inv;
    y->pos.w = (x2->pos.w - x1->pos.w) * inv;
    y->tc.u = (x2->tc.u - x1->tc.u) * inv;
    y->tc.v = (x2->tc.v - x1->tc.v) * inv;
    y->color.r = (x2->color.r - x1->color.r) * inv;
    y->color.g = (x2->color.g - x1->color.g) * inv;
    y->color.b = (x2->color.b - x1->color.b) * inv;
    y->rhw = (x2->rhw - x1->rhw) * inv;
}

void vertex_add(vertex_t *y, const vertex_t *x) {
    y->pos.x += x->pos.x;
    y->pos.y += x->pos.y;
    y->pos.z += x->pos.z;
    y->pos.w += x->pos.w;
    y->rhw += x->rhw;
    y->tc.u += x->tc.u;
    y->tc.v += x->tc.v;
    y->color.r += x->color.r;
    y->color.g += x->color.g;
    y->color.b += x->color.b;
}

// 根据三角形生成 0-2 个梯形，并且返回合法梯形的数量
int trapezoid_init_triangle(trapezoid_t *trap, const vertex_t *p1,
    const vertex_t *p2, const vertex_t *p3) {
    const vertex_t *p;
    float k, x;

    if (p1->pos.y > p2->pos.y) p = p1, p1 = p2, p2 = p;
    if (p1->pos.y > p3->pos.y) p = p1, p1 = p3, p3 = p;
    if (p2->pos.y > p3->pos.y) p = p2, p2 = p3, p3 = p;
    if (p1->pos.y == p2->pos.y && p1->pos.y == p3->pos.y) return 0;
    if (p1->pos.x == p2->pos.x && p1->pos.x == p3->pos.x) return 0;

    if (p1->pos.y == p2->pos.y) {	// triangle down
        if (p1->pos.x > p2->pos.x) p = p1, p1 = p2, p2 = p;
        trap[0].top = p1->pos.y;
        trap[0].bottom = p3->pos.y;
        trap[0].left.v1 = *p1;
        trap[0].left.v2 = *p3;
        trap[0].right.v1 = *p2;
        trap[0].right.v2 = *p3;
        return (trap[0].top < trap[0].bottom) ? 1 : 0;
    }

    if (p2->pos.y == p3->pos.y) {	// triangle up
        if (p2->pos.x > p3->pos.x) p = p2, p2 = p3, p3 = p;
        trap[0].top = p1->pos.y;
        trap[0].bottom = p3->pos.y;
        trap[0].left.v1 = *p1;
        trap[0].left.v2 = *p2;
        trap[0].right.v1 = *p1;
        trap[0].right.v2 = *p3;
        return (trap[0].top < trap[0].bottom) ? 1 : 0;
    }

    trap[0].top = p1->pos.y;
    trap[0].bottom = p2->pos.y;
    trap[1].top = p2->pos.y;
    trap[1].bottom = p3->pos.y;

    k = (p3->pos.y - p1->pos.y) / (p2->pos.y - p1->pos.y);
    x = p1->pos.x + (p2->pos.x - p1->pos.x) * k;

    if (x <= p3->pos.x) {		// triangle left
        trap[0].left.v1 = *p1;
        trap[0].left.v2 = *p2;
        trap[0].right.v1 = *p1;
        trap[0].right.v2 = *p3;
        trap[1].left.v1 = *p2;
        trap[1].left.v2 = *p3;
        trap[1].right.v1 = *p1;
        trap[1].right.v2 = *p3;
    }
    else {					// triangle right
        trap[0].left.v1 = *p1;
        trap[0].left.v2 = *p3;
        trap[0].right.v1 = *p1;
        trap[0].right.v2 = *p2;
        trap[1].left.v1 = *p1;
        trap[1].left.v2 = *p3;
        trap[1].right.v1 = *p2;
        trap[1].right.v2 = *p3;
    }

    return 2;
}

// 按照 Y 坐标计算出左右两条边纵坐标等于 Y 的顶点
void trapezoid_edge_interp(trapezoid_t *trap, float y) {
    float s1 = trap->left.v2.pos.y - trap->left.v1.pos.y;
    float s2 = trap->right.v2.pos.y - trap->right.v1.pos.y;
    float t1 = (y - trap->left.v1.pos.y) / s1;
    float t2 = (y - trap->right.v1.pos.y) / s2;
    vertex_interp(&trap->left.v, &trap->left.v1, &trap->left.v2, t1);
    vertex_interp(&trap->right.v, &trap->right.v1, &trap->right.v2, t2);
}

// 根据左右两边的端点，初始化计算出扫描线的起点和步长
void trapezoid_init_scan_line(const trapezoid_t *trap, scanline_t *scanline, int y) {
    float width = trap->right.v.pos.x - trap->left.v.pos.x;
    scanline->x = (int)(trap->left.v.pos.x + 0.5f);
    scanline->w = (int)(trap->right.v.pos.x + 0.5f) - scanline->x;
    scanline->y = y;
    scanline->v = trap->left.v;
    if (trap->left.v.pos.x >= trap->right.v.pos.x) scanline->w = 0;
    vertex_division(&scanline->step, &trap->left.v, &trap->right.v, width);
}



//=====================================================================
// 简单光照-平行光
//=====================================================================
typedef struct { float ambiStrength, diffStrength; color_t lightColor; vector_t direction; }light_t;

//=====================================================================
// 渲染设备
//=====================================================================
typedef struct {
    transform_t transform;      // 坐标变换器
    int width;                  // 窗口宽度
    int height;                 // 窗口高度
    IUINT32 **framebuffer;      // 像素缓存：framebuffer[y] 代表第 y行
    float **zbuffer;            // 深度缓存：zbuffer[y] 为第 y行指针
    IUINT32 **texture;          // 纹理：同样是每行索引
    int tex_width;              // 纹理宽度
    int tex_height;             // 纹理高度
    float max_u;                // 纹理最大宽度：tex_width - 1
    float max_v;                // 纹理最大高度：tex_height - 1
    int render_state;           // 渲染状态
    int cull_mode;              // 剔除模式
    IUINT32 background;         // 背景颜色
    IUINT32 foreground;         // 线框颜色
    light_t light;              // 光源
}	device_t;

#define RENDER_STATE_WIREFRAME      1		// 渲染线框
#define RENDER_STATE_TEXTURE        2		// 渲染纹理
#define RENDER_STATE_COLOR          4		// 渲染颜色

#define CULL_MODE_NONE              1       // 禁止剔除
#define CULL_MODE_FRONT             2       // 正面剔除
#define CULL_MODE_BACK              4       // 背面剔除

// 设备初始化，fb为外部帧缓存，非 NULL 将引用外部帧缓存（每行 4字节对齐）
void device_init(device_t *device, int width, int height, void *fb) {
    int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
    char *ptr = (char*)malloc(need + 64);
    char *framebuf, *zbuf;
    int j;
    assert(ptr);
    device->framebuffer = (IUINT32**)ptr;
    device->zbuffer = (float**)(ptr + sizeof(void*) * height);
    ptr += sizeof(void*) * height * 2;
    device->texture = (IUINT32**)ptr;
    ptr += sizeof(void*) * 1024;
    framebuf = (char*)ptr;
    zbuf = (char*)ptr + width * height * 4;
    ptr += width * height * 8;
    if (fb != NULL) framebuf = (char*)fb;
    for (j = 0; j < height; j++) {
        device->framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
        device->zbuffer[j] = (float*)(zbuf + width * 4 * j);
    }
    device->texture[0] = (IUINT32*)ptr;
    device->texture[1] = (IUINT32*)(ptr + 16);
    memset(device->texture[0], 0, 64);
    device->tex_width = 2;
    device->tex_height = 2;
    device->max_u = 1.0f;
    device->max_v = 1.0f;
    device->width = width;
    device->height = height;
    device->background = 0xc0c0c0;
    device->foreground = 0;
    transform_init(&device->transform, width, height);
    device->render_state = RENDER_STATE_WIREFRAME;
}

// 删除设备
void device_destroy(device_t *device) {
    if (device->framebuffer)
        free(device->framebuffer);
    device->framebuffer = NULL;
    device->zbuffer = NULL;
    device->texture = NULL;
}

// 设置当前纹理
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h) {
    char *ptr = (char*)bits;
    int j;
    assert(w <= 1024 && h <= 1024);
    for (j = 0; j < h; ptr += pitch, j++) 	// 重新计算每行纹理的指针
        device->texture[j] = (IUINT32*)ptr;
    device->tex_width = w;
    device->tex_height = h;
    device->max_u = (float)(w - 1);
    device->max_v = (float)(h - 1);
}

// 清空 framebuffer 和 zbuffer
void device_clear(device_t *device, int mode) {
    int y, x, height = device->height;
    for (y = 0; y < device->height; y++) {
        IUINT32 *dst = device->framebuffer[y];
        IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
        cc = (cc << 16) | (cc << 8) | cc;
        if (mode == 0) cc = device->background;
        for (x = device->width; x > 0; dst++, x--) dst[0] = cc;
    }
    for (y = 0; y < device->height; y++) {
        float *dst = device->zbuffer[y];
        for (x = device->width; x > 0; dst++, x--) dst[0] = 0.0f;
    }
}

// 画点
void device_pixel(device_t *device, int x, int y, IUINT32 color) {
    if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) {
        device->framebuffer[y][x] = color;
    }
}

// 绘制线段
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c) {
    int x, y, rem = 0;
    if (x1 == x2 && y1 == y2) {
        device_pixel(device, x1, y1, c);
    }
    else if (x1 == x2) {
        int inc = (y1 <= y2) ? 1 : -1;
        for (y = y1; y != y2; y += inc) device_pixel(device, x1, y, c);
        device_pixel(device, x2, y2, c);
    }
    else if (y1 == y2) {
        int inc = (x1 <= x2) ? 1 : -1;
        for (x = x1; x != x2; x += inc) device_pixel(device, x, y1, c);
        device_pixel(device, x2, y2, c);
    }
    else {
        int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
        int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
        if (dx >= dy) {
            if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; x <= x2; x++) {
                device_pixel(device, x, y, c);
                rem += dy;
                if (rem >= dx) {
                    rem -= dx;
                    y += (y2 >= y1) ? 1 : -1;
                    device_pixel(device, x, y, c);
                }
            }
            device_pixel(device, x2, y2, c);
        }
        else {
            if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
            for (x = x1, y = y1; y <= y2; y++) {
                device_pixel(device, x, y, c);
                rem += dx;
                if (rem >= dy) {
                    rem -= dy;
                    x += (x2 >= x1) ? 1 : -1;
                    device_pixel(device, x, y, c);
                }
            }
            device_pixel(device, x2, y2, c);
        }
    }
}

// 根据坐标读取纹理
IUINT32 device_texture_read(const device_t *device, float u, float v) {
    int x, y;
    u = u * device->max_u;
    v = v * device->max_v;
    x = (int)(u + 0.5f);
    y = (int)(v + 0.5f);
    x = CMID(x, 0, device->tex_width - 1);
    y = CMID(y, 0, device->tex_height - 1);
    return device->texture[y][x];
}


//=====================================================================
// 渲染实现
//=====================================================================

// 绘制扫描线
void device_draw_scanline(device_t *device, scanline_t *scanline) {
    IUINT32 *framebuffer = device->framebuffer[scanline->y];
    float *zbuffer = device->zbuffer[scanline->y];
    int x = scanline->x;
    int w = scanline->w;
    int width = device->width;
    int render_state = device->render_state;
    for (; w > 0; x++, w--) {
        if (x >= 0 && x < width) {
            float rhw = scanline->v.rhw;
            if (rhw >= zbuffer[x]) {
                float w = 1.0f / rhw;
                zbuffer[x] = rhw;
                if (render_state & RENDER_STATE_COLOR) {
                    float r = scanline->v.color.r * w;
                    float g = scanline->v.color.g * w;
                    float b = scanline->v.color.b * w;
                    int R = (int)(r * 255.0f);
                    int G = (int)(g * 255.0f);
                    int B = (int)(b * 255.0f);
                    R = CMID(R, 0, 255);
                    G = CMID(G, 0, 255);
                    B = CMID(B, 0, 255);
                    framebuffer[x] = (R << 16) | (G << 8) | (B);
                }
                if (render_state & RENDER_STATE_TEXTURE) {
                    float u = scanline->v.tc.u * w;
                    float v = scanline->v.tc.v * w;
                    IUINT32 cc = device_texture_read(device, u, v);
                    color_t pixel;
                    pixel.r = (float)((cc & 0x00ff0000)>>16);
                    pixel.g = (float)((cc & 0x0000ff00)>>8);
                    pixel.b = (float)(cc & 0x000000ff);
                    color_t lightcolor;
                    device->light.ambiStrength = 0.1f;
                    lightcolor.r = (device->light.ambiStrength + device->light.diffStrength)*device->light.lightColor.r;
                    lightcolor.g = (device->light.ambiStrength + device->light.diffStrength)*device->light.lightColor.g;
                    lightcolor.b = (device->light.ambiStrength + device->light.diffStrength)*device->light.lightColor.b;
                    lightcolor.r = lightcolor.r > 1.0f ? 1.0f : lightcolor.r;
                    lightcolor.g = lightcolor.g > 1.0f ? 1.0f : lightcolor.g;
                    lightcolor.b = lightcolor.b > 1.0f ? 1.0f : lightcolor.b;

                    color_modulated(&pixel, &pixel, &lightcolor);
                    int R = (int)(pixel.r);
                    int G = (int)(pixel.g);
                    int B = (int)(pixel.b);
                    framebuffer[x] = (R << 16) | (G << 8) | (B);

                    //framebuffer[x] = cc;
                }
            }
        }
        vertex_add(&scanline->v, &scanline->step);
        if (x >= width) break;
    }
}

// 主渲染函数
void device_render_trap(device_t *device, trapezoid_t *trap) {
    scanline_t scanline;
    int j, top, bottom;
    top = (int)(trap->top + 0.5f);
    bottom = (int)(trap->bottom + 0.5f);
    for (j = top; j < bottom; j++) {
        if (j >= 0 && j < device->height) {
            trapezoid_edge_interp(trap, (float)j + 0.5f);
            trapezoid_init_scan_line(trap, &scanline, j);
            device_draw_scanline(device, &scanline);
        }
        if (j >= device->height) break;
    }
}

bool device_cullface(const device_t *device, const point_t *p1, const point_t *p2, const point_t *p3)
{
    int cull_mode = device->cull_mode;
    vector_t v1, v2, normal;
    v1 = *p2 - *p1;
    v2 = *p3 - *p1;
    normal = crossProduct(v1, v2);
    normal.normalize();

    vector_t viewDirection(0.0f, 0.0f, -1.0f);

    switch (cull_mode)
    {
    case CULL_MODE_NONE:
        return false;
    case CULL_MODE_FRONT:
        if (normal*viewDirection >= 0.0f)
            return false;
        else
            return true;
    case CULL_MODE_BACK:
        if (normal*viewDirection >= 0.0f)
            return true;
        else
            return false;
    default:
        return false;
    }
}

void device_compute_diff(device_t *device, const point_t *p1, const point_t *p2, const point_t *p3)
{
    vector_t lightdir = device->light.direction;
    lightdir.normalize();
    vector_t v1, v2, normal;
    v1 = *p2 - *p1;
    v2 = *p3 - *p1;
    normal = crossProduct(v1, v2);
    normal.normalize();

    float diffStrength = (normal * lightdir) < 0 ? 0 : (normal * lightdir);

    device->light.diffStrength = diffStrength;
}


// 根据 render_state 绘制原始三角形
void device_draw_primitive(device_t *device, const vertex_t *v1,
    const vertex_t *v2, const vertex_t *v3) {
    point_t p1, p2, p3, c1, c2, c3;
    int render_state = device->render_state;

    // 按照 Transform 变化
    transform_apply(&device->transform, &c1, &v1->pos);
    transform_apply(&device->transform, &c2, &v2->pos);
    transform_apply(&device->transform, &c3, &v3->pos);

    // 裁剪，注意此处可以完善为具体判断几个点在 cvv内以及同cvv相交平面的坐标比例
    // 进行进一步精细裁剪，将一个分解为几个完全处在 cvv内的三角形
    if (transform_check_cvv(&c1) != 0) return;
    if (transform_check_cvv(&c2) != 0) return;
    if (transform_check_cvv(&c3) != 0) return;

    // 归一化
    transform_homogenize(&device->transform, &p1, &c1);
    transform_homogenize(&device->transform, &p2, &c2);
    transform_homogenize(&device->transform, &p3, &c3);

    // 剔除检测
    bool faceCull;
    faceCull = device_cullface(device, &p1, &p2, &p3);

    if (!faceCull)
    {
        // 纹理或者色彩绘制
        if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
            vertex_t t1 = *v1, t2 = *v2, t3 = *v3;
            trapezoid_t traps[2];
            int n;

            t1.pos = p1;
            t2.pos = p2;
            t3.pos = p3;
            t1.pos.w = c1.w;
            t2.pos.w = c2.w;
            t3.pos.w = c3.w;

            device_compute_diff(device, &c1, &c2, &c3);


            vertex_rhw_init(&t1);	// 初始化 w
            vertex_rhw_init(&t2);	// 初始化 w
            vertex_rhw_init(&t3);	// 初始化 w

            // 拆分三角形为0-2个梯形，并且返回可用梯形数量
            n = trapezoid_init_triangle(traps, &t1, &t2, &t3);

            if (n >= 1) device_render_trap(device, &traps[0]);
            if (n >= 2) device_render_trap(device, &traps[1]);
        }

        if (render_state & RENDER_STATE_WIREFRAME) {		// 线框绘制
            device_draw_line(device, (int)p1.x, (int)p1.y, (int)p2.x, (int)p2.y, device->foreground);
            device_draw_line(device, (int)p1.x, (int)p1.y, (int)p3.x, (int)p3.y, device->foreground);
            device_draw_line(device, (int)p3.x, (int)p3.y, (int)p2.x, (int)p2.y, device->foreground);
        }
    }
}



//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================
int screen_w, screen_h, screen_exit = 0;
int screen_mx = 0, screen_my = 0, screen_mb = 0;
int screen_keys[512];	// 当前键盘按下状态
static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP
unsigned char *screen_fb = NULL;		// frame buffer
long screen_pitch = 0;

int screen_init(int w, int h, const TCHAR *title);	// 屏幕初始化
int screen_close(void);								// 关闭屏幕
void screen_dispatch(void);							// 处理消息
void screen_update(void);							// 显示 FrameBuffer

// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif

// 初始化窗口并设置标题
int screen_init(int w, int h, const TCHAR *title) {
    WNDCLASS wc = { CS_BYTEALIGNCLIENT, (WNDPROC)screen_events, 0, 0, 0,
        NULL, NULL, NULL, NULL, _T("SCREEN3.1415926") };
    BITMAPINFO bi = { { sizeof(BITMAPINFOHEADER), w, -h, 1, 32, BI_RGB,
        w * h * 4, 0, 0, 0, 0 } };
    RECT rect = { 0, 0, w, h };
    int wx, wy, sx, sy;
    LPVOID ptr;
    HDC hDC;

    screen_close();

    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    if (!RegisterClass(&wc)) return -1;

    screen_handle = CreateWindow(_T("SCREEN3.1415926"), title,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        0, 0, 0, 0, NULL, NULL, wc.hInstance, NULL);
    if (screen_handle == NULL) return -2;

    screen_exit = 0;
    hDC = GetDC(screen_handle);
    screen_dc = CreateCompatibleDC(hDC);
    ReleaseDC(screen_handle, hDC);

    screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);
    if (screen_hb == NULL) return -3;

    screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
    screen_fb = (unsigned char*)ptr;
    screen_w = w;
    screen_h = h;
    screen_pitch = w * 4;

    AdjustWindowRect(&rect, GetWindowLong(screen_handle, GWL_STYLE), 0);
    wx = rect.right - rect.left;
    wy = rect.bottom - rect.top;
    sx = (GetSystemMetrics(SM_CXSCREEN) - wx) / 2;
    sy = (GetSystemMetrics(SM_CYSCREEN) - wy) / 2;
    if (sy < 0) sy = 0;
    SetWindowPos(screen_handle, NULL, sx, sy, wx, wy, (SWP_NOCOPYBITS | SWP_NOZORDER | SWP_SHOWWINDOW));
    SetForegroundWindow(screen_handle);

    ShowWindow(screen_handle, SW_NORMAL);
    screen_dispatch();

    memset(screen_keys, 0, sizeof(int) * 512);
    memset(screen_fb, 0, w * h * 4);

    return 0;
}

int screen_close(void) {
    if (screen_dc) {
        if (screen_ob) {
            SelectObject(screen_dc, screen_ob);
            screen_ob = NULL;
        }
        DeleteDC(screen_dc);
        screen_dc = NULL;
    }
    if (screen_hb) {
        DeleteObject(screen_hb);
        screen_hb = NULL;
    }
    if (screen_handle) {
        CloseWindow(screen_handle);
        screen_handle = NULL;
    }
    return 0;
}

static LRESULT screen_events(HWND hWnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE: screen_exit = 1; break;
    case WM_KEYDOWN: screen_keys[wParam & 511] = 1; break;
    case WM_KEYUP: screen_keys[wParam & 511] = 0; break;
    default: return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

void screen_dispatch(void) {
    MSG msg;
    while (1) {
        if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) break;
        if (!GetMessage(&msg, NULL, 0, 0)) break;
        DispatchMessage(&msg);
    }
}

void screen_update(void) {
    HDC hDC = GetDC(screen_handle);
    BitBlt(hDC, 0, 0, screen_w, screen_h, screen_dc, 0, 0, SRCCOPY);
    ReleaseDC(screen_handle, hDC);
    screen_dispatch();
}


//=====================================================================
// 主程序
//=====================================================================
vertex_t mesh[8] = {
    { { -1, -1,  1 }, { 0, 0 }, { 1.0f, 0.2f, 0.2f }, 1 },
    { {  1, -1,  1 }, { 0, 1 }, { 0.2f, 1.0f, 0.2f }, 1 },
    { {  1,  1,  1 }, { 1, 1 }, { 0.2f, 0.2f, 1.0f }, 1 },
    { { -1,  1,  1 }, { 1, 0 }, { 1.0f, 0.2f, 1.0f }, 1 },
    { { -1, -1, -1 }, { 0, 0 }, { 1.0f, 1.0f, 0.2f }, 1 },
    { {  1, -1, -1 }, { 0, 1 }, { 0.2f, 1.0f, 1.0f }, 1 },
    { {  1,  1, -1 }, { 1, 1 }, { 1.0f, 0.3f, 0.3f }, 1 },
    { { -1,  1, -1 }, { 1, 0 }, { 0.2f, 1.0f, 0.3f }, 1 },
};

void draw_plane(device_t *device, int a, int b, int c, int d) {
    vertex_t p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
    p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
    p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
    device_draw_primitive(device, &p1, &p2, &p3);
    device_draw_primitive(device, &p3, &p4, &p1);
}

void draw_box(device_t *device, float theta) {
    matrix_t m;
    //matrix_set_rotate(&m, -1, -0.5, 1, theta);
    m.setRotate(-1.0f, -0.5f, 1.0f, theta);
    device->transform.world = m;
    transform_update(&device->transform);
    draw_plane(device, 0, 1, 2, 3);
    //draw_plane(device, 4, 5, 6, 7); //顺时针绕序修正
    draw_plane(device, 7, 6, 5, 4);
    draw_plane(device, 0, 4, 5, 1);
    draw_plane(device, 1, 5, 6, 2);
    draw_plane(device, 2, 6, 7, 3);
    draw_plane(device, 3, 7, 4, 0);
}

void camera_at_zero(device_t *device, float x, float y, float z) {
    //point_t eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 0, 1, 1 };
    point_t eye = point_t(x, y, z), at = point_t(0, 0, 0), up = point_t(0, 0, 1);
    //matrix_set_lookat(&device->transform.view, &eye, &at, &up);
    device->transform.view = lookAt(eye, at, up);
    transform_update(&device->transform);
}

void init_texture(device_t *device) {
    static IUINT32 texture[256][256];
    int i, j;
    for (j = 0; j < 256; j++) {
        for (i = 0; i < 256; i++) {
            int x = i / 32, y = j / 32;
            texture[j][i] = ((x + y) & 1) ? 0xffffff : 0x3fbcef;
        }
    }
    device_set_texture(device, texture, 256 * 4, 256, 256);
}

int main(void)
{
    device_t device;
    int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
    int mode[] = { CULL_MODE_NONE,CULL_MODE_FRONT,CULL_MODE_BACK };
    int indicator = 0;
    int kbhit = 0;
    float alpha = 1;
    float pos = 3.5;

    TCHAR *title = _T("Mini3d (software render tutorial) - ")
        _T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

    if (screen_init(800, 600, title))
        return -1;

    device_init(&device, 800, 600, screen_fb);
    camera_at_zero(&device, 3, 0, 0);

    init_texture(&device);
    device.render_state = RENDER_STATE_TEXTURE;
    
    device.cull_mode = CULL_MODE_BACK;
    //device.cull_mode = CULL_MODE_NONE;
    //device.cull_mode = CULL_MODE_FRONT;

    device.light.ambiStrength = 0.1f;
    device.light.lightColor = { 1.0f,1.0f,1.0f };
    device.light.direction = { 0.0f,0.0f,-1.0f };

    while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
        screen_dispatch();
        device_clear(&device, 1);
        camera_at_zero(&device, pos, 0, 0);

        if (screen_keys[VK_UP]) pos -= 0.01f;
        if (screen_keys[VK_DOWN]) pos += 0.01f;
        if (screen_keys[VK_LEFT]) alpha += 0.01f;
        if (screen_keys[VK_RIGHT]) alpha -= 0.01f;

        if (screen_keys[VK_SPACE]) {
            if (kbhit == 0) {
                kbhit = 1;
                if (++indicator >= 3) indicator = 0;
                device.render_state = states[indicator];
            }
        }
        else {
            kbhit = 0;
        }

        draw_box(&device, alpha);
        screen_update();
        Sleep(1);
    }
    return 0;
}

