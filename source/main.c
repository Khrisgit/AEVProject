#include "sprite_animation_manager.h"
#include "main.h"

// Atributos requeridos por las animaciones de sprites. 

object_2d_info_t			barrier_object;
object_2d_info_t			boss1_object;
object_2d_info_t			boss1_izq_object; 
object_2d_info_t			hit_object;
object_2d_info_t 			player_object;
object_2d_info_t 			player_shot_impact_obj[300]; 
object_2d_info_t			robot_1_obj; 

C2D_Sprite					barrier_sprite[11];
C2D_Sprite 					boss1_sprite[6];
C2D_Sprite					boss1_izq_sprite[3];
C2D_Sprite					hit_sprite[5];
C2D_Sprite 					player_sprite[8];
C2D_Sprite 					player_shot_impact_sprite[3];
C2D_Sprite					robot_1_spr[1];

sprite_pivot_t 				barrier_pivot = {0.5f, 0.5f};
sprite_pivot_t				boss1_pivot = {0.34f, 0.47f}; 
sprite_pivot_t				boss1_izq_pivot = {0.34f, 0.47f};
sprite_pivot_t				hit_pivot = {0.5f, 0.5f};
sprite_pivot_t 				player_pivot = {0.5f, 0.5f};
sprite_pivot_t 				player_shot_impact_pivot = {0.5f, 0.5f};
sprite_pivot_t				robot_1_pivot = {0.5f, 0.5f};

sprite_position_t 			barrier_position = {SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2}; 
sprite_position_t 			boss1_position = {SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2};
sprite_position_t 			boss1_izq_position;
sprite_position_t			hit_position;
sprite_position_t 			player_position = {SCREEN_WIDTH_BOT/2, SCREEN_HEIGHT_BOT/2};
sprite_position_t 			player_shot_impact_position = {SCREEN_WIDTH_BOT/2, SCREEN_HEIGHT_BOT/2};
sprite_position_t 			robot_1_pos;

float 						barrier_rotation = 0.0f;
float 						boss1_rotation = 0.0f;
float 						boss1_izq_rotation = 0.0f;
float 						hit_rotation = 0.0f;
float 						player_rotation = 0.0f;
float 						player_shot_impact_rotation = 0.0f;
float 						robot_1_rotation = 0.0f;

float 						barrier_rotation_speed = 0.0f;
float 						boss1_rotation_speed = 0.0f;
float 						boss1_izq_rotation_speed = 0.0f;
float 						hit_rotation_speed = 0.0f;
float 						player_rotation_speed = 0.0f;
float 						player_shot_impact_rotation_speed = 0.0f;
float 						robot_1_rotation_speed = 0.0f;

uint64_t 					barrier_refresh_ms_time = 100; 
uint64_t					boss1_refresh_ms_time = 200;
uint64_t 					boss1_izq_refresh_ms_time = 300;
uint64_t 					hit_refresh_ms_time = 100;
uint64_t 					player_refresh_ms_time = 100;
uint64_t 					player_shot_impact_refresh_ms_time = 100;
uint64_t					robot_1_refresh_ms_time = 100;

// Atributos de sprites. 

C2D_SpriteSheet general_spritesheet;

Sprite sprites[MAX_SPRITES];

Sprite* fondo1_top;
Sprite* fondo2_top;
Sprite* fondo1_bottom;
Sprite* fondo2_bottom;
Sprite* fondoout;

player_str 		  player;
orb_obj      	  player_orbs[4];
enemy_ship_t      enemy_ships[MAX_BOSSES];
minion      	  minions[MAX_BOSSES];

u32 			  bulletmask;

bullet_t          bullets[MAX_BULLETS];
bullet_enemy      enemy_bullets[MAX_ENEMY_BULLETS];
laser          	  enemy_lasers[MAX_ENEMY_BULLETS];

C2D_Sprite        hitbox;
C2D_Sprite        bullet_normal_sprite;
C2D_Sprite        bullet_normal_sprite2;
C2D_Sprite        bullet_yellow;
C2D_Sprite        bullet_orange;
C2D_Sprite        bullet_red;
C2D_Sprite        shell_red;
C2D_Sprite        blue_plasma_mid_ball;
C2D_Sprite        orb_green;
C2D_Sprite        orb_player;
C2D_Sprite        yellow_mid_ball;
C2D_Sprite        laser_yellow;
C2D_Sprite        red_big_ball;
C2D_Sprite        star_yellow;
C2D_Sprite        star_blue;
C2D_Sprite        star_mini_red;
C2D_Sprite        enemy_sprites[SPRITE_ENEMY_TOTAL];

C2D_Sprite        UI;

// Atributos de control. 

u32 			currentTime_1;
u32 			currentTime_2;
u32 			currentTime_3;
u32 			currentTime_4;
u32 			currentTime_5;
u32 			currentTime_6;
u32 			currentTime_7;
u32 			currentTime_8;
u32 			currentTime_9;
u32 			currentTime_10;
//u32 currentTime_aux = osGetTime();
u32 			playerShotTimeCurrent;


u32 lastPrintTime_1 = 0;
u32 lastPrintTime_2 = 0;
u32 lastPrintTime_3 = 0;
u32 lastPrintTime_4 = 0;
u32 lastPrintTime_5 = 0;
u32 lastPrintTime_6 = 0;
u32 lastPrintTime_7 = 0;
u32 lastPrintTime_8 = 0;
u32 lastPrintTime_9 = 0;
u32 lastPrintTime_10 = 0;

u32 skill_ini;
u32 skill_end;
u32 skill_time_elapsed;

float level_angle_1 = 0;
float level_angle_2 = 0;
float level_angle_3 = 0;
float level_angle_4;
float level_angle_5;
float level_angle_6;
float level_angle_7;
float level_angle_8;
float level_angle_9;
float level_angle_10;

bool level_helper_1 = false;
bool level_helper_2 = false;
bool level_helper_3 = false;
bool level_helper_4 = false;
bool level_helper_5 = false;
bool level_helper_6 = false;
bool level_helper_7 = false;
bool level_helper_8 = false;
bool level_helper_9 = false;
bool level_helper_10 = false;
bool level_helper_11 = false;

static bool focus = false; 	// Indica si el PJ está en estado "focus" 
static bool focus_helper = false; 	// Indica si el PJ está en estado "focus" 
static float r = 30.0f; 	// Radio de giro de los orbes del PJ
static float max_enemy_hp_value = 250.0f;
static float current_enemy_hp_value = 0;
static float t_par[4] = {0.0f, 1.0f, 0.5f, 1.5f};
static float t_impar[3] = {0.0f, 0.666f, 1.333f};
static float ts_radius;
//static size_t bullets = 0;  // ID de los proyectiles del PJ

