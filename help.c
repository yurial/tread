#include "help.h"

void help(FILE* out, int argc, char* argv[])
{
fprintf( out, "'tread' - timed read, read from file while not timed out\n" );
fprintf( out, "usage: %s [-h] [-v] [-t timeout] [file]\n", argv[0] );
fprintf( out, "    -h  --help           show help\n" );
fprintf( out, "    -v  --version        show version\n" );
fprintf( out, "    -t  --timeout <msec> set read timeout in milliseconds\n" );
fprintf( out, "    -T  --time <date>    set target date, program be read file, while date not present\n" );
fprintf( out, "    -u  --uts            use UTS time\n" );
fprintf( out, "    [file]               set file to read, default stdin\n" );
}

void version()
{
fprintf( stdout, "%s\n", __INFO__ );
}

