#include <stdlib.h>
#include <sys/time.h>
#include "utils.h"

double getRealTime ()
{
    struct timeval tm;
    gettimeofday (&tm, NULL);
    return (double) tm.tv_sec + (double) tm.tv_usec / 1000000.0;
}
