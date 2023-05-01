#include "sprite_animation_manager.h"
#include "main.h"
#include <atomic>


game_state_t 				game_state;
game_state_t				pre_game_state;

// Atributos requeridos por las animaciones de sprites. 

object_2d_info_t			barrier_object;
object_2d_info_t			boss1_object;
object_2d_info_t			boss1_izq_object; 
object_2d_info_t			hit_object;
object_2d_info_t 			player_object;
object_2d_info_t 			player_iz_object;
object_2d_info_t 			player_der_object;
object_2d_info_t 			player_shot_impact_obj; 
object_2d_info_t			robot_1_obj; 
object_2d_info_t  			shot_impacts[MAX_SPRITES];

C2D_Sprite					barrier_sprite[75];
C2D_Sprite 					boss1_sprite[6];
C2D_Sprite					boss1_izq_sprite[3];
C2D_Sprite					hit_sprite[5];
C2D_Sprite 					player_sprite[8];
C2D_Sprite 					player_iz_sprite[5];
C2D_Sprite 					player_der_sprite[5];
C2D_Sprite 					player_shot_impact_sprite[3];
C2D_Sprite					robot_1_spr[1];

sprite_pivot_t 				common_pivot = {0.5f, 0.5f};
sprite_pivot_t 				common_r_pivot = {0.35f, 0.5f};
sprite_pivot_t 				common_l_pivot = {0.65f, 0.5f};
sprite_pivot_t 				barrier_pivot = {0.5f, 0.5f};
sprite_pivot_t				boss1_pivot = {0.38, 0.47f}; 
sprite_pivot_t				boss1_izq_pivot = {0.38, 0.47f};
sprite_pivot_t				hit_pivot = {0.5f, 0.5f};
sprite_pivot_t 				player_pivot = {0.5f, 0.5f};
sprite_pivot_t 				player_shot_impact_pivot = {0.5f, 0.5f};
sprite_pivot_t				robot_1_pivot = {0.5f, 0.5f};

sprite_position_t 			common_pos = {0, 0};
sprite_position_t 			barrier_position = {SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2}; 
sprite_position_t 			boss1_pos = {SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2};
sprite_position_t 			boss1_izq_position;
sprite_position_t			hit_position;
sprite_position_t 			player_position = {SCREEN_WIDTH_BOT/2, SCREEN_HEIGHT_BOT/2};
sprite_position_t 			player_shot_impact_position = {10, 10};
sprite_position_t 			robot_1_pos;

float 						common_rotation = 0.0f;
float 						barrier_rotation = 0.0f;
float 						boss1_rotation = 0.0f;
float 						boss1_izq_rotation = 0.0f;
float 						hit_rotation = 0.0f;
float 						player_rotation = 0.0f;
float 						player_shot_impact_rotation = 0.0f;
float 						robot_1_rotation = 0.0f;

float 						common_rotation_speed = 0.0f;
float 						barrier_rotation_speed = 0.0f;
float 						boss1_rotation_speed = 0.0f;
float 						boss1_izq_rotation_speed = 0.0f;
float 						hit_rotation_speed = 0.0f;
float 						player_rotation_speed = 0.0f;
float 						player_shot_impact_rotation_speed = 0.0f;
float 						robot_1_rotation_speed = 0.0f;

uint64_t 					common_refresh_ms_time = 100; 
uint64_t 					barrier_refresh_ms_time = 100; 
uint64_t					boss1_refresh_ms_time = 200;
uint64_t 					boss1_izq_refresh_ms_time = 300;
uint64_t 					hit_refresh_ms_time = 100;
uint64_t 					player_refresh_ms_time = 100;
uint64_t 					player_shot_impact_refresh_ms_time = 100;
uint64_t					robot_1_refresh_ms_time = 100;

C2D_TextBuf g_dynamicBuf;	

// Atributos de sprites. 

C2D_SpriteSheet general_spritesheet;
C2D_SpriteSheet screen_spritesheet;
C2D_SpriteSheet bg_spritesheet;

Sprite sprites[MAX_SPRITES];

Sprite* fondo1_top;
Sprite* fondo2_top;
Sprite* fondo1_bottom;
Sprite* fondo2_bottom;
Sprite* fondoout;

player_str 		  player;
orb_obj      	  player_orbs[4];
enemy_ship_t      enemy_ships[MAX_BOSSES];
minion      	  minions[MAX_MINIONS];

u32 			  bulletmask;

bullet_t          bullets[MAX_BULLETS];
bullet_enemy      enemy_bullets[MAX_ENEMY_BULLETS];
laser          	  enemy_lasers[MAX_ENEMY_BULLETS];
heal          	  heals[MAX_HEALS];
orb_point         orb_points[MAX_ORB_POINTS];

C2D_Sprite        hitbox;
C2D_Sprite        bullet_normal_sprite;			// 1 px
C2D_Sprite        bullet_normal_sprite2;
C2D_Sprite        bullet_yellow;				// 2 px
C2D_Sprite        bullet_orange;
C2D_Sprite        bullet_red;
C2D_Sprite        blue_plasma_mid_ball;			// 15 px
C2D_Sprite        enemy_sphere_blue;
C2D_Sprite        enemy_sphere_green;
C2D_Sprite        enemy_sphere_red;
C2D_Sprite        enemy_sphere_yellow;
C2D_Sprite        heal_item;
C2D_Sprite        item_frame;
C2D_Sprite        orb_blue;						// 3 px
C2D_Sprite        orb_charge;
C2D_Sprite        orb_charge_b;
C2D_Sprite        orb_green;
C2D_Sprite        orb_orange;
C2D_Sprite        orb_red;
C2D_Sprite        orb_yellow;
C2D_Sprite        orb_player;
C2D_Sprite        yellow_mid_ball;				// 11 px
C2D_Sprite        laser_yellow;
C2D_Sprite        red_big_ball;					// 23 px
C2D_Sprite        shell_red;					// 1 px
C2D_Sprite        star_yellow;
C2D_Sprite        star_blue;					// 11 px
C2D_Sprite        star_mid_red;					// 2 px
C2D_Sprite        star_mini_red;				// 1 px
C2D_Sprite        sphere_tuto;

C2D_Sprite        tuto_1_orb;
C2D_Sprite        tuto_2_orbs;
C2D_Sprite        tuto_damaged;
C2D_Sprite        tuto_focus;
C2D_Sprite        tuto_frame;
C2D_Sprite        tuto_hp;
C2D_Sprite        tuto_medkit;					// 8 px
C2D_Sprite        tuto_mp;
C2D_Sprite        tuto_orb_p;
C2D_Sprite        tuto_orb_p_2;
C2D_Sprite        tuto_ts;
C2D_Sprite        tuto_number_0;
C2D_Sprite        tuto_number_1;
C2D_Sprite        tuto_graze;




std::atomic<std::chrono::steady_clock::time_point> start;
std::chrono::steady_clock::time_point lastTime;
std::chrono::steady_clock::time_point ltime;
long long totalPausedTime = 0;


C2D_Sprite        main_menu_top;
C2D_Sprite        main_menu_bot;

C2D_Sprite        bg_tuto_top;
C2D_Sprite        bg_tuto_bot;

C2D_Sprite        tittle_logo;

C2D_Sprite        UI_bars;
C2D_Sprite        player_hp;
C2D_Sprite        hp_points[PLAYER_HP_MAX];
C2D_Sprite        player_mp;
C2D_Sprite        mp_points[PLAYER_SP_MAX];
C2D_Sprite        player_ts;
C2D_Sprite        ts_points[PLAYER_TS_MAX];
C2D_Sprite        UI_boss;


// Atributos de control. 
u32				playerShotTime;
u32 			playerCurrentInvencibility;
u32 			playerInvencibility;

u32 			playerCurrentSkill;
u32 			playerSkill;

u32 			textControl;


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
u32 skill_time_in_skill;
u32 skill_real_time;

u32 pause_ini;

u32 blink_ini;
u32 blink_current;

auto start_time = std::chrono::steady_clock::now();
auto stored_time = 0;

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
static float r = 45.0f; 	// Radio de giro de los orbes del PJ
static float max_enemy_hp_value = 250.0f;
static float current_enemy_hp_value = 0;
static float t_par[4] = {0.0f, 1.0f, 0.5f, 1.5f};
static float t_impar[3] = {0.0f, 0.666f, 1.333f};
static float ts_radius;
//static size_t bullets = 0;  // ID de los proyectiles del PJ

static bool skill = false;  // Condición de botón presionado
static bool dead = false;  // Condición de botón presionado
static bool blink = false;  // Condición de botón presionado
static u32 lastPrintTime = 0;		// Tiempo transcurrido
unsigned int      framecount; // NOTE: PRINTFRAME needs this name to be unchanged
unsigned int      last_hit_frame;

bool boss_movement_helper = false;

bool rotacionFondo_top = true;	
bool rotacionFondo_bottom = true;	
bool direction_helper = false; 	// Indica si el PJ está en estado "focus" 

bool up = true;
bool left = true;

bool shots = false;

bool fade = false;
float fade_number = 0.0f;
bool menu_helper = false;

bool menu_intro_helper = false;
float menu_pos = 0.0f;

bool paused = false;
bool pause_helper = false;
float pause_transparency = 0.0f;

bool text = false;
float rect_text_height = 0.0f;
float rect_y = 180.0f;

bool dialog = false;
int dialog_index = 0;

int tutorialStep = 1;
float textAlpha = 0.0f;

bool createRect = false;
bool tutorial = false;
bool tutorial_helper = false;
float tutorial_transparency = 0.0f;
float white_transparency = 0.0f;
float black_transparency = 1.0f; // SI SE VE TODO NEGRO PONER ESTO A 0

bool in_intro = false;
bool show_tittle = false;
bool fade_white = false;
bool fade_black = false;
bool fade_normal = true;
int game_mode = 1;


bool helper = false;
bool helper_1 = false;
bool helper_2 = false;
bool helper_3 = false;
bool helper_4 = false;
bool helper_5 = false;
bool helper_6 = false;
bool helper_7 = false;
bool helper_8 = false;
bool helper_9 = false;

int bullet_counter_1 = 0;
int bullet_counter_2 = 0;
int bullet_counter_3 = 0;
int bullet_counter_4 = 0;
int bullet_counter_5 = 0;
int bullet_counter_6 = 0;
int bullet_counter_7 = 0;
int bullet_counter_8 = 0;
int bullet_counter_9 = 0;


bool ts_helper = false;

