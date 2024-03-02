#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

#include <globals.hh>
#include <common.hh>
#include <color.hh>
#include <init.hh>
#include <signalhandlers.hh>

void Init::Main(vector<string> argv) {
    Common::OpenLog();
    Common::Log("cascade", "initialising...", GOOD);
    Common::Log("common", "getting system information...", DEFAULT);
    Common::GetSystemInformation();
    Common::WelcomeBanner();
    Common::Log("signalhandlers", "registering signal handlers...", DEFAULT);
    SignalHandlers::Register();
    if (NOLOAD_EMERGENCY) {
        Common::Log("init", "requesting root login for debug...", WARN);
        pid_t i = fork();
        if (i == 0) {
            execl("/sbin/sulogin", "/sbin/sulogin");
        } else if (i == -1) {
            Common::Log("fork", "failed to fork process", ERROR);
        } else {
            waitpid(i, NULL, 0);
        }
    }
    // Init::loadjobs();
    // vector<vector<job>> order = Init::GetOrder(range(0, ID_COUNT-1));
    // Init::ExecOrder(order);

    while (true);
}