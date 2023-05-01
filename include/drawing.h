#ifndef DRAWING_H
#define DRAWING_H

#include "main.h"


void draw_stats(int player_health, int player_orb_e, int player_skills, C2D_Sprite* hp, C2D_Sprite* mp, C2D_Sprite* ts){

	for(int i = 0; i < player_health; i++){
		
		C2D_SpriteSetPos(&hp[i], 16 + i*8, 196);
		C2D_DrawSprite(&hp[i]);
	}

	for(int j = 0; j < player_orb_e; j++){
		
		C2D_SpriteSetPos(&mp[j], 16 + j*8, 207);
		C2D_DrawSprite(&mp[j]);
	}

	for(int k = 0; k < player_skills; k++){
		
		C2D_SpriteSetPos(&ts[k], 16 + k*8, 218);
		C2D_DrawSprite(&ts[k]);
	}
}



#endif
