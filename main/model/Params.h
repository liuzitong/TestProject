#ifndef PARAMS_H
#define PARAMS_H

#define OPTION int
#define ON_OFF bool
#include <tuple>


enum class CursorColor{white,red,blue};
enum class StrategyMode{ageRelated,ThresholdRelated,singleStimulation};
enum class BackGroundColor{white,yellow};
enum class FixationViewSelection{centerPoint,smallDiamond,bigDiamond,bottomPoint};
enum class EyeMoveAlarmMode{dontAlarm,onlyAlarm,alarmAndPause};

struct StaticParams
{
    struct CommonParams
    {
        enum class Strategy{fullThreshold,smartInteractive,fastInterative,oneStage,twoStages,quantifyDefects};
        std::tuple<int,int>     Range;                      ON_OFF                      responseAutoAdapt;
        /*int                     DotCount;*/               int                         intervalTime;
        Strategy                strategy;                   ON_OFF                      centeralDotCheck;
        StrategyMode            strategyMode;               ON_OFF                      shortTermFluctuation;
        CursorColor             cursorColor;                FixationViewSelection       fixationViewSelection;
        BackGroundColor         backGroundColor;            EyeMoveAlarmMode            eyeMoveAlarmMode;
        ON_OFF                  cyanYellTest;               ON_OFF                      blindDotTest;
    };

    struct FixedParams
    {
        int stimulationTime;            int singleStimulationDB;
        int intervalTime;               int blindDotStimulationDB;
        int falsePositiveCycle;         int shortTermFluctuationCount;
        int falseNegativeCycle;
        int fixationViewLossCycle;
    };
};

struct MoveParams
{
    enum class Strategy{standard,blindArea,darkArea,straightLine};
    enum class CursorColor{white,red,blue};
    enum class CursorSize{I,_II,III,IV,V};
    enum class MoveMethod{_4Lines,_6Lines,_8Lines};
    enum class MoveDistance{_5,_10,_15};
    std::tuple<int,int>         Range;                      FixationViewSelection   fixationViewSelection;
                                                            float                   spead;
    Strategy                    strategy;                   int                     brightness;
    CursorColor                 cursorColor;                MoveMethod              moveMethod;
    CursorSize                  cursorSize;                 MoveDistance            moveDistance;
    BackGroundColor             backGroudColor;             EyeMoveAlarmMode        eyeMoveAlarmMode;
};

#endif // PARAMS_H
