#ifndef MAIN_H
#define MAIN_H

#include <citro2d.h>
#include <citro3d.h>
#include <3ds.h>
#include <tex3ds.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <mutex>
#include <thread>



#include "sprite_animation_manager.h"
#include "system_math.h"
#include "drawing.h"
#include "audio.h"


#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define MAX_MINIONS              120
#define MAX_BOSSES               5
#define MAX_HEALS                20
#define MAX_ORB_POINTS           500
#define MAX_SPRITES   			     300
#define MAX_BULLETS   			     300
#define MAX_ENEMY_BULLETS        5000
#define MAX_ENEMY_SHIPS          120

// Tamaño de las 2 pantallas. 

#define SCREEN_WIDTH_TOP         400
#define SCREEN_HEIGHT_TOP        240

#define SCREEN_WIDTH_BOT         320
#define SCREEN_HEIGHT_BOT        240

// Limites internos de las 2 pantallas. 

#define BG_LIMIT_LEFT_TOP        15.0f
#define BG_LIMIT_RIGHT_TOP       385.0f
#define BG_LIMIT_UP_TOP          25.0f
#define BG_LIMIT_DOWN_TOP        225.0f

#define BG_LIMIT_LEFT_BOT        15.0f
#define BG_LIMIT_RIGHT_BOT       305.0f
#define BG_LIMIT_UP_BOT          25.0f
#define BG_LIMIT_DOWN_BOT        225.0f

// Refresco mínimo de las animaciones de los sprites. 

#define ANIMATION_REFRESH_TIME_MIN  17


#define PLAYER_HP_START           5
#define PLAYER_HP_MAX             8

#define PLAYER_SP_START           0
#define PLAYER_SP_MAX             7

#define PLAYER_TS_START           2
#define PLAYER_TS_MAX             5

#define PLAYER_SPEED_DIV          35
#define PLAYER_SPEED_FOCUS_DIV    100
#define BULLET_INITIAL_SPEED      15
#define PLAYER_BULLET_MS          120
#define PLAYER_BULLET_MS_POWERUP  100

#define GRACE_PERIOD_AFTER_HIT    120



typedef enum {
              GAME_STATE_INTRO,             // 0 
              GAME_STATE_MANIN_MENU,        // 1
              GAME_STATE_TUTORIAL,          // 2
              GAME_STATE_GAMEPLAY,          // 3
              GAME_STATE_CREDITS,           // 4
              GAME_STATE_LOADING            // 5
} game_state_t;

typedef enum {
              ENEMY_STATE_INACTIVE,         // 0 
              ENEMY_STATE_ACTIVE,           // 1
} enemy_state_t;

typedef enum {
              HEAL_STATE_INACTIVE,          // 0 
              HEAL_STATE_ACTIVE,            // 1
} heal_state_t;

typedef enum {
              ORB_POINT_STATE_INACTIVE,     // 0 
              ORB_POINT_STATE_ACTIVE,       // 1
} orb_point_state_t;

typedef enum {
              ORB_POINT_TYPE_Y,             // 0 
              ORB_POINT_TYPE_B,             // 1
} orb_point_type_t;

typedef enum {
              ORB_STATE_INACTIVE,           // 0 
              ORB_STATE_ACTIVE,             // 1
} orb_state_t;

typedef enum {
              BULLET_STATE_INACTIVE,        // 0 
              BULLET_STATE_ACTIVE,          // 1
} bullet_state_t;

extern std::mutex start_mutex;


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
  bool invencible;

  int orbs;
  int orb_points;
  float orb_energy;
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
  u32 color;
  union {
    struct {
      vec2f v1;
      vec2f v2;
      vec2f v3;
    };
    float vertices[XY_TOTAL];
  };
  object_2d_info_t* enemy_spr;
  object_2d_info_t* barrier_spr;
  unsigned int curr_sprite;
  bool invencible;
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
  float rotation_speed;
  float radius;
  
  bool invencible;
  enemy_state_t state;
  C2D_Sprite* enemy_spr;
  bool top;
  
} minion;


typedef struct heal {

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

  float radius;
 
  heal_state_t state;
  C2D_Sprite* heal_spr;
  bool top;
  
} heal;

