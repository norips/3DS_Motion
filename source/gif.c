#include <stdio.h>
#include <stdlib.h>
#include <gif_lib.h>
#include "gif.h"
#include "app.h"
#include "memory.h"


void closeGIF(GifFileType* GIFfile)
{
  printf("closeGIF\n");
  EGifCloseFile(GIFfile,NULL);
}

GifFileType* initGIF(int cTable[9][3],ColorMapObject *GIFcmap,const char* path)
{
  GifFileType *GIFfile;  
  GIFfile = EGifOpenFileName(path,false, 0);
  
  GIFcmap = GifMakeMapObject(256, NULL);
  for(int i=0;i<9;i++){
      GIFcmap->Colors[i].Red=cTable[i][0];
      GIFcmap->Colors[i].Green=cTable[i][1];
      GIFcmap->Colors[i].Blue=cTable[i][2];
  }
  EGifPutScreenDesc(GIFfile, BOTTOM_WIDTH, BOTTOM_HEIGHT, 256, 0, GIFcmap);
  return GIFfile;
  
}

void save_GIF(canvas* canvasarray,int canvassize,GifFileType* GIFfile,ColorMapObject *GIFcmap)
{
    char **frametempo = (char **)malloc(BOTTOM_WIDTH * sizeof(char *));
    for (int i=0; i<BOTTOM_WIDTH; i++)
         frametempo[i] = (char *)malloc(BOTTOM_HEIGHT * sizeof(char));
    for(int i=0;i<canvassize;i++){
       
        for (int y = 0; y < BOTTOM_HEIGHT; y++)
        {
            for (int x = 0; x < BOTTOM_WIDTH; x++)
            {
                frametempo[x][y] = 0;
            }
        }
        if(canvasarray==NULL) {
            errorPopup = true;
            mode = 1;
            return;
        }
        for (int j = 0; j < canvasarray[i].size; j++){
            frametempo[canvasarray[i].point[j].x][canvasarray[i].point[j].y]=canvasarray[i].point[j].color;
        }
        if(EGifPutImageDesc(GIFfile, 0, 0, BOTTOM_WIDTH, BOTTOM_HEIGHT, 0, GIFcmap)==GIF_ERROR){
            errorPopup = true;
            mode = 1;
            return;
        }
        for (int y = 0; y < 240; y++)
        {
            for (int x = 0; x < 320; x++)
            {
                EGifPutPixel(GIFfile,(const GifPixelType) frametempo[x][y]);
            }
        }
    }
}