#include "sprite_animation_manager.h"
#include "main.h"


// Atributos requeridos por las animaciones de sprites. 

static object_2d_info_t player_object, orb_object, boss1_object;

static C2D_Sprite player_sprite[8], orb_sprite[MAX_SPRITES], boss1_sprite[5];

static sprite_pivot_t player_pivot = {0.5f, 0.5f}, orb_pivot = {0.5f, 0.5f},
boss1_pivot = {0.5f, 0.5f};

static sprite_position_t player_position = {SCREEN_WIDTH_BOT/2, SCREEN_HEIGHT_BOT/2},
orb_position = {0.0f, 0.0f}, boss1_position = {SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2};

static float player_roation = 0.0f, orb_rotation = 0.0f, boss1_rotation = 0.0f;

static float player_roation_speed = 0.0f, orb_rotation_speed = 2.0f, boss1_rotation_speed = 0.0f;

static uint64_t sprite_refresh_ms_time = 100, orb_refresh_ms_time = 100, boss1_refresh_ms_time = 100 ;


// Atributos de sprites. 

static C2D_SpriteSheet general_spritesheet;

static Sprite sprites[MAX_SPRITES];

Sprite* fondo1;
Sprite* fondo2;
Sprite* fondoout;

u32 bulletmask;
bullet_t          bullets[MAX_BULLETS];
player_str player;
C2D_Sprite        bullet_normal_sprite;

// Atributos de control. 

static bool focus = false; 	// Indica si el PJ está en estado "focus" 
static float r = 35.0f; 	// Radio de giro de los orbes del PJ
static float t = 0.0f;  	// Tiempo transcurrido para el cálculo de movimiento de los orbes
//static size_t bullets = 0;  // ID de los proyectiles del PJ

static bool buttonPressed = false;  // Condición de botón presionado
static u32 lastPrintTime = 0;		// Tiempo transcurrido

