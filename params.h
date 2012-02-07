#ifndef PARAMSH
#define PARAMSH

extern  int     g_timeout;
extern  int     g_time;
extern  int     g_fd;
extern  char*   g_filename;
extern  int     g_uts;
extern  size_t  g_buffsize;

void params(int argc, char* argv[]);

#endif

