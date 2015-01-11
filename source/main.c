#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "main.h"
#include "app.h"
#include "draw.h"
#include "input.h"
#include "rendering.h"
#include "mem.h"
#include "POP_bin.h"

//FPS Counter
u64 tickOld;
int fps;
bool GW_MODE;

int main()
{
	// Initialize services
	srvInit();
	aptInit();
	hidInit(NULL);
	gfxInitDefault();
	fsInit();
	sdmcArchive = (FS_archive){ 0x9, (FS_path){ PATH_EMPTY, 1, (u8*)"" } };
	FSUSER_OpenArchive(NULL, &sdmcArchive);

	// Check if user is in GW mode (for pop sound)
	if (CSND_initialize(NULL) == 0) GW_MODE = false;
	else GW_MODE = true;

    u8 *POP = linearAlloc(POP_bin_size);
    memcpy(POP, POP_bin, POP_bin_size);

	// Main loop
	while (aptMainLoop())
	{

		//FPS Counter
		if (svcGetSystemTick() >= tickOld + 268123480)
		{
			tickOld = svcGetSystemTick();
			printFPS = fps;
			fps = 0;
		}

		//As nop90 suggested
		getFB();

		//Gets input (keys and touch)
		getInput();

		//Prints the GUI
		printGUI();  

		//Do stuff
		app();

		//Exit code
		if (mode==5) break;

		//saveDrawing- taken from blargSnes
		if (save==1)
		{
			u32 timestamp = (u32)(svcGetSystemTick() / 446872);
			char file[256];
			snprintf(file, 256, "/3dspaint_drawing_%08d.bmp", timestamp);
			if (saveDrawing(file))
				paddle1R = 0;
			else
				paddle1G = 0;

			count = 1;
			save = 0;

			//Visual confirmation that the drawings has been saved
			drawFillRect(257, 138, 400, 168, 255, 255, 51, screenTopLeft);
			drawFillRect(257, 138, 400, 168, 255, 255, 51, screenTopRight);

			rendered = 0; //Redraw the GUI
			sound = 1;
		}

		if (count)
			screenWait--;
		if (screenWait <= 0)
		{
			paddle1R = 255;
			paddle1G = 255;
			paddle1B = 255;
			count = 0;
			screenWait = 60 * 2;
		}

		//Plays pop sound if "sound"==1 and if CSND is initialized
		if (sound == 1 && GW_MODE==false)
		{
			CSND_playsound(0x8, CSND_LOOP_ENABLE, CSND_ENCODING_PCM16, 44100, (u32*)POP, NULL, POP_bin_size, 2, 0);
			sound = 0;
		}
			
		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();

		fps++;
	}

	if (GW_MODE == false) CSND_shutdown();

	linearFree(POP);

	// Exit services
	gfxExit();
	hidExit();
	aptExit();
	srvExit();
	return 0;
}


bool saveDrawing(char* path)
{
	int x, y;

	Handle file;
	FS_path filePath;
	filePath.type = PATH_CHAR;
	filePath.size = strlen(path) + 1;
	filePath.data = (u8*)path;

	Result res = FSUSER_OpenFile(NULL, &file, sdmcArchive, filePath, FS_OPEN_CREATE | FS_OPEN_WRITE, FS_ATTRIBUTE_NONE);
	if (res)
		return false;

	u32 byteswritten;

	u32 bitmapsize = 320 * 205 * 3;
	u8* tempbuf = (u8*)MemAlloc(0x36 + bitmapsize);
	memset(tempbuf, 0, 0x36 + bitmapsize);

	FSFILE_SetSize(file, (u16)(0x36 + bitmapsize));

	*(u16*)&tempbuf[0x0] = 0x4D42;
	*(u32*)&tempbuf[0x2] = 0x36 + bitmapsize;
	*(u32*)&tempbuf[0xA] = 0x36;
	*(u32*)&tempbuf[0xE] = 0x28;
	*(u32*)&tempbuf[0x12] = 320; // width
	*(u32*)&tempbuf[0x16] = 205; // height
	*(u32*)&tempbuf[0x1A] = 0x00180001;
	*(u32*)&tempbuf[0x22] = bitmapsize;

	u8* framebuf = (u8*)gfxGetFramebuffer(GFX_BOTTOM, GFX_LEFT, NULL, NULL);
	for (y = 35; y < 240; y++)
	{
		for (x = 0; x < 320; x++)
		{
			int si = ((239 - y) + (x * 240)) * 3;
			int di = 0x36 + (x + ((239 - y) * 320)) * 3;

			tempbuf[di++] = framebuf[si++];
			tempbuf[di++] = framebuf[si++];
			tempbuf[di++] = framebuf[si++];
		}
	}

	FSFILE_Write(file, &byteswritten, 0, (u32*)tempbuf, 0x36 + bitmapsize, 0x10001);

	FSFILE_Close(file);
	MemFree(tempbuf);
	return true;
}