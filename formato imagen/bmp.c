//mas informacion sobre el formato https://en.wikipedia.org/wiki/BMP_file_format#:~:text=The%20BMP%20file%20format%2C%20also,and%20OS%2F2%20operating%20systems
//#include <stdio.h>//se importa la libreria de input output estandar para poder usar printf y FILE
#include <stdlib.h>//se importa la libreria estandar para poder usar malloc y free
//#include "../utilities/commonStructs.c"//se importa las structuras comunes de imagenes para usar la estructuara RGB
#include "../utilities/transforms.c"
/*
* en esta estructura se guardara los primeros 14 bytes del archivos,
* donde se incluye el tipo del archivo, el tamaño total en bytes, 
* algunos metadatos (recuerdas que estan en forma de int), y ya donde comienza la imagen
*/
struct bmpHeader
{
    char fileType[2];// siempre debe ser bm
    unsigned int totalSize;// aca se guardara el tamaño de la imagen bmp en bytes
    int metaData;//aca se guardaran algunos metadatos inutiles, pero, puedes sobreescribirlos
    unsigned int offset;//aca se guardara la posicion de donde comienza los pixeles de la imagen
};

typedef struct bmpHeader bmpHeader;//se define el tipo de dato

/*
Nota mas especifica sobre los bmpInformation, estos siguen siendo encabezados los cuales guardan informacion mas
especifica del archivo, existen varias versiones, pero aca se maneja la primera version llamada Windows BITMAPINFOHEADER
y la ultima que es la DIBV5.
*/

/*
* aca se guardara informacion general de la imagen
*/
struct bmpInformation40bits{
    unsigned int headerSize;//aca se guardara el tamaño en bytes del encabezado, normlmente es 40
    unsigned int width;//aca se guarda el ancho en pixeles de la imagen
    unsigned int height;//aca se guarda el alto en pixeles de la imagen
    unsigned short colors;//aca se guarda el numero de colores de la imagen
    unsigned short bitsPixel;//aca se guardara cuantos bytes ocupa un pixel, normalmente en 24
    unsigned int compression;//aca se guardara el tipo de conversion que tiene el archivo
    unsigned int bodysize;//aca se guardara el tamaño en bytes de todos los pixeles de la imagen
    unsigned int horizontalResolution;//aca se guardara la relacion que hay entre el ancho en pixeles y el ancho en cm
    unsigned int verticalResolution;//aca se guardara la relacion que hay entre el alto en pixeles y el alto en cm
    unsigned int palette;//aca se guarda la papeleta de colores
    unsigned int importantColor;//aca se guarda los colores importantes de la imagen
};

/*
*aca se guarda informacion mas especifica sobre la imagen esta es la version DIBV5
*/
struct bmpInformation124bits {
    unsigned int        headerSize;//aca se guardara el tamaño d en bytes del header
    unsigned long        width;//aca se guardara el tamaño en pixeles del ancho de la imagen
    unsigned long        height;//aca se guardara el tamaño en pixeles del alto de la imagen
    unsigned short         colors;//aca se guarda el numero de colores de la imagen
    unsigned short         bitsPixel;//aca se guardara cuantos bytes ocupa un pixel, normalmente en 24
    unsigned int compression;//aca se guardara el tipo de conversion que tiene el archivo
    unsigned int bodysize;//aca se guardara el tamaño en bytes de todos los pixeles de la imagen
    unsigned int horizontalResolution;//aca se guardara la relacion que hay entre el ancho en pixeles y el ancho en cm
    unsigned int verticalResolution;//aca se guardara la relacion que hay entre el alto en pixeles y el alto en cm
    unsigned int palette;//aca se guarda la papeleta de colores
    unsigned int importantColor;//aca se guarda los colores importantes de la imagen
    unsigned int        redMask;//aca se gaurdara la mascara de rojos de la imagen
    unsigned int        greenMask;//aca se gaurdara la mascara de verdes de la imagen
    unsigned int        blueMask;//aca se gaurdara la mascara de azules de la imagen
    unsigned int        alphaMask;//aca se guardara la mascara de alfa de la imagen
    //el resto aun no se para que sirven
    unsigned int        bv5CSType;
    char endpoints[36];
    unsigned int        gammaRed;
    unsigned int        gammaGreen;
    unsigned int        gammaBlue;
    unsigned int        intent;
    unsigned int        profileData;
    unsigned int        profileSize;
    unsigned int        metadata;
};

