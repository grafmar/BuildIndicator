#ifndef _CONFIGHANDLER_h
#define _CONFIGHANDLER_h

#include "arduino.h"
#include "interpretation.h"
#include <list>

class ConfigHandler {
public:
    ConfigHandler();
    ~ConfigHandler();

    void readBuildKeys(String* buildKeys);
    void saveBuildKeys(String* buildKeys);
    void readInterpretations(std::list<Interpretation>& interpretations);
    void saveInterpretations(std::list<Interpretation>& interpretations);

    //void readBuildResultInterpretation(String* buildKeys);
    //void saveBuildResultInterpretation(String* buildKeys);

private:
};


#endif


