#ifndef GUI_H
#define GUI_H
#include "app.h"
void guiTopPaint();
void guiTopAnimation();
void guiTopMenu();
void guiBottomPaint(int color, int cTable[8][3], char posxy[320][240],canvas* canvasarray); 
void guiBottomMenu();
void guiClock();
void guiFrame();
void guiPopup(char* title, char* line1, char* line2, char* line3, char* button1, char* button2, bool closeonly);
void guiDebug(int mode, int state, int color, int rendered, int sound, int printFPS, int posX, int posY,int,int);
void guiBottomPaintAnimation(int cTable[8][3],canvas* canvasarray,int frame,int fps);

#endif
