#include <string>
#include <sstream>

#include <job.hh>
#include <subprocess.hh>
#include <globals.hh>
#include <common.hh>
#include <color.hh>

using namespace std;

Job::Job() {
    this -> AllocateID();
    this -> announcement = (void (*)(int))&Job::Announcement;
}

void Job::Announcement(int) {
    if (this -> proc -> returncode == 0) {
        if (this -> flag_keepalive) {
            Common::Log(
                this -> idname,
                "job finished early, restarting...",
                WARN
            );
            this -> proc -> Run();
        } else {
            Common::Log(
                this -> idname,
                "job finished successfully.",
                GOOD
            );
            this -> hasrun = true;
        }
    } else {
        stringstream getreturncode;
        getreturncode << this -> proc -> returncode;
        string msg = "job finished with errors [code ";
        getreturncode >> msg;
        msg.append("]");
        Common::Log(
            this -> idname,
            msg,
            ERROR
        );
    }
}

void Job::AllocateID() {
    this -> id = IDCOUNT;
    IDCOUNT++;
}

void Job::Execute() {
    this -> proc = new Subprocess(this -> exec);
    this -> proc -> SetFunctionOnFinish(announcement);
    this -> proc -> Run();
}