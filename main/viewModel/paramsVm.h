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
    Q_PROPERTY(int strategyMode READ getStrategyMode WRITE setStrategyMode)
    Q_PROPERTY(int cursorColor READ getCursorColor WRITE setCursorColor)
    Q_PROPERTY(int cursorSize READ getCursorSize WRITE setCursorSize)
    Q_PROPERTY(int backGroundColor READ getBackGroundColor WRITE setBackGroundColor)
    Q_PROPERTY(bool cyanYellowTest READ getCyanYellowTest WRITE setCyanYellowTest)
    Q_PROPERTY(bool responseAutoAdapt READ getResponseAutoAdapt WRITE setResponseAutoAdapt)
    Q_PROPERTY(int intervalTime READ getIntervalTime WRITE setIntervalTime)
    Q_PROPERTY(bool centerDotCheck READ getCenterDotCheck WRITE setCenterDotCheck)
    Q_PROPERTY(bool shortTermFluctuation READ getShortTermFluctuation WRITE setShortTermFluctuation)
    Q_PROPERTY(int fixationTarget READ getFixationTarget WRITE setFixationTarget)
    Q_PROPERTY(int fixationMonitor READ getFixationMonitor WRITE setFixationMonitor)
    Q_PROPERTY(bool blindDotTest READ getBlindDotTest WRITE setBlindDotTest)
public:
    CommonParamsVM()=default;
    CommonParamsVM(StaticParams::CommonParams& commonParams){m_commomParams=commonParams;}
    CommonParamsVM& operator=(CommonParamsVM& other)
    {
        m_commomParams=other.m_commomParams;
        return *this;
    }
    StaticParams::CommonParams getData(){return m_commomParams;}
    QList<int> getRange(){return QList<int>{m_commomParams.Range[0],m_commomParams.Range[1]};} void setRange(QList<int> value){m_commomParams.Range[0]=value[0],m_commomParams.Range[1]=value[1];}
    int getStrategy(){return int(m_commomParams.strategy);}void setStrategy(int value){m_commomParams.strategy=StaticParams::CommonParams::Strategy(value);};
    int getStrategyMode(){return int(m_commomParams.strategyMode);}void setStrategyMode(int value){m_commomParams.strategyMode=StaticParams::CommonParams::StrategyMode(value);}
    int getCursorColor(){return int(m_commomParams.cursorColor);}void setCursorColor(int value){m_commomParams.cursorColor=CursorColor(value);}
    int getCursorSize(){return int(m_commomParams.cursorSize);}void setCursorSize(int value){m_commomParams.cursorSize=CursorSize(value);}
    int getBackGroundColor(){return int(m_commomParams.backGroundColor);}void setBackGroundColor(int value){m_commomParams.backGroundColor=BackGroundColor(value);}
    bool getCyanYellowTest(){return m_commomParams.cyanYellowTest;}void setCyanYellowTest(bool value){m_commomParams.cyanYellowTest=value;}
    bool getResponseAutoAdapt(){return m_commomParams.responseAutoAdapt;}void setResponseAutoAdapt(bool value){m_commomParams.responseAutoAdapt=value;}
    int getIntervalTime(){return m_commomParams.intervalTime;}void setIntervalTime(int value){m_commomParams.intervalTime=value;}
    bool getCenterDotCheck(){return m_commomParams.centerDotCheck;}void setCenterDotCheck(bool value){m_commomParams.centerDotCheck=value;}
    bool getShortTermFluctuation(){return m_commomParams.shortTermFluctuation;}void setShortTermFluctuation(bool value){m_commomParams.shortTermFluctuation=value;}
    int getFixationTarget(){return int(m_commomParams.fixationTarget);}void setFixationTarget(int value){m_commomParams.fixationTarget=FixationTarget(value);}
    int getFixationMonitor(){return int(m_commomParams.fixationMonitor);}void setFixationMonitor(bool value){m_commomParams.fixationMonitor=FixationMonitor(value);}
    bool getBlindDotTest(){return m_commomParams.blindDotTest;}void setBlindDotTest(bool value){m_commomParams.blindDotTest=value;}
