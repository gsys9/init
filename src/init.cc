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
        Job tmpjob;
        try {
            JobFromTable(&tmpjob, section, master -> GetInformationForEntity(section));
        } catch(runtime_error) {
            string msg = "parsing section ";
            msg.append(section); msg.append(" failed.");
            Common::Log("jobmgmt", msg, ERROR);
        }
        JOBS.push_back(tmpjob);
    }
    try {
        JOBSDIR = master -> GetKeyForChild("nulltarget", "cfdir")[1];
    } catch(out_of_range) {
        Common::Log("jobmgmt", "parsing section nulltarget failed.", ERROR);
    }
}

// Must run only after JOBSDIR has been set.
void Init::ParseConfigFiles() {
    // Not implemented.
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