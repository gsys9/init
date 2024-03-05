#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>

#include <config.hh>
#include <common.hh>
#include <color.hh>

Config::Config(string input) {
    this -> fdata = input;
    this -> ReadToStore();
}

Config::Config(fstream* stream) {
    char strbuf[2] = {'\0'};
    while (!stream -> eofbit) {
        stream -> read(strbuf, 1);
        this -> fdata.append(strbuf);
    }
    this -> ReadToStore();
}

vector<string> Config::ListEntities() {
    vector<string> names;
    for (pair<string, vector<vector<string>>> i: this -> store) {
        names.push_back(i.first);
    }
    return names;
}

int Config::ReadToStore() {
    vector<string> lines = Common::SplitString(this -> fdata, "\n");
    string sectionname = "";
    vector<vector<string>> section;
    for (string line: lines) {
        Common::TrimString(line);
        if (line == "")
            continue;
        else if (line[0] == ';')
            continue;
        else if (line[0] == '@') {
            if (section.size() != 0)
                this -> store[sectionname] = section;
            sectionname = line;
            sectionname.erase(0, 1);
        }
        else {
            if (sectionname == "") {
                Common::Log("config", "failed to parse to table: no table declared", ERROR);
                continue;
            }
            section.push_back(Common::SplitString(line, ":"));
        }
    }
    if (section.size() != 0)
        this -> store[sectionname] = section;
    return 0;
}

unordered_map<string, vector<vector<string>>> Config::GetStore() {
    return this -> store;
}

vector<vector<string>> Config::GetInformationForEntity(string name) {
    for (pair<string, vector<vector<string>>> i: this -> store)
        if (i.first == name) return i.second;
    string name2 = "no table matches: ";
    name2.append(name);
    throw out_of_range(name2);
}

vector<string> Config::GetKeyForChild(string name, string name2) {
    vector<vector<string>> tmp = this -> GetInformationForEntity(name);
    for (vector<string> i: tmp)
        if (i[0] == name) return i;
    string name3 = "no record matches: ";
    name3.append(name);
    name3.append(":");
    name3.append(name2);
    throw out_of_range(name3);
}

vector<string> Config::GetRowForNumber(string name, int index) {
    vector<vector<string>> tmp = this -> GetInformationForEntity(name);
    return tmp[index];
}