bool rotacionFondo = true;	// ID de los proyectiles del PJ

	void init_player()
	{
		initialize_object(&player_object, \
						  player_sprite,	\
						  "romfs:/gfx/player.t3x", \
						  player_pivot, \
						  player_position, \
						  player_roation, \
						  player_roation_speed, \
						  sprite_refresh_ms_time, \
						  false);
						  
		initialize_object(&orb_object, \
						  orb_sprite,	\
						  "romfs:/gfx/orb.t3x", \
						  orb_pivot, \
						  orb_position, \
						  orb_rotation, \
						  orb_rotation_speed, \
						  orb_refresh_ms_time, \
						  false);
						  
		initialize_object(&boss1_object, \
						  boss1_sprite,	\
						  "romfs:/gfx/Boss-1.t3x", \
						  boss1_pivot, \
						  boss1_position, \
						  boss1_rotation, \
						  boss1_rotation_speed, \
						  boss1_refresh_ms_time, \
						  false);
						  
		player.x = SCREEN_WIDTH_BOT/2;
		player.y = SCREEN_HEIGHT_BOT/2;
		player.yspeed = 0.0f;
		player.xspeed = 0.0f;
		player.radius = 10.0f;
		player.health = PLAYER_HP_START;
		//player.color  = RED;
		player.effects &= 0;
		player.player_spr = &player_object;
		player.orb_spr = &orb_object;
		
	    player.vertices[X0] = -player.radius;        // vertex 0
	    player.vertices[Y0] =  player.radius;
	    player.vertices[X1] =  0.0f;                      // vertex 1
	    player.vertices[Y1] = -player.radius*2.0f;
	    player.vertices[X2] =  player.radius;        // vertex 2
	    player.vertices[Y2] =  player.radius;
		
		C2D_SpriteFromSheet(&bullet_normal_sprite, general_spritesheet, 0);
		C2D_SpriteSetCenter(&bullet_normal_sprite, 0.5f, 0.5f);
		
	}

	void Init_BG()
	{
		
		fondo1 = &sprites[0];
		C2D_SpriteFromSheet(&fondo1->spr, general_spritesheet, 2);
		C2D_SpriteSetCenter(&fondo1->spr, 0.5f, 1.0f);
		C2D_SpriteSetPos(&fondo1->spr, SCREEN_WIDTH_TOP/2, 240.0f);
		fondo1->dy = 1.0f;		
		
		fondo2 = &sprites[1];
		C2D_SpriteFromSheet(&fondo2->spr, general_spritesheet, 2);
		C2D_SpriteSetCenter(&fondo2->spr, 0.5f, 1.0f);
		C2D_SpriteSetPos(&fondo2->spr, SCREEN_WIDTH_TOP/2, 0.0f);
		fondo2->dy = 1.0f;	
		
		// fondoout = &sprites[2];
		// C2D_SpriteFromSheet(&fondoout->spr, general_spritesheet, 2);
		// C2D_SpriteSetCenter(&fondoout->spr, 0.5f, 1.0f);
		// C2D_SpriteSetPos(&fondoout->spr, SCREEN_WIDTH_TOP/2, 0.0f);
		
	}
	

	//---------------------------------------------------------------------------------
	 void rotacionDelFondo(Sprite* fondo1, Sprite* fondo2){
	//---------------------------------------------------------------------------------

		if(fondo2->spr.params.pos.y == SCREEN_HEIGHT_TOP)
					{
						C2D_SpriteSetPos(&fondo1->spr, SCREEN_WIDTH_TOP/2, 0.0f);
						rotacionFondo = true;
					}		
		if(rotacionFondo == true)
		{
			if(fondo1->spr.params.pos.y==SCREEN_HEIGHT_TOP && fondo2->spr.params.pos.y> 300.0f)
				{
						C2D_SpriteSetPos(&fondo2->spr, SCREEN_WIDTH_TOP/2, 0.0f);
						
				}	
			if(fondo2->spr.params.pos.y==SCREEN_HEIGHT_TOP && fondo1->spr.params.pos.y> 300.0f)
				{
						C2D_SpriteSetPos(&fondo1->spr, SCREEN_WIDTH_TOP/2, 0.0f);
				}			
		}	
}
	
	
	 // void player_shot()    // Función que genera los proyectiles del PJ.
	// {
		 ////EN PROGRESO
		// if(bullets < MAX_SPRITES / 2)	 // si el nº de proyectiles es < MAX_SPRITES / 2
		// {	
			
			// Sprite* sprite = &shots[bullets];						 //  agarra el sprite con ID = bullets

			// C2D_SpriteFromSheet(&sprite->spr, general_spritesheet, 0);	
			// C2D_SpriteSetCenter(&sprite->spr, 0.5f, 0.5f);
			
			// sprite->x = orb_object.position.x;						//  pos x del proyetil = pos x del orbe
			// sprite->y = orb_object.position.y;						//  pos y del proyetil = pos y del orbe
			// C2D_SpriteSetPos(&sprite->spr, sprite->x, sprite->y);
			
			// sprite->dx = ((float) rand() / RAND_MAX) * 2 - 1;		//  dx aleatoria para dar sensación de desvío a los lados
			// sprite->dy = -12.0f;									//  dy fija hacia arriba
			
			// sprite->pasa = false;	//  aún no pasa a la otra pantalla
			
			// bullets++;	//  Actualiza la lista bullets + 1 para generar el siguiente proyectil
			
		// } else { bullets = 100; }	//  si nº proyectiles > MAX_SPRITES / 2 borra los sprites más antiguos lanzados
		
	// }
	
	void shoot_bullet(void)
	{
	  /* Check first free bullet size in array */
	  int i = 0;
	  for (; i < MAX_BULLETS; i++)
		if (~(bulletmask >> i) & 1) break;
	  

	  /* If there is none ignore bullet fire */
	  if (i >= MAX_BULLETS) return;
	  
	  /* Otherwise initialize bullet */
	  bulletmask = bulletmask | (1 << i);
	  bullets[i].x = orb_object.position.x;
	  bullets[i].y = orb_object.position.y;


	  bullets[i].xspeed = 0;
	  bullets[i].yspeed = -BULLET_INITIAL_SPEED;
	  bullets[i].sprite = &bullet_normal_sprite;
	  bullets[i].bot_screen = true;
	  bullets[i].dup = false;
	}
	
	void shoot_bullet_2(float x, float y)
	{
	  /* Check first free bullet size in array */
	  int i = 0;
	  for (; i < MAX_BULLETS; i++)
		if (~(bulletmask >> i) & 1) break;
	  

	  /* If there is none ignore bullet fire */
	  if (i >= MAX_BULLETS) return;
	  
	  /* Otherwise initialize bullet */
	  bulletmask = bulletmask | (1 << i);
	  bullets[i].x = x + 40;
	  bullets[i].y = 240 + y;


	  bullets[i].xspeed = 0;
	  bullets[i].yspeed = -BULLET_INITIAL_SPEED;
	  bullets[i].sprite = &bullet_normal_sprite;
	  bullets[i].bot_screen = false;
	  bullets[i].dup = false;
	}
	
	void draw_bullets(void)
	{

	  // for (int j = 0; j < MAX_ENEMY_BULLETS; ++j) {
		// if (enemy_bullets[j].state) { // not inactive
		  // C2D_SpriteSetPos(enemy_bullets[j].sprite, enemy_bullets[j].x, enemy_bullets[j].y);
		  // C2D_SpriteSetRotation(enemy_bullets[j].sprite, deg_to_rad(-enemy_bullets[j].angle+90.0f));
		  // C2D_DrawSprite(enemy_bullets[j].sprite);
		// }
	  // }
	  
	  int i = 0;
	    for (; i < MAX_BULLETS; i++) {
		    if (bulletmask >> i & 1) {
				if(bullets[i].bot_screen == true){
					C2D_SpriteSetPos(bullets[i].sprite, bullets[i].x, bullets[i].y);
					C2D_DrawSprite(bullets[i].sprite);
				}
	        }  
	    }
	  
	  
	}
	
		void draw_bullets_top(void)
	{

	  // for (int j = 0; j < MAX_ENEMY_BULLETS; ++j) {
		// if (enemy_bullets[j].state) { // not inactive
		  // C2D_SpriteSetPos(enemy_bullets[j].sprite, enemy_bullets[j].x, enemy_bullets[j].y);
		  // C2D_SpriteSetRotation(enemy_bullets[j].sprite, deg_to_rad(-enemy_bullets[j].angle+90.0f));
		  // C2D_DrawSprite(enemy_bullets[j].sprite);
		// }
	  // }
	  
	  int i = 0;
	    for (; i < MAX_BULLETS; i++) {
		    if (bulletmask >> i & 1) {
				if(bullets[i].bot_screen == false){
					C2D_SpriteSetPos(bullets[i].sprite, bullets[i].x, bullets[i].y);
					C2D_DrawSprite(bullets[i].sprite);
				}
	        }  
	    }
	}
	
	/*
	void moveSprites()   // Función que mueve cada proyectil (sprite sin animación) lanzado por el PJ.
	{
		for (size_t i = 0; i < bullets; i++) // para todos los proyectiles que pueden haber...
		{
			Sprite* sprite = &shots[i];	 // agarra el sprite en la pos "i"
			//Sprite* spriteup = &shotsup[i];
			// mueve cada sprite a partir de su velocidad
			C2D_SpriteMove(&sprite->spr, sprite->dx, sprite->dy);

			//if(&sprite->dx, )


			//  EN PROGRESO
			
			//Comprueba colisiones con los bordes de la pantalla       
			if ((sprite->spr.params.pos.y < 
					 ((sprite->spr.params.pos.h / 2.0f) - 50.0f)) && sprite->dy < 0.0f){
						
						sprite->pasa = true;
			}	
		}
	}*/
	
	
	void orb_movement()		// Función que determina el movimiento del orbe que gira alrededor del PJ.
	{	
		
		if(!focus)			// Si el PJ   NO   está "focus" (el orbe debe de girar alrededor del PJ)
		{
			
			t += 0.001f;	// El tiempo sigue pasando
		
			 // ARREGLAR LA VUELTA DE FOCUS A NORMAL
			 // IF NO SE CUMPLE
			
			// Si el orbe se encuentra dentro de la esfera que debe de recorrer
			
			if(pow(r,2) >
			(pow(player_object.position.x + r * cos(t * (180.0f / M_PI)),2) + 
			pow(player_object.position.y - r * sin(t * (180.0f / M_PI)),2)))
			{
				// velocidad = punto donde se encuentra
				orb_object.position_velocity.dx = 
				(player_object.position.x + (player_object.position.x + r * cos(t * (180.0f / M_PI)))) / 5;
				orb_object.position_velocity.dy = 
				(player_object.position.y + (player_object.position.y - r * sin(t * (180.0f / M_PI)))) /5;
			}
			else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
			{		
				//  posición del orbe = posición del PJ + giro alrededor
				orb_object.position.x = player_object.position.x + r * cos(t * (180.0f / M_PI));
				orb_object.position.y = player_object.position.y - r * sin(t * (180.0f / M_PI));
			}
		}
		else  // si el PJ    SI   está en modo "focus"
		{
			// si el orbe no está enfrente del PJ
			if(orb_object.position.x != player_object.position.x &&
			orb_object.position.y != player_object.position.y - 20.0f)
			{
				// velocidad del orbe = posición del PJ - 20 Y  (el orbe se mueve hasta donde está el PJ (-20 en la Y, es dcir, frente a él))
				orb_object.position_velocity.dx = (player_object.position.x - orb_object.position.x) / 5;
				orb_object.position_velocity.dy = ((player_object.position.y - 20.0f) - orb_object.position.y)/5;
			}
		}
		
	}
	

	void player_logic(float x, float y, u32 kHeld)
	{

	  /* If player is dead, reset game and return */
	  // TODO: game-over screen and better things here
	  // if (player.health == 0) {
		// player.effects &= 0;
		// player.effects |= (PLAYER_EFFECT_DEAD);
		// game_state = GAMEOVER_GAMESTATE;
		// gameover_frame = framecount;
		// return;
	  //}
	  /* Make sure we don't overheal */
	  //player.health = player.health > PLAYER_STARTING_HP ? PLAYER_STARTING_HP : player.health;
	  
	  /* Apply input sensitivity */
	  
	  		if ((player_object.position.y <= BG_LIMIT_UP_BOT) && (player_object.position_velocity.dy < 0))
		{
			if(!focus)  // si el PJ NO está "focus"
			{
				player.xspeed = x/33;				
				player.y = BG_LIMIT_UP_BOT;				
				player_object.position_velocity.dx = x/33;	// velocidad normal del PJ en X (izq y derecha)
				player_object.position.y = BG_LIMIT_UP_BOT; // posición permanente en ell borde
			} 
			else 		// si el PJ SI está "focus"
			{
				player.xspeed = x/100;				
				player.y = BG_LIMIT_UP_BOT;				
				player_object.position_velocity.dx = x/100; // velocidad reducida del PJ en X
				player_object.position.y = BG_LIMIT_UP_BOT; // posición permanente en ell borde
			}
		}
		if ((player_object.position.y >= BG_LIMIT_DOWN_BOT) && (player_object.position_velocity.dy > 0))
		{
			if(!focus)
			{
				player.xspeed = x/33;				
				player.y = BG_LIMIT_DOWN_BOT;				
				player_object.position_velocity.dx = x/33;
				player_object.position.y = BG_LIMIT_DOWN_BOT;
			} 
			else 
			{
				player.xspeed = x/100;				
				player.y = BG_LIMIT_DOWN_BOT;
				player_object.position_velocity.dx = x/100;
				player_object.position.y = BG_LIMIT_DOWN_BOT;
			}
		}
		if ((player_object.position.x <= BG_LIMIT_LEFT_BOT) && (player_object.position_velocity.dx < 0))
		{
			if(!focus)
			{
				player.yspeed = -y/33;
				player.x = BG_LIMIT_LEFT_BOT;				
				player_object.position_velocity.dy = -y/33;
				player_object.position.x = BG_LIMIT_LEFT_BOT;
			} 
			else 
			{
				player.yspeed = -y/100;
				player.x = BG_LIMIT_LEFT_BOT;				
				player_object.position_velocity.dy = -y/100;
				player_object.position.x = BG_LIMIT_LEFT_BOT;
			}
		}
		if ((player_object.position.x >= BG_LIMIT_RIGHT_BOT) && (player_object.position_velocity.dx > 0))
		{
			if(!focus)
			{
				player.yspeed = -y/33;
				player.x = BG_LIMIT_RIGHT_BOT;				
				player_object.position_velocity.dy = -y/33;
				player_object.position.x = BG_LIMIT_RIGHT_BOT;
			} 
			else 
			{
				player.yspeed = -y/100;
				player.x = BG_LIMIT_RIGHT_BOT;
				player_object.position_velocity.dy = -y/100;
				player_object.position.x = BG_LIMIT_RIGHT_BOT;
			}
		}
	
		if(!focus)
		{
		
			player.xspeed = x/33;
			player.yspeed = -y/33;
			player_object.position_velocity.dx = x/33;
			player_object.position_velocity.dy = -y/33;		 
		} 
		else 
		{
			player.xspeed = x/100;
			player.yspeed = -y/100;
			player_object.position_velocity.dx = x/100;
			player_object.position_velocity.dy = -y/100;			
		}
		
		player.x += player.xspeed;
		player.y += player.yspeed;
		orb_movement();
		
	  
	  // xinput = xinput * xinput_sensitivity;
	  // yinput = yinput * yinput_sensitivity;
	  
	  /* Update angle based on player input */
	  // float new_angle = player.angle - xinput;
	  // player.angle = clamp_deg(new_angle);

	  /* Cache cos and sin for the duration of the frame */
	  // float fsin = sin(deg_to_rad(new_angle));
	  // float fcos = cos(deg_to_rad(new_angle));

	  /* Update speed vector */
	  // player.yspeed = player.yspeed + yinput;
	  // player.xspeed = player.xspeed + yinput;
	  
	  /* Update position */
	  // float old_x = player.x;
	  // float new_x = old_x + player.xspeed;

	  // float old_y = player.y;
	  // float new_y = old_y + player.yspeed;

	  // if (new_y > TOP_SCREEN_HEIGHT) {
		// new_y = 0.0f;
		// PRINTDLOGIC("Ship went out of bounds downwards\n");
	  // } else if (new_y < 0) {
		// new_y = (float) TOP_SCREEN_HEIGHT;
		// PRINTDLOGIC("Ship went out of bounds upwards\n");
	  // }

	  // if (new_x > TOP_SCREEN_WIDTH) {
		// new_x = 0;
		// PRINTDLOGIC("Ship went out of bounds rightwards\n");
	  // } else if (new_x < 0) {
		// new_x = (float) TOP_SCREEN_WIDTH;
		// PRINTDLOGIC("Ship went out of bounds leftwards\n");
	  // }
	  
	  // player.y = new_y;
	  // player.x = new_x;

	  /* Update vertex positions for figure */
	  // rotate_2f_deg(&player.v1, xinput);
	  // rotate_2f_deg(&player.v2, xinput);
	  // rotate_2f_deg(&player.v3, xinput);

	  /* Get rid / apply blinking effect if needed */
	  // player.effects &= framecount - last_hit_frame < GRACE_PERIOD_AFTER_HIT ?
		// (~0) : ~(PLAYER_EFFECT_BLINKING);
	}


	void move_player(float x, float y, u32 kHeld)	// Función que se encarga de mover al PJ por la pantalla INFERIOR sin salirse de ella.
	{ 
		// Si el PJ está en el borde SUPERIOR de la pantalla Y quiere ir hacia ARRIBA (dy negativa)
		if ((player_object.position.y <= BG_LIMIT_UP_BOT) && (player_object.position_velocity.dy < 0))
		{
			if(!focus)  // si el PJ NO está "focus"
			{
				player_object.position_velocity.dx = x/33;	// velocidad normal del PJ en X (izq y derecha)
				player_object.position.y = BG_LIMIT_UP_BOT; // posición permanente en ell borde
			} 
			else 		// si el PJ SI está "focus"
			{
				player_object.position_velocity.dx = x/100; // velocidad reducida del PJ en X
				player_object.position.y = BG_LIMIT_UP_BOT; // posición permanente en ell borde
			}
		}
		if ((player_object.position.y >= BG_LIMIT_DOWN_BOT) && (player_object.position_velocity.dy > 0))
		{
			if(!focus)
			{
				player_object.position_velocity.dx = x/33;
				player_object.position.y = BG_LIMIT_DOWN_BOT;
			} 
			else 
			{
				player_object.position_velocity.dx = x/100;
				player_object.position.y = BG_LIMIT_DOWN_BOT;
			}
		}
		if ((player_object.position.x <= BG_LIMIT_LEFT_BOT) && (player_object.position_velocity.dx < 0))
		{
			if(!focus)
			{
				player_object.position_velocity.dy = -y/33;
				player_object.position.x = BG_LIMIT_LEFT_BOT;
			} 
			else 
			{
				player_object.position_velocity.dy = -y/100;
				player_object.position.x = BG_LIMIT_LEFT_BOT;
			}
		}
		if ((player_object.position.x >= BG_LIMIT_RIGHT_BOT) && (player_object.position_velocity.dx > 0))
		{
			if(!focus)
			{
				player_object.position_velocity.dy = -y/33;
				player_object.position.x = BG_LIMIT_RIGHT_BOT;
			} 
			else 
			{
				player_object.position_velocity.dy = -y/100;
				player_object.position.x = BG_LIMIT_RIGHT_BOT;
			}
		}
	
		if(!focus)
		{
		
			player_object.position_velocity.dx = x/33;
			player_object.position_velocity.dy = -y/33;
		 
		} 
		else 
		{
			player_object.position_velocity.dx = x/100;
			player_object.position_velocity.dy = -y/100;
		}
		
		orb_movement();
		
	}
	
	
	void bullet_logic(void)
	{
	  /* Check enemy bullets */
	  // for (int n = 0; n < MAX_ENEMY_BULLETS; ++n) {
		// if (enemy_bullets[n].state) { // not inactive
		  // float ebx = enemy_bullets[n].x + enemy_bullets[n].xspeed;
		  // float eby = enemy_bullets[n].y + enemy_bullets[n].yspeed;
		  // enemy_bullets[n].x = ebx;
		  // enemy_bullets[n].y = eby;

		  // /* Check collision with player */
		  // if (inside_circle(ebx, eby, player.x, player.y, player.radius) &&
			  // framecount - last_hit_frame > GRACE_PERIOD_AFTER_HIT) {
			// --player.health;
			// last_hit_frame = framecount;
			// player.effects = PLAYER_EFFECT_BLINKING;
			// enemy_bullets[n].state = BULLET_STATE_INACTIVE;
		  // }
		  // /* Disable bullet if it went offscreen */
		  // if (!inside_top_screen(ebx, eby))
			// enemy_bullets[n].state = BULLET_STATE_INACTIVE;
		// }
	  // }


	  /* Check player bullets */
	  int i = 0;
	  for (; i < MAX_BULLETS; i++) {
		if (bulletmask >> i & 1) {
		  float bx     = bullets[i].x + bullets[i].xspeed;
		  float by     = bullets[i].y + bullets[i].yspeed;
		  bullets[i].x = bx;
		  bullets[i].y = by;

		  /* Check for enemy hit */
		  // enemy_ship_t *enemy = NULL;
		  // for (int k = 0; k < MAX_ENEMY_SHIPS; ++k)
			// if ((enemy = &enemy_ships[i]) && enemy->state == ENEMY_STATE_ACTIVE &&
				// inside_circle(bx,by,enemy->x,enemy->y,enemy->radius)) {
			  // bulletmask = bulletmask & ~(1 << i);
			  // --enemy->health;
			  // return;
			// }
			  

		  /* Check for asteroid hit */

		  // Ignore inactive asteroids
		  // int j = 0;
		  // for (; j < MAX_ASTEROIDS; j++) {
			// if (asteroidmask >> j & 1) {
			  // float ax   = asteroids[j].x;
			  // float ay   = asteroids[j].y;
			  // float arad = asteroids[j].radius;
			  // if (inside_circle(bx, by, ax, ay, arad)) {
				// PRINTDBULLETS("Bullet hit at %3.2f, %3.2f\n", bx, by);
				// /* TODO: FX on bullet hit */
				// bulletmask = bulletmask & ~(1 << i);
				// break_asteroid(&asteroids[j], j);
				// return;
			  // }
			// }
		  // }bulletmask &= ~(1 << i);
		  // /* If bullet went out of bounds we disable it */
		  if (bullets[i].y < 15 && bullets[i].bot_screen == true && bullets[i].dup == false) {
				shoot_bullet_2(bullets[i].x, bullets[i].y);
				bullets[i].dup = true;
			// bullets[i].x = bx + 40;
			// bullets[i].y = 250.0f;
		  }
		  if (bullets[i].y < -15 && bullets[i].bot_screen == true && bullets[i].dup == true) {
			//bullets[i].bot_screen = false;
			bulletmask &= ~(1 << i);
			
		  }
		  if (bullets[i].y < -15 && bullets[i].bot_screen == false) {
			bulletmask &= ~(1 << i);
			
		  }
		}
	  }
	}
	