void init_player(){

	initialize_object(&player_object, \
						player_sprite,	\
						"romfs:/gfx/player.t3x", \
						player_pivot, \
						player_position, \
						player_rotation, \
						player_rotation_speed, \
						player_refresh_ms_time, \
						false, \
						true);

	initialize_object(&player_iz_object, \
						player_iz_sprite,	\
						"romfs:/gfx/player_iz.t3x", \
						common_r_pivot, \
						common_pos, \
						common_rotation, \
						common_rotation_speed, \
						common_refresh_ms_time, \
						false, \
						true);

	initialize_object(&player_der_object, \
						player_der_sprite,	\
						"romfs:/gfx/player_der.t3x", \
						common_l_pivot, \
						common_pos, \
						common_rotation, \
						common_rotation_speed, \
						common_refresh_ms_time, \
						false, \
						true);


	initialize_object(&boss1_object, \
						boss1_sprite,	\
						"romfs:/gfx/Boss-1.t3x", \
						boss1_pivot, \
						boss1_pos, \
						boss1_rotation, \
						boss1_rotation_speed, \
						boss1_refresh_ms_time, \
						false, \
						true);
						  
	initialize_object(&boss1_izq_object, \
						boss1_izq_sprite,	\
						"romfs:/gfx/Boss-1-izq.t3x", \
						boss1_izq_pivot, \
						boss1_izq_position, \
						boss1_izq_rotation, \
						boss1_izq_rotation_speed, \
						boss1_izq_refresh_ms_time, \
						true, \
						true);
						  
	initialize_object(&barrier_object, \
						barrier_sprite,	\
						"romfs:/gfx/shield.t3x", \
						barrier_pivot, \
						barrier_position, \
						barrier_rotation, \
						barrier_rotation_speed, \
						barrier_refresh_ms_time, \
						false, \
						true);

		  
	initialize_object(&player_shot_impact_obj, \
						player_shot_impact_sprite,	\
						"romfs:/gfx/impact_shot.t3x", \
						player_shot_impact_pivot, \
						player_shot_impact_position, \
						player_shot_impact_rotation, \
						player_shot_impact_rotation_speed, \
						player_shot_impact_refresh_ms_time, \
						false, \
						true);
	

	initialize_object(&player_shot_impact_obj, \
						player_shot_impact_sprite,	\
						"romfs:/gfx/impact_shot.t3x", \
						player_shot_impact_pivot, \
						player_shot_impact_position, \
						player_shot_impact_rotation, \
						player_shot_impact_rotation_speed, \
						player_shot_impact_refresh_ms_time, \
						false, \
						true);
	
						  
	initialize_object(&robot_1_obj, \
						robot_1_spr,	\
						"romfs:/gfx/robot_1.t3x", \
						robot_1_pivot, \
						robot_1_pos, \
						robot_1_rotation_speed, \
						robot_1_rotation_speed, \
						robot_1_refresh_ms_time, \
						false, \
						true);
						  
	player.x = SCREEN_WIDTH_BOT/2;
	player.y = SCREEN_HEIGHT_BOT/2;
	player.yspeed = 0.0f;
	player.xspeed = 0.0f;
	player.radius = 3.0f;
	player.health = PLAYER_HP_START;
	player.invencible = false;
	
	player.skills = PLAYER_TS_START;
	player.skill_power = 0.0f;

	player.player_spr = &player_object;

	player.orbs = 3;
	player.orb_points = 6;
	player.orb_energy = 0;

	player.power = PLAYER_SP_START;
	

	C2D_SpriteFromSheet(&hitbox, general_spritesheet, 12);
		C2D_SpriteSetCenter(&hitbox, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&heal_item, general_spritesheet, 37);
		C2D_SpriteSetCenter(&heal_item, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_charge, general_spritesheet, 17);
		C2D_SpriteSetCenter(&orb_charge, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_charge_b, general_spritesheet, 39);
		C2D_SpriteSetCenter(&orb_charge_b, 0.5f, 0.5f);

	C2D_SpriteFromSheet(&enemy_sphere_blue, general_spritesheet, 28);
		C2D_SpriteSetCenter(&enemy_sphere_blue, 0.5f, 0.5f);
			C2D_SpriteSetRotation(&enemy_sphere_blue, 1.0f);
	C2D_SpriteFromSheet(&enemy_sphere_green, general_spritesheet, 29);
		C2D_SpriteSetCenter(&enemy_sphere_green, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&enemy_sphere_red, general_spritesheet, 30);
		C2D_SpriteSetCenter(&enemy_sphere_red, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&enemy_sphere_yellow, general_spritesheet, 31);
		C2D_SpriteSetCenter(&enemy_sphere_yellow, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&sphere_tuto, general_spritesheet, 32);
		C2D_SpriteSetCenter(&sphere_tuto, 0.5f, 0.5f);
			C2D_SpriteSetRotation(&sphere_tuto, 1.0f);


	C2D_SpriteFromSheet(&bullet_normal_sprite, general_spritesheet, 0);
		C2D_SpriteSetCenter(&bullet_normal_sprite, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&bullet_normal_sprite2, general_spritesheet, 2);
		C2D_SpriteSetCenter(&bullet_normal_sprite2, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&bullet_yellow, general_spritesheet, 18);
		C2D_SpriteSetCenter(&bullet_yellow, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&bullet_orange, general_spritesheet, 19);
		C2D_SpriteSetCenter(&bullet_orange, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&bullet_red, general_spritesheet, 20);
		C2D_SpriteSetCenter(&bullet_red, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&shell_red, general_spritesheet, 21);
		C2D_SpriteSetCenter(&shell_red, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&blue_plasma_mid_ball, general_spritesheet, 6);
		C2D_SpriteSetCenter(&blue_plasma_mid_ball, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_blue, general_spritesheet, 33);
		C2D_SpriteSetCenter(&orb_blue, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_green, general_spritesheet, 16);
		C2D_SpriteSetCenter(&orb_green, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_orange, general_spritesheet, 34);
		C2D_SpriteSetCenter(&orb_orange, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_red, general_spritesheet, 35);
		C2D_SpriteSetCenter(&orb_red, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_yellow, general_spritesheet, 36);
		C2D_SpriteSetCenter(&orb_yellow, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&orb_player, general_spritesheet, 22);
		C2D_SpriteSetCenter(&orb_player, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&yellow_mid_ball, general_spritesheet, 7);
		C2D_SpriteSetCenter(&yellow_mid_ball, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&red_big_ball, general_spritesheet, 8);
		C2D_SpriteSetCenter(&laser_yellow, 0.45f, 0.0f);
	C2D_SpriteFromSheet(&laser_yellow, general_spritesheet, 11);
		C2D_SpriteSetCenter(&red_big_ball, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&star_yellow, general_spritesheet, 13);
		C2D_SpriteSetCenter(&star_yellow, 0.5f, 0.5f);
				C2D_SpriteSetRotation(&star_yellow, 1.5f);
	C2D_SpriteFromSheet(&star_blue, general_spritesheet, 15);
		C2D_SpriteSetCenter(&star_blue, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&star_mini_red, general_spritesheet, 14);
		C2D_SpriteSetCenter(&star_mini_red, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&star_mid_red, general_spritesheet, 38);
		C2D_SpriteSetCenter(&star_mid_red, 0.5f, 0.5f);

	C2D_SpriteFromSheet(&UI_bars, general_spritesheet, 9);
		C2D_SpriteSetPos(&UI_bars, 0, 192);
	C2D_SpriteFromSheet(&UI_boss, general_spritesheet, 25);
		C2D_SpriteSetCenter(&UI_boss, 1.0f, 0.0f);
			C2D_SpriteSetPos(&UI_boss, 400, 8);
	C2D_SpriteFromSheet(&item_frame, general_spritesheet, 26);
		C2D_SpriteSetCenter(&item_frame, 1.0f, 0.0f);
			C2D_SpriteSetPos(&item_frame, 400, 192);

	C2D_SpriteFromSheet(&player_hp, general_spritesheet, 23);
	C2D_SpriteFromSheet(&player_mp, general_spritesheet, 24);
	C2D_SpriteFromSheet(&player_ts, general_spritesheet, 27);

	C2D_SpriteFromSheet(&main_menu_top, screen_spritesheet, 0);
	C2D_SpriteFromSheet(&main_menu_bot, screen_spritesheet, 1);

	C2D_SpriteFromSheet(&tittle_logo, bg_spritesheet, 2);
			C2D_SpriteSetPos(&tittle_logo, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 5);
				C2D_SpriteSetCenter(&tittle_logo, 0.5f, 0.5f);

	C2D_SpriteFromSheet(&bg_tuto_top, bg_spritesheet, 0);
		C2D_SpriteSetPos(&bg_tuto_top, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2);
			C2D_SpriteSetCenter(&bg_tuto_top, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&bg_tuto_bot, bg_spritesheet, 1);
		C2D_SpriteSetPos(&bg_tuto_bot, SCREEN_WIDTH_BOT/2, SCREEN_HEIGHT_BOT/2);
			C2D_SpriteSetCenter(&bg_tuto_bot, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_frame, screen_spritesheet, 2);
			C2D_SpriteSetPos(&tuto_frame, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2);
				C2D_SpriteSetCenter(&tuto_frame, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_hp, screen_spritesheet, 3);
			C2D_SpriteSetPos(&tuto_hp, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_hp, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_damaged, screen_spritesheet, 4);
			C2D_SpriteSetPos(&tuto_damaged, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_damaged, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_medkit, screen_spritesheet, 5);
			C2D_SpriteSetPos(&tuto_medkit, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_medkit, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_focus, screen_spritesheet, 6);
			C2D_SpriteSetPos(&tuto_focus, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_focus, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_mp, screen_spritesheet, 7);
			C2D_SpriteSetPos(&tuto_mp, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_mp, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_1_orb, screen_spritesheet, 8);
			C2D_SpriteSetPos(&tuto_1_orb, SCREEN_WIDTH_TOP/2 - 70, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_1_orb, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_2_orbs, screen_spritesheet, 9);
			C2D_SpriteSetPos(&tuto_2_orbs, SCREEN_WIDTH_TOP/2 + 70, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_2_orbs, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_orb_p, screen_spritesheet, 10);
			C2D_SpriteSetPos(&tuto_orb_p, SCREEN_WIDTH_TOP/2 - 70, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_orb_p, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_orb_p_2, screen_spritesheet, 11);
			C2D_SpriteSetPos(&tuto_orb_p_2, SCREEN_WIDTH_TOP/2 + 70, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_orb_p_2, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_ts, screen_spritesheet, 12);
			C2D_SpriteSetPos(&tuto_ts, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_ts, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_number_0, screen_spritesheet, 13);
			C2D_SpriteSetPos(&tuto_number_0, SCREEN_WIDTH_TOP/2 - 100, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_number_0, 0.5f, 0.5f);		
	C2D_SpriteFromSheet(&tuto_number_1, screen_spritesheet, 14);
			C2D_SpriteSetPos(&tuto_number_1, SCREEN_WIDTH_TOP/2 + 100, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_number_1, 0.5f, 0.5f);
	C2D_SpriteFromSheet(&tuto_graze, screen_spritesheet, 15);
			C2D_SpriteSetPos(&tuto_graze, SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2 - 35);
				C2D_SpriteSetCenter(&tuto_graze, 0.5f, 0.5f);
														
	int j = 0;
	for (; j < 4; ++j) {

		if (!player_orbs[j].state) { // inactive, free to use slot
			  

			player_orbs[j].x = player_object.position.x;
			player_orbs[j].y = player_object.position.y;				  
			  
			player_orbs[j].xspeed = 0;
			player_orbs[j].yspeed = 0;			

			player_orbs[j].spr = &orb_player;
			  
			player_orbs[j].state = ORB_STATE_ACTIVE;
			
		}		
	}

	for (int k = 0; k < PLAYER_HP_MAX; k++) {

		hp_points[k] = player_hp;

	}

	for (int l = 0; l < PLAYER_SP_MAX; l++) {

		mp_points[l] = player_mp;

	}

	for (int m = 0; m < PLAYER_TS_MAX; m++) {

		ts_points[m] = player_ts;

	}

}

