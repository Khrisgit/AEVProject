#include <3ds.h>
#include <stdio.h>
#include "math.h"
#include <vector>
#include <string>
#include <tuple>

#include <cwav.h>
#include <ncsnd.h>

std::vector<int> cwavStatus;

void updateScreens()
{
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();
		//Wait for VBlank
		gspWaitForVBlank();
}

const char* fileList[] =
{
	"romfs:/audio/Intro.cwav",          // 0
	"romfs:/audio/skill.cwav",          // 1   
    "romfs:/audio/e_damage.cwav",       // 2 
    "romfs:/audio/p_shot.cwav",         // 3
    "romfs:/audio/healing.cwav",        // 4
};

typedef enum {
              AUDIO_INTRO,              // 0 
              AUDIO_SKILL,              // 1
              AUDIO_E_DAMAGE,           // 2
              AUDIO_P_SHOT,             // 3
              AUDIO_HEALING,            // 4
              AUDIO_E_Death             // 5
} audio_enum_t;

const u8 maxSPlayList[] =
{
	3,
	3,
	3,
	4,
	4,
	1,
	1,
	1
};

const char* bit_str[] = 
{
    "0000", "0001", "0010", "0011",
    "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011",
    "1100", "1101", "1110", "1111"
};

void print_u32_binary(u32 val)
{
	for (u32 i = 0; i < 4; i++)
	{
		u32 toprint = val >> ((3 - i) * 8);
		printf("%s%s", bit_str[(toprint >> 4) & 0xF], bit_str[toprint & 0x0F]);
	}
}

std::vector<std::tuple<std::string, CWAV*>> cwavList;

void populateCwavList()
{
	
	for (u32 i = 0; i < sizeof(fileList) / sizeof(char*); i++)
	{

		CWAV* cwav = (CWAV*)malloc(sizeof(CWAV));

		FILE* file = fopen(fileList[i], "rb");
		if (!file)
		{
			cwavFree(cwav);
			free(cwav);
			continue;
		}

		fseek(file, 0, SEEK_END);
		u32 fileSize = ftell(file);
		void* buffer = linearAlloc(fileSize);
		if (!buffer) // This should never happen (unless we load a file too big to fit)
			svcBreak(USERBREAK_PANIC);

		fseek(file, 0, SEEK_SET); 
		fread(buffer, 1, fileSize, file);
		fclose(file);

		// Since we manually allocated the buffer, we use cwavLoad directly...
		cwavLoad(cwav, buffer, maxSPlayList[i]);
		cwav->dataBuffer = buffer; // (We store the buffer pointer here for convinience, but it's not required.)
		// or, we could have let the library handle it...
		// cwavFileLoad(cwav, fileList[i], maxSPlayList[i]);

		if (cwav->loadStatus == CWAV_SUCCESS)
		{
			cwavList.push_back(std::make_tuple(std::string(fileList[i]), cwav));
		}
		else
		{
			// Manually de-allocating the buffer
			cwavFree(cwav);
			linearFree(cwav->dataBuffer);
			// or, if we used cwavFileLoad, let the library handle it.
			// cwavFileFree(cwav);

			free(cwav);
		}
	}
}

void freeCwavList()
{
	for (auto it = cwavList.begin(); it != cwavList.end(); it++)
	{
		// Manually de-allocating the buffer
		cwavFree(std::get<1>(*it));
		void* buffer = std::get<1>(*it)->dataBuffer;
		if (buffer)
			linearFree(buffer);
		// or, if we used cwavFileLoad, let the library handle it.
		// cwavFileFree(std::get<1>(*it));
		free(std::get<1>(*it));
	}
}

void playAudio(int track)
{
	CWAV* cwav = std::get<1>(cwavList[track]);
	if (cwav->numChannels == 2)
	{
		cwavPlay(cwav, 0, 1);
	}
	else
	{
		cwavPlay(cwav, 0, -1);
	}
	if (cwav->isLooped)
	cwavStatus[track] = 1;    
}

void pauseAudio(int track)
{
	CWAV* cwav = std::get<1>(cwavList[track]);
	if (cwav->numChannels == 2)
		{
			cwavStop(cwav, 0, 1);
		}
		else
		{
			cwavStop(cwav, 0, -1);
		}
		cwavStatus[track] = 0;   
}
