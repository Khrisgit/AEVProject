#ifndef BACKGROUND_H
#define BACKGROUND_H
#include <citro2d.h>
#include "myType.h"

typedef struct
{
	C2D_Sprite spr;
    bool isActive;
	bool collider;
} Block;
typedef struct 
{
    Block *blocks;
    int r,c;
    float playerX,playerY;
    C2D_Sprite background;
}Background;

Background level1();
Enemy* level1Enemy(int* num);

#endif