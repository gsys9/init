#ifndef INIT_HH
#define INIT_HH

#include <vector>
#include <string>

using namespace std;

namespace Init {
    void ParseMasterConfig(void);
    void ParseConfigFiles(void);
    void Main(vector<string>);
}

#endif /* INIT_HH */