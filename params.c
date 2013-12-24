#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <getopt.h>
#include <time.h>

char* strptime(const char* buf, const char* format, struct tm* timeptr);

#define BUFFSIZE (4*1024)

static const char optstring[] = "hvut:T:";
static const struct option longopts[] = {
        { "help",       0,  0,  'h' },
        { "version",    0,  0,  'v' },
        { "timeout",    1,  0,  't' },
        { "time",       1,  0,  'T' },
        { "uts",        0,  0,  'u' },
        { NULL,         0,  0,   0  }
    };

int     g_timeout = 0;
int     g_time = 0;
int     g_fd = STDIN_FILENO;
char*   g_filename = NULL;
int     g_uts = 0;
size_t  g_buffsize = BUFFSIZE;

void params(int argc, char* argv[])
{
struct tm tm = { 0 };
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
            exit( EXIT_SUCCESS );
        case 'v':
            version();
            exit( EXIT_SUCCESS );
        case 't':
            g_timeout = atoi( optarg );
            break;
        case 'u':
            g_uts = 1;
            break;
        case 'T':
            {
            char* ret = strptime( optarg, "%Y-%m-%d %H:%M:%S", &tm );
            if ( NULL == ret || '\0' != *ret )
                {
                fprintf( stderr, "invalid date time\n" );
                exit( EXIT_FAILURE );
                }
            }
            break;
        case 0:
        case 1:
        case 2:
        case '?':
        case ':':
        default:
            help( stderr, argc, argv );
            exit( EXIT_FAILURE );
        }
    }
if ( optind < argc )
    {
    g_filename = argv[optind];
    }
if ( 0 != tm.tm_year )
    {
    g_time = ( 0 != g_uts )? uts2local( mktime( &tm ) ) : mktime( &tm );
    }
}

