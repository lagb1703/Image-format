#include "commonStructs.c"
#include <stdio.h>

static void mixPixel(RGB *pixel, unsigned char r, unsigned char g, unsigned char b){
    unsigned char promedio = (pixel->red*(r/255) + pixel->green*(g/255) + pixel->blue*(b/255))/3;
    pixel->red = pixel->green = pixel->blue = promedio;
    return;
}

void tranformGrayScale(RGB **body, unsigned int width, unsigned int height){
    unsigned int w = width;
    while(height > 0){
        height -= 1;
        while(w > 0){
            w -= 1;
            mixPixel(&body[height][w], 255, 255, 255);
        }
        w = width;
    }
    return;
}