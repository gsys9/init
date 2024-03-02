#include <signal.h>
#include <sys/wait.h>

#include <globals.hh>
#include <signalhandlers.hh>

void SignalHandlers::Register() {
    signal(SIGCHLD, SignalHandlers::HandleSIGCHLD);
    signal(SIGALRM, SignalHandlers::HandleSIGCHLD);
}

void SignalHandlers::HandleSIGCHLD(int) {
    while (waitpid(-1, NULL, WNOHANG) > 0); 
    alarm(TIMEO);
}