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
enum class FixationTarget{centerPoint,smallDiamond,bigDiamond,bottomPoint};
enum class FixationMonitor{dontAlarm,onlyAlarm,alarmAndPause};
enum class Type { ThreshHold=0, Screening,Dynamic  };
enum class Category { ThreshHold=0, Screening,Special,Dynamic,Custom  };


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

        int                         Range[2];
        Strategy                    strategy;
        StrategyMode                strategyMode;
        CursorColor                 cursorColor;
        CursorSize                  cursorSize;
        BackGroundColor             backGroundColor;

        ON_OFF                      cyanYellowTest;
        ON_OFF                      responseAutoAdapt;
        int                         responseDelayTime;
        ON_OFF                      centerDotCheck;
        ON_OFF                      shortTermFluctuation;
        FixationTarget              fixationTarget;
        FixationMonitor             fixationMonitor;                //即是eye move alarm mode
        ON_OFF                      blindDotTest;

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
            archive & BOOST_SERIALIZATION_NVP(responseDelayTime);
            archive & BOOST_SERIALIZATION_NVP(centerDotCheck);
            archive & BOOST_SERIALIZATION_NVP(shortTermFluctuation);
            archive & BOOST_SERIALIZATION_NVP(fixationTarget);
            archive & BOOST_SERIALIZATION_NVP(fixationMonitor);
            archive & BOOST_SERIALIZATION_NVP(blindDotTest);
        }

    };

    struct FixedParams/*:public QObject*/
    {
//        Q_OBJECT

        int stimulationTime;
        int intervalTime;
        int falsePositiveCycle;
        int falseNegativeCycle;
        int fixationViewLossCycle;

        int singleStimulationDB;
        int blindDotStimulationDB;
        int shortTermFluctuationCount;

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

struct DynamicParams
{
    enum class Strategy{standard,blindArea,darkArea,straightLine};
    enum class DynamicMethod{_4Lines,_6Lines,_8Lines};
    enum class DynamicDistance{_5,_10,_15};
    int                         Range[2];
    Strategy                    strategy;
    CursorColor                 cursorColor;
    CursorSize                  cursorSize;
    BackGroundColor             backGroundColor;

//    FixationTarget       fixationTarget;
    int                         speed;
    int                         brightness;
    DynamicMethod               dynamicMethod;
    DynamicDistance             dynamicDistance;
    FixationMonitor             fixationMonitor;

    template<class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(Range);
        archive & BOOST_SERIALIZATION_NVP(strategy);
        archive & BOOST_SERIALIZATION_NVP(cursorColor);
        archive & BOOST_SERIALIZATION_NVP(cursorSize);
        archive & BOOST_SERIALIZATION_NVP(backGroundColor);
//        archive & BOOST_SERIALIZATION_NVP(fixationTarget);
        archive & BOOST_SERIALIZATION_NVP(speed);
        archive & BOOST_SERIALIZATION_NVP(brightness);
        archive & BOOST_SERIALIZATION_NVP(dynamicMethod);
        archive & BOOST_SERIALIZATION_NVP(dynamicDistance);
        archive & BOOST_SERIALIZATION_NVP(fixationMonitor);
    }

//    QString ToQString()
//    {
//        std::stringstream ss;
//        boost::archive::xml_oarchive oa(ss);
//        oa& BOOST_SERIALIZATION_NVP(*this);
//        return ss.str().c_str();
//    }
};

//template <Type T>
//struct ParamTraits
//{
//    typedef StaticParams params;
//};


//template <>
//struct ParamTraits<Type::Dynamic>
//{
//    typedef DynamicParams params;
//};



#endif // PARAMS_H
