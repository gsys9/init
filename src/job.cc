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
}

void Job::Announcement() {
    if (proc -> returncode == 0) {
        if (flag_keepalive) {
            Common::Log(
                idname,
                "job finished early, restarting...",
                WARN
            );
            proc -> Run();
        } else {
            Common::Log(
                idname,
                "job finished successfully.",
                GOOD
            );
            hasrun = true;
        }
    } else {
        stringstream getreturncode;
        getreturncode << proc -> returncode;
        string msg = "job finished with errors [code ";
        getreturncode >> msg;
        msg.append("]");
        Common::Log(
            idname,
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

    this -> proc -> SetFunctionOnFinish(&this -> Announcement, &this -> Announcement);
    this -> proc -> Run();
}