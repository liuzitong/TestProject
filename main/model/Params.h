#ifndef PARAMS_H
#define PARAMS_H

#define OPTION int
#define ON_OFF bool
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <utility>
#include "Point.h"
#include <QString>
#include <sstream>
#include <exception>
#include <iostream>
#include <QObject>


enum class CursorColor{white,red,blue};
enum class CursorSize{I,II,III,IV,V};
enum class BackGroundColor{white,yellow};
enum class FixationViewSelection{centerPoint,smallDiamond,bigDiamond,bottomPoint};
enum class EyeMoveAlarmMode{dontAlarm,onlyAlarm,alarmAndPause};
enum class Type { ThreshHold=0, Screening,Move  };
enum class Category { ThreshHold=0, Screening,Special,Move,Custom  };


struct StaticParams/*:public QObject*/
{
//    Q_OBJECT
//    Q_PROPERTY(QObject commonParams READ getCommonParams WRITE setCommonParams)
//    Q_PROPERTY(QObject patientId READ getPatientID WRITE setPatientID)
    struct CommonParams/*:public QObject*/
    {
        enum class Strategy{fullThreshold,smartInteractive,fastInterative,oneStage,twoStages,quantifyDefects,singleStimulation};
//        Q_OBJECT
        enum class StrategyMode{ageRelated,ThresholdRelated};
        int                     Range[2];                   ON_OFF                      responseAutoAdapt;
        /*int                     DotCount;*/               int                         intervalTime;
        Strategy                strategy;                   ON_OFF                      centerDotCheck;
        StrategyMode            strategyMode;               ON_OFF                      shortTermFluctuation;
        CursorColor             cursorColor;                FixationViewSelection       fixationViewSelection;
        CursorSize              cursorSize;                 EyeMoveAlarmMode            eyeMoveAlarmMode;
        BackGroundColor         backGroundColor;            ON_OFF                      blindDotTest;
        ON_OFF                  cyanYellowTest;

        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(Range);
            archive & BOOST_SERIALIZATION_NVP(strategy);
            archive & BOOST_SERIALIZATION_NVP(strategyMode);
            archive & BOOST_SERIALIZATION_NVP(cursorColor);
            archive & BOOST_SERIALIZATION_NVP(cursorSize);
            archive & BOOST_SERIALIZATION_NVP(backGroundColor);
            archive & BOOST_SERIALIZATION_NVP(cyanYellowTest);
            archive & BOOST_SERIALIZATION_NVP(responseAutoAdapt);
            archive & BOOST_SERIALIZATION_NVP(intervalTime);
            archive & BOOST_SERIALIZATION_NVP(centerDotCheck);
            archive & BOOST_SERIALIZATION_NVP(shortTermFluctuation);
            archive & BOOST_SERIALIZATION_NVP(fixationViewSelection);
            archive & BOOST_SERIALIZATION_NVP(eyeMoveAlarmMode);
            archive & BOOST_SERIALIZATION_NVP(blindDotTest);
        }

    };

    struct FixedParams/*:public QObject*/
    {
//        Q_OBJECT

        int stimulationTime;            int singleStimulationDB;
        int intervalTime;               int blindDotStimulationDB;
        int falsePositiveCycle;         int shortTermFluctuationCount;
        int falseNegativeCycle;
        int fixationViewLossCycle;

        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(stimulationTime);
            archive & BOOST_SERIALIZATION_NVP(intervalTime);
            archive & BOOST_SERIALIZATION_NVP(falsePositiveCycle);
            archive & BOOST_SERIALIZATION_NVP(falseNegativeCycle);
            archive & BOOST_SERIALIZATION_NVP(fixationViewLossCycle);
            archive & BOOST_SERIALIZATION_NVP(singleStimulationDB);
            archive & BOOST_SERIALIZATION_NVP(blindDotStimulationDB);
            archive & BOOST_SERIALIZATION_NVP(shortTermFluctuationCount);
        }

    };


    CommonParams commonParams;
    FixedParams  fixedParams;

//    CommonParams& getCommonParams(){return commonParams;}

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(commonParams);
        archive & BOOST_SERIALIZATION_NVP(fixedParams);
    }
};

struct MoveParams
{
    enum class Strategy{standard,blindArea,darkArea,straightLine};
    enum class MoveMethod{_4Lines,_6Lines,_8Lines};
    enum class MoveDistance{_5,_10,_15};
    int                         Range[2];                   FixationViewSelection   fixationViewSelection;
                                                            float                   speed;
    Strategy                    strategy;                   int                     brightness;
    CursorColor                 cursorColor;                MoveMethod              moveMethod;
    CursorSize                  cursorSize;                 MoveDistance            moveDistance;
    BackGroundColor             backGroundColor;            EyeMoveAlarmMode        eyeMoveAlarmMode;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(Range);
        archive & BOOST_SERIALIZATION_NVP(strategy);
        archive & BOOST_SERIALIZATION_NVP(cursorColor);
        archive & BOOST_SERIALIZATION_NVP(cursorSize);
        archive & BOOST_SERIALIZATION_NVP(backGroundColor);
        archive & BOOST_SERIALIZATION_NVP(fixationViewSelection);
        archive & BOOST_SERIALIZATION_NVP(speed);
        archive & BOOST_SERIALIZATION_NVP(brightness);
        archive & BOOST_SERIALIZATION_NVP(moveMethod);
        archive & BOOST_SERIALIZATION_NVP(moveDistance);
        archive & BOOST_SERIALIZATION_NVP(eyeMoveAlarmMode);
    }

//    QString ToQString()
//    {
//        std::stringstream ss;
//        boost::archive::xml_oarchive oa(ss);
//        oa& BOOST_SERIALIZATION_NVP(*this);
//        return ss.str().c_str();
//    }
};

template <Type T>
struct ParamTraits
{
    typedef StaticParams params;
};


template <>
struct ParamTraits<Type::Move>
{
    typedef MoveParams params;
};



#endif // PARAMS_H
