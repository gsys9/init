#include <string>
#include <sstream>

#include <job.hh>
#include <subprocess.hh>
#include <globals.hh>
#include <common.hh>
#include <color.hh>

using namespace std;

void JobFromTable(Job* tmp, string idname, vector<vector<string>> table) {
    int count;
    tmp -> idname = idname;
    for (vector<string> line: table) {
        if (line.size() != 2) {
            Common::Log("jobmgmt", "record length is not 2");
            goto error;
        }
        if (line[1] == "")
            continue;
        if (line[0] == "description")
            tmp -> description = line[1];
        if (line[0] == "exec")
            tmp -> exec = line[1];
        if (line[0] == "depends")
            tmp -> depends = Common::SplitString(line[1], " ");
        if (line[0] == "flags") {
            if (line[1] == "!") continue;
            for (string item: line) {
                if (item == "flags") continue;
                if (item == "+last")        tmp -> flag_last = true;
                if (item == "+keepalive")   tmp -> flag_keepalive = true;
                if (item == "+wait")        tmp -> flag_wait = true;
            }
        }
        
    }
error:
    throw runtime_error("exception during runtime.");
}

Job::Job() {
    this -> AllocateID();
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
    this -> proc -> Run();
    this -> proc -> Wait();
    this -> Announcement(proc -> returncode);
}