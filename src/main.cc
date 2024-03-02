#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

#include <common.hh>
#include <globals.hh>
#include <color.hh>
#include <signalhandlers.hh>

using namespace std;

vector<string> ConvertArgs(int argc, const char* argv[]) {
    vector<string> argv_new;
    int counter = 0;
    while (counter < argc) {
        string new_arg = argv[counter];
        argv_new.push_back(new_arg);
        counter++;
    }
    return argv_new;
}

int NewMain(vector<string> argv) {
    if (getpid() == 1) {
        Common::OpenLog();
        Common::Log("runtimed", "we are PID 1, initialising...", GOOD);
        Common::Log("signalhandlers", "registering signal handlers.", GOOD);
        SignalHandlers::Register();
    } else if (argv.size() == 1) {
        cout << 
            "Options are required unless we are PID 1.\n"
            "Usage:\n" 
            "        runtimed COMMAND\n"
            "        runtimed [RUNLEVEL]\n"
            "        init [RUNLEVEL]\n"
            "        bootstrapinit [RUNLEVEL]\n"
            "        COMMAND [OPTIONS]\n"
            "\n"            
            "    Invokes COMMAND with [OPTIONS], or, as\n"
            "    PID 1, runs as init [RUNLEVEL].\n";
        return 1;
    } else if (argv[1] == "init") {
        Common::Log("init", "we are not PID 1. skipping initialisation.");
        Common::Panic();
    }
    return 0;
}

int main(int argc, const char* argv[]) {
    vector<string> argv_new = ConvertArgs(argc, argv);
    return NewMain(argv_new);
}