void reset_helpers(){  //helpers, lastprints, level angles, bulletCounter

	helper = false;
	helper_1 = false;
	helper_2 = false;
	helper_3 = false;
	helper_4 = false;
	helper_5 = false;
	helper_6 = false;
	helper_7 = false;
	helper_8 = false;
	helper_9 = false;
	lastPrintTime_1 = 0;
	lastPrintTime_2 = 0;
	lastPrintTime_3 = 0;
	lastPrintTime_4 = 0;
	lastPrintTime_5 = 0;
	lastPrintTime_6 = 0;
	lastPrintTime_7 = 0;
	lastPrintTime_8 = 0;
	lastPrintTime_9 = 0;
	level_angle_1 = 0;
	level_angle_2 = 0;
	level_angle_3 = 0;
	level_angle_4 = 0;
	level_angle_5 = 0;
	level_angle_6 = 0;
	level_angle_7 = 0;
	level_angle_8 = 0;
	level_angle_9 = 0;
	bullet_counter_1 = 0;
	bullet_counter_2 = 0;
	bullet_counter_3 = 0;
	bullet_counter_4 = 0;
	bullet_counter_5 = 0;
	bullet_counter_6 = 0;
	bullet_counter_7 = 0;
	bullet_counter_8 = 0;
	bullet_counter_9 = 0;

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

void draw_hitbox(void){
	C2D_SpriteSetPos(&hitbox, player.x, player.y);
	C2D_DrawSprite(&hitbox);
}

void draw_impact(float x, float y, object_2d_info_t *o){

	o->position_velocity.dx = 1;
	o->position.x = x;
	o->position.y = y;
	o->active = true;

}

void draw_minions_top(void){

	int i = 0;
	for (; i < MAX_MINIONS; i++) {
		if (minions[i].state) { // not inactive
			if(minions[i].top == true){
					C2D_SpriteSetPos(minions[i].enemy_spr, minions[i].x, minions[i].y);
					C2D_SpriteSetRotation(minions[i].enemy_spr, deg_to_rad(-minions[i].angle + 90));
					C2D_DrawSprite(minions[i].enemy_spr);
			}
	    }  
	}
}

void draw_minions_bot(void){

	int i = 0;
	for (; i < MAX_MINIONS; i++) {
		if (minions[i].state) { // not inactive
			if(minions[i].top == false){
					C2D_SpriteSetPos(minions[i].enemy_spr, minions[i].x, minions[i].y);
					C2D_SpriteSetRotation(minions[i].enemy_spr, deg_to_rad(-minions[i].angle + 90));
					C2D_DrawSprite(minions[i].enemy_spr);
			}
	    }  
	}
}

void update_all(){

	//a
	

}

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

			if(player.orbs % 2 == 0)
			{
				if(inside_circle(player_orbs[i].x, player_orbs[i].y, player_object.position.x, player_object.position.y, r) && focus_helper)
				{
					// velocidad = punto donde se encuentra
					player_orbs[i].xspeed =
					((player.x + r * cos(t_par[i] * M_PI)) - player_orbs[i].x) / 5;
					player_orbs[i].yspeed =
					((player.y - r * sin(t_par[i] * M_PI)) - player_orbs[i].y) / 5;
							
				}		
				else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
				{		
					// posición del orbe = posición del PJ + giro alrededor	

					focus_helper = false;
					player_orbs[i].x = player.x + r * cos(t_par[i] * M_PI);
					player_orbs[i].y = player.y - r * sin(t_par[i] * M_PI);

				}
			}
			else
			{
				if(inside_circle(player_orbs[i].x, player_orbs[i].y, player_object.position.x, player_object.position.y, r) && focus_helper)
				{
					// velocidad = punto donde se encuentra
					player_orbs[i].xspeed =
					((player.x + r * cos(t_impar[i] * M_PI)) - player_orbs[i].x) / 5;
					player_orbs[i].yspeed =
					((player.y - r * sin(t_impar[i] * M_PI)) - player_orbs[i].y) / 5;
							
				}		
				else  // sino (el orbe se encuentra en el límite de la esfera que debe recorrer)
				{		

					focus_helper = false;
					player_orbs[i].x = player.x + r * cos(t_impar[i] * M_PI);
					player_orbs[i].y = player.y - r * sin(t_impar[i] * M_PI);

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

			if(player_orbs[j].x != player.x &&
			player_orbs[j].y != player.y - 20.0f)
			{

				if(player.orbs % 2 == 0)
				{

					focus_helper = true;
					player_orbs[j].xspeed = ((player.x + x_par[j]) - player_orbs[j].x) / 5;
					player_orbs[j].yspeed = ((player.y + y_par[j] - 20.0f) - player_orbs[j].y) / 5;

				}
				else
				{
					focus_helper = true;
					player_orbs[j].xspeed = ((player.x + x_inpar[j]) - player_orbs[j].x) / 5;
					player_orbs[j].yspeed = ((player.y + y_inpar[j] - 20.0f) - player_orbs[j].y) / 5;
				}	
			}
		}
	}	
}

void orb_logic(void){

	int i = 0;
	for(; i < 4; i++){

		player_orbs[i].x += player_orbs[i].xspeed;
		player_orbs[i].y += player_orbs[i].yspeed;

		if((i+1) > player.orbs)
		player_orbs[i].state = ORB_STATE_INACTIVE;
		else
		player_orbs[i].state = ORB_STATE_ACTIVE;

	}
		
}

void spawn_minion_top(float x, float y, float xs, float ys, float r, float rotation_speed, float hp, 
								C2D_Sprite* e_spr, bool top){

	for (int i = MAX_MINIONS/2; i < MAX_MINIONS; i++) {

		if (!minions[i].state) { // inactive, free to use slot

			minions[i] = spawn_minion(x-40,y-240,xs,ys,r,rotation_speed,hp,e_spr,!top);

			break;

		}
			
	}
	
	for (int j = 0; j < MAX_MINIONS/2; j++) {

		if (!minions[j].state) { // inactive, free to use slot

			minions[j] = spawn_minion(x,y,xs,ys,r,rotation_speed,hp,e_spr,top);

			break;

		}		
	}

}

void set_minions_speed(float x, float y, int minion){

	minions[minion].xspeed = x;
	minions[minion].yspeed = y;
	minions[minion + MAX_MINIONS/2].xspeed = x;
	minions[minion + MAX_MINIONS/2].yspeed = y;

}

void set_minions_delete_border_top(float lx, float rx, float uy, float dy, int minion){

	if(!inside_rect(minions[minion].x, minions[minion].y, lx, rx, uy, dy) 
		&& minions[minion].top == true) // TOP
	{
		minions[minion].state = ENEMY_STATE_INACTIVE;
	}

	if(!inside_rect(minions[minion + MAX_MINIONS/2].x, minions[minion + MAX_MINIONS/2].y, lx-40, rx-40, uy-240, dy-240) 
		&& minions[minion + MAX_MINIONS/2].top == false) // BOT
	{
		minions[minion + MAX_MINIONS/2].state = ENEMY_STATE_INACTIVE;
	}

}

void minion_delete_list(){

	set_minions_delete_border_top(0, 400, -500, 300, 0);

}

void spawn_orb_point(float x, float y, bool top, orb_point_type_t type, C2D_Sprite* orb_spr){

	for (int i = MAX_ORB_POINTS/2; i < MAX_ORB_POINTS; i++) {

		if (!orb_points[i].state) { // inactive, free to use slot

			orb_points[i] = spawn_orb_p(x-40,y-240,!top,type,orb_spr);

			break;

		}
			
	}
	
	for (int j = 0; j < MAX_ORB_POINTS/2; j++) {

		if (!orb_points[j].state) { // inactive, free to use slot

			orb_points[j] = spawn_orb_p(x,y,top,type,orb_spr);

			break;

		}		
	}

}

void spawn_heal_top(float x, float y, bool top, C2D_Sprite* heal_spr){

	for (int i = MAX_HEALS/2; i < MAX_HEALS; i++) {

		if (!heals[i].state) { // inactive, free to use slot

			heals[i] = spawn_heal(x-40,y-240,!top,heal_spr);

			break;

		}
			
	}
	
	for (int j = 0; j < MAX_HEALS/2; j++) {

		if (!heals[j].state) { // inactive, free to use slot

			heals[j] = spawn_heal(x,y,top,heal_spr);

			break;

		}		
	}

}

void player_hit_effect(){

	playerCurrentInvencibility = osGetTime();

	if(playerCurrentInvencibility - playerInvencibility > 4000)
	{		
		dead = !dead;
	}

	blink_current = osGetTime();

	u32 blink_elapsed = (blink_current - blink_ini);

	if(blink_elapsed >= 200)
	{
		blink = !blink;
		blink_ini = osGetTime();
	}
	
}
	
