#ifndef JOB_HH
#define JOB_HH

#include <string>
#include <vector>
#include <subprocess.hh>

using namespace std;

class Job {
private:
    void Announcement(int);
    void (*announcement)(int);

public:
    string idname;
    string description;
    string exec;
    int runlevel;
    vector<string> depends;
    int id;

    Subprocess* proc;

    bool flag_wait;
    bool flag_last;
    bool flag_keepalive;

    bool hasrun;

    void AllocateID(void);
    void Execute(void);

    Job(void);
};

void JobFromTable(Job*, string, vector<vector<string>>);

#endif /* JOB_HH */