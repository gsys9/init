#ifndef SIGNALHANDLERS_HH
#define SIGNALHANDLERS_HH

namespace SignalHandlers {
    void Register(void);
    void HandleSIGCHLD(int);
    void TriggerAlarm(void);
}

#endif /* SIGNALHANDLERS_HH */