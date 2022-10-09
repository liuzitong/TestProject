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


template<Type T>
struct CheckDataTraits
{
    typedef std::vector<int> checkData;
};

template<>
struct CheckDataTraits<Type::Dynamic>
{
    typedef std::vector<Point> checkData;
};


struct ResultData1
{
    int alarm;
    float pupilDiameter;
    int testTimespan;
    int ambientLight;
    int E_Light_pv;
    int T_Light_pv;
    std::vector<int> fixationDeviation;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(alarm);
        archive & BOOST_SERIALIZATION_NVP(pupilDiameter);
        archive & BOOST_SERIALIZATION_NVP(testTimespan);
        archive & BOOST_SERIALIZATION_NVP(ambientLight);
        archive & BOOST_SERIALIZATION_NVP(E_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(T_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(fixationDeviation);
    }
};

struct StaticResultData:public ResultData1
{

    int falsePositiveCount;
    int falsePositiveTestCount;
    int falseNegativeCount;
    int falseNegativeTestCount;
    int fixationLostCount;
    int fixationLostTestCount;
    std::vector<int> checkData;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(falsePositiveCount);
        archive & BOOST_SERIALIZATION_NVP(falsePositiveTestCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeTestCount);
        archive & BOOST_SERIALIZATION_NVP(fixationLostCount);
        archive & BOOST_SERIALIZATION_NVP(fixationLostTestCount);
        archive & BOOST_SERIALIZATION_NVP(fixationDeviation);
        archive & BOOST_SERIALIZATION_NVP(checkData);
    }
};

struct DynamicDataNode
{
    std::string name;
    Point start;
    Point end;
    bool isSeen;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(name);
        archive & BOOST_SERIALIZATION_NVP(start);
        archive & BOOST_SERIALIZATION_NVP(end);
        archive & BOOST_SERIALIZATION_NVP(isSeen);
    }
};


struct DynamicResultData:public ResultData1
{
    std::vector<DynamicDataNode> checkData;
    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(checkData);
    }
};


template<Type T>
struct ResultData
{
    int alarm;
    float pupilDiameter;
    int falsePositiveCount;
    int falsePositiveTestCount;
    int falseNegativeCount;
    int falseNegativeTestCount;
    int fixationLostCount;
    int fixationLostTestCount;
    int stimulusCount;
    int thresholdCount;
    int testTimespan;
    int ambientLight;
    int E_Light_pv;
    int T_Light_pv;
    std::vector<int> fixationDeviation;
    typename CheckDataTraits<T>::checkData checkdata;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(alarm);
        archive & BOOST_SERIALIZATION_NVP(pupilDiameter);
        archive & BOOST_SERIALIZATION_NVP(falsePositiveCount);
        archive & BOOST_SERIALIZATION_NVP(falsePositiveTestCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeCount);
        archive & BOOST_SERIALIZATION_NVP(falseNegativeTestCount);
        archive & BOOST_SERIALIZATION_NVP(fixationLostCount);
        archive & BOOST_SERIALIZATION_NVP(fixationLostTestCount);
        archive & BOOST_SERIALIZATION_NVP(stimulusCount);
        archive & BOOST_SERIALIZATION_NVP(thresholdCount);
        archive & BOOST_SERIALIZATION_NVP(testTimespan);
        archive & BOOST_SERIALIZATION_NVP(ambientLight);
        archive & BOOST_SERIALIZATION_NVP(E_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(T_Light_pv);
        archive & BOOST_SERIALIZATION_NVP(fixationDeviation);
        archive & BOOST_SERIALIZATION_NVP(checkdata);
    }
};



template<Type type>
struct CheckResultModel
{
    long m_id;
    Type m_type;
    int m_OS_OD;
    typename ParamTraits<type>::params m_params;
    ResultData<type> m_data;
    std::vector<std::vector<std::vector<std::string>>> m_pic;     //[点序号][每个点图片序号][1为DB大小,2为图片]
    QString m_diagnosis;
    QDateTime m_time;
    int m_patient_id;
    int m_program_id;


    CheckResultModel()=default;

    CheckResultModel(CheckResult_ptr checkResult_ptr)
    {
        m_id=checkResult_ptr->m_id;
        m_type=static_cast<Type>(checkResult_ptr->m_type);
        m_OS_OD=checkResult_ptr->m_OS_OD;
        m_params=Utility::QStringToEntity<ParamTraits<type>::params>(checkResult_ptr->m_params);
        qDebug()<<checkResult_ptr->m_data;
        m_data=Utility::QStringToEntity<ResultData<type>>(checkResult_ptr->m_data);
        qDebug()<<checkResult_ptr->m_pic;
        m_pic=Utility::QStringToEntity<std::vector<std::vector<std::vector<std::string>>>>(checkResult_ptr->m_pic);
//        qDebug()<<checkResult_ptr->m_data;
//        qDebug()<<m_data.pupilDiameter;
        m_diagnosis=checkResult_ptr->m_diagnosis;
        m_time=checkResult_ptr->m_time;
        m_patient_id=checkResult_ptr->m_patient->m_id;
        m_program_id=checkResult_ptr->m_program->m_id;
    }

    CheckResult_ptr ModelToDB()
    {
        auto checkResult_ptr=CheckResult_ptr(new CheckResult());
        checkResult_ptr->m_patient=Patient_ptr(new Patient());
        checkResult_ptr->m_program=Program_ptr(new Program());
        checkResult_ptr->m_OS_OD=m_OS_OD;
        checkResult_ptr->m_id=m_id;
        checkResult_ptr->m_type=static_cast<int>(m_type);
        checkResult_ptr->m_params=Utility::entityToQString(m_params);
        qDebug()<<checkResult_ptr->m_params;
        checkResult_ptr->m_data=Utility::entityToQString(m_data);
        checkResult_ptr->m_pic=Utility::entityToQString(m_pic);
        qDebug()<<checkResult_ptr->m_data;
        checkResult_ptr->m_time=m_time;
        checkResult_ptr->m_diagnosis=m_diagnosis;
        checkResult_ptr->m_patient->m_id=m_patient_id;
        checkResult_ptr->m_program->m_id=m_program_id;
        return checkResult_ptr;
    }

};






#endif // CHECKRESULTMODEL_H
