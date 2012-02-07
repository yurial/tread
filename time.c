#include "time.h"
#include "params.h"

#ifndef MAX
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

time_t uts2local(time_t uts)
{
struct tm local;
localtime_r( &uts, &local );
return timegm( &local );
}

time_t local2uts(time_t local)
{
struct tm uts;
gmtime_r( &local, &uts );
uts.tm_isdst = -1; 
return mktime( &uts );
}

int gettimeout()
{
int time_timeout = ( g_time - time( NULL ) ) * 1000;
if ( 0 == g_timeout )
    {
    return MAX( 0, time_timeout );
    }
if ( 0 == g_time )
    {
    return MAX( 0, g_timeout );
    }
return MAX( g_timeout, time_timeout );
}

