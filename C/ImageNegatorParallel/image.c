#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "image.h"

/* prototype for nanosleep, this is required as nanosleep is not
   defined in ANSI C, it is a POSIX function instead */
int nanosleep (struct timespec *req, struct timespec *rem);

void delay (int usec)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = usec * 1000 /* ns/us */;
    nanosleep (&ts, NULL);
}

image *read_image (const char *filename)
{
    image *photo = NULL;
    int width, height, max_value, n;
    char p6 [3];
    unsigned char dummy;

    FILE *file = fopen (filename, "r");
    if (file == NULL)
    {
        return NULL;
    }
    
    fscanf (file, "%s", p6);
    if (strcmp (p6, "P6") != 0)
    {
        fclose (file);
        return NULL;
    }

    fscanf (file, "%ud", &width);
    fscanf (file, "%ud", &height);
    fscanf (file, "%ud", &max_value);

    n = width * height * 3; /* 3 bytes per pixel */

    photo = (image *) malloc (sizeof (image));
    if (photo == NULL)
    {
        fclose (file);
        return NULL;
    }

    photo->width = width;
    photo->height = height;
    photo->max_value = max_value;
    
    photo->data = (unsigned char *) malloc (n);

    if (photo->data == NULL)
    {
        free (photo);
        fclose (file);
        return NULL;
    }

    fread (&dummy, 1, 1, file); /* read the required white space */
    fread (photo->data, n, 1, file);
    return photo;
}

unsigned char negate (unsigned char value, unsigned int max_value)
{
    /*delay (value);*/
    return max_value - value;
}

int write_image (const char *filename, image *photo)
{
    FILE *file = fopen (filename, "w");
    int length = photo->width * photo->height * 3;
    unsigned char *p = photo->data;
    int i;
    
    if (file == NULL)
    {
        return FALSE;
    }
    
    fprintf (file, "P6\n");
    fprintf (file, "%u %u\n", photo->width, photo->height);
    fprintf (file, "%u\n", photo->max_value);

    for (i = 0; i < length; i++)
    {
        putc (*p, file);
        p++;
    }

    fclose (file);
    return TRUE;
}

void clear_image (image *photo)
{
    free (photo -> data);
    free (photo);
}