void player_logic(float x, float y, u32 kHeld){
	  
	if ((player.player_spr->position.y <= BG_LIMIT_UP_BOT) && (player.player_spr->position_velocity.dy < 0))
	{
		if(!focus)  
		{
			player.xspeed = x/PLAYER_SPEED_DIV;				
			player.y = BG_LIMIT_UP_BOT;				
			player.player_spr->position_velocity.dx = x/PLAYER_SPEED_DIV;	
			player.player_spr->position.y = BG_LIMIT_UP_BOT; 
		} 
		else 	
		{
			player.xspeed = x/PLAYER_SPEED_FOCUS_DIV;				
			player.y = BG_LIMIT_UP_BOT;				
			player.player_spr->position_velocity.dx = x/PLAYER_SPEED_FOCUS_DIV;
			player.player_spr->position.y = BG_LIMIT_UP_BOT; 
		}
	}
	if ((player.player_spr->position.y >= BG_LIMIT_DOWN_BOT) && (player.player_spr->position_velocity.dy > 0))
	{
		if(!focus)
		{
			player.xspeed = x/PLAYER_SPEED_DIV;				
			player.y = BG_LIMIT_DOWN_BOT;				
			player.player_spr->position_velocity.dx = x/PLAYER_SPEED_DIV;
			player.player_spr->position.y = BG_LIMIT_DOWN_BOT;
		} 
		else 
		{
			player.xspeed = x/PLAYER_SPEED_FOCUS_DIV;				
			player.y = BG_LIMIT_DOWN_BOT;
			player.player_spr->position_velocity.dx = x/PLAYER_SPEED_FOCUS_DIV;
			player.player_spr->position.y = BG_LIMIT_DOWN_BOT;
		}
	}
	if ((player.player_spr->position.x <= BG_LIMIT_LEFT_BOT) && (player.player_spr->position_velocity.dx < 0))
	{
		if(!focus)
		{
			player.yspeed = -y/PLAYER_SPEED_DIV;
			player.x = BG_LIMIT_LEFT_BOT;				
			player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_DIV;
			player.player_spr->position.x = BG_LIMIT_LEFT_BOT;
		} 
		else 
		{
			player.yspeed = -y/PLAYER_SPEED_FOCUS_DIV;
			player.x = BG_LIMIT_LEFT_BOT;				
			player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_FOCUS_DIV;
			player.player_spr->position.x = BG_LIMIT_LEFT_BOT;
		}
	}
	if ((player.player_spr->position.x >= BG_LIMIT_RIGHT_BOT) && (player.player_spr->position_velocity.dx > 0))
	{
		if(!focus)
		{
			player.yspeed = -y/PLAYER_SPEED_DIV;
			player.x = BG_LIMIT_RIGHT_BOT;				
			player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_DIV;
			player.player_spr->position.x = BG_LIMIT_RIGHT_BOT;
		} 
		else 
		{
			player.yspeed = -y/PLAYER_SPEED_FOCUS_DIV;
			player.x = BG_LIMIT_RIGHT_BOT;
			player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_FOCUS_DIV;
			player.player_spr->position.x = BG_LIMIT_RIGHT_BOT;
		}
	}

	if(!focus)
	{
		
		player.xspeed = x/PLAYER_SPEED_DIV;
		player.yspeed = -y/PLAYER_SPEED_DIV;
		player.player_spr->position_velocity.dx = x/PLAYER_SPEED_DIV;
		player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_DIV;		 
	} 
	else 
	{
		player.xspeed = x/PLAYER_SPEED_FOCUS_DIV;
		player.yspeed = -y/PLAYER_SPEED_FOCUS_DIV;
		player.player_spr->position_velocity.dx = x/PLAYER_SPEED_FOCUS_DIV;
		player.player_spr->position_velocity.dy = -y/PLAYER_SPEED_FOCUS_DIV;			
	}
	
	player.x += player.xspeed;
	player.y += player.yspeed;

	if(player.xspeed < 0){
		player_iz_object.position.x = player.x;
		player_iz_object.position.y = player.y;
		player.player_spr = &player_iz_object;
	}
	
	if(player.xspeed > 0){
		player_der_object.position.x = player.x;
		player_der_object.position.y = player.y;
		player.player_spr = &player_der_object;
	}

	if(player.xspeed == 0){
		player_object.position.x = player.x;
		player_object.position.y = player.y;
		player.player_spr = &player_object;
	}
	

	orb_movement();

	if(player.skill_power >= 100.0f && player.skills < 5)
	{
		player.skills++;
		player.skill_power = 0.0f;
	}
	if(player.skill_power >= 0.0f && player.skills == 5)
	{
		player.skill_power = 0.0f;
	}

	if(player.orbs < 4)
	{
		if(player.orb_energy >= 100.0f && player.orb_points < 7)
		{
			player.orb_points++;
			player.orb_energy = 0.0f;
		}
		if(player.orb_energy >= 100.0f && player.orb_points == 7)
		{
			player.orbs++;
			player.orb_points = 0;
			player.orb_energy = 0.0f;
		}
	}
	if(player.orbs == 4)
	{
		player.orb_points = 7;
		player.orb_energy = 0.0f;
	}
	if(!dead)
	blink = true;
	else
	player_hit_effect();

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
				enemy_ships[n].enemy_spr->refresh_info.refresh_time = 300;
				enemy_ships[n].enemy_spr->frame_info.loop_once = true;
			}
			if(enemy_ships[n].xspeed > 0)
			{  
				enemy_ships[n].enemy_spr->object_sprite = boss1_izq_sprite;
				enemy_ships[n].enemy_spr->frame_info.current_frame_index = 0;
				enemy_ships[n].enemy_spr->frame_info.num_of_sprites = 3;
				enemy_ships[n].enemy_spr->refresh_info.refresh_time = 300;
				enemy_ships[n].enemy_spr->frame_info.loop_once = true;
			}
		}
	}

	for (int i = 0; i < MAX_MINIONS/2; i++) {
			
		if (minions[i].state) { // not inactive

			minions[i].top = true;
			
			float pbx = minions[i].x + minions[i].xspeed;
			float pby = minions[i].y + minions[i].yspeed;
			  
			minions[i].x = pbx;
			minions[i].y = pby;

			minions[i].angle +=  minions[i].rotation_speed;

			if (minions[i].current_health <= 0 || minions[i+MAX_MINIONS/2].current_health  <= 0)
			{
				for (int ii = 0; ii < 5; ii++){
					spawn_orb_p(minions[i].x + randomBetween(-50, 50), minions[i].y + randomBetween(-50, 50), true, ORB_POINT_TYPE_B, &orb_charge);
					spawn_orb_p(minions[i].x + randomBetween(-50, 50), minions[i].y + randomBetween(-50, 50), true, ORB_POINT_TYPE_Y, &orb_charge);
				}
				minions[i].state = ENEMY_STATE_INACTIVE;
			}  
		}
	} 
	
	  
	for (int j = MAX_MINIONS/2; j < MAX_MINIONS; j++) {
			
		if (minions[j].state) { // not inactive

			minions[j].top = false;
			
			float pbx = minions[j].x + minions[j].xspeed;
			float pby = minions[j].y + minions[j].yspeed;
			  
			minions[j].x = pbx;
			minions[j].y = pby;

			minions[j].angle +=  minions[j].rotation_speed;

			if (minions[j].current_health <= 0 || minions[j-MAX_MINIONS/2].current_health  <= 0)
			{
				for (int jj = 0; jj < 5; jj++){
				spawn_orb_p(minions[j].x + randomBetween(-50, 50), minions[j].y + randomBetween(-50, 50), false, ORB_POINT_TYPE_B, &orb_charge);
				spawn_orb_p(minions[j].x + randomBetween(-50, 50), minions[j].y + randomBetween(-50, 50), false, ORB_POINT_TYPE_Y, &orb_charge);
				}
				minions[j].state = ENEMY_STATE_INACTIVE;
			}  
		}
	}  
}

void draw_boss_pos_bar(void){

	for (int n = 0; n < MAX_BOSSES; n++) {

		C2D_DrawRectSolid(enemy_ships[n].x/2, 240, 0, -20, -10, 
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

			}
			 
			break;

		}		
	}
}
		
void spread(float x, float y, float angle, float direction, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, int total_bullets, C2D_Sprite *sprite, bool bot, bullet_enemy* e){

	int i = 0;
	for (; i < total_bullets; ++i) {

		shoot_enemy_bullet(x, y, (angle * i) / (total_bullets - 1) + direction - 90, speed, rotation_spd, radius, graze_radius, graze_power, sprite, bot, e);
			
    }
}

void burst_aim(float x, float y, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, int total_bullets, C2D_Sprite *sprite, bool bot, bullet_enemy* e){
		
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

		shoot_enemy_bullet(x, y, ((360)/total_bullets) * i - rad_to_deg(a), speed, rotation_spd, radius, graze_radius, graze_power, sprite, bot, e);
			
    }
}

double calculateRatio(u32 playerCurrentInvencibility, u32 playerInvencibility) {
    double ratio = 1.0;   // Valor inicial de la proporción

    // Calcular el tiempo transcurrido en milisegundos
    double elapsed_tim = ((double)(playerCurrentInvencibility - playerInvencibility)) / 4000;

    if (elapsed_tim < 1.0) {   // Si el tiempo transcurrido es menor a 1.0
        ratio = 1.0 - elapsed_tim;   // Calcular la proporción
    }

    return ratio;
}

void to_white(){

	if(white_transparency < 1.0f)
	white_transparency += 0.01f;
	else
	fade_white = false;
}

void to_black(){

	if(black_transparency < 1.0f)
	black_transparency += 0.005f;
	else
	fade_black = false;
}

void to_normal(){

	if(black_transparency > 0)
	black_transparency -= 0.01f;
	
	if(white_transparency > 0)
	white_transparency -= 0.01f;
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
				playerCurrentInvencibility = osGetTime();
				
				if(playerCurrentInvencibility - playerInvencibility > 4000){
					
					playerInvencibility = playerCurrentInvencibility;

					if(!player.invencible)
					--player.health; 

					dead = true;
					enemy_bullets[n].state = BULLET_STATE_INACTIVE;

				}
		    }

			if (enemy_bullets[n].bot_screen && 
				cirlce_inside_circle(pbx, pby, player.x, player.y, player.radius, enemy_bullets[n].graze_radius) && 
				enemy_bullets[n].grazed == false)
			{

				enemy_bullets[n].grazed = true;

				if(player.skill_power < 100.0f)
				player.skill_power += enemy_bullets[n].graze_power;
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
			
			for (int j = 0; j < MAX_BOSSES; j++) { 
				
			 	if (!bullets[i].bot_screen && cirlce_inside_circle(bx, by, enemy_ships[j].x, enemy_ships[j].y, enemy_ships[j].radius, bullets[i].radius) && enemy_ships[j].state)
			 	{
			 		if(!enemy_ships[j].invencible)
			 		enemy_ships[j].current_health -= 0.1f;
					
			 		bullets[i].state = BULLET_STATE_INACTIVE;
			 	}	
			}

			for (int k = 0; k < MAX_MINIONS; k++) { 
				
			 	if (!bullets[i].bot_screen && cirlce_inside_circle(bx, by, minions[k].x, minions[k].y, minions[k].radius, bullets[i].radius) && minions[k].state && minions[k].top)
			 	{
			 		if(!minions[k].invencible)
			 		minions[k].current_health -= 0.1f;
					playAudio(AUDIO_E_DAMAGE);
			 		bullets[i].state = BULLET_STATE_INACTIVE;
			 	}	

				if (bullets[i].bot_screen && cirlce_inside_circle(bx, by, minions[k].x, minions[k].y, minions[k].radius, bullets[i].radius) && minions[k].state && !minions[k].top)
			 	{
			 		if(!minions[k].invencible)
			 		minions[k].current_health -= 0.1f;
					playAudio(2);
			 		bullets[i].state = BULLET_STATE_INACTIVE;
			 	}
			}

		    if(!inside_rect(bullets[i].x, bullets[i].y, -20, 420, -50, 500) 
				&& bullets[i].bot_screen == false) // TOP
		    {
				bullets[i].state = BULLET_STATE_INACTIVE;
		    }
		  
		    if(!inside_rect(bullets[i].x, bullets[i].y, -60, 340, -260, 280) 
				&& bullets[i].bot_screen == true) // BOT
		    {
				bullets[i].state = BULLET_STATE_INACTIVE;
		    }
		}
	}
}

void enemy_movement(enemy_ship_t* e, float v){

	if(e->xspeed == 0)
	{
		if(up && (e->y >= e->y ))
		{
			e->yspeed = -0.05;
		}
		if(up && (e->y < e->y ))
		e->yspeed = 0.05;
	}

	if(left && (e->x < 40))
	{

		left = !left;

		e->xspeed = v;

	}
	if(!left && (e->x > 360))
	{

		left = !left;

		e->xspeed = -v;

	}
	if(e->x >= 40 && e->x <= 360)
	e->xspeed = left ? -v : v;

}

