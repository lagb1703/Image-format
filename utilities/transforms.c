#include "commonStructs.c"
#include <math.h>
#include <stdio.h>

static void mixPixel(RGB *pixel, unsigned char r, unsigned char g, unsigned char b){
    unsigned char promedio = (pixel->red*(r/255) + pixel->green*(g/255) + pixel->blue*(b/255))/3;
    pixel->red = pixel->green = pixel->blue = promedio;
    return;
}

/*
* contrato: RGB **, ussigned int, unsigned int -> void
* convierte los pixeles de la imagen en su respectivo valor en blanco y negro
*/
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

/*
* contrato: RGB **, unsigned int, unsigned int, int -> void
* convierte los pixeles de la imagen y se suma por cada pixel el valor de la cantidad,
* se le suma en sus cuatro componentes Rojo, verde y azul
*/
void tranformSaturate(RGB **body, unsigned int width, unsigned int height, int cantidad){
    int *pixel = 0;
    int w = width;
    if(cantidad > 255 || cantidad < -255){
        cantidad = 255;
    }
    if(cantidad > 0)
      cantidad = cantidad*pow(2, 16) + cantidad*pow(2, 8) + cantidad;
    else{
        cantidad *= -1;
        cantidad = cantidad*pow(2, 16) + cantidad*pow(2, 8) + cantidad;
        cantidad *= -1;
    }
    while(height > 0){
        height -= 1;
        while(w > 0){
            w -= 1;
            pixel = (int *) &body[height][w];
            *pixel += cantidad;
        }
        w = width;
    }
    return;
}

/*
* contrato: RGB **, ussigned int, unsigned int -> RGB **
* devulve una copia de la imagen con las lineas de accion de ella.
*/
RGB **transformActionsLines(RGB **body, unsigned int width, unsigned int height){
    unsigned int rgbNumber = (((24 * width + 31)/32)*4)/sizeof(RGBA) + 1;
    RGB **alt = (RGB **) malloc(height * sizeof(RGB **));
    width -= 1;
    height -= 1;
    int w = width, h = height, *pixelPrincipal = 0;
    while(h >= 0){
        alt[h] = (RGB *) malloc(rgbNumber*sizeof(RGB *));
        while(w >= 0){
            alt[h][w] = body[h][w];
            alt[h][w].red *= 8;
            alt[h][w].green *= 8;
            alt[h][w].blue *= 8;
            pixelPrincipal = (int *) &alt[h][w];
            if(h != height){
                alt[h][w].red -= body[h + 1][w].red;
                alt[h][w].green -= body[h + 1][w].green;
                alt[h][w].blue -= body[h + 1][w].blue;
                if(w != width){
                    alt[h][w].red -= body[h + 1][w + 1].red;
                    alt[h][w].green -= body[h + 1][w + 1].green;
                    alt[h][w].blue -= body[h + 1][w + 1].blue;
                }
                if(w != 0){
                    alt[h][w].red -= body[h + 1][w - 1].red;
                    alt[h][w].green -= body[h + 1][w - 1].green;
                    alt[h][w].blue -= body[h + 1][w - 1].blue;
                }
            }
            if(h != 0){
                alt[h][w].red -= body[h - 1][w].red;
                alt[h][w].green -= body[h - 1][w].green;
                alt[h][w].blue -= body[h - 1][w].blue;
                if(w != width){
                    alt[h][w].red -= body[h - 1][w + 1].red;
                    alt[h][w].green -= body[h - 1][w + 1].green;
                    alt[h][w].blue -= body[h - 1][w + 1].blue;
                }
                if(w != 0){
                    alt[h][w].red -= body[h - 1][w - 1].red;
                    alt[h][w].green -= body[h - 1][w - 1].green;
                    alt[h][w].blue -= body[h - 1][w - 1].blue;
                }
            }
            if(w != width){
                alt[h][w].red -= body[h][w + 1].red;
                alt[h][w].green -= body[h][w + 1].green;
                alt[h][w].blue -= body[h][w + 1].blue;
            }
            if(w != 0){
                alt[h][w].red -= body[h][w - 1].red;
                alt[h][w].green -= body[h][w - 1].green;
                alt[h][w].blue -= body[h][w - 1].blue;
            }
            w -= 1;
        }
        w = width;
        h -= 1;
    }
    return alt;
}
