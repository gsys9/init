#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <locale>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/reboot.h>

#include <common.hh>
#include <globals.hh>
#include <color.hh>

using namespace std;

void Common::ClearScreen() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void Common::OpenLog() {
    LOGFILE.open("/run/cascade.log", ios::out);
    if (!LOGFILE.is_open()) {
        Common::Log("log", "logging has been disabled.", WARN);
    } else {
        LOGFILE_isopen = true;
        Common::Log("log", "opened system log.");
    }
}

void Common::Log(string name, string output, string color, bool quiet) {
    if (name.length() > 22)
        name.resize(22);

    // Make everything align on screen.
    string spaces;
    int numberofspaces = 24 - (name.length()+2);
    int i = 0; 
    while (i < numberofspaces) {
        spaces.append(" ");
        i++;
    }

    string fulloutput;
    fulloutput.append("\033[1m");
    fulloutput.append(color);
    fulloutput.append(name);
    fulloutput.append(": ");
    fulloutput.append(RESET);
    fulloutput.append(spaces);
    fulloutput.append(output);
    fulloutput.append("\n");

    cout << fulloutput;
    cout.flush();

    if (ARGS_TACET && quiet);
    else if (LOGFILE_isopen) {
        LOGFILE << fulloutput;
        LOGFILE.flush();
    }
}

void Common::Log(string name, string output, string color) {
    Common::Log(name, output, color, false);
}

void Common::Log(string name, string output) {
    Common::Log(name, output, DEFAULT, false);
}

void Common::Panic() {
    Common::Log("panic", "program lifecycle ended prematurely.", ERROR);
    if (getpid() == 1) {
        cout << Common::panicmsg;
        while (true);
    } else {
        exit(7);
    }
}

void Common::Reboot(int magicnum) {
    Common::Log("internals", "requesting system reboot...");
    reboot(magicnum);
}

void Common::GetSystemInformation() {
    uname(&SYSINFO);
}

void Common::WelcomeBanner() {
    string msg;
    msg.append("starting ");
    msg.append(SYSINFO.sysname);
    msg.append(" (version ");
    msg.append(SYSINFO.release);
    msg.append(")");

    Common::Log(
        "cascade",
        msg
    );
}

vector<string> Common::SplitString(string str, string delim) {
    string tmp = str;
    vector<string> total;
    string cur_elem;
    bool is_done;
    while (!is_done) {
        size_t place = tmp.find(delim);
        if (place == string::npos) {
            cur_elem = tmp.substr(0, tmp.size()); 
            is_done = true;
        } else {
            cur_elem = tmp.substr(0, place-1);
            tmp.replace(0, place+delim.size(), "");
        }
        total.push_back(cur_elem);
    }
    return total;
}

void Common::TrimString(string& s) {
    Common::RightTrimString(s);
    Common::LeftTrimString(s);
}

void Common::RightTrimString(string& s) {
    s.erase(
        find_if(
            s.rbegin(), 
            s.rend(),
            [](unsigned char ch) {
                return !isspace(ch);
            }
        ).base(), 
        s.end()
    );
}

void Common::LeftTrimString(string& s) {
    s.erase(
        s.begin(),
        find_if(
            s.begin(), 
            s.end(),
            [](unsigned char ch) {
                return !isspace(ch);
            }
        )
    );
}