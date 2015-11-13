#ifndef APP_H
#define APP_H
#define MALLOC_SIZE 128
typedef struct point {
    int x;
    int y;
    int color;
} point ;
typedef struct canvas {
    point *point;
    int size;
} canvas ;

void app();
void printGUI();
void variableReset();
bool writeToArray(char posxy[320][240],canvas* arrayCanvas);
bool writeToScreen(char posxy[320][240],const canvas* arrayCanvas);
void colorSelection(int posX,int posY,int*);
extern int mode;
extern int save;
extern int saveGIF;
extern int printFPS;
extern int framepersecond;
extern int rendered;
extern int sound;
extern int frame;
extern bool showLastFrame;
extern int fps;
extern canvas *canvasarray;
extern int canvassize;
extern int cTable[9][3];
extern int sizeCANVAS;
extern bool errorPopup;

#endif
