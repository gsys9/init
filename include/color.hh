#ifndef COLOR_HH
#define COLOR_HH

    #include <globals.hh>

    #if ENABLE_COLORS == true
    
        #define RESET       "\033[0m"

        #define ERROR       "\033[31m"
        #define GOOD        "\033[32m"
        #define WARN        "\033[33m"
        #define DEFAULT     "\033[39m"
        
        #define BOLD        "\033[1m"
    
    #else
    
        #define RESET       ""

        #define ERROR       ""
        #define GOOD        ""
        #define WARN        ""
        #define DEFAULT     ""
        
        #define BOLD        ""
    
    #endif
    
    #define CLEAR "\033[2J\033[H"

#endif /* COLOR_HH */