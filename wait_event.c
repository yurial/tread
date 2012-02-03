#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "wait_event.h"

#ifdef INOTIFY_ENGINE

#include <sys/inotify.h>
int wait_event(int fd, const char* filename, int timeout)
{
struct pollfd fds[ 1 ];
int ifd;
int ed;
int nready;
int ret;

ifd = inotify_init();
if ( -1 == ifd )
    {
    fprintf( stderr, "can't open inotify for monitoring file\n" );
    exit( EXIT_FAILURE );
    }

ed = inotify_add_watch( ifd, filename, IN_MODIFY );
if ( -1 == ed )
    {
    fprintf( stderr, "can't open file for monitoring\n" );
    exit( EXIT_FAILURE );
    }

fds->fd = ifd;
fds->events = POLLIN | POLLERR | POLLHUP;
fds->revents = 0;
nready = poll( fds, 1, timeout );
ret = ( 0 == nready )? 0 : -1;

close( ifd );
return ret;
}

#else
#ifdef KQUEUE_ENGINE

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

int wait_event(int fd, const char* filename, int timeout)
{
int kq;
int nready;
struct kevent ke;
struct timespec ts;
EV_SET( &ke, fd, EVFILT_VNODE, EV_ADD, EVFILT_READ, 0, NULL );
ts.tv_sec = timeout / 1000;
ts.tv_nsec = ( timeout % 1000 ) * 1000 * 1000;

kq = kqueue();
if ( 0 > kq )
	{
	fprintf( stderr, "can't create kqueue for monitoring file\n" );
	exit( EXIT_FAILURE );
	}

nready = kevent( kq, &ke, 1, NULL, 0, &ts );
if ( 0 > nready )
	{
	fprintf( stderr, "can't register filter for waiting file events\n" );
	exit( EXIT_FAILURE );
	}

while ( 1 )
	{
	nready = kevent( kq, NULL, 0, &ke, 1, &ts );
	if ( -1 == nready )
		{
		switch ( errno )
			{
			case ENOMEM:
			case EMFILE:
			case ENFILE:
			case EACCES:
			case EFAULT:
			case EINVAL:
			case ENOENT:
			case ESRCH:
				fprintf( stderr, "kevent() error\n" );
				exit( EXIT_FAILURE );
			case EINTR:
				continue;
			}
		}
	close( kq );
	return nready;
	}
}

#endif
#endif