/*
void shoting(void){
	
	if(!level_helper_1)
	{
	currentTime_4 = osGetTime();
	level_helper_1 = true;
	}
	
	u32 currentTime_2 = osGetTime();

    if (currentTime_2 - skill_time_elapsed - lastPrintTime_2 >= 2000) 
	{
        lastPrintTime_2 = currentTime_2 - skill_time_elapsed;
		//skill_time_elapsed = 0;
		//int rand = randomBetween(0,2);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(2.0f,2.5f), 0.0f, 2.0f, &bullet_yellow, false);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(2.6f,3.1f), 0.0f, 2.0f, &bullet_orange, false);
		//shoot_enemy_bullet(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(-25.0f,25.0f) - 90, randomBetween(3.2f,3.6f), 0.0f, 2.0f, &bullet_red, false);
		//shoot_enemy_bullet_aim(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, randomBetween(6.2f,7.0f), 0.0f, 2.0f, &bullet_red, false);

	//	float x, float y, float angle, float speed, float rotation_spd, float radius, float graze_radius, float graze_power, int total_bullets, C2D_Sprite *sprite, bool bot){

		//spread(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_1, 0, 2.5f, 2.0f, 3.0f, 3, &orb_green, false);

		burst(minions[0].x, minions[0].y, level_angle_1, 1.0f, 0.0f, 2.0f, 6.0f, 2.0f, 4, &orb_green, false);

		//burst(SCREEN_WIDTH_TOP/2, SCREEN_HEIGHT_TOP/2, level_angle_1+30.0f, 2.5f, 2.0f, 10.0f, 16.0f, 5.0f, 6, &star_blue, false);
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
*/
void time_in_skill(){

	skill_end = osGetTime();

	

	if (skill_end - skill_ini >= 1400 && skill)
	{
		skill = false;
		skill_time_elapsed += skill_time_in_skill;
	}

	if(!skill)
	skill_time_in_skill = 0;
	if(skill)
	skill_time_in_skill = skill_end - skill_ini;

	skill_real_time = skill_time_elapsed + skill_time_in_skill;
	

}

void game_intro(){

	if(!helper_9)
	{
		lastPrintTime_9 = osGetTime();
		helper_9 = true;
	}

	C2D_SpriteSetPos(&main_menu_top, 0, menu_pos);
	C2D_SpriteSetPos(&main_menu_bot, 0, menu_pos);

	if(!menu_intro_helper)
	{
		if(menu_pos > -65.0f)
		menu_pos -= 0.4f;

		if(menu_pos < -65.0f)
		menu_intro_helper = !menu_intro_helper;
	}

	if(time_passed(lastPrintTime_9, 3000))
	{
		fade_normal = false;
		if(!time_passed(lastPrintTime_9, 5000))
		to_white();
	}

	if(time_passed(lastPrintTime_9, 5000))
	{
		to_normal();
		if(!helper_8)
		{
			playAudio(AUDIO_INTRO);
			helper_8 = true;
		}
		show_tittle = true;
	}

	if(time_passed(lastPrintTime_9, 8000))
	{
		in_intro = false;
	}

	if(fade_white)
	to_white();

	if(fade_black)
	to_black();

	if(fade_normal)
	to_normal();

	if(pre_game_state == GAME_STATE_GAMEPLAY)
	{
		to_black();
	}
	if(pre_game_state == GAME_STATE_TUTORIAL)
	{
		to_black();

		if(!helper_7)
		{
			lastPrintTime_8 = osGetTime();
			helper_7 = true;
		}

		if(time_passed(lastPrintTime_8, 2500))
		{
			reset_helpers();
			game_state = GAME_STATE_TUTORIAL;
		}
			
		
	}
	if(pre_game_state == GAME_STATE_CREDITS)
	{

	}


}

void pause_filter(){
	
	if(pause_transparency < 0.55f && pause_helper){
	pause_transparency += 0.05f;
	}
	if(pause_transparency >= 0.0f && !pause_helper){
	pause_transparency -= 0.05f;
	}
	
}

void tutorial_filter(){
	
	if(tutorial_transparency < 0.4f && tutorial_helper){
	tutorial_transparency += 0.05f;
	}
	if(tutorial_transparency >= 0.0f && !tutorial_helper){
	tutorial_transparency -= 0.05f;
	}
	
}

void numberRender(){
	// Clear the dynamic text buffer
	C2D_TextBufClear(g_dynamicBuf);

	// Generate and draw dynamic text
	char buf[160];
	C2D_Text dynText;
	snprintf(buf, sizeof(buf), " %.0f%% ", player.skill_power);
	C2D_TextParse(&dynText, g_dynamicBuf, buf);
	C2D_TextOptimize(&dynText);
	C2D_DrawText(&dynText, C2D_AlignCenter | C2D_WithColor, 20.0f, 220.0f, 0.5f, 0.52f, 0.52f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));
}

void enemy_wave(){

	//a
	
}

void text_rectangle(){

	if(rect_text_height > -80 && createRect){
		rect_text_height -= 4;
		rect_y += 2;
	}
	if(rect_text_height < 0 && !createRect){
		rect_text_height += 4;
		rect_y -= 2;
	}
	if(rect_text_height == 0 && !createRect){
		text = false;
	}
}

void logic(u32 kHeld){

	if(!pause_helper)
	{
			
		circlePosition pos;
				
		hidCircleRead(&pos);
		 
		player_logic(pos.dx, pos.dy, kHeld);
		orb_logic();
	}
	
	if(!skill && !pause_helper)
	{
				
		bullet_logic();
		enemy_ship_logic();
		orb_points_logic(orb_points, &player);
		heal_logic(heals, &player);
				
	}

	if(skill)
	time_in_skill();

	minion_delete_list();

}

void hit_effect(){
	
	float bar_pos_y = player.y;
	float bar_pos_x = player.x;

	if(player.y > 190)
	bar_pos_y = 190.0f;
	else
	bar_pos_y = player.y;

	if(player.x < 20)
	bar_pos_x = 20.0f;
	if(player.x > 300)
	bar_pos_x = 300.0f;
	if(player.x >= 20 && player.x <= 300)
	bar_pos_x = player.x;

	C2D_DrawRectSolid(bar_pos_x - 16, bar_pos_y + 20,  0,  
		32 * calculateRatio(playerCurrentInvencibility, playerInvencibility), 
			6, C2D_Color32f(0.0f, 0.95f, 0.6f, 1.0f));

}