typedef struct orb_point {

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

  float radius;
 
  orb_point_state_t state;
  C2D_Sprite* orb_p_spr;
  bool top;
  orb_point_type_t type;
  
} orb_point;

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
  float spd;       
  bool bot_screen; 
  bool dup;           
  float radius;
  float angle;       
  float rotation_spd;        
  float sprite_angle;        
  int state;  

  float graze_radius;
  float graze_power;        
  bool grazed;   

  C2D_Sprite *sprite; 

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

  enemy_ship_t spawn_enemy_ship(float x, float y, float xs, float ys, float r, int lives, float hp, 
									object_2d_info_t* e_spr, object_2d_info_t* b_spr){
	  
	enemy_ship_t new_enemy = {
	.x            		= x,
	.y            		= y,
	.xspeed       		= xs,
	.yspeed       		= ys,
	.radius       		= r,
	.lives        		= lives,
	.health       		= hp,
	.current_health 	= hp,
	.state        		= ENEMY_STATE_ACTIVE,  
	.enemy_spr    		= e_spr,
	.barrier_spr 	 	= b_spr,
	.invencible   		= false,
	};

	return new_enemy;

}
	
minion spawn_minion(float x, float y, float xs, float ys, float r, float rs, float hp, C2D_Sprite* e_spr, bool t) {
  
  minion new_minion  = {
    .health          = hp,
    .current_health  = hp,
    .pos             = { x, y },
    .speed           = { xs, ys },
    .angle           = 0,
    .rotation_speed  = rs,
    .radius          = r,
    .invencible      = false,
    .state           = ENEMY_STATE_ACTIVE,
    .enemy_spr       = e_spr,
    .top             = t,
  };
  
  return new_minion;

}

heal spawn_heal(float x, float y, bool t, C2D_Sprite* heal_item) {
  
  heal new_heal 	 = {
    .pos             = { x, y },
    .speed           = { 0, -1.0f },
    .radius          = 8,
    .state           = HEAL_STATE_ACTIVE,
    .heal_spr        = heal_item,
    .top             = t,
  };
  
  return new_heal;

}

orb_point spawn_orb_p(float x, float y, bool t, orb_point_type_t type, C2D_Sprite* orb_charge) {
  
  orb_point new_orb_p 	 = {
    .pos             = { x, y },
    .speed           = { 0, -0.05f },
    .radius          = 2,
    .state           = ORB_POINT_STATE_ACTIVE,
    .orb_p_spr       = orb_charge,
    .top             = t,
    .type            = type,
  };
  
  return new_orb_p;

}

void draw_orbs(orb_obj* player_orbs){

	int i = 0;
	for (; i < 4; i++) {
		if (player_orbs[i].state) { // not inactive
				C2D_SpriteSetPos(player_orbs[i].spr, player_orbs[i].x, player_orbs[i].y);
				C2D_DrawSprite(player_orbs[i].spr);
	    }
	}
}

void draw_bullets_bot(bullet_t* bullets, bullet_enemy* enemy_bullets){

	int i = 0;
	for (; i < MAX_BULLETS; i++) {
		if (bullets[i].state) { // not inactive
			if(bullets[i].bot_screen == true){
				C2D_SpriteSetPos(bullets[i].sprite, bullets[i].x, bullets[i].y);
				C2D_DrawSprite(bullets[i].sprite);
			}
	    }  
	}

	for (int j = 0; j < MAX_ENEMY_BULLETS; ++j) {
		if (enemy_bullets[j].state) { // not inactive
			if(enemy_bullets[j].bot_screen == true){
			  	C2D_SpriteSetPos(enemy_bullets[j].sprite, enemy_bullets[j].x, enemy_bullets[j].y);
			  	C2D_SpriteSetRotation(enemy_bullets[j].sprite, deg_to_rad(-enemy_bullets[j].sprite_angle + 90));
			  	C2D_DrawSprite(enemy_bullets[j].sprite);
			}
		}
	}
}
	
void draw_bullets_top(bullet_t* bullets, bullet_enemy* enemy_bullets){

	int i = 0;
	for (; i < MAX_BULLETS; i++) {
		if (bullets[i].state) { // not inactive
			if(bullets[i].bot_screen == false){
					C2D_SpriteSetPos(bullets[i].sprite, bullets[i].x, bullets[i].y);
					C2D_DrawSprite(bullets[i].sprite);
			}
	    }  
	}

	for (int j = 0; j < MAX_ENEMY_BULLETS; ++j) {
		if (enemy_bullets[j].state) { // not inactive
			if(enemy_bullets[j].bot_screen == false){
			  	C2D_SpriteSetPos(enemy_bullets[j].sprite, enemy_bullets[j].x, enemy_bullets[j].y);
			  	C2D_SpriteSetRotation(enemy_bullets[j].sprite, deg_to_rad(-enemy_bullets[j].sprite_angle + 90));
			  	C2D_DrawSprite(enemy_bullets[j].sprite);
			}
		}
	}

}

