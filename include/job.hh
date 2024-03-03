#ifndef JOB_HH
#define JOB_HH

#include <string>
#include <vector>
#include <subprocess.hh>

using namespace std;

class Job {
private:
    static void Announcement(void);

public:
    static string idname;
    static string description;
    static string exec;
    static int runlevel;
    static vector<string> depends;
    static int id;

    static Subprocess* proc;

    static bool flag_wait;
    static bool flag_last;
    static bool flag_keepalive;

    static bool hasrun;

    void AllocateID(void);
    void Execute(void);

    Job(void);
};

#endif /* JOB_HH */