static bool skill = false;  // Condición de botón presionado
static bool dead = false;  // Condición de botón presionado
static u32 lastPrintTime = 0;		// Tiempo transcurrido
unsigned int      framecount; // NOTE: PRINTFRAME needs this name to be unchanged
unsigned int      last_hit_frame;

bool rotacionFondo_top = true;	
bool rotacionFondo_bottom = true;	
bool direction_helper = false; 	// Indica si el PJ está en estado "focus" 
bool left = true;
bool shots = false;


void init_player(){

	initialize_object(&player_object, \
						player_sprite,	\
						"romfs:/gfx/player.t3x", \
						player_pivot, \
						player_position, \
						player_rotation, \
						player_rotation_speed, \
						player_refresh_ms_time, \
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
						  
	initialize_object(&boss1_izq_object, \
						boss1_izq_sprite,	\
						"romfs:/gfx/Boss-1-izq.t3x", \
						boss1_izq_pivot, \
						boss1_izq_position, \
						boss1_izq_rotation, \
						boss1_izq_rotation_speed, \
						boss1_izq_refresh_ms_time, \
						true);
						  
	initialize_object(&barrier_object, \
						barrier_sprite,	\
						"romfs:/gfx/barrier.t3x", \
						barrier_pivot, \
						barrier_position, \
						barrier_rotation, \
						barrier_rotation_speed, \
						barrier_refresh_ms_time, \
						false);
for(int a = 0; a < MAX_BULLETS; a++){ 
	initialize_object(&player_shot_impact_obj[a], \
						player_shot_impact_sprite,	\
						"romfs:/gfx/impact_shot.t3x", \
						player_shot_impact_pivot, \
						player_shot_impact_position, \
						player_shot_impact_rotation, \
						player_shot_impact_rotation_speed, \
						player_shot_impact_refresh_ms_time, \
						true);
}  
	initialize_object(&robot_1_obj, \
						robot_1_spr,	\
						"romfs:/gfx/robot_1.t3x", \
						robot_1_pivot, \
						robot_1_pos, \
						robot_1_rotation_speed, \
						robot_1_rotation_speed, \
						robot_1_refresh_ms_time, \
						false);
						  
	player.x = SCREEN_WIDTH_BOT/2;
	player.y = SCREEN_HEIGHT_BOT/2;
	player.yspeed = 0.0f;
	player.xspeed = 0.0f;
	player.radius = 3.0f;
	player.health = PLAYER_HP_START;
	player.effects &= 0;
	player.player_spr = &player_object;
	player.orbs = 1;

	C2D_SpriteFromSheet(&hitbox, general_spritesheet, 12);
	C2D_SpriteFromSheet(&bullet_normal_sprite, general_spritesheet, 0);
	C2D_SpriteFromSheet(&bullet_normal_sprite2, general_spritesheet, 2);
	C2D_SpriteFromSheet(&bullet_yellow, general_spritesheet, 18);
	C2D_SpriteFromSheet(&bullet_orange, general_spritesheet, 19);
	C2D_SpriteFromSheet(&bullet_red, general_spritesheet, 20);
	C2D_SpriteFromSheet(&shell_red, general_spritesheet, 21);
	C2D_SpriteFromSheet(&blue_plasma_mid_ball, general_spritesheet, 6);
	C2D_SpriteFromSheet(&orb_green, general_spritesheet, 16);
	C2D_SpriteFromSheet(&orb_player, general_spritesheet, 22);
	C2D_SpriteFromSheet(&yellow_mid_ball, general_spritesheet, 7);
	C2D_SpriteFromSheet(&red_big_ball, general_spritesheet, 8);
	C2D_SpriteFromSheet(&laser_yellow, general_spritesheet, 11);
	C2D_SpriteFromSheet(&star_yellow, general_spritesheet, 13);
	C2D_SpriteFromSheet(&star_blue, general_spritesheet, 15);
	C2D_SpriteFromSheet(&star_mini_red, general_spritesheet, 14);

	C2D_SpriteFromSheet(&UI, general_spritesheet, 9);
	C2D_SpriteSetCenter(&UI, 0.5f, 0.5f);
	C2D_SpriteSetPos(&UI, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2);

	C2D_SpriteSetRotation(&star_yellow, 1.5f);
		
	C2D_SpriteSetCenter(&hitbox, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&bullet_normal_sprite, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&bullet_normal_sprite2, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&bullet_yellow, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&bullet_orange, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&bullet_red, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&shell_red, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&blue_plasma_mid_ball, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&orb_green, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&orb_player, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&yellow_mid_ball, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&laser_yellow, 0.45f, 0.0f);
	C2D_SpriteSetCenter(&red_big_ball, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&star_mini_red, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&star_yellow, 0.5f, 0.5f);
	C2D_SpriteSetCenter(&star_blue, 0.5f, 0.5f);

	int j = 0;
	for (; j < 4; ++j) {

		if (!player_orbs[j].state) { // inactive, free to use slot
			  

			player_orbs[j].x = player_object.position.x;
			player_orbs[j].y = player_object.position.y;				  
			  
			player_orbs[j].xspeed = 0;
			player_orbs[j].yspeed = 0;			

			player_orbs[j].spr = &orb_player;
			  
			player_orbs[j].state = BULLET_STATE_ACTIVE;
			
		}		
	}
			
}

void Init_BG(){
		
	fondo1_top = &sprites[0];
	C2D_SpriteFromSheet(&fondo1_top->spr, general_spritesheet, 10);
	C2D_SpriteSetCenter(&fondo1_top->spr, 0.5f, 1.0f);
	C2D_SpriteSetPos(&fondo1_top->spr, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP);
		
		
	fondo2_top = &sprites[1];
	C2D_SpriteFromSheet(&fondo2_top->spr, general_spritesheet, 10);
	C2D_SpriteSetCenter(&fondo2_top->spr, 0.5f, 1.0f);
	C2D_SpriteSetPos(&fondo2_top->spr, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP - 600.0f);
		
	fondo1_bottom = &sprites[2];
	C2D_SpriteFromSheet(&fondo1_bottom->spr, general_spritesheet, 10);
	C2D_SpriteSetCenter(&fondo1_bottom->spr, 0.5f, 1.0f);
	C2D_SpriteSetPos(&fondo1_bottom->spr, SCREEN_WIDTH_TOP/2 - 40.0f, 600.0f);
		
		
	fondo2_bottom = &sprites[3];
	C2D_SpriteFromSheet(&fondo2_bottom->spr, general_spritesheet, 10);
	C2D_SpriteSetCenter(&fondo2_bottom->spr, 0.5f, 1.0f);
	C2D_SpriteSetPos(&fondo2_bottom->spr, SCREEN_WIDTH_TOP/2 - 40.0f, 0.0f);
		
		
	// fondoout = &sprites[2];
	// C2D_SpriteFromSheet(&fondoout->spr, general_spritesheet, 2);
	// C2D_SpriteSetCenter(&fondoout->spr, 0.5f, 1.0f);
	// C2D_SpriteSetPos(&fondoout->spr, SCREEN_WIDTH_TOP/2, 0.0f);
		
}

void rotacionDelFondo(Sprite* fondo1, Sprite* fondo2, float img_size, bool bottom){

	if(skill)
	{
		fondo1->dy = 0.0f; 
		fondo2->dy = 0.0f;		
	} 
	else // poner velocidades tipo 2, 4, 5...  3 NO
	{ 
		fondo1->dy = 4.0f; 
		fondo2->dy = 4.0f; 
	}
		
		
	if(!bottom)
	{
		if(fondo1->spr.params.pos.y >= SCREEN_HEIGHT_TOP + img_size + 100.0f)
		{
			C2D_SpriteSetPos(&fondo1->spr, SCREEN_WIDTH_TOP/2, -(img_size - SCREEN_HEIGHT_TOP - 100.0f));
		}	
		if(fondo2->spr.params.pos.y >= SCREEN_HEIGHT_TOP + img_size + 100.0f)
		{
			C2D_SpriteSetPos(&fondo2->spr, SCREEN_WIDTH_TOP/2, -(img_size - SCREEN_HEIGHT_TOP - 100.0f));
		}					
	}
	else
	{		
		if(fondo1->spr.params.pos.y >= SCREEN_HEIGHT_TOP + img_size + 100.0f)
		{
			C2D_SpriteSetPos(&fondo1->spr, SCREEN_WIDTH_TOP/2 - 40.0f, -(img_size - SCREEN_HEIGHT_TOP - 100.0f));
		}	
		if(fondo2->spr.params.pos.y >= SCREEN_HEIGHT_TOP + img_size + 100.0f)
		{
			C2D_SpriteSetPos(&fondo2->spr, SCREEN_WIDTH_TOP/2 - 40.0f, -(img_size - SCREEN_HEIGHT_TOP - 100.0f));
		}			
	}

	C2D_SpriteMove(&fondo1->spr, 0, +fondo1->dy);		
	C2D_SpriteMove(&fondo2->spr, 0, +fondo2->dy);

}
	
void draw_impact(object_2d_info_t *s[]){


	 	int i = 0;
	for (; i < MAX_BULLETS; i++) {
		s[i]->position.y += 1*i;
		s[i]->position.x += 1*i;
		
		draw_sprite_animation(s[i]);
	}	

	 


	

}
	
void draw_bullets_bot(void){

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
	
void draw_bullets_top(void){

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

void draw_lasers_top(void){

	for (int j = 0; j < MAX_ENEMY_BULLETS; ++j) 
	{
		if (enemy_lasers[j].state) { // not inactive
			if(enemy_lasers[j].bot_screen == false){
				  C2D_SpriteSetPos(enemy_lasers[j].sprite, enemy_lasers[j].x, enemy_lasers[j].y);
				  C2D_SpriteSetRotation(enemy_lasers[j].sprite, deg_to_rad(-enemy_lasers[j].angle));
				  C2D_DrawSprite(enemy_lasers[j].sprite);
			}
		}
	}	
}
	
void draw_lasers_bot(void){

	for (int i = 0; i < MAX_ENEMY_BULLETS; ++i) 
	{
		if (enemy_lasers[i].state) { // not inactive
			if(enemy_lasers[i].bot_screen == true){
				C2D_SpriteSetPos(enemy_lasers[i].sprite, enemy_lasers[i].x, enemy_lasers[i].y);
				C2D_SpriteSetRotation(enemy_lasers[i].sprite, deg_to_rad(-enemy_lasers[i].angle));
				C2D_DrawSprite(enemy_lasers[i].sprite);
			}
		}
	}	
}

void draw_hitbox(void){
	C2D_SpriteSetPos(&hitbox, player.x, player.y);
	C2D_DrawSprite(&hitbox);
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
	
void orb_movement(){		// Función que determina el movimiento del orbe que gira alrededor del PJ.
	
	if(!focus)			// Si el PJ   NO   está "focus" (el orbe debe de girar alrededor del PJ)
	{
			
		// El tiempo sigue pasando

		t_par[0] += 0.03f;
		t_par[1] += 0.03f;
		t_par[2] += 0.03f;
		t_par[3] += 0.03f;

		t_impar[0] += 0.03f;
		t_impar[1] += 0.03f;
		t_impar[2] += 0.03f;
			

		for(int i = 0; i < 4; i++){

			if(player.orbs%2 == 0)
			{
				if(inside_circle(player_orbs[i].x, player_orbs[i].y, player_object.position.x, player_object.position.y, r) && focus_helper)
				{
					// velocidad = punto donde se encuentra
					player_orbs[i].xspeed =
					((player_object.position.x + r * cos(t_par[i] * M_PI)) - player_orbs[i].x) / 5;
					player_orbs[i].yspeed =
					((player_object.position.y - r * sin(t_par[i] * M_PI)) - player_orbs[i].y) / 5;
							
				}		
				else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
				{		
					// posición del orbe = posición del PJ + giro alrededor	

					

					focus_helper = false;
					player_orbs[i].x = player_object.position.x + r * cos(t_par[i] * M_PI);
					player_orbs[i].y = player_object.position.y - r * sin(t_par[i] * M_PI);

				}
			}
			else
			{
				if(inside_circle(player_orbs[i].x, player_orbs[i].y, player_object.position.x, player_object.position.y, r) && focus_helper)
				{
					// velocidad = punto donde se encuentra
					player_orbs[i].xspeed =
					((player_object.position.x + r * cos(t_impar[i] * M_PI)) - player_orbs[i].x) / 5;
					player_orbs[i].yspeed =
					((player_object.position.y - r * sin(t_impar[i] * M_PI)) - player_orbs[i].y) / 5;
							
				}		
				else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
				{		
					// posición del orbe = posición del PJ + giro alrededor	

					

					focus_helper = false;
					player_orbs[i].x = player_object.position.x + r * cos(t_impar[i] * M_PI);
					player_orbs[i].y = player_object.position.y - r * sin(t_impar[i] * M_PI);

				}
			}

			

		}

	}
	else
	{

		float x_inpar[] = {0.0f, -15.0f, 15.0f};
		float x_par[] = {-10.0f, 10.0f, -20.0f, 20.0f};

		float y_inpar[] = {0.0f, 10.0f, 10.0f};
		float y_par[] = {0.0f, 0.0f, 10.0f, 10.0f};

		for(int j = 0; j < 4; j++){

			if(player_orbs[j].x != player_object.position.x &&
			player_orbs[j].y != player_object.position.y - 20.0f)
			{

				if(player.orbs%2 == 0)
				{

					focus_helper = true;
					player_orbs[j].xspeed = ((player_object.position.x + x_par[j]) - player_orbs[j].x) / 5;
					player_orbs[j].yspeed = ((player_object.position.y + y_par[j] - 20.0f) - player_orbs[j].y) / 5;

				}
				else
				{
					focus_helper = true;
					player_orbs[j].xspeed = ((player_object.position.x + x_inpar[j]) - player_orbs[j].x) / 5;
					player_orbs[j].yspeed = ((player_object.position.y + y_inpar[j] - 20.0f) - player_orbs[j].y) / 5;
				}	
			}
		}
	}

	// 	// Si el orbe se encuentra dentro de la esfera que debe de recorrer
			
	// 	if(inside_circle(orb_object.position.x, orb_object.position.y, player_object.position.x, player_object.position.y, r) && focus_helper)
	// 	{
	// 		// velocidad = punto donde se encuentra
	// 		orb_object.position_velocity.dx =
	// 		((player_object.position.x + r * cos(t * (180.0f / M_PI))) - orb_object.position.x) / 5;
	// 		orb_object.position_velocity.dy =
	// 		((player_object.position.y - r * sin(t * (180.0f / M_PI))) - orb_object.position.y) / 5;
					
	// 	}
	// 	else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
	// 	{		
	// 		// posición del orbe = posición del PJ + giro alrededor	
	// 		focus_helper = false;
	// 		orb_object.position.x = player_object.position.x + r * cos(t * (180.0f / M_PI));
	// 		orb_object.position.y = player_object.position.y - r * sin(t * (180.0f / M_PI));

	// 	}
	// }
	// else  // si el PJ    SI   está en modo "focus"
	// {
	// 	// si el orbe no está enfrente del PJ
	// 	if(orb_object.position.x != player_object.position.x &&
	// 	orb_object.position.y != player_object.position.y - 20.0f)
	// 	{
	// 		// velocidad del orbe = posición del PJ - 20 Y  (el orbe se mueve hasta donde está el PJ (-20 en la Y, es dcir, frente a él))
	// 		focus_helper = true;
	// 		orb_object.position_velocity.dx = (player_object.position.x - orb_object.position.x) / 5;
	// 		orb_object.position_velocity.dy = ((player_object.position.y - 20.0f) - orb_object.position.y) / 5;
				
	// 	}
	// }
		
}

void orb_logic(void){

	int i = 0;
	for(; i < 4; i++){

		float pox;
		float poy;

		pox = player_orbs[i].x + player_orbs[i].xspeed;
		poy = player_orbs[i].y + player_orbs[i].yspeed;

		player_orbs[i].x = pox;
		player_orbs[i].y = poy;

		if((i+1) > player.orbs)
		player_orbs[i].state = ORB_STATE_INACTIVE;
		else
		player_orbs[i].state = ORB_STATE_ACTIVE;

	}
		
}

void draw_orbs(void){

	int i = 0;
	for (; i < 4; i++) {
		if (player_orbs[i].state) { // not inactive
				C2D_SpriteSetPos(player_orbs[i].spr, player_orbs[i].x, player_orbs[i].y);
				C2D_DrawSprite(player_orbs[i].spr);
	    }
	}
}
	
enemy_ship_t spawn_enemy_ship(float x, float y, float xs, float ys, float r, float lives, float hp, 
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
	.attspeed     		= 60,
	.state        		= ENEMY_STATE_ACTIVE,  
	.enemy_spr    		= e_spr,
	.barrier_spr 	 	= b_spr,
	.invencible   		= false,
	};
	// loot_table_t new_loot_table = {
									 // .probabilities = {0.1f, 0.5f, 1.0f},
									 // .items = {NOTHING, EXTRA_SCORE, PICKUP_HP},
	// };
	// new_enemy.loot_table = new_loot_table;
	return new_enemy;
}
	
minion spawn_minion(float x, float y, float xs, float ys, float r, float hp, 
								object_2d_info_t* e_spr, object_2d_info_t* b_spr){

	minion new_enemy = {
	.x            		= x,
	.y            		= y,
	.xspeed       		= xs,
	.yspeed       		= ys,
	.radius       		= r,
	.health       		= hp,
	.current_health 	= hp,
	.state        		= ENEMY_STATE_ACTIVE,  
	.enemy_spr    		= e_spr,
	.invencible   		= false,
	};

	  // loot_table_t new_loot_table = {
									 // .probabilities = {0.1f, 0.5f, 1.0f},
									 // .items = {NOTHING, EXTRA_SCORE, PICKUP_HP},
	  // };
	  // new_enemy.loot_table = new_loot_table;
	  return new_enemy;
}
	

void player_logic(float x, float y, u32 kHeld){

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
			player.xspeed = x/25;				
			player.y = BG_LIMIT_UP_BOT;				
			player_object.position_velocity.dx = x/25;	// velocidad normal del PJ en X (izq y derecha)
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
			player.xspeed = x/25;				
			player.y = BG_LIMIT_DOWN_BOT;				
			player_object.position_velocity.dx = x/25;
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
			player.yspeed = -y/25;
			player.x = BG_LIMIT_LEFT_BOT;				
			player_object.position_velocity.dy = -y/25;
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
			player.yspeed = -y/25;
			player.x = BG_LIMIT_RIGHT_BOT;				
			player_object.position_velocity.dy = -y/25;
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
		
		player.xspeed = x/25;
		player.yspeed = -y/25;
		player_object.position_velocity.dx = x/25;
		player_object.position_velocity.dy = -y/25;		 
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


void enemy_ship_logic(void){
		
	for (int n = 0; n < MAX_BOSSES; ++n) {
			
		if (enemy_ships[n].state) { // not inactive
			
			float pbx = enemy_ships[n].x + enemy_ships[n].xspeed;
			float pby = enemy_ships[n].y + enemy_ships[n].yspeed;
			  
			enemy_ships[n].x = pbx;
			enemy_ships[n].y = pby;
			enemy_ships[n].enemy_spr->position.x = pbx;
			enemy_ships[n].enemy_spr->position.y = pby;
			enemy_ships[n].barrier_spr->position.x = pbx;
			enemy_ships[n].barrier_spr->position.y = pby;

			if (enemy_ships[n].current_health <= 0)
			{
				enemy_ships[n].state = ENEMY_STATE_INACTIVE;
				//last_hit_frame = framecount;
			}
			  
			if(enemy_ships[n].xspeed < 0)
			{  
				enemy_ships[n].enemy_spr->object_sprite = boss1_izq_sprite;
				enemy_ships[n].enemy_spr->frame_info.current_frame_index = 0;
				enemy_ships[n].enemy_spr->frame_info.num_of_sprites = 3;
				enemy_ships[n].enemy_spr->refresh_info.refresh_time = boss1_izq_refresh_ms_time;
				enemy_ships[n].enemy_spr->frame_info.loop_once = true;
			}
		}



	}
	
	  
	for (int j = 0; j < MAX_BOSSES; ++j) {
			
		if (minions[j].state) { // not inactive
			
			float pbx = minions[j].x + minions[j].xspeed;
			float pby = minions[j].y + minions[j].yspeed;
			  
			minions[j].x = pbx;
			minions[j].y = pby;
			minions[j].enemy_spr->position.x = pbx;
			minions[j].enemy_spr->position.y = pby;

			if (minions[j].current_health <= 0)
			{
				minions[j].state = ENEMY_STATE_INACTIVE;
			}  
		}
	}  
}

void draw_boss_pos_bar(void){

	for (int n = 0; n < MAX_BOSSES; n++) {

		C2D_DrawRectSolid(enemy_ships[n].x - 20, 240, 0, -20, -10, 
				C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f));
	}

}

void move_player(float x, float y, u32 kHeld){	// Función que se encarga de mover al PJ por la pantalla INFERIOR sin salirse de ella.
		
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


void shoot_bullet(void){

	for (int i = MAX_BULLETS/2; i < MAX_BULLETS; i+=player.orbs) {

		if (!bullets[i].state) { // inactive, free to use slot
			  
			for(int ij = 0; ij < player.orbs; ij++){

				bullets[i+ij].x = player_orbs[ij].x + 40;
				bullets[i+ij].y = player_orbs[ij].y + 240 - 7.0f;				  
				
				bullets[i+ij].xspeed = 0;
				bullets[i+ij].yspeed = -BULLET_INITIAL_SPEED;			
				
				bullets[i+ij].bot_screen = false;
				bullets[i+ij].radius = 2.0f;

				bullets[i+ij].sprite = &bullet_normal_sprite;
				
				bullets[i+ij].state = BULLET_STATE_ACTIVE;

				bullets[i+ij].aim = false;
			}

			break;

		}	
	}
	
	for (int j = 0; j < MAX_BULLETS/2; j+=player.orbs) {

		if (!bullets[j].state) { // inactive, free to use slot
			  
			for(int jj = 0; jj < player.orbs; jj++){

			bullets[j+jj].x = player_orbs[jj].x;
			bullets[j+jj].y = player_orbs[jj].y - 7.0f;				  
			  
			bullets[j+jj].xspeed = 0;
			bullets[j+jj].yspeed = -BULLET_INITIAL_SPEED;			
			  
			bullets[j+jj].bot_screen = true;
			bullets[j+jj].radius = 2.0f;

			bullets[j+jj].sprite = &bullet_normal_sprite;
			  
			bullets[j+jj].state = BULLET_STATE_ACTIVE;

			bullets[j+jj].aim = false;

			}
			 
			break;

		}		
	}
}
	
void shoot_enemy_bullet_aim(float x, float y, float speed, float rotation_spd, float radius, C2D_Sprite *sprite, bool bot){
	  
	int i = 0;
	for (; i < MAX_ENEMY_BULLETS/2; ++i) {

		if (!enemy_bullets[i].state) { // inactive, free to use slot
			  
			float dxt, dyt, tangle;
			  
			if(!bot)
			{
				dxt = (player_object.position.x + 40) - x;
				dyt = (player_object.position.y + 240) - y;

				enemy_bullets[i].x = x;
				enemy_bullets[i].y = y;
			}
			else
			{
				dxt = (player_object.position.x + 40) - (x + 40);
				dyt = (player_object.position.y + 240) - (y + 240);

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
				dxb = player_object.position.x - (x - 40);
				dyb = player_object.position.y - (y - 240);
				
				enemy_bullets[j].x = x - 40;
				enemy_bullets[j].y = y - 240;
			}
			else
			{
				dxb = player_object.position.x - x;
				dyb = player_object.position.y - y;
				
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

void shoot_enemy_bullet(float x, float y, float angle, float speed, float rotation_spd, float radius, C2D_Sprite *sprite, bool bot){

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
			  
			enemy_bullets[j].state = BULLET_STATE_ACTIVE;
			 
			break;

		}	
	}
}
		
void burst(float x, float y, float angle, float speed, float rotation_spd, float radius, int total_bullets, C2D_Sprite *sprite, bool bot){

	int i = 0;
	for (; i < total_bullets; ++i) {

		//u32 currentTime = osGetTime();
		//if (currentTime - lastPrintTime >= time) {
		//lastPrintTime = currentTime;
		shoot_enemy_bullet(x, y, (360/total_bullets) * i + angle, speed, rotation_spd, radius, sprite, bot );
			
    }
}

void spread(float x, float y, float angle, float direction, float speed, float rotation_spd, float radius, int total_bullets, C2D_Sprite *sprite, bool bot){

	int i = 0;
	for (; i < total_bullets; ++i) {

		//u32 currentTime = osGetTime();
		//if (currentTime - lastPrintTime >= time) {
		//lastPrintTime = currentTime;
		shoot_enemy_bullet(x, y, (angle * i) / (total_bullets - 1) + direction - 90, speed, rotation_spd, radius, sprite, bot );
			
    }
}
	
void burst_aim(float x, float y, float speed, float rotation_spd, float radius, int total_bullets, C2D_Sprite *sprite, bool bot){
		
	float dx, dy, a;
			  
	if(bot)
	{
		dx = player_object.position.x - x;
		dy = player_object.position.y - y;
	}
	else
	{
		dx = player_object.position.x - x + 40;
		dy = player_object.position.y - y + 240;				  
	}
	
	a = atan2f(dy, dx);
	
	int i = 0;
	for (; i < total_bullets; ++i) {

		shoot_enemy_bullet(x, y, ((360)/total_bullets) * i - rad_to_deg(a), speed, rotation_spd, radius, sprite, bot );
			
    }
}
	
void bullet_logic(void){

	/* Check enemy bullets */
	for (int n = 0; n < MAX_ENEMY_BULLETS; ++n) {
		if (enemy_bullets[n].state) { // not inactive

			float pbx;
			float pby;

			pbx = enemy_bullets[n].x + enemy_bullets[n].xspeed;
		  	pby = enemy_bullets[n].y + enemy_bullets[n].yspeed;

		    enemy_bullets[n].x = pbx;
		    enemy_bullets[n].y = pby;

		    enemy_bullets[n].sprite_angle +=  enemy_bullets[n].rotation_spd;
		    //C2D_SpriteSetRotationDegrees(enemy_bullets[n].sprite, enemy_bullets[n].sprite_angle);

		    /* Check collision with player */
		    // if (inside_circle(sbx, sby, player.x, player.y, player.radius) &&
			// framecount - last_hit_frame > GRACE_PERIOD_AFTER_HIT) {
				  
		    if (enemy_bullets[n].bot_screen && cirlce_inside_circle(pbx, pby, player.x, player.y, player.radius, enemy_bullets[n].radius)){
				// framecount - last_hit_frame > GRACE_PERIOD_AFTER_HIT) {
				//--player.health; 
				dead = true;
				last_hit_frame = framecount;
				//player.effects = PLAYER_EFFECT_BLINKING;
				enemy_bullets[n].state = BULLET_STATE_INACTIVE;
		    }
		
		    /* Disable bullet if it went offscreen */
		
		
		    if(!inside_rect(enemy_bullets[n].x, enemy_bullets[n].y, -100, 500, -100, 600) 
				&& enemy_bullets[n].bot_screen == false) // TOP
		    {
			  enemy_bullets[n].state = BULLET_STATE_INACTIVE;
		    }
		  
		  if(!inside_rect(enemy_bullets[n].x, enemy_bullets[n].y, -100, 500, -400, 300) 
				 && enemy_bullets[n].bot_screen == true) // BOT
		    {
			   enemy_bullets[n].state = BULLET_STATE_INACTIVE;
		    }
		}
	}

	/* Check player bullets */
	int i = 0;
	for (; i < MAX_BULLETS; i++) {
		if (bullets[i].state) { // not inactive

			float by;
			float bx;

			bx     = bullets[i].x + bullets[i].xspeed;
			by     = bullets[i].y + bullets[i].yspeed;

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
			
			 for (int j = 0; j < MAX_BOSSES; j++) { 
				
			 	if (!bullets[i].bot_screen && cirlce_inside_circle(bx, by, enemy_ships[j].x, enemy_ships[j].y, enemy_ships[j].radius, bullets[i].radius) && enemy_ships[j].state)
			 	{
			 		if(!enemy_ships[j].invencible)
			 		enemy_ships[j].current_health -= 0.1f;
			 		bullets[i].state = BULLET_STATE_INACTIVE;
					
			 		//draw_impact(bullets[i].x, bullets[i].y);
			 	}	
			}

		    if(!inside_rect(bullets[i].x, bullets[i].y, -40, 500, -100, 520) 
				&& bullets[i].bot_screen == false) // TOP
		    {
				bullets[i].state = BULLET_STATE_INACTIVE;
		    }
		  
		    if(!inside_rect(bullets[i].x, bullets[i].y, -100, 360, -300, 280) 
				&& bullets[i].bot_screen == true) // BOT
		    {
				bullets[i].state = BULLET_STATE_INACTIVE;
		    }
		}
	}
}

void laser_logic(void){

	/* Check enemy bullets */
	for (int n = 0; n < MAX_ENEMY_BULLETS; ++n) {

		if (enemy_lasers[n].state) { // not inactive

		float pbx = enemy_lasers[n].x + enemy_lasers[n].xspeed;
		float pby = enemy_lasers[n].y + enemy_lasers[n].yspeed;

		enemy_lasers[n].x = pbx;
		enemy_lasers[n].y = pby;
		  
		C2D_SpriteSetScale (enemy_lasers[n].sprite, 1, 3);

		  /* Check collision with player */
		  // if (inside_circle(sbx, sby, player.x, player.y, player.radius) &&
			  // framecount - last_hit_frame > GRACE_PERIOD_AFTER_HIT) {
				  
		    // if (enemy_lasers[n].bot_screen && cirlce_inside_circle(pbx, pby, player.x, player.y, player.radius, enemy_lasers[n].radius) ){
			  // framecount - last_hit_frame > GRACE_PERIOD_AFTER_HIT) {
			//--player.health; 
			// dead = true;
			// last_hit_frame = framecount;
			//player.effects = PLAYER_EFFECT_BLINKING;
			
			//enemy_lasers[n].state = BULLET_STATE_INACTIVE;
		  //}
		  /* Disable bullet if it went offscreen */
		  
		  
		  
		  // if (enemy_lasers[n].y > 245 && enemy_lasers[n].bot_screen == false && enemy_lasers[n].dup == false) 
		  // {
			
			// change_enemy_bullet(enemy_lasers[n].x, 0, enemy_lasers[n].angle,
				// enemy_lasers[n].spd, enemy_lasers[n].radius, enemy_lasers[n].sprite, true );
							
			// enemy_lasers[n].dup = true;
		  // }
		  // if (enemy_lasers[n].y < -5 && enemy_lasers[n].bot_screen == true && enemy_lasers[n].dup == false) 
		  // {
			  
			// change_enemy_bullet(enemy_lasers[n].x+40, 240, enemy_lasers[n].angle,
				// enemy_lasers[n].spd, enemy_lasers[n].radius, enemy_lasers[n].sprite, false );
				
			// enemy_lasers[n].dup = true;
		  // }
		}
	}
}

void level_1(enemy_ship_t* e){

	if(left && e->xspeed == 0 && !direction_helper)
	{
		float rand_x = randomBetween(e->x, 50);
		float rand_y = randomBetween(80, 140);

		e->xspeed = (rand_x - e->x) / 100;
		e->yspeed = (rand_y - e->y) / 100;

		left = !left;

		if(e->x > 50 && !direction_helper)
		direction_helper = !direction_helper;

	}
	if(!left && e->xspeed == 0 && !direction_helper)
	{
		float rand_x = randomBetween(e->x, 350);
		float rand_y = randomBetween(80, 140);

		e->xspeed = (rand_x - e->x) / 100;
		e->yspeed = (rand_y - e->y) / 100;

		left = !left;

		if(e->x < 40 && !direction_helper)
		direction_helper = !direction_helper;		

	}

	//ARREGLAR ESTO PORFA

	if(e->x < 50 && direction_helper){ e->xspeed = 0; e->yspeed = 0; direction_helper = !direction_helper;}
	if(e->x > 350 && direction_helper){ e->xspeed = 0; e->yspeed = 0; direction_helper = !direction_helper;}
	
}

void shoting(void){
	
	if(!level_helper_1)
	{
	currentTime_4 = osGetTime();
	level_helper_1 = true;
	}

	currentTime_2 = osGetTime();

    if (currentTime_2 - skill_time_elapsed - lastPrintTime_2 >= 350) 
	{
        lastPrintTime_2 = currentTime_2;
		skill_time_elapsed = 0;
		//int rand = randomBetween(0,2);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(2.0f,2.5f), 0.0f, 2.0f, &bullet_yellow, false);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(2.6f,3.1f), 0.0f, 2.0f, &bullet_orange, false);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(3.2f,3.6f), 0.0f, 2.0f, &bullet_red, false);
		//shoot_enemy_bullet_aim(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(6.2f,7.0f), 0.0f, 2.0f, &bullet_red, false);

    

		//spread(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_1, 0, 2.5f, 2.0f, 3.0f, 3, &orb_green, false);

		burst(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_1, 2.5f, 2.0f, 10.0f, 6, &star_yellow, false);

		burst(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_1+30.0f, 2.5f, 2.0f, 10.0f, 6, &star_blue, false);
		level_angle_1 += 10.0f;
	}

	//currentTime_3 = osGetTime();

	// if (currentTime_2 - skill_time_elapsed - lastPrintTime_2 >= 350) 
	// {
    //     lastPrintTime_2 = currentTime_2;
	// 	skill_time_elapsed = 0;
	// 	burst(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_2+45.0f, 3.0f, 2.0f, 10.0f, 8, &star_blue, false);
	// 	//level_angle_2 += 10.0f;
	// }

	// if ((currentTime_3 - skill_time_elapsed - lastPrintTime_3 >= 350) && (currentTime_3 - currentTime_4 > 0)) 
	// {
    //     lastPrintTime_3 = currentTime_3;
	// 	skill_time_elapsed = 0;
	// 	burst(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_2, 3.0f, 2.0f, 10.0f, 2, &star_blue, false);
	// 	level_angle_2 += 10.0f;
	// }
	


}

void time_in_skill(void){

	skill_end = osGetTime();
	
    skill_time_elapsed = skill_end - skill_ini;

}


void shot_laser(float x, float y, float angle, float max_scale, 
						float rot_speed, int time, C2D_Sprite *sprite, bool bot ){

	int i = 0;
	for (; i < MAX_ENEMY_BULLETS; ++i) {
		if (!enemy_lasers[i].state) { // inactive, free to use slot
			  
			if(!bot)
			{
				enemy_lasers[i].x = x;
				enemy_lasers[i].y = y;
			}
			else
			{
				enemy_lasers[i].x = x - 40;
				enemy_lasers[i].y = y - SCREEN_HEIGHT_BOT;				  
			}
			  
			//enemy_laser[i].bot_screen = bot;
			  
			//enemy_lasers[i].radius = 0.0f;
			enemy_lasers[i].max_scale = max_scale;
			enemy_lasers[i].angle = angle;
			  
			enemy_lasers[i].spd = 0.0f;
			enemy_lasers[i].rotation_speed = rot_speed;
			  
			//enemy_lasers[i].xspeed = 0.0f * cosf(deg_to_rad(-angle));
			//enemy_lasers[i].yspeed = 0.0f * sinf(deg_to_rad(-angle));

			enemy_lasers[i].sprite = sprite;
			  
			enemy_lasers[i].bot_screen = bot;
			enemy_lasers[i].state = BULLET_STATE_ACTIVE;
			  
			enemy_lasers[i].dup = false;
			 
			break;

		}	
	}	
}


//------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//------------------------------------------------------------------------------------------
	
	// Init libs
	romfsInit();
	srvInit();
    aptInit();
	
	//gfxInitDefault();
	gfxInit(GSP_BGR8_OES, GSP_BGR8_OES, false);
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
	
	enemy_ships[0] = spawn_enemy_ship(200.0f, 120.0f, 0.0f, 0.0f, 30.0f, 4, 100.0f, &boss1_object, &barrier_object);
	

	// Main loop
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (aptMainLoop())
	{
		
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		u32 kHeld = hidKeysHeld();
		
		if (kDown & KEY_START) {
			skill = !skill;
		}

		// Congfigure a sprite refresh time [ms]
		
		
		if (kHeld & KEY_A) {
			//skill = true;
			// enemy_ship_t e = spawn_enemy_ship(float SCREEN_WIDTH_BOT/2, float SCREEN_HEIGHT_TOP/2,
			// float 0, float 0, float 0, u32 255);
			            // Verifica si han pasado al menos 150 milisegundos desde la última vez que se imprimió el sprite.
            u32 currentTime = osGetTime();
            if (currentTime - lastPrintTime >= 150) 
			{

                lastPrintTime = currentTime;
				// barrage_2(200.0f, 100.0f, 
				// 2.0f, 2.0f, 90.0f, &bullet_normal_sprite2,
				// 2.0f, 24, 150.0f, 10);
				
				// shoot_enemy_bullet(200.0f, 100.0f, 
				// player.x, player.y, 45.0f, &bullet_normal_sprite2,
				// 2.0f);
				
				
				//shoot_enemy_bullet(200.0f, 100.0f, 90.0f, 2.0f, 2.0f, &bullet_normal_sprite2, true);
				
				//burst_aim(200.0f, 100.0f, 1.8f, 25.0f, 7, &red_big_ball, false);
			 
				shots = !shots;
				level_angle_1 = 10.0f;
			 
        	}
			
        }

		if(shots && !skill)
		shoting();
		
		if (kDown & KEY_B) 
		{
		
            skill = !skill;
			
			ts_radius = 700.0f;

			if(skill)
			skill_ini = osGetTime();
			
		}
		
		if ((kHeld & KEY_B) && player_object.refresh_info.refresh_time > ANIMATION_REFRESH_TIME_MIN) {
			//player_object.refresh_info.refresh_time--;
			
		}
		if (kDown & KEY_A) {
			//shoot_enemy_bullet_aim(200.0f, 100.0f, 2.0f, 0.0f, 2.0f, &bullet_normal_sprite2, true);
			//player_object.frame_info.loop_once = true; Z
			//move_enemy_sprite_to(1.0f, &boss1_object);
			//level_1(&boss1_object);

			
		}
		if (kDown & KEY_L) {
			
			if(player.orbs == 4)
			player.orbs = 1;
			else
			player.orbs ++;
			// shot_laser(enemy_ships[0].x, enemy_ships[0].y, 0.0f, 1.0f, 
			// 		0.0f, 5, &laser_yellow, false);
					
			// shot_laser(enemy_ships[0].x, enemy_ships[0].y, 0.0f, 1.0f, 
			// 		0.0f, 5, &laser_yellow, true);
			//player_object.frame_info.loop_once = false; X
		}
		if (kHeld & KEY_R) {
			//enemy_ships[0].xspeed = -0.7f;
			level_1(&enemy_ships[0]);
		}
		if (kHeld & KEY_Y) {
			focus = true;
		}
		else focus = false;
		if (kHeld & KEY_X && !skill) {
			
            // Verifica si han pasado al menos X milisegundos desde la última vez que se imprimió el sprite.
            u32 playerShotTime = osGetTime();
            if (playerShotTime - playerShotTimeCurrent >= PLAYER_BULLET_MS) 
			{
                playerShotTimeCurrent = playerShotTime;
                shoot_bullet();
            }
			
			//deinitialize_object(&player_shot_impact_obj);
        } 


	
		circlePosition pos;
		
		hidCircleRead(&pos); 

		//move_player(pos.dx, pos.dy, kHeld);
		// enemy_ships[0] = spawn_enemy_ship(200.0f, 120.0f, 1.0f, 1.0f, 200.0f, 20.0f, &boss1_object, &barrier_object);
		
		player_logic(pos.dx, pos.dy, kHeld);
		
		if(!skill)
		{
			bullet_logic();
			laser_logic();
			enemy_ship_logic();
			
		}

		if(skill)
		time_in_skill();

		orb_logic();
		
		
		// Print debug messages on the bottom screen
		// printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		// printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		// printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		// printf("\x1b[6;1HTime elapsed:  %lld ms\x1b[K", player_object.refresh_info.elapsed);
		// printf("\x1b[7;1HSprite refresh time:  %lld ms\x1b[K", player_object.refresh_info.refresh_time);
		// printf("\x1b[8;1HX:  %f", orb_object.position.x);
		// printf("\x1b[9;1HY:  %f", orb_object.position.y);

		// Render the scene
		
		
		update_object(&player_object);

		update_object(&player_shot_impact_obj[22]);
		update_object(&boss1_object);
		
		update_object(&barrier_object);
		//moveSprites();
		
		if(current_enemy_hp_value > -max_enemy_hp_value){current_enemy_hp_value -= 5.0f;}
	
		
		// -------------------------------------------------------------------------------------------------------
		// --------------------- G A M E P L A Y    P A N T A L L A     I N F E R I O R --------------------------
		// -------------------------------------------------------------------------------------------------------
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(bottom);
			
			
		for (size_t i = 2; i < 4; i ++)
		{
			C2D_DrawSprite(&sprites[i].spr);
		}
		rotacionDelFondo(fondo1_bottom, fondo2_bottom, 600.0f, true);
		draw_orbs();
		if(!dead){
		draw_sprite_animation(&player_object);
		}
		
		// if(dead){
			
			// draw_gameover_fade();
		// }

		
		draw_lasers_bot();
		draw_bullets_bot();
		
		draw_sprite_animation(&player_shot_impact_obj[22]);

		if(focus)
			draw_hitbox();
			
		
		
		if(!skill){
			
			C2D_DrawLine(enemy_ships[0].x-40, enemy_ships[0].y - SCREEN_HEIGHT_BOT, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 
				player_object.position.x, player_object.position.y, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 1.0f, 0.0f);
		}
		

		if(skill){

			
			
			
			if(ts_radius > 0){
			
				C2D_DrawCircleSolid(player_object.position.x , player_object.position.y, 0, ts_radius, 
					C2D_Color32f(1.0f, 0.0f, 0.0f, 0.3f));
					
				ts_radius -= 15.0f;
				 
				}
			//skill = !skill;
		}
		

		draw_boss_pos_bar();

		
		C2D_Flush(); // Limpiar buffer de pantalla inferior
		
		// -------------------------------------------------------------------------------------------------------
		// --------------------- G A M E P L A Y    P A N T A L L A    S U P E R I O R --------------------------
		// -------------------------------------------------------------------------------------------------------
		
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);

		for (size_t i = 0; i < 2; i ++)
		{
			C2D_DrawSprite(&sprites[i].spr);
		}
		rotacionDelFondo(fondo1_top, fondo2_top, 600.0f, false);

		

	
		
		
		if(!skill){
		draw_sprite_animation(&boss1_object);
		draw_sprite_animation(&barrier_object);
		
		C2D_DrawLine (enemy_ships[0].x, enemy_ships[0].y, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 
			player_object.position.x + 40.0f, player_object.position.y + 240.0f, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 1.0f, 0.0f);
		}
		if(skill){
		draw_sprite_only(&boss1_object);
		draw_sprite_only(&barrier_object);
		}
		C2D_DrawSprite(&UI);
		
		draw_lasers_top();
		draw_bullets_top();
		
		if(skill){

			
			
			
			if(ts_radius > 0){
			
				C2D_DrawCircleSolid(player_object.position.x + 40.0f, player_object.position.y + 240.0f, 0, ts_radius, 
					C2D_Color32f(1.0f, 0.0f, 0.0f, 0.3f));
					
				ts_radius -= 22.0f;
				 
				}
			//skill = !skill;
		}
		

		
		//draw_sprite_animation(&barrier_object);
		

		

		C2D_DrawRectangle(SCREEN_WIDTH_TOP, 14, 0, -enemy_ships[0].current_health*(250/enemy_ships[0].health), 6, 
		C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f), C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f), C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f), C2D_Color32f(0.0f, 1.0f, 0.0f, 1.0f));
		
		


		C3D_FrameEnd(0);
		
	}

	// Delete graphics
	deinitialize_object(&player_object);
	deinitialize_object(&boss1_object);
	deinitialize_object(&barrier_object);
	C2D_SpriteSheetFree(general_spritesheet);
	
	// Deinit libs
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
    aptExit();
    srvExit();
	romfsExit();

	return 0;
}