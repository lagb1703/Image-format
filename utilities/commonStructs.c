struct RGB
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct RGBA
{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    unsigned char A;
};

typedef struct RGB RGB;
typedef struct RGB bits24;
typedef struct RGBA RGBA;
typedef struct RGBA bits32;