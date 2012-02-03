#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>

#include "wait_event.h"

#define BUFFSIZE (4*1024)

#if 0
struct pollfd {
    int    fd;       /* file descriptor */
    short  events;   /* events to look for */
    short  revents;  /* events returned */
    };
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
#endif

void help(FILE* out, int argc, char* argv[])
{
fprintf( out, "'tread' - timed read, read from file while not timed out\n" );
fprintf( out, "usage: %s [-h] [-v] [-t timeout] [file]\n", argv[0] );
fprintf( out, "    -h  --help           show help\n" );
fprintf( out, "    -v  --version        show version\n" );
fprintf( out, "    -t  --timeout <msec> set read timeout in milliseconds\n" );
fprintf( out, "    [file]               set file to read, default stdin\n" );
}

void version()
{
fprintf( stdout, "%s\n", __INFO__ );
}

static const char optstring[] = "hvt:";
static const struct option longopts[] = {
        { "help",       0,  0,  'h' },
        { "version",    0,  0,  'v' },
        { "timeout",    1,  0,  't' },
        { NULL,         0,  0,   0  }
    };

int main(int argc, char* argv[])
{
char* filename = NULL;
int timeout = 0;
int fd = STDIN_FILENO;
int nready;
int nread;
int nwrite;
char buff[BUFFSIZE];
struct pollfd fds[1];
int opt;
int longindex;

while ( 1 )
    {
    opt = getopt_long( argc, argv, optstring, longopts, &longindex );
    if ( -1 == opt )
        break;
    switch ( opt )
        {
        case 'h':
            help( stdout, argc, argv );
            return EXIT_SUCCESS;
        case 'v':
            version();
            return EXIT_SUCCESS;
        case 't':
            timeout = atoi( optarg );
            break;
        case 0:
        case 1:
        case 2:
        case '?':
        case ':':
            help( stderr, argc, argv );
            return EXIT_FAILURE;
        }
    }
if ( optind < argc )
    {
    filename = argv[optind];
    fd = open( filename, O_RDONLY );
    if ( 0 > fd )
        {
        fprintf( stderr, "can't open input file '%s'\n", argv[optind] );
        return EXIT_FAILURE;
        }
    }

fds->fd = fd;
fds->events = POLLIN | POLLERR | POLLHUP;
fds->revents = 0;
while ( 1 )
    {
    nready = poll( fds, 1, timeout );
    if ( 0 > nready )
        {
        switch ( errno )
            {
            case EINTR:
                continue; //normal
            case EFAULT:
            case EINVAL:
            case ENOMEM:
                return EXIT_FAILURE;
            }
        }
    if ( 0 == nready )
        break; //timed out
    if ( fds->revents & POLLIN )
        {
        nread = read( fd, buff, sizeof(buff) );
        if ( 0 == nread )
            {
            if ( NULL == filename )
                return EXIT_SUCCESS;
            if ( 0 == wait_event( fd, filename, timeout ) )
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
return EXIT_SUCCESS;
}

