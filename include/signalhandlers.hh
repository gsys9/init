#ifndef SIGNALHANDLERS_HH
#define SIGNALHANDLERS_HH

namespace SignalHandlers {
    void Register(void);
    void HandleSIGCHLD(int);
}

#endif /* SIGNALHANDLERS_HH */