typedef struct bmpInformation40bits information40bits;
typedef struct bmpInformation124bits information124bits;

/*
* aca se guardara el cuerpo, la cabeza, la informacion y la cantidad de pixeles del archivo.
* se guardara en una relacion 24 pixeles con un encabesado info de 40 bits
*/
struct bmp2440bits{
    struct bmpHeader head;//aca se guardara la cabeza del archivo
    struct bmpInformation40bits info;//aca se guardara la informacion del archivo
    unsigned int size;//aca se guardara la cantidad de pixeles de la imagen
    RGB **body;//aca se guardaran todos lo pixeles de la imagen.
};

/*
* aca se guardara el cuerpo, la cabeza, la informacion y la cantidad de pixeles del archivo.
* se guardara en una relacion 32 pixeles con un encabesado info de 40 bits
*/
struct bmp3240bits
{
    struct bmpHeader head;//aca se guardara la cabeza del archivo
    struct bmpInformation40bits info;//aca se guardara la informacion del archivo
    unsigned int size;//aca se guardara la cantidad de pixeles de la imagen
    RGBA **body;//aca se guardaran todos lo pixeles de la imagen.
};

/*
* aca se guardara el cuerpo, la cabeza, la informacion y la cantidad de pixeles del archivo.
* se guardara en una relacion 24 pixeles con un encabesado info de 124 bits
*/
struct bmp24124bits
{
    struct bmpHeader head;//aca se guardara la cabeza del archivo
    information124bits info;//aca se guardara la informacion del archivo
    unsigned int size;//aca se guardara la cantidad de pixeles de la imagen
    RGB **body;//aca se guardaran todos lo pixeles de la imagen.
};

/*
* aca se guardara el cuerpo, la cabeza, la informacion y la cantidad de pixeles del archivo.
* se guardara en una relacion 32 pixeles con un encabesado info de 124 bits
*/
struct bmp32124bits
{
    struct bmpHeader head;//aca se guardara la cabeza del archivo
    information124bits info;//aca se guardara la informacion del archivo
    unsigned int size;//aca se guardara la cantidad de pixeles de la imagen
    RGBA **body;//aca se guardaran todos lo pixeles de la imagen.
};

typedef struct bmp2440bits bmp2440;
typedef struct bmp3240bits bmp3240;
typedef struct bmp24124bits bmp24124;
typedef struct bmp32124bits bmp32124;

/*funciones principales*/

bmpHeader *createBmpHeader(unsigned int totalSize, unsigned int offset, int metadata){
    bmpHeader *header = (bmpHeader *) malloc(sizeof(bmpHeader));
    header->fileType[0] = 66;
    header->fileType[1] = 77;
    header->totalSize = totalSize;
    header->metaData = metadata;
    header->offset = offset;
    return header;
}

information40bits *createBmpinformation40(unsigned int headerSize, unsigned int width
, unsigned int height, unsigned short colors, unsigned short bitsPixel, unsigned int bodySize,
unsigned int verticalCM, unsigned int horizontalCM, unsigned int palette, unsigned int importantColor){
    information40bits *info = (information40bits *) malloc(sizeof(information40bits));
    info->bitsPixel = headerSize;
    info->width = width;
    info->height = height;
    info->colors = colors;
    info->bitsPixel = bitsPixel;
    info->bodysize = bodySize;
    info->horizontalResolution = width/horizontalCM;
    info->horizontalResolution = height/horizontalCM;
    info->palette = palette;
    info->importantColor = importantColor;
    return info;
}

