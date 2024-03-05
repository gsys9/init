#ifndef COMMON_HH
#define COMMON_HH

#include <string>

using namespace std;

namespace Common {
    const string panicmsg = 
        "Your computer has had a fatal error and needs to "
        "be restarted. If the root filesystem had time to "
        "mount, then debugging information will be available "
        "in the system log at /var/runtimed.log. If the "
        "root filesystem was not mounted, then the debugging "
        "information is forever lost."
        "\n";
    
    void                ClearScreen             (void);
    void                OpenLog                 (void);

    void                Log                     (string, string, string, bool);
    void                Log                     (string, string, string);
    void                Log                     (string, string);

    void                Panic                   (void);
    void                Reboot                  (int);
    void                GetSystemInformation    (void);
    void                WelcomeBanner           (void);

    vector<string>      SplitString             (string, string);
    void                TrimString              (string&);
    void                RightTrimString         (string&);
    void                LeftTrimString          (string&);
}

#endif /* COMMON_HH */