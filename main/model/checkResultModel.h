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

template<Type T>
struct CheckDataTraits
{
    typedef std::vector<int> checkData;
};


template<>
struct CheckDataTraits<Type::Move>
{
    typedef std::vector<Point> checkData;
};


template<Type T>
struct ResultData
{
    int alarm;
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
    std::vector<int> shortTermFluctuation;
    std::vector<int> eyeMoveCurve;
    typename CheckDataTraits<T>::checkData m_data;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(alarm);
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
        archive & BOOST_SERIALIZATION_NVP(shortTermFluctuation);
        archive & BOOST_SERIALIZATION_NVP(eyeMoveCurve);
    }
};



template<Type type>
struct CheckResultModel
{
    long m_id;
    Type m_type;
    typename ParamTraits<type>::params m_params;
    ResultData<type> m_data;
    QDateTime m_time;
    Patient m_patient;
    ProgramModel<type> m_programModel;


    CheckResultModel()=default;

    CheckResultModel(CheckResult_ptr checkResult_ptr)
    {
        m_id=checkResult_ptr->m_id;
        m_type=static_cast<Type>(checkResult_ptr->m_type);
        m_params=Utility::QStringToEntity<ParamTraits<type>::params>(checkResult_ptr->m_params);
        m_data=Utility::QStringToEntity<ResultData<type>>(checkResult_ptr->m_data);
        m_time=checkResult_ptr->m_time;
        m_patient=*(checkResult_ptr->m_patient);
        m_programModel=ProgramModel<type>(checkResult_ptr->m_program);
    }

    CheckResult_ptr ModelToDB(Patient_ptr patient_ptr,Program_ptr program_ptr)
    {
        auto checkResult_ptr=CheckResult_ptr(new CheckResult());
        checkResult_ptr->m_id=m_id;
        checkResult_ptr->m_type=static_cast<int>(m_type);
        checkResult_ptr->m_params=Utility::entityToQString(m_params);
        checkResult_ptr->m_data=Utility::entityToQString(m_data);
        checkResult_ptr->m_time=m_time;
        checkResult_ptr->m_patient=patient_ptr;
        checkResult_ptr->m_program=program_ptr;
        return checkResult_ptr;
    }



};
}





#endif // CHECKRESULTMODEL_H
