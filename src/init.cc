#include <vector>
#include <string>
#include <unordered_map>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include <globals.hh>
#include <common.hh>
#include <color.hh>
#include <config.hh>
#include <init.hh>
#include <signalhandlers.hh>
#include <subprocess.hh>
#include <job.hh>

using namespace std;

void Init::ParseMasterConfig() {
    ofstream* master_file = new ofstream("/usr/share/cascade/master.cf");
    Config* master = new Config((fstream*)master_file);
    
    vector<string> sections = master -> ListEntities();
    for (string section: sections) {
        if (section == "nulltarget") continue;
        
    }
}

void Init::Main(vector<string> argv) {
    Common::OpenLog();
    Common::Log("cascade", "initialising...", GOOD);
    Common::Log("common", "getting system information...", DEFAULT);
    Common::GetSystemInformation();
    Common::WelcomeBanner();
    Common::Log("signalhandlers", "registering signal handlers...", DEFAULT);
    SignalHandlers::Register();
#if NOLOAD_EMERGENCY == true
    Common::Log("init", "requesting root login for debug...", WARN);
    Subprocess shell("/sbin/sulogin");
    shell.Run();
    shell.Wait();

#endif

    Init::ParseMasterConfig();
    // Init::loadjobs();    
    // vector<vector<Job>> order = Init::GetOrder(range(0, ID_COUNT-1));
    // Init::ExecOrder(order);

    while (true);
}