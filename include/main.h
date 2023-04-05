#ifndef MAIN_H
#define MAIN_H

#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>
#include <tex3ds.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "sprite_animation_manager.h"
#include "main.h"
#include "system_math.h"
#include "main_debug.h"

#define MAX_MINIONS              50
#define MAX_BOSSES               5
#define MAX_SPRITES   			     300
#define MAX_BULLETS   			     300
#define MAX_ENEMY_BULLETS        5000
#define MAX_ENEMY_SHIPS          32

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
#define BULLET_INITIAL_SPEED      15
#define PLAYER_BULLET_MS          120
#define PLAYER_BULLET_MS_POWERUP  100

#define GRACE_PERIOD_AFTER_HIT    120

typedef enum {
              ENEMY_STATE_INACTIVE,       // 0 
              ENEMY_STATE_ACTIVE,         // 1
              ENEMY_STATE_TOTAL           // 2
} enemy_state_t;

typedef enum {
              ORB_STATE_INACTIVE,       // 0 
              ORB_STATE_ACTIVE,         // 1
              ORB_STATE_TOTAL           // 2
} orb_state_t;

typedef enum {
              PICKUP_STATE_INACTIVE,       // 0 
              PICKUP_STATE_ACTIVE,         // 1
              PICKUP_STATE_TOTAL           // 2
} pickup_state_t;

typedef enum {
              BULLET_STATE_INACTIVE,       // 0 
              BULLET_STATE_ACTIVE,         // 1
              BULLET_STATE_TOTAL           // 2
} bullet_state_t;

typedef enum {
              SPRITE_BULLET_NORMAL,       // 0
              SPRITE_BULLET_ENEMY,        // 1
              SPRITE_BULLET_TOTAL         // 2
} bullet_spritesheet_idx_t;


typedef enum {
              SPRITE_ENEMY_NORMAL,        // 0
              SPRITE_ENEMY_BOOSTING,      // 1
              SPRITE_ENEMY_TOTAL          // 2
} enemy_spritesheet_idx_t;

typedef struct orb_obj {
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
  
  orb_state_t state;

  C2D_Sprite *spr;

} orb_obj;

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

  float radius;
  
  int health;

  int orbs;
  float power; 

  int skills;
  float skill_power; 

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




typedef struct enemy_ship_t {
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
  
  float angle;
  float radius;
  float turnrate;
  int attspeed;
  int last_frame_shot;
  int lives;
  float health;
  float current_health;
  enemy_state_t state;
  bool invencible;
  //loot_table_t loot_table;
  u32 color;
  union {
    struct {
      vec2f v1;
      vec2f v2;
      vec2f v3;
    };
    float vertices[XY_TOTAL]; // relative to local coordinates
  };
  //C2D_Sprite sprites[SPRITE_ENEMY_TOTAL];
  object_2d_info_t* enemy_spr;
  object_2d_info_t* barrier_spr;
  unsigned int curr_sprite;
} enemy_ship_t;

typedef struct minion {

  float health;
  float current_health;

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
  
  float angle;
  float radius;
  
  bool invencible;
  enemy_state_t state;
  //loot_table_t loot_table;
  object_2d_info_t* enemy_spr;
  
} minion;

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
  float spd;
  bool bot_screen;
  bool aim;
  float radius;
  float angle;
  int state;
  C2D_Sprite *sprite;
} bullet_t;

typedef struct bullet_enemy {  // BULLET OBJ
    union {
    struct {            // posicion
      float x;
      float y;
    };
    vec2f pos;
    float p[1];
  };

    union {
    struct {           // velocidad
      float xspeed; 
      float yspeed;
    };
    vec2f speed;
    float s[1];
  };
  float spd;        // angulo
  bool bot_screen;    // esta en pant. superior ?
  bool dup;           // esta duplicado ?
  float radius;
  float angle;        // angulo
  float rotation_spd;        // angulo
  float sprite_angle;        // angulo
  int state;          // estado
  C2D_Sprite *sprite; // sprite asociado
} bullet_enemy;

typedef struct laser {  // BULLET OBJ
    union {
    struct {            // posicion
      float x;
      float y;
    };
    vec2f pos;
    float p[1];
  };

    union {
    struct {           // velocidad
      float xspeed; 
      float yspeed;
    };
    vec2f speed;
    float s[1];
  };
  bool bot_screen;    // esta en pant. superior ?
  bool rot_horaria;    // esta en pant. superior ?
  bool dup;           // esta duplicado ?
  float max_scale;
  float rotation_speed;
  float spd;
  float angle;        // angulo
  int state;          // estado
  C2D_Sprite *sprite; // sprite asociado
} laser;

typedef struct barrage_enemy {  // BULLET OBJ
    union {
    struct {                  // posicion
      float x;
      float y;
    };
    vec2f pos;
    float p[1];
  };

    union {
    struct {                // velocidad
      float xspeed; 
      float yspeed;
    };
    vec2f speed;
    float s[1];
  };
  bool top_screen;          // esta en pant. superior ?
  bool dup;                 // esta duplicado ?
  float angle;              // angulo
  int state;                // estado
  C2D_Sprite *sprite;       // sprite asociado
  bullet_enemy *bullet;     // proyectil enemigo
} barrage_enemy;

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

inline void move_enemy_to(float x, float y, float accel, float max_accel, enemy_ship_t e)
{
	if(accel < max_accel){
	e.x += e.xspeed * accel;
	e.y += e.yspeed * accel;
	if(accel != 1.0f)
		accel += 0.1f;
	}
}

	// Funciones

	void init_player();
	void Init_BG();
	void rotacionDelFondo(Sprite* fondo1, Sprite* fondo2, float img_size, bool bottom);
	//void player_shot();
	//void moveSprites();
	void orb_movement();
	void move_player(float x, float y, u32 kHeld);
	void bullet_logic(void);
	void draw_bullets(void);
	

	
#endif