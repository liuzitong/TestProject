#ifndef CHECKRESULTDATA_H
#define CHECKRESULTDATA_H
#include <tuple>
#include <vector>
#include "Params.h"

struct ResultData
{
    int alarm;
    std::vector<int> DB;
    std::vector<int> shortTermFluctuation;
    int falsePositiveCount;
    int falsePositiveTestCount;
    int falseNegativeCount;
    int falseNegativeTestCount;
    int stimulusCount;
    int thresholdCount;
    int testTimespan;
    int ambientLight;
    int E_Light_pv;
    int T_Light_pv;
};


#endif // CHECKRESULTDATA_H
