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

namespace Model{
enum class CursorColor{white,red,blue};
enum class BackGroundColor{white,yellow};
enum class FixationViewSelection{centerPoint,smallDiamond,bigDiamond,bottomPoint};
enum class EyeMoveAlarmMode{dontAlarm,onlyAlarm,alarmAndPause};
enum class Type { ThreshHold=0, Screening,Move  };
enum class Category { ThreshHold=0, Screening,Special,Move,Custom  };
enum class Strategy{fullThreshold,smartInteractive,fastInterative,oneStage,twoStages,quantifyDefects,singleStimulation};

struct StaticParams
{
    struct CommonParams
    {
        enum class StrategyMode{ageRelated,ThresholdRelated};
        int                     Range[2];                   ON_OFF                      responseAutoAdapt;
        /*int                     DotCount;*/               int                         intervalTime;
        Strategy                strategy;                   ON_OFF                        centeralDotCheck;
        StrategyMode            strategyMode;               ON_OFF                      shortTermFluctuation;
        CursorColor             cursorColor;                FixationViewSelection       fixationViewSelection;
        BackGroundColor         backGroundColor;            EyeMoveAlarmMode            eyeMoveAlarmMode;
        ON_OFF                  cyanYellowTest;             ON_OFF                      blindDotTest;


        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(Range);
            archive & BOOST_SERIALIZATION_NVP(responseAutoAdapt);
            archive & BOOST_SERIALIZATION_NVP(intervalTime);
            archive & BOOST_SERIALIZATION_NVP(strategy);
            archive & BOOST_SERIALIZATION_NVP(centeralDotCheck);
            archive & BOOST_SERIALIZATION_NVP(strategyMode);
            archive & BOOST_SERIALIZATION_NVP(shortTermFluctuation);
            archive & BOOST_SERIALIZATION_NVP(cursorColor);
            archive & BOOST_SERIALIZATION_NVP(fixationViewSelection);
            archive & BOOST_SERIALIZATION_NVP(fixationViewSelection);
            archive & BOOST_SERIALIZATION_NVP(backGroundColor);
            archive & BOOST_SERIALIZATION_NVP(eyeMoveAlarmMode);
            archive & BOOST_SERIALIZATION_NVP(cyanYellowTest);
            archive & BOOST_SERIALIZATION_NVP(blindDotTest);
        }

    };

    struct FixedParams
    {
        int stimulationTime;            int singleStimulationDB;
        int intervalTime;               int blindDotStimulationDB;
        int falsePositiveCycle;         int shortTermFluctuationCount;
        int falseNegativeCycle;
        int fixationViewLossCycle;

        template<class Archive>
        void serialize(Archive& archive, const unsigned int version)
        {
            archive & BOOST_SERIALIZATION_NVP(stimulationTime);
            archive & BOOST_SERIALIZATION_NVP(singleStimulationDB);
            archive & BOOST_SERIALIZATION_NVP(intervalTime);
            archive & BOOST_SERIALIZATION_NVP(blindDotStimulationDB);
            archive & BOOST_SERIALIZATION_NVP(falsePositiveCycle);
            archive & BOOST_SERIALIZATION_NVP(shortTermFluctuationCount);
            archive & BOOST_SERIALIZATION_NVP(falseNegativeCycle);
            archive & BOOST_SERIALIZATION_NVP(fixationViewLossCycle);
        }

    };



    CommonParams commonParams;
    FixedParams  fixedParams;

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
    enum class CursorColor{white,red,blue};
    enum class CursorSize{I,_II,III,IV,V};
    enum class MoveMethod{_4Lines,_6Lines,_8Lines};
    enum class MoveDistance{_5,_10,_15};
    Point                       Range;                      FixationViewSelection   fixationViewSelection;
                                                            float                   spead;
    Strategy                    strategy;                   int                     brightness;
    CursorColor                 cursorColor;                MoveMethod              moveMethod;
    CursorSize                  cursorSize;                 MoveDistance            moveDistance;
    BackGroundColor             backGroudColor;             EyeMoveAlarmMode        eyeMoveAlarmMode;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(Range);
        archive & BOOST_SERIALIZATION_NVP(strategy);
        archive & BOOST_SERIALIZATION_NVP(cursorColor);
        archive & BOOST_SERIALIZATION_NVP(cursorSize);
        archive & BOOST_SERIALIZATION_NVP(backGroudColor);
        archive & BOOST_SERIALIZATION_NVP(fixationViewSelection);
        archive & BOOST_SERIALIZATION_NVP(spead);
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

}
#endif // PARAMS_H