private:
    StaticParams::CommonParams m_commomParams;
};

class FixedParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(int stimulationTime READ getStimulationTime WRITE setStimulationTime)
    Q_PROPERTY(int intervalTime READ getIntervalTime WRITE setIntervalTime)
    Q_PROPERTY(int falsePositiveCycle READ getFalsePositiveCycle WRITE setFalsePositiveCycle)
    Q_PROPERTY(int falseNegativeCycle READ getFalseNegativeCycle WRITE setFalseNegativeCycle)
    Q_PROPERTY(int fixationViewLossCycle READ getFixationViewLossCycle WRITE setFixationViewLossCycle)
    Q_PROPERTY(int singleStimulationDB READ getSingleStimulationDB WRITE setSingleStimulationDB)
    Q_PROPERTY(int blindDotStimulationDB READ getBlindDotStimulationDB WRITE setBlindDotStimulationDB)
    Q_PROPERTY(int shortTermFluctuationCount READ getShortTermFluctuationCount WRITE setShortTermFluctuationCount)
public:
    FixedParamsVM()=default;
    FixedParamsVM(StaticParams::FixedParams& fixedParams){m_fixedParams=fixedParams;}
    FixedParamsVM& operator=(FixedParamsVM& other)
    {
        m_fixedParams=other.m_fixedParams;
        return *this;
    }
    StaticParams::FixedParams getData(){return m_fixedParams;}
    int getStimulationTime(){return m_fixedParams.stimulationTime;}void setStimulationTime(int value){m_fixedParams.stimulationTime=value;}
    int getIntervalTime(){return m_fixedParams.intervalTime;}void setIntervalTime(int value){m_fixedParams.intervalTime=value;}
    int getFalsePositiveCycle(){return m_fixedParams.falsePositiveCycle;}void setFalsePositiveCycle(int value){m_fixedParams.falsePositiveCycle=value;}
    int getFalseNegativeCycle(){return m_fixedParams.falseNegativeCycle;}void setFalseNegativeCycle(int value){m_fixedParams.falseNegativeCycle=value;}
    int getFixationViewLossCycle(){return m_fixedParams.fixationViewLossCycle;}void setFixationViewLossCycle(int value){m_fixedParams.fixationViewLossCycle=value;}
    int getSingleStimulationDB(){return m_fixedParams.singleStimulationDB;}void setSingleStimulationDB(int value){m_fixedParams.singleStimulationDB=value;}
    int getBlindDotStimulationDB(){return m_fixedParams.blindDotStimulationDB;}void setBlindDotStimulationDB(int value){m_fixedParams.blindDotStimulationDB=value;}
    int getShortTermFluctuationCount(){return m_fixedParams.shortTermFluctuationCount;}void setShortTermFluctuationCount(int value){m_fixedParams.shortTermFluctuationCount=value;}
private:
    StaticParams::FixedParams m_fixedParams;
};

class StaticParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* commonParams READ getCommonParams )
    Q_PROPERTY(QObject* fixedParams READ getFixedParams )
public:
    StaticParamsVM()
    {
//        m_staticParams=new StaticParams();
        m_commonParamsVM=new CommonParamsVM();
        m_fixedParamsVM=new FixedParamsVM();
    }
    StaticParamsVM(StaticParams& staticParams)
    {
//        m_staticParams=staticParams;
        m_commonParamsVM=new CommonParamsVM(staticParams.commonParams);
        m_fixedParamsVM=new FixedParamsVM(staticParams.fixedParams);
    }
    StaticParamsVM& operator =(StaticParamsVM& other){
        *m_commonParamsVM=*(other.m_commonParamsVM);
        *m_fixedParamsVM=*(other.m_fixedParamsVM);
        return *this;
    }
    CommonParamsVM* getCommonParams(){return m_commonParamsVM;}/*void setCommonParams(QObject* value){m_commonParams=value;}*/
    FixedParamsVM* getFixedParams(){return m_fixedParamsVM;};/*void setFixedParams(QObject* value){m_fixedParams=value;};*/
    ~StaticParamsVM(){delete m_commonParamsVM;delete m_fixedParamsVM;/*delete m_staticParams;*/}
