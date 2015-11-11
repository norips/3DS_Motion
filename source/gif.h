/* 
 * File:   gif.h
 * Author: Silvermoon
 *
 * Created on 10 novembre 2015, 18:53
 */

#ifndef GIF_H
#define	GIF_H
#include <gif_lib.h>
#include "app.h"

void closeGIF(GifFileType*);
GifFileType* initGIF(int cTable[9][3],ColorMapObject *GIFcmap,const char* path);
void save_GIF(canvas* canvasarray,int canvassize,GifFileType*,ColorMapObject *GIFcmap );


#endif	/* GIF_H */

