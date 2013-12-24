#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "params.h"
#include "time.h"

#include "wait_event.h"

int main(int argc, char* argv[])
{
int nready;
int nread;
int nwrite;
int second_try = 0;
params( argc, argv );
char* buff = (char*)malloc( g_buffsize );
struct pollfd fds[1];

for(;;)
    {
    g_fd = open( g_filename, O_RDONLY );
    if ( g_fd > 0 )
        break; /* file opened successfully */

    if( g_time > time( NULL ) )
        {
        sleep( 1 );
        continue;
        }

    if( second_try || 0 == g_timeout )
        return EXIT_FAILURE;

    /* wait and try again */
    usleep( g_timeout*1000 );
    second_try = 1;
    }

fds->fd = g_fd;
fds->events = POLLIN | POLLERR | POLLHUP;
fds->revents = 0;
for(;;)
    {
    nready = poll( fds, 1, gettimeout() );
    if ( 0 > nready )
        {
        switch ( errno )
            {
            case EINTR:
                continue; /* normal */
            case EFAULT:
            case EINVAL:
            case ENOMEM:
                return EXIT_FAILURE;
            }
        }
    if ( 0 == nready )
        break; /* timed out */
    if ( fds->revents & POLLIN )
        {
        nread = read( g_fd, buff, g_buffsize );
        if ( 0 == nread )
            {
            if ( NULL == g_filename )
                return EXIT_SUCCESS;
            if ( 0 == wait_event( g_fd, g_filename, gettimeout() ) )
                return EXIT_SUCCESS;
            else
                continue;
            }
        if ( -1 == nread )
            return EXIT_FAILURE;
        nwrite = write( STDOUT_FILENO, buff, nread );
        if ( nwrite != nread )
            return EXIT_FAILURE;
        }
    else
        return EXIT_SUCCESS;
    }
free( buff );
return EXIT_SUCCESS;
}