void prueba_time(){

	char buf[256];
	C2D_Text dynText;

	//if(!skill)
	//shoting();
	if(tutorialStep == 1)
	{
		tutorial = false;
		tutorial_helper = false;
		player.skills = 4;
	}
	else
	{
		tutorial = true;
		tutorial_helper = true;
	}

	if(minions[0].y < SCREEN_HEIGHT_TOP/2)
		set_minions_speed(0, 1.5f, 0);
	else
	{
		set_minions_speed(0, 0, 0);

		if(tutorialStep == 1)
		tutorialStep++;
	}
	

	if(tutorialStep == 2)
	snprintf(buf, sizeof(buf), "Hola y bienvenido/a a sala de entrena-\nmiento, donde se repasarán los aspectos\nbásicos necesarios para poder pelear\ncontra los invasores que se aproximan.");

	if(tutorialStep == 3)
	snprintf(buf, sizeof(buf), "Durante esta sesión, se guiará al\nusuario en el uso del equipo de\nmaniobras aéreo, la interpretación de\nla información mostrada en pantalla y\nel uso de las habilidades personales.");

	if(tutorialStep == 4)
	snprintf(buf, sizeof(buf), "Comenzemos. En primer lugar...");

	if(tutorialStep == 5)
	snprintf(buf, sizeof(buf), "La barra de 'Puntos de Salud' (PS)\ndetermina tu resistencia actual.");

	if(tutorialStep == 6)
	snprintf(buf, sizeof(buf), "Perderás 1 PS cada vez que te impacte\nun proyectil o choques con un enemigo.");

	if(tutorialStep == 7)
	snprintf(buf, sizeof(buf), "Si se agotan los PS será Game Over.\n¡Asi que no dejes que te den!.");

	if(tutorialStep == 8) // PSS
	{
		if(!helper)
		{
			lastPrintTime_1 = osGetTime();
			lastPrintTime_2 = osGetTime();
			lastPrintTime_3 = osGetTime();
			lastPrintTime_4 = osGetTime();
			lastPrintTime_5 = osGetTime();
			lastPrintTime_6 = osGetTime();
			bullet_counter_1 = 3;
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;	

		if (time_passed(lastPrintTime_1, 350) && !time_passed(lastPrintTime_2 + skill_real_time, 10000)) 
		{
			lastPrintTime_1 = osGetTime() - skill_time_elapsed;
			burst(minions[0].x, minions[0].y, level_angle_1, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 4, &orb_green, false, enemy_bullets);
			level_angle_1 += 10;
		}

		if (time_passed(lastPrintTime_3, 350) && !time_passed(lastPrintTime_4 + skill_real_time, 10000)) 
		{
			lastPrintTime_3 = osGetTime() - skill_time_elapsed;
			burst(minions[0].x, minions[0].y, level_angle_2, 1.0f, 0.0f, 2.0f, 0.0f, 0.0f, 4, &orb_blue, false, enemy_bullets);
			level_angle_2 -= 10;
		}

		if (time_passed(lastPrintTime_5 + skill_real_time, 10000) && bullet_counter_1 < 30)
		{

			//lastPrintTime_5 = osGetTime() - skill_time_elapsed;
			burst(minions[0].x, minions[0].y, level_angle_3, 3.0f, 0.0f, 2.0f, 0.0f, 0.0f, 30, &orb_red, false, enemy_bullets);
			level_angle_3 -= randomBetween(0,15);

			bullet_counter_1++;

		}

		if (time_passed(lastPrintTime_6 + skill_real_time, 17000))
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}	
	}

	if(tutorialStep == 9)
	snprintf(buf, sizeof(buf), "Si te dañan, te volverás invulnerable\npor un pequeño instante...");

	if(tutorialStep == 10)
	snprintf(buf, sizeof(buf), "Y no podrás disparar ni usar tu\nhabilidad (luego hablaremos de esta...)");

	if(tutorialStep == 11)
	snprintf(buf, sizeof(buf), "Si te estás quedando sin PS, cuando\npodamos te proporcionaremos botiquines.");

	if(tutorialStep == 12)
	snprintf(buf, sizeof(buf), "Te restablecerán 1 PS cada uno, pero\nten cuidado, son muy escasos.");

	if(tutorialStep == 13) // BOTIQUINES
	{

		if(!helper)
		{
			spawn_heal_top(150, 50, true, &heal_item);
			spawn_heal_top(250, 50, true, &heal_item);
			start = std::chrono::steady_clock::now();
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;	

		if (hasElapsed(8000, start))
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}
	}

	if(tutorialStep == 14)
	snprintf(buf, sizeof(buf), "Por cierto, si te resulta complicado\nesquivar algunos proyectiles...");

	if(tutorialStep == 15)
	snprintf(buf, sizeof(buf), "puedes reducir la velocidad y ganar\nprecisión manteniendo pulsado 'R'.");

	if(tutorialStep == 16)
	snprintf(buf, sizeof(buf), "Además, podrás ver un punto verde\nen mitad del personaje...");

	if(tutorialStep == 17)
	snprintf(buf, sizeof(buf), "se trata del punto real de colisión.");

	if(tutorialStep == 18)
	snprintf(buf, sizeof(buf), "Si éste choca con el punto de colisión\nde un proyectil enemigo te dañarán.");

	if(tutorialStep == 19)
	snprintf(buf, sizeof(buf), "Un consejo: el punto de colisión de los\nproyectiles enemigos...");

	if(tutorialStep == 20)
	snprintf(buf, sizeof(buf), "normalmente son menores de lo\n que parecen, así que...");

	if(tutorialStep == 21)
	snprintf(buf, sizeof(buf), "si te ves en apuros, trata de evitarlos\naunque parezca que te van a dar.");

	if(tutorialStep == 22)
	snprintf(buf, sizeof(buf), "Intenta esquivar estos proyectiles\n(no letales) y experimenta...");
	
	if(tutorialStep == 23)
	snprintf(buf, sizeof(buf), "con sus colisiones, pero cuidado,\nporque cada proyectil las tiene distintas.");

	if(tutorialStep == 24) // PRUEBA DE BALAS
	{

		if(!helper)
		{
			start = std::chrono::steady_clock::now();
			player.invencible = true;
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;	

		if (time_passed(lastPrintTime_1, 2500) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_1 = osGetTime();
			shoot_enemy_bullet(90, -15, -90, 1.0f, 0.0f, 23.0f, 0.0f, 0.0f, &red_big_ball, false, enemy_bullets);
		}

		if (time_passed(lastPrintTime_2, 450) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_2 = osGetTime();
			shoot_enemy_bullet(160, -15, -90, 2.0f, 0.0f, 3.0f, 0.0f, 0.0f, &orb_blue, false, enemy_bullets);
		}

		if (time_passed(lastPrintTime_3, 350) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_3 = osGetTime();
			shoot_enemy_bullet(200, -15, -90, 2.3f, 0.0f, 1.0f, 0.0f, 0.0f, &shell_red, false, enemy_bullets);
		}

		if (time_passed(lastPrintTime_4, 350) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_4 = osGetTime();
			shoot_enemy_bullet(230, -15, -90, 2.0f, 0.0f, 1.0f, 0.0f, 0.0f, &shell_red, false, enemy_bullets);
		}
		if (time_passed(lastPrintTime_5, 500) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_5 = osGetTime();
			shoot_enemy_bullet(270, -15, -90, 1.8f, 0.0f, 2.0f, 0.0f, 0.0f, &bullet_yellow, false, enemy_bullets);
		}
		if (time_passed(lastPrintTime_6, 1500) && !hasElapsed(30000, start)) 
		{
			lastPrintTime_6 = osGetTime();
			shoot_enemy_bullet(320, -15, -90, 1.4f, 0.0f, 11.0f, 0.0f, 0.0f, &yellow_mid_ball, false, enemy_bullets);
		}

		if (hasElapsed(40000, start))
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}	
	}

	if(tutorialStep == 25)
	snprintf(buf, sizeof(buf), "Ahora hablaremos sobre la barra de\n'Puntos de Energía' (PE).");

	if(tutorialStep == 26)
	snprintf(buf, sizeof(buf), "Se irá rellenando a medida que recojas\n'orbes energéticos'.");

	if(tutorialStep == 27)
	snprintf(buf, sizeof(buf), "Estos son orbes energéticos y los\nhay de dos tipos distintos.");

	if(tutorialStep == 28)
	snprintf(buf, sizeof(buf), "Los azules se dirigirán atraídos a ti\npero proporcionan poca energía.");

	if(tutorialStep == 29)
	snprintf(buf, sizeof(buf), "Y los amarillos los tendrás que recoger\ntú, pero proporcionan más energía.");

	if(tutorialStep == 30)
	snprintf(buf, sizeof(buf), "Si recoges muchos orbes, conseguirás PE,\ny si llenas la barra del todo...");

	if(tutorialStep == 31)
	snprintf(buf, sizeof(buf), "conseguirás un 'Gran Orbe' más con el\ncualpodrás disparar. (MÁX. 4)");

	if(tutorialStep == 32) // PUNTOS ORBES
	{

		if(!helper)
		{
			for(int i = 0; i < 70; i++){
				spawn_orb_point(randomBetween(50, 350), randomBetween(30, 80), true, ORB_POINT_TYPE_Y, &orb_charge);
				spawn_orb_point(randomBetween(50, 350), randomBetween(30, 80), true, ORB_POINT_TYPE_B, &orb_charge_b);
			}
			start = std::chrono::steady_clock::now();
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;	




		if (hasElapsed(8000, start))
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}

	}
	

	if(tutorialStep == 33)
	snprintf(buf, sizeof(buf), "Estos 'Gran Orbes' te permitirán disparar,\ny cuantos más tengas...");

	if(tutorialStep == 34)
	snprintf(buf, sizeof(buf), "más proyectiles podrás lanzar...\nY ahora sí...");

	if(tutorialStep == 35)
	snprintf(buf, sizeof(buf), "mantén presionado 'A' para disparar.");

	if(tutorialStep == 36) // MINIONS DISPARO
	{

		if(!helper)
		{
			spawn_minion_top(75 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true);  //1
			spawn_minion_top(325 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //2
			spawn_minion_top(100 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //3
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //4
			spawn_minion_top(300 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //5
			start = std::chrono::steady_clock::now();
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;


		if(!minions[0].state && minions[1].y < SCREEN_HEIGHT_TOP/2 && minions[2].y < SCREEN_HEIGHT_TOP/2)
		{
			set_minions_speed(0, 1.5f, 1);
			set_minions_speed(0, 1.5f, 2);
		}
		else
		{
			set_minions_speed(0, 0, 1);
			set_minions_speed(0, 0, 2);
		}	

		if(!minions[1].state && !minions[2].state &&  minions[3].y < SCREEN_HEIGHT_TOP/2 && minions[5].y < SCREEN_HEIGHT_TOP/2)
		{
			set_minions_speed(0, 1.5f, 3);
			set_minions_speed(0, 1.5f, 5);
		}
		else
		{
			set_minions_speed(0, 0, 3);
			set_minions_speed(0, 0, 5);
		}	

		if(!minions[1].state && !minions[2].state &&  minions[4].y < SCREEN_HEIGHT_TOP/2 - 50)
		{
			set_minions_speed(0, 1.5f, 4);
		}
		else
		{
			set_minions_speed(0, 0, 4);
		}	

		if (!minions[3].state && !minions[4].state && !minions[5].state)
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}

	}

	if(tutorialStep == 37)
	snprintf(buf, sizeof(buf), "Los proyectiles que disparas no tienen\nlímite, así que no te preocupes.");

	if(tutorialStep == 38)
	snprintf(buf, sizeof(buf), "Ah por cierto, si te concentras manteniendo\npulsado 'R'...");	

	if(tutorialStep == 39)
	snprintf(buf, sizeof(buf), "Reunirás los orbes delante tuyo, así\nganarás precisión de tiro frente a tí...");	

	if(tutorialStep == 40)
	snprintf(buf, sizeof(buf), "pero sacrificarás rango de tiro por los\nlado, así que tenlo en cuenta.");

	if(tutorialStep == 41) // PRUEBA DE MINIONS DE FOCUS
	{

		if(!helper)
		{
			spawn_minion_top(150 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //0
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //1
			spawn_minion_top(250 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //2
			spawn_minion_top(175 , SCREEN_HEIGHT_TOP/2 - 250, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //3
			spawn_minion_top(225 , SCREEN_HEIGHT_TOP/2 - 250, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //4

			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //5
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 250, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //6
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 300, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //7
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 350, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //8
			spawn_minion_top(200 , SCREEN_HEIGHT_TOP/2 - 400, 0, 0, 16.0f, 0.5f, 0.5f, &sphere_tuto, true); //9

			start = std::chrono::steady_clock::now();
			helper = true;
		}

		tutorial = false;
		tutorial_helper = false;


		if(minions[0].y < SCREEN_HEIGHT_TOP/2 && minions[1].y < SCREEN_HEIGHT_TOP/2 && minions[2].y < SCREEN_HEIGHT_TOP/2)
		{
			set_minions_speed(0, 1.5f, 0);
			set_minions_speed(0, 1.5f, 1);
			set_minions_speed(0, 1.5f, 2);
			
		}
		else
		{
			set_minions_speed(0, 0, 0);
			set_minions_speed(0, 0, 1);
			set_minions_speed(0, 0, 2);
		}	

		if(minions[3].y < SCREEN_HEIGHT_TOP/2 - 50 && minions[4].y < SCREEN_HEIGHT_TOP/2 - 50)
		{
			set_minions_speed(0, 1.5f, 3);
			set_minions_speed(0, 1.5f, 4);
		}
		else
		{
			set_minions_speed(0, 0, 3);
			set_minions_speed(0, 0, 4);
		}	

		if(!minions[0].state && !minions[1].state && !minions[2].state && !minions[3].state && !minions[4].state)
		
			for(int i = 5; i < 10; i++)
			{
				if(minions[i].y < 240-50*(i-5) )
					set_minions_speed(0, 1.5f, i);
				else
					set_minions_speed(0, 0, i);
			}
		

		if (!minions[5].state && !minions[6].state && !minions[7].state && !minions[8].state && !minions[9].state)
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}
	}

	if(tutorialStep == 42)
	snprintf(buf, sizeof(buf), "Por último y no menos importante...");	

	if(tutorialStep == 43)
	snprintf(buf, sizeof(buf), "Presentamos la barra de\n'Puntos de Parada de Tiempo' (PPT)");	

	if(tutorialStep == 44)
	snprintf(buf, sizeof(buf), "Si presionas 'L', se producirá\n una 'Parada de Tiempo'...");

	if(tutorialStep == 45)
	snprintf(buf, sizeof(buf), "en la que solo tu podrás moverte,\n y que durará 1 segundo.");

	if(tutorialStep == 46)
	snprintf(buf, sizeof(buf), "Cada 'Parada de Tiempo' consumirá\n 1 PPT y si se acaban...");

	if(tutorialStep == 47)
	snprintf(buf, sizeof(buf), "no podrás usar más esta habilidad...");

	if(tutorialStep == 48)
	snprintf(buf, sizeof(buf), "Prueba a esquivar esta ráfaga de\nproyectiles usando la habilidad!");

	if(tutorialStep == 49) // PARADA DE TIEMPO
	{

		if(!helper)
		{
			spawn_minion_top(SCREEN_WIDTH_TOP/2 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 999, &sphere_tuto, true);  //0
			
			lastPrintTime_1 = osGetTime();
			lastPrintTime_2 = osGetTime();
			lastPrintTime_3 = osGetTime();
			lastPrintTime_4 = osGetTime();
			lastPrintTime_5 = osGetTime();
			lastPrintTime_6 = osGetTime();
			
			helper = true;
			player.invencible = true;
		}

		tutorial = false;
		tutorial_helper = false;


		if(minions[0].y < SCREEN_HEIGHT_TOP/2)
			set_minions_speed(0, 1.5f, 0);
		else
		{

			set_minions_speed(0, 0, 0);


			if (bullet_counter_1 < 20 && time_passed(lastPrintTime_1 + skill_real_time, 5000)) 
			{

				burst(minions[0].x, minions[0].y, level_angle_3, 3.0f, 0.0f, 1.5f, 0.0f, 0.0f, 30, &orb_blue, false, enemy_bullets);
				level_angle_3 -= randomBetween(0,15);

				bullet_counter_1++;

			}

			if (time_passed(lastPrintTime_2 + skill_real_time, 10000) && bullet_counter_2 < 20)
			{

				burst(minions[0].x, minions[0].y, level_angle_3, 3.0f, 0.0f, 1.5f, 0.0f, 0.0f, 30, &orb_green, false, enemy_bullets);
				level_angle_3 -= randomBetween(0,15);

				bullet_counter_2++;

			}

			if (time_passed(lastPrintTime_3 + skill_real_time, 15000) && bullet_counter_3 < 40)
			{

				//lastPrintTime_3 = osGetTime();
				burst(minions[0].x, minions[0].y, level_angle_3, 3.0f, 0.0f, 2.5f, 0.0f, 0.0f, 30, &orb_red, false, enemy_bullets);
				level_angle_3 -= randomBetween(0,15);

				bullet_counter_3++;

			}

		}
	

		if (time_passed(lastPrintTime_6 + skill_real_time, 20000))
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}

	}

	if(tutorialStep == 50)
	snprintf(buf, sizeof(buf), "Para cargar la barra de PPT,\nnecesitarás aprovechar la energía de...");

	if(tutorialStep == 51)
	snprintf(buf, sizeof(buf), "los proyectiles enemigos para\ncargar el reloj.");

	if(tutorialStep == 52)
	snprintf(buf, sizeof(buf), "Esto se puede hacer pasando muy\ncerca de los proyectiles enemigos.");

	if(tutorialStep == 53)
	snprintf(buf, sizeof(buf), "(No hace falta hacerlo manteniendo\npulsado 'R', pero se aconseja...)");

	if(tutorialStep == 54)
	snprintf(buf, sizeof(buf), "Una vez pases cerca de un proyectil,\nrobarás su energía (1 vez / proyectil)");

	if(tutorialStep == 55)
	snprintf(buf, sizeof(buf), "Y ésta se sumará al contador de\nenergía, situado debajo a la izquierda.");

	if(tutorialStep == 56)
	snprintf(buf, sizeof(buf), "Una vez que el contador llegue al 100,\nse reseteará y sumarás 1 PPT.");

	if(tutorialStep == 57)
	{
		if(!helper)
		{
			lastPrintTime_1 = osGetTime();
			lastPrintTime_2 = osGetTime();
			lastPrintTime_3 = osGetTime();
			helper = true;	
		}

		C2D_Sprite* sp[3] = {&orb_blue, &orb_green, &orb_red};
		tutorial = false;
		tutorial_helper = false;



		if (time_passed(lastPrintTime_1 + skill_real_time, 250) && !time_passed(lastPrintTime_3 + skill_real_time, 10000))
		{
			
			lastPrintTime_1 = osGetTime() - skill_time_elapsed;
			for(int i = 0; i < 20; i++)
			shoot_enemy_bullet(40 + i*8, -10, -90, 1.5f, 0.0f, 2.0f, 6.0f, 5.0f, sp[i%3], false, enemy_bullets);
			for(int i = 21; i < 40; i++)
			shoot_enemy_bullet(40 + i*8, -10, -90, 1.5f, 0.0f, 2.0f, 6.0f, 5.0f, sp[i%3], false, enemy_bullets);
			//shoot_enemy_bullet_aim(minions[0].x, minions[0].y, 1.5f, 0.0f, 2.0f, 8.0f, 1.0f, sp[intRandomBetween(0,2)], false, enemy_bullets, &player);
		}

		//randomBetween(0,15)

		if (time_passed(lastPrintTime_2 + skill_real_time, 15000) || !minions[0].state)
		{
			reset_helpers();
			tutorial = true;
			tutorial_helper = true;
			tutorialStep++;
		}	

	}

	if(tutorialStep == 58)
	snprintf(buf, sizeof(buf), "Y esto ha sido todo. Ahora esfuérzate\nahí fuera, y buena suerte.");

	if((tutorialStep > 1 && tutorialStep <= 4) || tutorialStep == 58)
	{
		C2D_DrawSprite(&tuto_frame);
	}
	if(tutorialStep >= 5 && tutorialStep <= 7)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_hp);
	}
	if(tutorialStep >= 9 && tutorialStep <= 10)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_damaged);
	}
	if(tutorialStep >= 11 && tutorialStep <= 12)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_medkit);
	}
	if(tutorialStep >= 14 && tutorialStep <= 18)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_focus);
	}
	if(tutorialStep >= 19 && tutorialStep <= 23)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_graze);
	}
	if(tutorialStep >= 25 && tutorialStep <= 26)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_mp);
	}
	if(tutorialStep >= 27 && tutorialStep <= 29)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_orb_p);
		C2D_DrawSprite(&tuto_orb_p_2);
	}
	if(tutorialStep == 30)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_mp);
	}
	if(tutorialStep == 31)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_1_orb);
		C2D_DrawSprite(&tuto_2_orbs);
	}
	if(tutorialStep >= 33 && tutorialStep <= 35)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_1_orb);
		C2D_DrawSprite(&tuto_2_orbs);
	}
	if(tutorialStep == 37)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_1_orb);
		C2D_DrawSprite(&tuto_2_orbs);
	}
	if(tutorialStep >= 38 && tutorialStep <= 40)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_focus);
	}
	if(tutorialStep >= 42 && tutorialStep <= 48)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_ts);
	}
	if(tutorialStep == 50)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_ts);
	}
	if(tutorialStep >= 51 && tutorialStep <= 54)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_graze);
	}
	if(tutorialStep >= 55 && tutorialStep <= 56)
	{
		C2D_DrawSprite(&tuto_frame);
		C2D_DrawSprite(&tuto_number_0);
		C2D_DrawSprite(&tuto_number_1);
		C2D_DrawSprite(&tuto_graze);
	}

	if(tutorialStep >= 5 && tutorialStep <= 57 && tutorialStep != 8 && tutorialStep != 13 && tutorialStep != 24 && tutorialStep != 32 && tutorialStep != 36 && tutorialStep != 41 && tutorialStep != 49 && tutorialStep != 57)
	{
		C2D_TextParse(&dynText, g_dynamicBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignCenter | C2D_WithColor, 200.0f, 138.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));
	}

	if(tutorialStep == 58)
	{
		C2D_TextParse(&dynText, g_dynamicBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignCenter | C2D_WithColor, 200.0f, 100.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));
	}

	if(tutorialStep == 2 || tutorialStep == 3)
	{
		C2D_TextParse(&dynText, g_dynamicBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignCenter | C2D_WithColor, 200.0f, 70.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));
	}
	if(tutorialStep == 4)
	{
		C2D_TextParse(&dynText, g_dynamicBuf, buf);
		C2D_TextOptimize(&dynText);
		C2D_DrawText(&dynText, C2D_AlignCenter | C2D_WithColor, 200.0f, 100.0f, 0.5f, 0.6f, 0.6f, C2D_Color32f(1.0f,1.0f,1.0f,1.0f));
	}

	if(tutorialStep == 59)
	{
		
		if(!helper)
		{
			start = std::chrono::steady_clock::now();
			helper = true;
			
		}

		tutorial = false;
		tutorial_helper = false;
		to_black();



		if (hasElapsed(6000, start))
		{
			reset_helpers();
			game_state = GAME_STATE_INTRO;
		}
	}

	if(tutorialStep == 60)
	{
		
		if(!helper)
		{
			lastPrintTime_1 = osGetTime();
			lastPrintTime_2 = osGetTime();
			helper = true;
			
		}

		tutorial = false;
		tutorial_helper = false;
		

		if (time_passed(lastPrintTime_2 + skill_real_time, 1000) && time_passed(lastPrintTime_1 + skill_real_time, 4500)) 
		{
			lastPrintTime_2 = osGetTime() - skill_time_elapsed;
			shoot_enemy_bullet(90, -15, -90, 1.0f, 0.0f, 23.0f, 0.0f, 0.0f, &red_big_ball, false, enemy_bullets);
		}
	}		
	
	
}





