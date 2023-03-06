#ifndef MAIN_H
#define MAIN_H

#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "sprite_animation_manager.h"
#include "main.h"
#include "system_math.h"
#include "main_debug.h"

#define MAX_SPRITES   300
#define MAX_BULLETS   30

// Tamaño de las 2 pantallas. 

#define SCREEN_WIDTH_TOP  400
#define SCREEN_HEIGHT_TOP 240

#define SCREEN_WIDTH_BOT  320
#define SCREEN_HEIGHT_BOT 240

// Limites internos de las 2 pantallas. 

#define BG_LIMIT_LEFT_TOP 15.0f
#define BG_LIMIT_RIGHT_TOP 385.0f
#define BG_LIMIT_UP_TOP 25.0f
#define BG_LIMIT_DOWN_TOP 225.0f

#define BG_LIMIT_LEFT_BOT 15.0f
#define BG_LIMIT_RIGHT_BOT 305.0f
#define BG_LIMIT_UP_BOT 25.0f
#define BG_LIMIT_DOWN_BOT 225.0f

// Refresco mínimo de las animaciones de los sprites. 

#define ANIMATION_REFRESH_TIME_MIN  17

#define PLAYER_HP_START  5
#define BULLET_INITIAL_SPEED      10

typedef struct player_str {  // PLAYER OBJ
  union {
    struct {
      float x;
      float y;
    };
    vec2f pos;
    float p[1];
  };
  
  union {
    struct {
      float xspeed;
      float yspeed;
    };
    vec2f speed;
    float s[1];
  };
  object_2d_info_t* player_spr;
  object_2d_info_t* orb_spr;
  float radius;
  float power;  
  int health;
  int orbs;
  u32 effects;
  u32 color;
  union {
    struct {
      vec2f v1;
      vec2f v2;
      vec2f v3;
    };
    float vertices[XY_TOTAL]; // relative to local coordinates
  };
} player_str;

	typedef struct bullet_t {  // BULLET OBJ
    union {
    struct {
      float x;
      float y;
    };
    vec2f pos;
    float p[1];
  };

    union {
    struct {
      float xspeed;
      float yspeed;
    };
    vec2f speed;
    float s[1];
  };
  bool bot_screen;
   bool dup;
  float angle;
  int state;
  C2D_Sprite *sprite;
} bullet_t;


inline int inside_top_screen(float x, float y)
{
  return inside_rect(x, y, 0.0f, SCREEN_WIDTH_TOP, 0.0f, SCREEN_HEIGHT_TOP);
}

inline int inside_bottom_screen(float x, float y)
{
  return inside_rect(x, y, 0.0f, SCREEN_WIDTH_BOT, 0.0f, SCREEN_HEIGHT_BOT);
}

inline int inside_bottom(float x, float y)
{
  return inside_rect(x, y, 0.0f, SCREEN_WIDTH_BOT, 0.0f, SCREEN_HEIGHT_BOT);
}

	// Funciones

	void init_player();
	void Init_BG();
	void rotacionDelFondo(Sprite* fondo1, Sprite* fondo2);
	//void player_shot();
	//void moveSprites();
	void orb_movement();
	void move_player(float x, float y, u32 kHeld);
	void bullet_logic(void);
	void draw_bullets(void);
	

	
#endif