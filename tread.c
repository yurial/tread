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
params( argc, argv );
char* buff = (char*)malloc( g_buffsize );
struct pollfd fds[1];

fds->fd = g_fd;
fds->events = POLLIN | POLLERR | POLLHUP;
fds->revents = 0;
while ( 1 )
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
        nread = read( g_fd, buff, sizeof(buff) );
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