void draw_orb_points_bot(orb_point* orb_points){
	int i = 0;
	for (; i < MAX_ORB_POINTS; i++) {
		if (orb_points[i].state) { // not inactive
			if(orb_points[i].top == false){
					C2D_SpriteSetPos(orb_points[i].orb_p_spr, orb_points[i].x, orb_points[i].y);
					C2D_DrawSprite(orb_points[i].orb_p_spr);
			  }
	    }  
	}
}

void draw_orb_points_top(orb_point* orb_points){
	int i = 0;
	for (; i < MAX_ORB_POINTS; i++) {
		if (orb_points[i].state) { // not inactive
			if(orb_points[i].top == true){
					C2D_SpriteSetPos(orb_points[i].orb_p_spr, orb_points[i].x, orb_points[i].y);
					C2D_DrawSprite(orb_points[i].orb_p_spr);
			  }
	    }  
	}
}

void draw_heal_bot(heal* heals){
	int i = 0;
	for (; i < MAX_ORB_POINTS; i++) {
		if (heals[i].state) { // not inactive
			if(heals[i].top == false){
					C2D_SpriteSetPos(heals[i].heal_spr, heals[i].x, heals[i].y);
					C2D_DrawSprite(heals[i].heal_spr);
			  }
	    }  
	}
}

void draw_heal_top(heal* heals){
	int i = 0;
	for (; i < MAX_ORB_POINTS; i++) {
		if (heals[i].state) { // not inactive
			if(heals[i].top == true){
					C2D_SpriteSetPos(heals[i].heal_spr, heals[i].x, heals[i].y);
					C2D_DrawSprite(heals[i].heal_spr);
			  }
	    }  
	}
}











void orb_points_logic(orb_point* orb_points, player_str* player){
      
  for (int i = 0; i < MAX_ORB_POINTS/2; i++) {
        
    if (orb_points[i].state) { // not inactive

      orb_points[i].top = true;

      float x_diff = player->x - orb_points[i].x + 40;
      float y_diff = player->y - orb_points[i].y + 240;
      float distance = sqrt(x_diff * x_diff + y_diff * y_diff); // distancia a recorrer
      float speed = 6; // velocidad necesaria para recorrer la distancia en 2 segundos

      if(orb_points[i].type == ORB_POINT_TYPE_Y)
      {
        orb_points[i].xspeed = 0;
        orb_points[i].yspeed = 1.0f;
      }
      if(orb_points[i].type == ORB_POINT_TYPE_B)
      {
        orb_points[i].xspeed = x_diff * speed / distance;
        orb_points[i].yspeed = y_diff * speed / distance;
      }

      if (cirlce_inside_circle(orb_points[i].x, orb_points[i].y, player->x + 40, player->y + 240, player->radius, orb_points[i].radius + 30) && orb_points[i].type == ORB_POINT_TYPE_Y)
      {
        orb_points[i].xspeed = x_diff * speed / distance;
        orb_points[i].yspeed = y_diff * speed / distance;
      }  

      orb_points[i].x +=  orb_points[i].xspeed;
      orb_points[i].y +=  orb_points[i].yspeed;

    }
  } 
    
      
  for (int j = MAX_ORB_POINTS/2; j < MAX_ORB_POINTS; j++) {
        
    if (orb_points[j].state) { // not inactive

      orb_points[j].top = false;

      float x_diff = player->x - orb_points[j].x;
      float y_diff = player->y - orb_points[j].y;
      float distance = sqrt(x_diff * x_diff + y_diff * y_diff); // distancia a recorrer
      float speed = 6; // velocidad necesaria para recorrer la distancia en 2 segundos
        
        
      if(orb_points[j].type == ORB_POINT_TYPE_Y)
      {
        orb_points[j].xspeed = 0;
        orb_points[j].yspeed = 1.0f;
      }
      if(orb_points[j].type == ORB_POINT_TYPE_B)
      {
        orb_points[j].xspeed = x_diff * speed / distance;
        orb_points[j].yspeed = y_diff * speed / distance;
      }

      if (cirlce_inside_circle(orb_points[j].x, orb_points[j].y, player->x, player->y, player->radius, orb_points[j].radius + 5))
      {
        if(orb_points[j].type == ORB_POINT_TYPE_Y)
        player->orb_energy += 10; // CAMBIAR AQUÍ

        if(orb_points[j].type == ORB_POINT_TYPE_B)
        player->orb_energy += 4; // CAMBIAR AQUÍ

        orb_points[j].state = ORB_POINT_STATE_INACTIVE;
        orb_points[j-MAX_ORB_POINTS/2].state = ORB_POINT_STATE_INACTIVE;
      }  

      if (cirlce_inside_circle(orb_points[j].x, orb_points[j].y, player->x, player->y, player->radius, orb_points[j].radius + 30) && orb_points[j].type == ORB_POINT_TYPE_Y)
      {
        orb_points[j].xspeed = x_diff * speed / distance;
        orb_points[j].yspeed = y_diff * speed / distance;
      }  


      if(!inside_rect(orb_points[j].x, orb_points[j].y, -10, 410, -500, 600) 
				&& orb_points[j].top == false)
		  {
			  orb_points[j].state = ORB_POINT_STATE_INACTIVE;
        orb_points[j-MAX_ORB_POINTS/2].state = ORB_POINT_STATE_INACTIVE;
		  }
      
      orb_points[j].x +=  orb_points[j].xspeed;
      orb_points[j].y +=  orb_points[j].yspeed;

    }
  }
}