//------------------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//------------------------------------------------------------------------------------------
	
	// Init libs
	srvInit();
    aptInit();

	/* Initialize ROM filesystem */
	romfsInit();
	ndspInit();

	//int currsound = 0;
	cwavEnvMode_t mode = CWAV_ENV_DSP;

	//gfxInitDefault();
	gfxInit(GSP_BGR8_OES , GSP_BGR8_OES , false);
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	//consoleInit(GFX_TOP, NULL);
	
	game_state = GAME_STATE_MANIN_MENU;

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);
	
 
	// Load graphics and initialize player sprites
	general_spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!general_spritesheet) svcBreak(USERBREAK_PANIC);

	screen_spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/screen.t3x");
	if (!screen_spritesheet) svcBreak(USERBREAK_PANIC);

	bg_spritesheet = C2D_SpriteSheetLoad("romfs:/gfx/bgs.t3x");
	if (!bg_spritesheet) svcBreak(USERBREAK_PANIC);



	g_dynamicBuf = C2D_TextBufNew(4096);

	
	
	//Inicializar
	init_player();
	Init_BG();

	cwavUseEnvironment(mode);
	populateCwavList();

	for (u32 i = 0; i < cwavList.size(); i++) 
	cwavStatus.push_back(0);

	//enemy_ships[0] = spawn_enemy_ship(200.0f, 120.0f, 0.0f, 0.0f, 30.0f, 4, 100.0f, &boss1_object, &barrier_object);
	/*
	for(int m = 0; m < 2; m++){
		
		spawn_minion_top(106.6f*(m+1) + 40, -20, 0, 0.3f, 15.0f, 1.0f, 1, &enemy_sphere_blue, true);

	}

	for(int n = 2; n < 4; n++){

		spawn_minion_top(65*(n-1) + 185*(n-2), -20, 0, 0.4f, 15.0f, 1.0f, 1, &enemy_sphere_red, true);

	}
	*/

	tutorial_helper = true;
	tutorial = true;
	player.orbs = 0;
	tutorialStep = 57;
	in_intro = true;
	
	spawn_minion_top(SCREEN_WIDTH_TOP/2 , SCREEN_HEIGHT_TOP/2 - 200, 0, 0, 16.0f, 0.5f, 1.0f, &sphere_tuto, true);

	// Main loop
	//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	while (aptMainLoop())
	{
		if(game_state == GAME_STATE_MANIN_MENU)
		{

			hidScanInput();

			// Respond to user input
			u32 kDown = hidKeysDown();

			game_intro();

			
			if(!in_intro)
			{
				if (kDown & KEY_B) 
				{
					//pauseAudio(1);
					//fade = !fade;
					//game_state = GAME_STATE_GAMEPLAY;
					//currsound++;
					//if (currsound >= (int)cwavList.size())
					//	currsound = 0;
					pre_game_state = (game_state_t)game_mode;
					
				}
				if (kDown & KEY_A) 
				{
					fade_black = true;
					//playAudio(AUDIO_INTRO);
				}
				if (kDown & KEY_RIGHT) 
				{
					if(game_mode < 3)
					game_mode++;
					else
					game_mode = 1;
				}
				if (kDown & KEY_LEFT) 
				{
					if(game_mode > 1)
					game_mode--;
					else
					game_mode = 3;
				}

				if(fade && fade_number < 1.0f)
				fade_number += 0.05f;
				if(!fade && fade_number > 0)
				fade_number -= 0.05f;

				pause_filter();

			}
			

			// -------------------------------------------------------------------------------------------------------
			// --------------------- G A M E P L A Y    P A N T A L L A     I N F E R I O R --------------------------
			// -------------------------------------------------------------------------------------------------------
			
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(bottom);

			C2D_DrawSprite(&main_menu_bot);

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, pause_transparency));

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(1.0f, 1.0f, 1.0f, white_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, black_transparency));

			

			C2D_Flush();

			// -------------------------------------------------------------------------------------------------------
			// --------------------- G A M E P L A Y    P A N T A L L A    S U P E R I O R --------------------------
			// -------------------------------------------------------------------------------------------------------
			
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(top);

			C2D_DrawSprite(&main_menu_top);

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, pause_transparency));

			if(show_tittle)
			C2D_DrawSprite(&tittle_logo);

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(1.0f, 1.0f, 1.0f, white_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, black_transparency));

			C3D_FrameEnd(0);

		}

		if(game_state == GAME_STATE_GAMEPLAY)
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

			//if(shots && !skill)
			//shoting();
			
			if (kDown & KEY_B) 
			{

				u32 playerCurrentSkill = osGetTime();

				if (playerCurrentSkill - playerSkill >= 1500) 
				{
					playerSkill = playerCurrentSkill;

					if(player.skills > 0)
					{
						--player.skills;

						skill = true;
						
						ts_radius = 700.0f;

						if(skill)
						skill_ini = osGetTime();
					}
				}
				
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
			if (kDown & KEY_R) {
				//enemy_ships[0].xspeed = -0.7f;
				//level_1(&enemy_ships[0]);
				//player.player_spr = &player_iz_object;
				
				
				direction_helper = !direction_helper;
				
			}
			if (kHeld & KEY_Y) {
				focus = true;
			}
			else focus = false;
			if (kHeld & KEY_X && !skill) {
				
				// Verifica si han pasado al menos X milisegundos desde la última vez que se imprimió el sprite.
				u32 playerShotTimeCurrent = osGetTime();
				if (playerShotTimeCurrent - playerShotTime >= PLAYER_BULLET_MS) 
				{
					playerShotTime = playerShotTimeCurrent;
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
				enemy_ship_logic();
				
			}

			if(skill)
			time_in_skill();

			orb_logic();
			//orb_point_logic(orb_points, player.x, player.y, player.radius);
			
			
			// Print debug messages on the bottom screen
			// printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
			// printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
			// printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

			// printf("\x1b[6;1HTime elapsed:  %lld ms\x1b[K", player_object.refresh_info.elapsed);
			// printf("\x1b[7;1HSprite refresh time:  %lld ms\x1b[K", player_object.refresh_info.refresh_time);
			// printf("\x1b[8;1HX:  %f", orb_object.position.x);
			// printf("\x1b[9;1HY:  %f", orb_object.position.y);

			// Render the scene
			
			
			update_object(player.player_spr);
			update_object(&player_iz_object);
			update_object(&boss1_object);


			//update_object(&player_shot_impact_obj);

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
			draw_orbs(player_orbs);
			if(!dead){
			draw_sprite_animation(player.player_spr);

			}
			

			
			draw_bullets_bot(bullets, enemy_bullets);
			draw_orb_points_top(orb_points);
			draw_minions_bot();
			
			

			if(focus)
				draw_hitbox();
				
			
			
			if(!skill){
				
				//C2D_DrawLine(enemy_ships[0].x-40, enemy_ships[0].y - SCREEN_HEIGHT_BOT, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 
					//player.x, player.y, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 1.0f, 0.0f);
			}
			

			if(skill){

				
				
				
				if(ts_radius > 0)
				{
				
					C2D_DrawCircleSolid(player.x , player.y, 0, ts_radius, 
						C2D_Color32f(1.0f, 0.0f, 0.0f, 0.2f));
						
					ts_radius -= 15.0f;

				}
				//skill = !skill;
			}

			numberRender();

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
			//draw_sprite_animation(&boss1_object);
			//draw_sprite_animation(&barrier_object);
			
			//C2D_DrawLine (enemy_ships[0].x, enemy_ships[0].y, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 
				//player.x + 40.0f, player.y + 240.0f, C2D_Color32f(1.0f, 0.0f, 0.1f, 0.5f), 1.0f, 0.0f);
			}
			if(skill){
			//draw_sprite_only(&boss1_object);
			//draw_sprite_only(&barrier_object);
			}
			
			draw_bullets_top(bullets, enemy_bullets);
			draw_orb_points_top(orb_points);
			draw_minions_top();

			
			if(skill){

				
				
				
				if(ts_radius > 0){
				
					C2D_DrawCircleSolid(player.x + 40.0f, player.y + 240.0f, 0, ts_radius, 
						C2D_Color32f(1.0f, 0.0f, 0.0f, 0.2f));
						
					ts_radius -= 22.0f;
					
					}
				//skill = !skill;
			}

			C2D_DrawSprite(&UI_bars);
			C2D_DrawSprite(&UI_boss);
			C2D_DrawSprite(&item_frame);

			draw_stats(player.health, player.orb_points, player.skills, hp_points, mp_points, ts_points);

			//C2D_DrawRectangle(SCREEN_WIDTH_TOP - 17, 13, 0, -enemy_ships[0].current_health*(220/enemy_ships[0].health), 4, 
				//C2D_Color32f(0.0f, 0.7f, 0.1f, 1.0f), C2D_Color32f(0.0f, 1.0f, 0.1f, 1.0f), C2D_Color32f(0.0f, 0.4f, 0.1f, 1.0f), C2D_Color32f(0.0f, 0.7f, 0.1f, 1.0f));
			

			C3D_FrameEnd(0);

		}

		if(game_state == GAME_STATE_TUTORIAL)
		{
		
			hidScanInput();

			// Respond to user input
			u32 kDown = hidKeysDown();
			u32 kHeld = hidKeysHeld();
			
			if (kDown & KEY_START && !skill) {
				
				//pause_
				pause_helper = !pause_helper;

				if(pause_helper == true)
				pause_ini = osGetTime();

				if(pause_helper == false)
				skill_time_elapsed += osGetTime() - pause_ini;
				
			}

			if(!pause_helper){

				if (kDown & KEY_B && !text && !tutorial && tutorialStep >= 49) 
				{

					if (time_passed(skill_ini, 1500)) 
					{

						if(player.skills > 0)
						{
							playAudio(AUDIO_SKILL);
							//player.skills;

							skill = true;
							ts_radius = 700.0f;
							skill_ini = osGetTime();
							
						}
					}
					
				}
				if (kDown & KEY_A && !tutorial) {

					//level_1(&boss1_object);
					
				}
				if (kDown & KEY_L && !tutorial) {

					if(player.orbs == 4)
					player.orbs = 1;
					else
					player.orbs ++;

				}
				if (kDown & KEY_R && !tutorial) {	

					direction_helper = !direction_helper;		
				}
				if (kHeld & KEY_Y && !tutorial && tutorialStep >= 15) {
					focus = true;
				}
				else focus = false;

				if (kHeld & KEY_X && !skill && !text && !tutorial && tutorialStep >= 36) {
					
					// Verifica si han pasado al menos X milisegundos desde la última vez que se imprimió el sprite.
					u32 playerShotTimeCurrent = osGetTime();

					if (playerShotTimeCurrent - playerShotTime >= PLAYER_BULLET_MS) 
					{
						playerShotTime = playerShotTimeCurrent;
						playAudio(AUDIO_P_SHOT);
						shoot_bullet();
					}
					
					//deinitialize_object(&player_shot_impact_obj);
				} 

				logic(kHeld);



				update_object(player.player_spr);
				update_object(&player_iz_object);
				update_object(&boss1_object);
				update_object(&barrier_object);

			}

			if(tutorial)
			{
				if (kDown & KEY_A) {
					
					u32 currentTime = osGetTime();
					if (currentTime - lastPrintTime >= 50) 
					{

						lastPrintTime = currentTime;

						tutorialStep++;

					
					}
					
				}
			}

			tutorial_filter();

			pause_filter();
		
			// -------------------------------------------------------------------------------------------------------
			// --------------------- G A M E P L A Y    P A N T A L L A     I N F E R I O R --------------------------
			// -------------------------------------------------------------------------------------------------------
			
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(bottom);
				

			C2D_DrawSprite(&bg_tuto_bot);

			draw_orbs(player_orbs);

			if(blink && !pause_helper)
			draw_sprite_animation(player.player_spr);
			
			if(dead)
			hit_effect();
			

			
			draw_bullets_bot(bullets, enemy_bullets);
			draw_orb_points_bot(orb_points);
			draw_heal_bot(heals);
			draw_minions_bot();
			
			if(pause_helper)
			draw_sprite_only(player.player_spr);

			if(focus)
				draw_hitbox();

			if(skill){

				if(ts_radius > 0)
				{
				
					C2D_DrawCircleSolid(player.x , player.y, 0, ts_radius, 
						C2D_Color32f(0.0f, 0.1f, 0.3f, 0.2f));
						
				}

			}

			draw_boss_pos_bar();

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_BOT, SCREEN_HEIGHT_BOT, C2D_Color32f(0.0f, 0.0f, 0.0f, pause_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, tutorial_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(1.0f, 1.0f, 1.0f, white_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, black_transparency));

			if(!pause_helper)
			numberRender();
			
			C2D_Flush(); // Limpiar buffer de pantalla inferior
			
			// -------------------------------------------------------------------------------------------------------
			// --------------------- G A M E P L A Y    P A N T A L L A    S U P E R I O R --------------------------
			// -------------------------------------------------------------------------------------------------------
			
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			C2D_SceneBegin(top);

			C2D_DrawSprite(&bg_tuto_top);
			

			draw_bullets_top(bullets, enemy_bullets);
			draw_orb_points_top(orb_points);
			draw_heal_top(heals);
			draw_minions_top();

			
			if(skill){

				if(ts_radius > 0)
				{
					C2D_DrawCircleSolid(player.x + 40.0f, player.y + 240.0f, 0, ts_radius, 
						C2D_Color32f(0.0f, 0.1f, 0.3f, 0.2f));
				}

			}

			if(text)
			{

				text_rectangle();

				if(rect_text_height >= -80)
				C2D_DrawRectSolid(20, rect_y,  0, 360, rect_text_height, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
			}
			else{

				C2D_DrawSprite(&UI_bars);
				draw_stats(player.health, player.orb_points, player.skills, hp_points, mp_points, ts_points);

				if(rect_text_height >= -80)
				{
					rect_text_height = 0.0f;
					rect_y = 180;
				}

			}
				

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, pause_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, tutorial_transparency));

			prueba_time();	

			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(1.0f, 1.0f, 1.0f, white_transparency));
			C2D_DrawRectSolid(0, 0, 0, SCREEN_WIDTH_TOP, SCREEN_HEIGHT_TOP, C2D_Color32f(0.0f, 0.0f, 0.0f, black_transparency));
			

			C3D_FrameEnd(0);

		}
	}

	// Delete graphics
	deinitialize_object(&player_object);
	deinitialize_object(&boss1_object);
	deinitialize_object(&barrier_object);
	C2D_SpriteSheetFree(general_spritesheet);

	// Free the audio thread
    //threadJoin(skillId, UINT64_MAX);
    //threadFree(skillId);
	freeCwavList();




    // Cleanup audio things and de-init platform features

    ndspExit();
	
	// Deinit libs
	
	C2D_Fini();
	C3D_Fini();
	gfxExit();
    aptExit();
    srvExit();
	romfsExit();

	return 0;
}