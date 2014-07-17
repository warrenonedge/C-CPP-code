#ifndef __CORE__

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef struct
{
    unsigned int width;
    unsigned int height;
    unsigned int max_value;
    unsigned char *data;
} image;

image *read_image (const char *filename);
unsigned char negate (unsigned char value, unsigned int max_value);
int write_image (const char *filename, image *photo);
void clear_image (image *photo);

#endif
