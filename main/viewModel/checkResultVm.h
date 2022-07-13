#ifndef CHECKRESULTVM_H
#define CHECKRESULTVM_H
#include <QObject>
#include <perimeter/main/model/checkResultModel.h>
#include <perimeter/main/viewModel/paramsVm.h>
namespace Perimeter
{
class ResultDataVm;
class CheckResultVm: public QObject
{

    Q_OBJECT
    Q_PROPERTY(long id READ getID WRITE setID)
    Q_PROPERTY(int type READ getType WRITE setType)
    Q_PROPERTY(int OS_OD READ getOS_OD WRITE setOS_OD)
    Q_PROPERTY(QDateTime time READ getTime WRITE setTime)
    Q_PROPERTY(QObject* params READ getParams WRITE setParams )
    Q_PROPERTY(ResultDataVm* resultData READ getResultData)

public:
    Q_INVOKABLE explicit CheckResultVm(const QVariantList &);
    Q_INVOKABLE virtual ~CheckResultVm();
    Q_INVOKABLE void insertCheckResult();
    int getID(){return m_id;}void setID(int value){m_id=value;}
    int getType(){return m_type;}void setType(int value){m_type=value;}
    int getOS_OD(){return m_OS_OD;}void setOS_OD(int value){m_OS_OD=value;}
    QDateTime getTime(){return m_time;}void setTime(QDateTime value){m_time=value;}
    QObject* getParams(){return m_params;}void setParams(QObject* value){
        if(m_type==0)
        {
            auto val=static_cast<StaticParamsVM*>(value);
            auto m_val=static_cast<StaticParamsVM*>(m_params);
            *m_val=*val;
        }
        else if(m_type==1){
            auto val=static_cast<MoveParamsVM*>(value);
            auto m_val=static_cast<MoveParamsVM*>(m_params);
            *m_val=*val;
        }
    }
    ResultDataVm* getResultData(){return m_resultData;}

private:
    CheckResult_ptr getCheckResultData();
    long m_id;
    int m_type;
    int m_OS_OD;
    QVector<QVector<QByteArray>> m_pic;      //算了 图片还是用方法调用
    QDateTime m_time;
    QObject* m_params;
    ResultDataVm* m_resultData;
    int m_patient_id;
    int m_program_id;
};

class ResultDataVm:public QObject
{
    Q_OBJECT
    Q_PROPERTY(int alarm READ getAlarm WRITE setAlarm)
    Q_PROPERTY(int falsePositiveCount READ getFalsePositiveCount WRITE setFalsePositiveCount)
    Q_PROPERTY(int falsePositiveTestCount READ getFalsePositiveTestCount WRITE setFalsePositiveTestCount)
    Q_PROPERTY(int falseNegativeCount READ getFalseNegativeCount WRITE setFalseNegativeCount)
    Q_PROPERTY(int falseNegativeTestCount READ getFalseNegativeTestCount WRITE setFalseNegativeTestCount)
    Q_PROPERTY(int fixationLostCount READ getFixationLostCount WRITE setFixationLostCount)
    Q_PROPERTY(int fixationLostTestCount READ getFixationLostTestCount WRITE setFixationLostTestCount)
    Q_PROPERTY(int stimulusCount READ getStimulusCount WRITE setStimulusCount)
    Q_PROPERTY(int thresholdCount READ getThresholdCount WRITE setThresholdCount)
    Q_PROPERTY(int testTimespan READ getTestTimespan WRITE setTestTimespan)
    Q_PROPERTY(int ambientLight READ getAmbientLight WRITE setAmbientLight)
    Q_PROPERTY(int E_Light_pv READ getE_Light_pv WRITE setE_Light_pv)
    Q_PROPERTY(int T_Light_pv READ getT_Light_pv WRITE setT_Light_pv)
    Q_PROPERTY(QVariantList fixationDeviation READ getFixationDeviation WRITE setFixationDeviation)
    Q_PROPERTY(QVariantList checkData READ getCheckData WRITE setCheckData)


public:
    Q_INVOKABLE explicit ResultDataVm()=default;
    explicit ResultDataVm(ResultData<Type::ThreshHold> resultData);
    explicit ResultDataVm(ResultData<Type::Move> resultData);
    Q_INVOKABLE virtual ~ResultDataVm()=default;
    int getAlarm(){return m_alarm;}void setAlarm(int value){m_alarm=value;}
    int getFalsePositiveCount(){return m_falsePositiveCount;}void setFalsePositiveCount(int value){m_falsePositiveCount=value;}
    int getFalsePositiveTestCount(){return m_falseNegativeTestCount;}void setFalsePositiveTestCount(int value){m_falsePositiveTestCount=value;}
    int getFalseNegativeCount(){return m_falseNegativeCount;}void setFalseNegativeCount(int value){m_falseNegativeCount=value;}
    int getFalseNegativeTestCount(){return m_falseNegativeTestCount;}void setFalseNegativeTestCount(int value){m_falseNegativeTestCount=value;}
    int getFixationLostCount(){return m_fixationLostCount;}void setFixationLostCount(int value){m_fixationLostCount=value;}
    int getFixationLostTestCount(){return m_fixationLostTestCount;}void setFixationLostTestCount(int value){m_fixationLostTestCount=value;}
    int getStimulusCount(){return m_stimulusCount;}void setStimulusCount(int value){m_stimulusCount=value;}
    int getThresholdCount(){return m_thresholdCount;}void setThresholdCount(int value){m_thresholdCount=value;}
    int getTestTimespan(){return m_testTimespan;}void setTestTimespan(int value){m_testTimespan=value;}
    int getAmbientLight(){return m_ambientLight;}void setAmbientLight(int value){m_ambientLight=value;}
    int getE_Light_pv(){return m_E_Light_pv;}void setE_Light_pv(int value){m_E_Light_pv=value;}
    int getT_Light_pv(){return m_T_Light_pv;}void setT_Light_pv(int value){m_T_Light_pv=value;}
    QVariantList getFixationDeviation(){return m_fixationDeviation;}void setFixationDeviation(QVariantList value){m_fixationDeviation=value;}
    QVariantList getCheckData(){return m_checkData;}void setCheckData(QVariantList value){m_checkData=value;}
    ResultData<Type::ThreshHold> getThresholdData();
    ResultData<Type::Move> getMoveData();
private:
    int m_alarm;
    int m_falsePositiveCount;
    int m_falsePositiveTestCount;
    int m_falseNegativeCount;
    int m_falseNegativeTestCount;
    int m_fixationLostCount;
    int m_fixationLostTestCount;
    int m_stimulusCount;
    int m_thresholdCount;
    int m_testTimespan;
    int m_ambientLight;
    int m_E_Light_pv;
    int m_T_Light_pv;
    QVariantList m_fixationDeviation;
    QVariantList m_checkData;       //非移动时为int 移动时是QPoint
};




}
#endif // CHECKRESULTVM_H
