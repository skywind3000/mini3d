#ifndef CAMERA_T_H
#define CAMERA_T_H

#include "matrix_t.h"
#include "vector_t.h"

// 虚拟按键定义
#define VK_A 0x41
#define VK_D 0x44
#define VK_S 0x53
#define VK_W 0x57


// 平移方向
enum cameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class camera_t
{
public:
    vector_t position;
    vector_t front;
    vector_t up;
    vector_t right;
    vector_t worldUp;

    float movementSpeed;

    // 默认构造函数，相机默认位置：0，0，0 默认方向：0，0，1
    camera_t() :position(vector_t(0.0f, 0.0f, 0.0f)),worldUp(vector_t(0.0f,1.0f,0.0f)), front(vector_t(0.0f, 0.0f, 1.0f)), movementSpeed(1.0f)
    {
        right = crossProduct(worldUp, front);
        right.normalize();
        up = crossProduct(front, right);
        up.normalize();
    }

    matrix_t getViewMatrix()
    {
        return lookAt(position, position + front, up);
    }

    void processKeyBoard(cameraMovement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        switch (direction)
        {
        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        default:
            break;
        }
    }
};


#endif // !CAMERA_T_H
