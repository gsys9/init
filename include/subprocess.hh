#ifndef SUBPROCESS_HH
#define SUBPROCESS_HH

#include <unistd.h>
#include <string>
#include <vector>
#include <thread>

using namespace std;

class Subprocess {
private:
    void InitInternal();
    static void WaitInternal();
    thread* waitthread;

public:
    static pid_t procpid;
    static int returncode;
    static void (*onfinish)();
    static void (*onfinish_error)();
    static vector<string> args;

    Subprocess(string);
    Subprocess(vector<string>);

    int Run();

    // Where this process is no longer a *sub*process.
    // In 99% of situations this would rarely be used.
    // Left as public for the other 1%.
    int Takeover();

    void SetFunctionOnFinish(void (*)(), void (*)());
    
};

#endif /* SUBPROCESS_HH */