private:
//    StaticParams* m_staticParams;
    CommonParamsVM* m_commonParamsVM;
    FixedParamsVM* m_fixedParamsVM;
};

class MoveParamsVM:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<int> Range READ getRange WRITE setRange)
    Q_PROPERTY(int strategy READ getStrategy WRITE setStrategy)
    Q_PROPERTY(int cursorColor READ getCursorColor WRITE setCursorColor)
    Q_PROPERTY(int cursorSize READ getCursorSize WRITE setCursorSize)
    Q_PROPERTY(int backGroundColor READ getBackGroundColor WRITE setBackGroundColor)
//    Q_PROPERTY(int fixationTarget READ getFixationTarget WRITE setFixationTarget)
    Q_PROPERTY(int speed READ getSpeed WRITE setSpeed)
    Q_PROPERTY(int brightness READ getBrightness WRITE setBrightness)
    Q_PROPERTY(int moveMethod READ getMoveMethod WRITE setMoveMethod)
    Q_PROPERTY(int moveDistance READ getMoveDistance WRITE setMoveDistance)
    Q_PROPERTY(int fixationMonitor READ getFixationMonitor WRITE setFixationMonitor)

public:
    MoveParamsVM()=default;
    MoveParamsVM(MoveParams& moveParams){m_moveParams=moveParams;}
    MoveParamsVM& operator=(MoveParamsVM& other){m_moveParams=other.m_moveParams;return *this;}
    MoveParams getData(){return m_moveParams;}
    QList<int> getRange(){return QList<int>{m_moveParams.Range[0],m_moveParams.Range[1]};}void setRange(QList<int> value){m_moveParams.Range[0]=value[0];m_moveParams.Range[1]=value[1];}
    int getStrategy(){return int(m_moveParams.strategy);}void setStrategy(int value){m_moveParams.strategy=MoveParams::Strategy(value);}
    int getCursorColor(){return int(m_moveParams.cursorColor);}void setCursorColor(int value){m_moveParams.cursorColor=CursorColor(value);}
    int getCursorSize(){return int(m_moveParams.cursorSize);}void setCursorSize(int value){m_moveParams.cursorSize=CursorSize(value);}
    int getBackGroundColor(){return int(m_moveParams.backGroundColor);}void setBackGroundColor(int value){m_moveParams.backGroundColor=BackGroundColor(value);}
//    int getFixationTarget(){return int(m_moveParams->fixationTarget);}void setFixationTarget(int value){m_moveParams->fixationTarget=FixationTarget(value);}
    int getSpeed(){return m_moveParams.speed;}void setSpeed(float value){m_moveParams.speed=value;}
    int getBrightness(){return m_moveParams.brightness;}void setBrightness(int value){m_moveParams.brightness=value;}
    int getMoveMethod(){return int(m_moveParams.moveMethod);}void setMoveMethod(int value){m_moveParams.moveMethod=MoveParams::MoveMethod(value);}
    int getMoveDistance(){return int(m_moveParams.moveDistance);}void setMoveDistance(int value){m_moveParams.moveDistance=MoveParams::MoveDistance(value);}
    int getFixationMonitor(){return int(m_moveParams.fixationMonitor);}void setFixationMonitor(int value){m_moveParams.fixationMonitor=FixationMonitor(value);}
private:
    MoveParams m_moveParams;
};

#endif // PARAMSVM_H