//------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) 
{
//------------------------------------------------------------------------------------------
	// Init libs
	romfsInit();
	
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	//consoleInit(GFX_TOP, NULL);


	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	


	// Load graphics and initialize player sprites
	general_spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!general_spritesheet) svcBreak(USERBREAK_PANIC);
	
	//Inicializar
	init_player();
	Init_BG();
	
	
	

	// Main loop
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (aptMainLoop())
	{
		
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		if (kDown & KEY_START) {
			break; // break in order to return to hbmenu
		}

		// Congfigure a sprite refresh time [ms]
		
		
		if (kHeld & KEY_A) {
			
		}
		if ((kHeld & KEY_B) && player_object.refresh_info.refresh_time > ANIMATION_REFRESH_TIME_MIN) {
			//player_object.refresh_info.refresh_time--;
		}
		if (kHeld & KEY_X) {
			//player_object.frame_info.loop_once = true; Z
		}
		if (kHeld & KEY_Y) {
			//player_object.frame_info.loop_once = false; X
		}
		if (kHeld & KEY_R) {
			focus = true;
		}
		else focus = false;
		if (kHeld & KEY_L) {
			
            // Verifica si han pasado al menos 150 milisegundos desde la última vez que se imprimió el sprite.
            u32 currentTime = osGetTime();
            if (currentTime - lastPrintTime >= 150) {
                buttonPressed = true;
                lastPrintTime = currentTime;
                shoot_bullet();
            }
        } else {
            buttonPressed = false;
        }

	
		circlePosition pos;
		
		hidCircleRead(&pos);

		//move_player(pos.dx, pos.dy, kHeld);
		player_logic(pos.dx, pos.dy, kHeld);
		bullet_logic();
		
	
		// Print debug messages on the bottom screen
		printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		printf("\x1b[6;1HTime elapsed:  %lld ms\x1b[K", player_object.refresh_info.elapsed);
		printf("\x1b[7;1HSprite refresh time:  %lld ms\x1b[K", player_object.refresh_info.refresh_time);
		printf("\x1b[8;1HX:  %f", orb_object.position.x);
		printf("\x1b[9;1HY:  %f", orb_object.position.y);

		// Render the scene
		
		
		update_object(&player_object);
		update_object(&orb_object);
		update_object(&boss1_object);
		//moveSprites();
	
		
		// -------------------------------------------------------------------------------------------------------
		// --------------------- G A M E P L A Y    P A N T A L L A     I N F E R I O R --------------------------
		// -------------------------------------------------------------------------------------------------------
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(bottom);
			
		
			
		for (size_t i = 0; i < 3; i ++)
		{
			C2D_DrawSprite(&sprites[i].spr);
		}
		C2D_SpriteMove(&fondo1->spr, 0, +fondo1->dy);		
		C2D_SpriteMove(&fondo2->spr, 0, +fondo2->dy);	
		
		rotacionDelFondo(fondo1, fondo2);
	
		
		
		draw_bullets();
		
		draw_sprite_animation(&orb_object);
		draw_sprite_animation(&player_object);
	
		
		
		C2D_Flush(); // Limpiar buffer de pantalla inferior
		
		// -------------------------------------------------------------------------------------------------------
		// --------------------- G A M E P L A Y    P A N T A L L A    S U P E R I O R --------------------------
		// -------------------------------------------------------------------------------------------------------
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);

		for (size_t i = 0; i < 3; i ++)
		{
			C2D_DrawSprite(&sprites[i].spr);
		}
		
		C2D_SpriteMove(&fondo1->spr, 0, +fondo1->dy);		
		C2D_SpriteMove(&fondo2->spr, 0, +fondo2->dy);	
		
		rotacionDelFondo(fondo1, fondo2);
		
		draw_bullets_top();
		
		draw_sprite_animation(&boss1_object);

		C3D_FrameEnd(0);
	}

	// Delete graphics
	deinitialize_object(&orb_object);
	deinitialize_object(&player_object);
	C2D_SpriteSheetFree(general_spritesheet);
	
	// Deinit libs
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();

	return 0;
}