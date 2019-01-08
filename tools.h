#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "vector_t.h"

bool cullface(vector_t &p1, vector_t &p2, vector_t &p3)
{
    vector_t v1, v2, normal;
    v1 = p2 - p1;
    v2 = p3 - p1;
    normal = crossProduct(v1, v2);
    normal.normalize();

    vector_t viewDirection(0.0f, 0.0f, 1.0f);
    
    if (normal*viewDirection >= 0)
        return true;
    else
        return false;
}

#endif // !_TOOLS_H_
