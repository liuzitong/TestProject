#ifndef PROGRAM_MODEL
#define PROGRAM_MODEL
#include "Params.h"
#include "programData.h"
#include "../database/program.h"
#include <string>
namespace Model{

template <Program::Type T>
struct ProgramTraits{};

template <>
struct ProgramTraits<Program::Type::ThreshHold>
{
    typedef StaticParams params;
    typedef StaticProgramData data;
};

template <>
struct ProgramTraits<Program::Type::Screening>
{
    typedef StaticParams params;
    typedef StaticProgramData data;
};

template <>
struct ProgramTraits<Program::Type::Move>
{
    typedef MoveParams params;
    typedef MoveProgramData data;
};


template <Program::Type type>
struct ProgramModel
{
public:

    typename ProgramTraits<type>::params params;
    typename ProgramTraits<type>::data data;
    Program::Category category;
};

}
#endif // PROGRAM_H

