#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H
#include "Params.h"
#include <tuple>
#include <array>
#include <vector>

struct StaticProgramData
{
    StaticParams::CommonParams::Strategy strategies[3];
    std::vector<std::tuple<float,float>> dots;
};

struct MoveProgramData
{
    std::vector<std::tuple<float,float>> dots;
};


#endif // PROGRAMDATA_H