void heal_logic(heal* heals, player_str* player){
      
  for (int i = 0; i < MAX_HEALS/2; i++) {
        
    if (heals[i].state) { // not inactive

      heals[i].top = true;

      heals[i].xspeed = 0;
      if(heals[i].yspeed < 1.8f)
      heals[i].yspeed += 0.02f;

      heals[i].x +=  heals[i].xspeed;
      heals[i].y +=  heals[i].yspeed;

    }
  } 
    
      
  for (int j = MAX_HEALS/2; j < MAX_HEALS; j++) {
        
    if (heals[j].state) { // not inactive

      heals[j].top = false;
        
      heals[j].xspeed = 0;
      if(heals[j].yspeed < 1.8f)
      heals[j].yspeed += 0.02f;

      heals[j].x +=  heals[j].xspeed;
      heals[j].y +=  heals[j].yspeed;


      if (cirlce_inside_circle(heals[j].x, heals[j].y, player->x, player->y, player->radius, heals[j].radius + 7))
      {
        player->health += 1;
        heals[j].state = HEAL_STATE_INACTIVE;
        heals[j-MAX_HEALS/2].state = HEAL_STATE_INACTIVE;
      }  

      if(!inside_rect(heals[j].x, heals[j].y, -10, 410, -500, 600) 
				&& heals[j].top == false) // TOP
		  {
			  heals[j].state = HEAL_STATE_INACTIVE;
        heals[j-MAX_HEALS/2].state = HEAL_STATE_INACTIVE;
		  }

    }
  }
}






void shoot_enemy_bullet(float x, float y, float angle, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, C2D_Sprite *sprite, bool bot, bullet_enemy* enemy_bullets){

	int i = 0;
	for (; i < MAX_ENEMY_BULLETS/2; ++i) {

		if (!enemy_bullets[i].state) { // inactive, free to use slot
			  
			if(!bot)
			{
				enemy_bullets[i].x = x;
				enemy_bullets[i].y = y;
			}
			else
			{
				enemy_bullets[i].x = x + 40;
				enemy_bullets[i].y = y + 240;				  
			}
			  
			enemy_bullets[i].bot_screen = bot;
			  
			enemy_bullets[i].radius = radius;
			enemy_bullets[i].angle = deg_to_rad(angle);
			  
			enemy_bullets[i].spd = speed;
			enemy_bullets[i].rotation_spd = rotation_spd;
			  
			enemy_bullets[i].xspeed = speed * cosf(deg_to_rad(-angle));
			enemy_bullets[i].yspeed = speed * sinf(deg_to_rad(-angle));

			enemy_bullets[i].sprite = sprite;
			enemy_bullets[i].sprite_angle = angle;

			enemy_bullets[i].grazed = false;
			enemy_bullets[i].graze_radius = graze_radius;
			enemy_bullets[i].graze_power = graze_power;
			  
			enemy_bullets[i].state = BULLET_STATE_ACTIVE;
			 
			break;

		}	
	}

	int j = MAX_ENEMY_BULLETS/2;
	for (; j < MAX_ENEMY_BULLETS; ++j) {

		if (!enemy_bullets[j].state) { // inactive, free to use slot
			  
			if(!bot)
			{
				enemy_bullets[j].x = x - 40;
				enemy_bullets[j].y = y - 240;
			}
			else
			{
				enemy_bullets[j].x = x;
				enemy_bullets[j].y = y;				  
			}
			  
			enemy_bullets[j].bot_screen = !bot;
			  
			enemy_bullets[j].radius = radius;
			enemy_bullets[j].angle = deg_to_rad(angle);
			  
			enemy_bullets[j].spd = speed;
			enemy_bullets[j].rotation_spd = rotation_spd;
			  
			enemy_bullets[j].xspeed = speed * cosf(deg_to_rad(-angle));
			enemy_bullets[j].yspeed = speed * sinf(deg_to_rad(-angle));

			enemy_bullets[j].sprite = sprite;
			enemy_bullets[j].sprite_angle = angle;

			enemy_bullets[j].grazed = false;
			enemy_bullets[j].graze_radius = graze_radius;
			enemy_bullets[j].graze_power = graze_power;
			  
			enemy_bullets[j].state = BULLET_STATE_ACTIVE;
			 
			break;

		}	
	}
}


