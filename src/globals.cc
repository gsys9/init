#include <string>
#include <vector>

#include <globals.hh>
#include <job.hh>

bool        ARGS_TACET      = false;

ofstream    LOGFILE;
bool        LOGFILE_isopen  = false;

string      JOBSDIR;

utsname     SYSINFO;
bool        SYSINFO_isopen  = false;

int         IDCOUNT;

vector<Job>  JOBS;