bmp2440 *createBmp2440(bmpHeader *header, information40bits *info, unsigned int width, unsigned int height){
    int bitsize = 0, rgbNumber = 0, i = 0;
    bmp2440 *image = (bmp2440 *) malloc(sizeof(bmp2440));
    image->head = *header;
    image->info = *info;
    image->size = width * height;
    image->body = (RGB **) malloc(height * sizeof(RGB **));
    bitsize = ((24 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGB) + 1;
    for (i = height - 1; i >= 0; i--) {
        image->body[i] = (RGB *) malloc(rgbNumber * sizeof(RGB));
    }
    return image;
}

bmp24124 *createBmp24124(bmpHeader *header, information124bits *info, unsigned int width, unsigned int height){
    int bitsize = 0, rgbNumber = 0, i = 0;
    bmp24124 *image = (bmp24124 *) malloc(sizeof(bmp24124));
    image->head = *header;
    image->info = *info;
    image->size = width * height;
    image->body = (RGB **) malloc(height * sizeof(RGB **));
    bitsize = ((24 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGB) + 1;
    for (i = height - 1; i >= 0; i--) {
        image->body[i] = (RGB *) malloc(rgbNumber * sizeof(RGB));
    }
    return image;
}

bmp3240 *createBmp3240(bmpHeader *header, information40bits *info, unsigned int width, unsigned int height){
    int bitsize = 0, rgbNumber = 0, i = 0;
    bmp3240 *image = (bmp3240 *) malloc(sizeof(bmp3240));
    image->head = *header;
    image->info = *info;
    image->size = width * height;
    image->body = (RGBA **) malloc(height * sizeof(RGBA **));
    bitsize = ((24 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGBA) + 1;
    for (i = height - 1; i >= 0; i--) {
        image->body[i] = (RGBA *) malloc(rgbNumber * sizeof(RGBA));
    }
    return image;
}

bmp32124 *createBmp32124(bmpHeader *header, information124bits *info, unsigned int width, unsigned int height){
    int bitsize = 0, rgbNumber = 0, i = 0;
    bmp32124 *image = (bmp32124 *) malloc(sizeof(bmp32124));
    image->head = *header;
    image->info = *info;
    image->size = width * height;
    image->body = (RGBA **) malloc(height * sizeof(RGBA **));
    bitsize = ((24 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGBA) + 1;
    for (i = height - 1; i >= 0; i--) {
        image->body[i] = (RGBA *) malloc(rgbNumber * sizeof(RGBA));
    }
    return image;
}

//metodos de archivo

//metodo interno
static RGB** getBodyRGB(int width, int height, int bits, FILE *image){
    int bitsize = 0, rgbNumber = 0, i = 0;
    bitsize = ((24 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGB) + 1;
    RGB **body = (RGB **) malloc(height * sizeof(RGB **));
    fseek(image, bits, SEEK_SET);
    for (i = height - 1; i >= 0; i--) {
        body[i] = (RGB *) malloc(rgbNumber * sizeof(RGB));
        fread(body[i], bitsize, 1, image);
    }
    return body;
}

//metodo interno
static RGBA** getBodyRGBA(int width, int height,int bit, FILE *image){
    unsigned int bitsize = 0, rgbNumber = 0;
    bitsize = ((32 * width + 31)/32)*4;
    rgbNumber = bitsize/sizeof(RGBA) + 1;
    RGBA **body = malloc(height * sizeof(RGBA *));
    fseek(image, bit, SEEK_SET);
    for (int i = height - 1; i >= 0; i--) {
        body[i] = malloc(rgbNumber * sizeof(RGB *));
        fread(body[i], 1, bitsize, image);
    }
    return body;
}

//metodo interno
static int setBodyRGB(RGB **body, int width, int height, int bits, FILE *image){
    int bitsize = 0, rgbNumber = 0;
    bitsize = ((24 * width + 31)/32)*4;
    fseek(image, bits, SEEK_SET);
    for(int i = height - 1; i >= 0; i--){
        fwrite(body[i], bitsize, 1, image);
    }
    return 1;
}

//metodo interno
static int setBodyRGBA(RGBA **body, int width, int height,int bit, FILE *image){
    unsigned int bitsize = 0, rgbNumber = 0;
    bitsize = ((32 * width + 31)/32)*4;
    fseek(image, bit, SEEK_SET);
    for(int i = height - 1; i >= 0; i--){
        fwrite(body[i], bitsize, 1, image);
    }
    return 1;
}

/*
* contrato: char *, void ** -> int
* esta funcion carga en memoria una imagen tipo bmp llamada filename, 
* la cual se guardara el el puntero *type, devulelve 0 si no se logra abrir la imagen
*/
int fGiveBMP(const char *filename, void **type){
    FILE *image = fopen(filename, "rb");
    if(!image){
        printf("algo fallo");
        return 0;
    }
    struct bmpHeader *head = malloc(sizeof(struct bmpHeader));
    fread( head->fileType, 2, 1, image);
    fread( &head->totalSize, sizeof(int), 1, image);
    fread( &head->metaData, sizeof(int), 1, image);
    fread( &head->offset, sizeof(int), 1, image);
    if(head->offset - 14 == 40){
        information40bits *info = (information40bits *) malloc(sizeof(information40bits));
        fread(info, sizeof(information40bits), 1, image);
        if(info->compression != 0)
            return 0;
        switch (info->bitsPixel){
            case 24:{
                bmp2440 *representation = malloc(sizeof(bmp2440));
                representation->head = *head;
                representation->info = *info;
                representation->size = info->height * info->width;
                representation->body = getBodyRGB(info->width, info->height, head->offset ,image);
                *type = representation;
                break;
            }
            case 32:{
                bmp3240 *representation = malloc(sizeof(bmp3240));
                representation->head = *head;
                representation->info = *info;
                representation->size = info->height * info->width;
                representation->body = getBodyRGBA(info->width, info->height, head->offset, image);
                *type = representation;
                break;
            }
            default:
                printf("no se puede obtener el archivo con las indicaciones de el %i %i\n", info->bitsPixel, head->offset);
                break;
        }
    }else if(head->offset - 14 == 124){
        information124bits *info = (information124bits *) malloc(sizeof(information124bits));
        fread(info, 60, 1, image);
        fread(info->endpoints, 36, 1, image);
        fread(&info->gammaRed, 4, 1, image);
        fread(&info->gammaGreen, 4, 1, image);
        fread(&info->gammaBlue, 4, 1, image);
        fread(&info->intent, 4, 1, image);
        fread(&info->profileData, 4, 1, image);
        fread(&info->profileSize, 4, 1, image);
        fread(&info->metadata, 4, 1, image);
        if(info->compression != 0)
            return 0;
        switch (info->bitsPixel) {
            case 24: {
                bmp24124 *representation = malloc(sizeof(bmp24124));
                representation->head = *head;
                representation->info = *info;
                representation->size = info->height * info->width;
                representation->body = getBodyRGB(info->width, info->height, head->offset, image);
                *type = representation;
                break;
            }
            case 32: {
                bmp32124 *representation = malloc(sizeof(bmp32124));
                representation->head = *head;
                representation->info = *info;
                representation->size = info->height * info->width;
                representation->body = getBodyRGBA(info->width, info->height, head->offset, image);
                *type = representation;
                break;
            }
            default:
                printf("No se puede obtener el archivo BMP con las indicaciones de %d %d\n", info->bitsPixel, head->offset);
                break;
        }
    }
    fclose(image);
    return 1;
}

/*
* contrato: char *, void * -> int
* esta funcion guarda en memoria la imagen contenida en el puntero image, con el nombre de filename
*/
int fSaveBMP(const char *filename, void *image){
    if(image == NULL){
        return 0;
    }
    unsigned int bitsize = 0;
    bmp2440 *representation  = image;
    if(representation->head.fileType[0] != 66 || representation->head.fileType[1] != 77){
        printf("el archivo a guardar no es BMP\n");
        return 0;
    }
    bitsize = ((representation->info.bitsPixel * representation->info.width + 31)/32)*4;
    representation->info.bodysize = bitsize*representation->info.height;
    representation->head.totalSize = representation->head.offset + representation->info.bodysize;
    FILE *file = fopen(filename, "wb");
    if(file == NULL)
        return 0;
    bmpHeader *header = &representation->head;
    fwrite(header->fileType, 1,2,file);
    fwrite(&header->totalSize, 4,1,file);
    fseek(file, 6, SEEK_SET);
    fwrite(&header->metaData, 4,1,file);
    fwrite(&header->offset, 4,1,file);
    switch (representation->head.offset - 14){
        case 40:{
            fwrite(&representation->info, 40, 1, file);
            switch(representation->info.bitsPixel){
                case 24:
                    setBodyRGB(representation->body ,representation->info.width, representation->info.height, header->offset, file);
                    break;
                case 32:{
                    bmp3240 *other = image;
                    setBodyRGBA(other->body ,other->info.width, representation->info.height, header->offset, file);
                    break;
                }
            }
            break;
        }
        case 124:{
            switch(representation->info.bitsPixel){
                case 24:{
                    int h = 0;
                    bmp24124 *other = image;
                    fwrite(&other->info.headerSize, sizeof(int), 1, file);
                    fwrite(&other->info.width, sizeof(long), 1, file);
                    fwrite(&other->info.height, sizeof(long), 1, file);
                    fwrite(&other->info.colors, sizeof(short), 1, file);
                    fwrite(&other->info.bitsPixel, sizeof(short), 1, file);
                    fwrite(&other->info.compression, sizeof(int), 1, file);
                    fwrite(&other->info.bodysize, sizeof(int), 1, file);
                    fwrite(&other->info.horizontalResolution, sizeof(long), 1, file);
                    fwrite(&other->info.verticalResolution, sizeof(long), 1, file);
                    fwrite(&other->info.palette, sizeof(int), 1, file);
                    fwrite(&other->info.importantColor, sizeof(int), 1, file);
                    fwrite(&other->info.redMask, sizeof(int), 1, file);
                    fwrite(&other->info.greenMask, sizeof(int), 1, file);
                    fwrite(&other->info.blueMask, sizeof(int), 1, file);
                    fwrite(&other->info.alphaMask, sizeof(int), 1, file);
                    fwrite(&other->info.bv5CSType, sizeof(int), 1, file);
                    fwrite(other->info.endpoints, 1, 36, file);
                    fseek(file, 110, SEEK_SET);
                    fwrite(&other->info.gammaRed, sizeof(int), 1, file);
                    fwrite(&other->info.gammaGreen, sizeof(int), 1, file);
                    fwrite(&other->info.gammaBlue, sizeof(int), 1, file);
                    fwrite(&other->info.intent, sizeof(int), 1, file);
                    fwrite(&other->info.profileData, sizeof(int), 1, file);
                    fwrite(&other->info.profileSize, sizeof(int), 1, file);
                    fwrite(&other->info.metadata, sizeof(int), 1, file);
                    RGB **body = other->body;
                    setBodyRGB(other->body ,other->info.width, representation->info.height, header->offset, file);
                    break;
                }
                case 32:{
                    bmp32124 *other = image;
                    fwrite(&other->info, sizeof(information124bits), 1, file);
                    fseek(file, other->head.offset, SEEK_SET);
                    setBodyRGBA(other->body ,other->info.width, representation->info.height, header->offset, file);
                    break;
                }
            }
            break;
        }
        default:
            break;
    }
    fclose(file);
    return 1;
}

/*
* contrato: void * -> int
* esta funcion elimina de memoria la imagen, es ideal para cerrar la imagen cuando ya no la necesites.
*/
int fCloseBmp(void *bmpImage){
    free(bmpImage);
    return 1;
}

//metodo principal

void printBmpInfomation(void *i){
    if(i == NULL){
        printf("El archivo esta vacio\n");
        return;
    }
    bmp2440 *imagen = i;
    information40bits *info = &imagen->info;
    printf("Precausion, esta informacion puede der herrada para tamaño de encabezado mayores a 40\n");
    printf("tipo de archivo: %c%c\n", imagen->head.fileType[0], imagen->head.fileType[1]);
    printf("tama%co total en bits de la imagen: %d\n", 164,imagen->head.totalSize);
    printf("metadata como entero: %i\n", imagen->head.metaData);
    printf("byte de comienzo del body: %d\n", imagen->head.offset);
    printf("el tamaño en bits del header: %d\n", info->headerSize);
    printf("el ancho de la imagen: %d\n", info->width);
    printf("el alto de la imagen: %d\n", info->height);
    printf("el plano de colores de la imagen: %d\n", info->colors);
    printf("bits por cada pixel de la imagen: %d\n", info->bitsPixel);
    printf("tipo de conpresion utilizada: %d\n", info->compression);
    printf("tamaño en bits de solo la imagen: %d\n", info->bodysize);
    printf("la resolucion horizontal de la imagen: %d\n", info->horizontalResolution);
    printf("la resolucion vertical de la imagen: %d\n", info->verticalResolution);
    printf("paleta de colores: %d\n", info->palette);
    printf("colores importantes: %d\n", info->importantColor);
    return;
}

int main(){
    bmp24124 *info = 0;
    int h = 0, w = 15;
    fGiveBMP("../imagenes/oceano.bmp",(void **) &info);
    printBmpInfomation(info);
    tranformGrayScale(info->body, info->info.width, info->info.height);
    fSaveBMP("../imagenes/omega.bmp", info);
    fCloseBmp(info);
    return 0;
}