#ifndef PROGRAM_MODEL
#define PROGRAM_MODEL
#include "Params.h"
#include "Point.h"
#include "../database/program.h"
#include "utility.h"


template <Type type>
struct StaticProgramData
{
    std::vector<typename StaticParams::CommonParams::Strategy> strategies;
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


template <Type type>
struct ProgramDataTraits
{
    typedef StaticProgramData<type> data;
};

template <>
struct ProgramDataTraits<Type::Move>
{
    typedef MoveProgramData data;
};



template <Type type>
struct ProgramModel
{
public:
    long m_id;
    Type m_type;
    QString m_name;
    typename ParamTraits<type>::params m_params;
    typename ProgramDataTraits<type>::data m_data;
    Category m_category;

    ProgramModel()=default;

    ProgramModel(Program_ptr pp)
    {
        m_id=pp->m_id;
        m_type=static_cast<Type>(pp->m_type);
        m_name=pp->m_name;
        m_params=Utility::QStringToEntity<ParamTraits<type>::params>(pp->m_params);
        m_data=Utility::QStringToEntity<ProgramDataTraits<type>::data>(pp->m_data);
        m_category=static_cast<Category>(pp->m_category);
    }

    Program_ptr ModelToDB()
    {
        auto pp=Program_ptr(new Program());
        pp->m_id=m_id;
        pp->m_type=static_cast<int>(m_type);
        pp->m_name=m_name;
        pp->m_params=Utility::entityToQString(m_params);
        pp->m_data=Utility::entityToQString(m_data);
        pp->m_category=static_cast<int>(m_category);
        return pp;
    }
};

#endif // PROGRAM_H

