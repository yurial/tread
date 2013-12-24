AC_INIT
AC_LANG_C
AC_CHECK_HEADERS([errno.h unistd.h stdlib.h getopt.h stdio.h])
AC_UNISTD_H
AC_CHECK_HEADER(sys/inotify.h, WAIT_EVENT_ENGINE=INOTIFY_ENGINE,
    AC_CHECK_HEADER(sys/event.h, WAIT_EVENT_ENGINE=KQUEUE_ENGINE,
        AC_MSG_ERROR([cannot determine event engine (inotify, kqueue) ])
        )
    )

AC_SUBST(WAIT_EVENT_ENGINE)
AC_OUTPUT(Makefile config.h)
