#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H
#include "Params.h"
#include <tuple>
#include <array>
#include <vector>
#include "commondef.h"

namespace Model{
struct StaticProgramData
{
    std::vector<StaticParams::CommonParams::Strategy> strategies;
    std::vector<Point> dots;
};

struct MoveProgramData
{
    std::vector<Point> dots;
};
}

#endif // PROGRAMDATA_H
