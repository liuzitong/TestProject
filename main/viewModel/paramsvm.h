#ifndef PARAMSVM_H
#define PARAMSVM_H
#include <QObject>
#include "../model/Params.h"
#include <QArrayData>


class CommonParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> Range READ getRange WRITE setRange)
    Q_PROPERTY(int strategy READ getStrategy WRITE setStrategy)
//    Q_PROPERTY(int strategyMode READ getStrategyMode WRITE setStrategyMode)
//    Q_PROPERTY(int cursorColor READ getCursorColor WRITE setCursorColor)
//    Q_PROPERTY(int backGroundColor READ getBackGroundColor WRITE setBackGroundColor)
//    Q_PROPERTY(bool cyanYellowTest READ getCyanYellowTest WRITE setCyanYellowTest)
//    Q_PROPERTY(bool responseAutoAdapt READ getResponseAutoAdapt WRITE setResponseAutoAdapt)
//    Q_PROPERTY(int intervalTime READ getIntevalTime WRITE setIntevalTime)
//    Q_PROPERTY(bool centerDotCheck READ getCenterDotCheck WRITE setCenterDockCheck)
//    Q_PROPERTY(bool shortTermFluctuation READ getShortTermFluctuation WRITE setShortTermFluctuation)
//    Q_PROPERTY(bool fixationViewSelection READ getFixationViewSelection WRITE setFixationViewSelection)
//    Q_PROPERTY(bool eyeMoveAlarmMode READ getEyeMoveAlarmMode WRITE setEyeMoveAlarmMode)
//    Q_PROPERTY(bool blindDotTest READ getBlindDotTest WRITE setBlindDotTest)
public:
    CommonParamsVM(StaticParams::CommonParams& commonParams){m_commomParams=commonParams;}
    QList<int> getRange(){return QList<int>{m_commomParams.Range[0],m_commomParams.Range[1]};} void setRange(QList<int> value){m_commomParams.Range[0]=value[0],m_commomParams.Range[1]=value[1];}
    int getStrategy(){return int(m_commomParams.strategy);}void setStrategy(int value){m_commomParams.strategy=StaticParams::CommonParams::Strategy(value);};
//        int getStrategyMode();void setStrategyMode(int value);
//        int getCursorColor();void setCursorColor(int value);
private:
    StaticParams::CommonParams m_commomParams;
};

class FixedParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(int stimulationTime READ getStimulationTime WRITE setStimulationTime)
//    Q_PROPERTY(int intervalTime READ getIntervalTime WRITE setIntervalTime)
public:
    FixedParamsVM(StaticParams::FixedParams& fixedParams){m_fixedParams=fixedParams;}
    int getStimulationTime(){return m_fixedParams.stimulationTime;}void setStimulationTime(int value){m_fixedParams.stimulationTime=value;}
private:
    StaticParams::FixedParams m_fixedParams;
};

class StaticParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* commonParams READ getCommonParams WRITE setCommonParams)
    Q_PROPERTY(QObject* fixedParams READ getFixedParams WRITE setFixedParams)
public:
    StaticParamsVM(StaticParams* staticParams)
    {
        m_staticParams=staticParams;
        m_commonParams=new CommonParamsVM(staticParams->commonParams);
        m_fixedParams=new FixedParamsVM(staticParams->fixedParams);
    }
    QObject* getCommonParams(){return m_commonParams;}void setCommonParams(QObject* value){m_commonParams=value;}
    QObject* getFixedParams(){return m_fixedParams;};void setFixedParams(QObject* value){m_fixedParams=value;};
    ~StaticParamsVM(){delete m_commonParams;delete m_fixedParams;delete m_staticParams;}
private:
    StaticParams* m_staticParams;
    QObject* m_commonParams;
    QObject* m_fixedParams;
};



#endif // PARAMSVM_H
