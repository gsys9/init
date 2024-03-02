#ifndef GLOBALS_HH
#define GLOBALS_HH

#include <fstream>
#include <string>
#include <sys/utsname.h>

using namespace std;

#define NOLOAD_EMERGENCY    true
#define ID_COUNT            0
#define ENABLE_COLORS
#define LOCALE              "ENGLISH"
#define TIMEO               30

extern bool        ARGS_TACET;

extern ofstream    LOGFILE;
extern bool        LOGFILE_isopen;

extern string      JOBSDIR;

extern utsname     SYSINFO;
extern bool        SYSINFO_isopen;

#endif /* GLOBALS_HH */