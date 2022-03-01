#ifndef PROGRAMDATA_H
#define PROGRAMDATA_H
#include "Params.h"
#include <tuple>
#include <array>
#include <vector>
#include "Point.h"

namespace Model{
struct StaticProgramData
{
    std::vector<StaticParams::CommonParams::Strategy> strategies;
    std::vector<Point> dots;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(strategies);
        archive & BOOST_SERIALIZATION_NVP(dots);
    }

};

struct MoveProgramData
{
    std::vector<Point> dots;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(dots);
    }
};
}

#endif // PROGRAMDATA_H
