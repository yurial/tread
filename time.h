#ifndef TREAD_TIMEH
#define TREAD_TIMEH

#include <time.h>

time_t uts2local(time_t uts);
time_t local2uts(time_t local);
int gettimeout();

#endif