void shoot_enemy_bullet_aim(float x, float y, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, C2D_Sprite *sprite, bool bot, bullet_enemy* enemy_bullets,  player_str* player_object){
	  
	int i = 0;
	for (; i < MAX_ENEMY_BULLETS/2; ++i) {

		if (!enemy_bullets[i].state) { // inactive, free to use slot
			  
			float dxt, dyt, tangle;
			  
			if(!bot)
			{
				dxt = (player_object->x + 40) - x;
				dyt = (player_object->y + 240) - y;

				enemy_bullets[i].x = x;
				enemy_bullets[i].y = y;
			}
			else
			{
				dxt = (player_object->x + 40) - (x + 40);
				dyt = (player_object->y + 240) - (y + 240);

				enemy_bullets[i].x = x + 40;
				enemy_bullets[i].y = y + 240;				  
			}
			   
      tangle = atan2f(dyt, dxt);
			  
			enemy_bullets[i].bot_screen = false;
			  
			enemy_bullets[i].radius = radius;
			enemy_bullets[i].angle = tangle;
			enemy_bullets[i].rotation_spd = rotation_spd;
			  
			enemy_bullets[i].spd = speed;
			  
			enemy_bullets[i].xspeed = speed * cosf(tangle);
			enemy_bullets[i].yspeed = speed * sinf(tangle);
			  
			enemy_bullets[i].sprite = sprite;
			enemy_bullets[i].sprite_angle = rad_to_deg(-tangle);
			  
			enemy_bullets[i].state = BULLET_STATE_ACTIVE;
			  
			break;

		}	
	}

	int j = MAX_ENEMY_BULLETS/2;
	for (; j < MAX_ENEMY_BULLETS; ++j) {

		if (!enemy_bullets[j].state) { // inactive, free to use slot
			  
			float dxb, dyb, bangle;
			  
			if(!bot)
			{
				dxb = player_object->x - (x - 40);
				dyb = player_object->y - (y - 240);
				
				enemy_bullets[j].x = x - 40;
				enemy_bullets[j].y = y - 240;
			}
			else
			{
				dxb = player_object->x - x;
				dyb = player_object->y - y;
				
				enemy_bullets[j].x = x;
				enemy_bullets[j].y = y;				  
			}
			   
            bangle = atan2f(dyb, dxb);
			  
			enemy_bullets[j].bot_screen = true;
			  
			enemy_bullets[j].radius = radius;
			enemy_bullets[j].angle = bangle;
			enemy_bullets[j].rotation_spd = rotation_spd;
			  
			enemy_bullets[j].spd = speed;
			  
			enemy_bullets[j].xspeed = speed * cosf(bangle);
			enemy_bullets[j].yspeed = speed * sinf(bangle);

			enemy_bullets[j].sprite = sprite;
			enemy_bullets[j].sprite_angle = rad_to_deg(-bangle);
			  
			enemy_bullets[j].state = BULLET_STATE_ACTIVE;

			break;

		}	
	}
}


void burst(float x, float y, float angle, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, int total_bullets, C2D_Sprite *sprite, bool bot, bullet_enemy* e){

	int i = 0;
	for (; i < total_bullets; ++i) {

		shoot_enemy_bullet(x, y, (360/total_bullets) * i + angle, speed, rotation_spd, radius, graze_radius, graze_power, sprite, bot, e);
			
  }
}

bool time_passed(u32 my_time, float time_ms){
    
  u32 current_time = osGetTime();

	if (current_time - my_time >= time_ms) 
  return true;
	else
	return false;

}

bool hasElapsed(int x, const std::chrono::steady_clock::time_point& start)
{

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();

    return elapsed >= x;
}





void init_player();
void Init_BG();
void rotacionDelFondo(Sprite* fondo1, Sprite* fondo2, float img_size, bool bottom);
void orb_movement();
void move_player(float x, float y, u32 kHeld);
void bullet_logic(void);
void draw_bullets(void);
	

	
#endif