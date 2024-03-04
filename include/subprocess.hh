#ifndef SUBPROCESS_HH
#define SUBPROCESS_HH

#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <functional>

using namespace std;

class Subprocess {
private:
    void InitInternal();
    void WaitInternal();
    thread *waitthread;
    void (*onfinish)(int);

public:
    pid_t procpid;
    int returncode;
    vector<string> args;

    Subprocess(string);
    Subprocess(vector<string>);

    int Run();

    // Where this process is no longer a *sub*process.
    // In 99% of situations this would rarely be used.
    // Left as public for the other 1%.
    int Takeover();

    void SetFunctionOnFinish(void (*)(int));
    
};

#endif /* SUBPROCESS_HH */