#ifndef GLOBALS_HH
#define GLOBALS_HH

#include <fstream>
#include <string>
#include <vector>
#include <sys/utsname.h>

#include <job.hh>

using namespace std;

#define NOLOAD_EMERGENCY    true
#define ENABLE_COLORS       true
#define LOCALE              "ENGLISH"
#define TIMEO               30

#define DEBUG_MARKER        cout << "*" << __LINE__ << ":" << __FILE__ << "\n";

extern bool         ARGS_TACET;

extern ofstream     LOGFILE;
extern bool         LOGFILE_isopen;

extern string       JOBSDIR;

extern utsname      SYSINFO;
extern bool         SYSINFO_isopen;

extern int          IDCOUNT;

extern vector<Job>  JOBS;

#endif /* GLOBALS_HH */