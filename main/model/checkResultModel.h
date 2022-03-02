#ifndef CHECKRESULTMODEL_H
#define CHECKRESULTMODEL_H
#include "Params.h"
#include "Point.h"
#include <QDate>
#include "../database/checkResult.h"
#include "../database/patient.h"
#include "../database/program.h"
#include "programModel.h"
#include "utility.h"


namespace Model{
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
    std::vector<int> eyeMoveCurve;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(alarm);
        archive & BOOST_SERIALIZATION_NVP(DB);
        archive & BOOST_SERIALIZATION_NVP(shortTermFluctuation);
        archive & BOOST_SERIALIZATION_NVP(falsePositiveCount);
        archive & BOOST_SERIALIZATION_NVP(falsePositiveTestCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeTestCount);
        archive & BOOST_SERIALIZATION_NVP(stimulusCount);
        archive & BOOST_SERIALIZATION_NVP(thresholdCount);
        archive & BOOST_SERIALIZATION_NVP(testTimespan);
        archive & BOOST_SERIALIZATION_NVP(ambientLight);
        archive & BOOST_SERIALIZATION_NVP(E_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(T_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(eyeMoveCurve);
    }
};

struct ResultDataDB:ResultData
{
    std::vector<int> DbVal;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(DbVal);
    }
};

struct ResultDataLoc:ResultData
{
    std::vector<Point> LocVal;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(LocVal);
    }
};

template <Type T>
struct CheckResultDataTraits
{
    typedef ResultDataDB data;
};

template <>
struct CheckResultDataTraits<Type::Move>
{
    typedef ResultDataLoc data;
};

template<Type type>
class CheckResultModel
{
    long m_id;
    Type m_type;
    typename ParamTraits<type>::params m_params;
    typename CheckResultDataTraits<type>::data m_data;
    QDateTime m_time;
    Patient m_patient;
    ProgramModel<type> m_programModel;

    CheckResultModel()=default;

    CheckResultModel(CheckResult_ptr cp)
    {
        m_id=cp->m_id;
        m_type=cp->m_type;
        m_params=Utility::entityToQString<ParamTraits<type>::params>(cp->m_params);
        m_data=Utility::entityToQString<CheckResultDataTraits<type>::data>(cp->m_data);
        m_time=cp->m_time;
        m_patient=*(cp->m_patient);
        m_programModel=ProgramModel<type>(cp->m_program);
    }
};
}





#endif // CHECKRESULTMODEL_H
