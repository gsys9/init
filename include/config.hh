#ifndef CONFIG_HH
#define CONFIG_HH

#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

using namespace std;

class Config {
    unordered_map<string, vector<vector<string>>> store;
    string fdata;
    fstream* file;
public:
    Config(string);
    Config(fstream*);

    int                     ReadToStore();
    unordered_map<string, vector<vector<string>>> GetStore();

    vector<string>          ListEntities();
    vector<vector<string>>  GetInformationForEntity(string);
    vector<string>          GetKeyForChild(string, string);
    vector<string>          GetRowForNumber(string, int);
};

#endif /* CONFIG_HH */