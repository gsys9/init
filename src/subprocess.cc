#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <thread>
#include <iostream>

#include <common.hh>
#include <subprocess.hh>

using namespace std;

void Subprocess::InitInternal() {
    // Reserved for future use.
}

Subprocess::Subprocess(string cmd) {
    this -> args.push_back("/bin/sh");
    this -> args.push_back("-c");
    this -> args.push_back(cmd);
    this -> InitInternal();
}

Subprocess::Subprocess(vector<string> args) {
    this -> args = args;
    this -> InitInternal();
}

int Subprocess::Run() {
    this -> procpid = fork();
    if (this -> procpid == 0) {
        int errexec = this -> Takeover();
        if (errexec != 0) {
            Common::Log("subprocess", "could not launch process");
        }
    } 
    else if (this -> procpid == -1) {
        Common::Log("subprocess", "failed to spawn child");
        return 1;
    }
    
    return 0;
}

int Subprocess::Takeover() {
    char **exec_args = (char**)malloc(this -> args.size() + 1);
    int counter = 0;
    for (string argument: this -> args) {
        exec_args[counter] = (char*)argument.c_str();
        counter++;
    }
    exec_args[counter] = NULL;
    int ret = execvp(exec_args[0], exec_args);
    return ret;
}

void Subprocess::Wait() {
    waitpid(this -> procpid, NULL, 0);
}