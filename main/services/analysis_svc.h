#ifndef ANALYSIS_SVC
#define ANALYSIS_SVC

#include <QObject>
#include <QVariant>
#include <QVariantList>
#include <QImage>
#include <QSize>
#include <functional>
#include <perimeter/main/viewModel/programVm.h>
#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/checkResultVm.h>
#include "perimeter/third-part/LimeReport/limereport/LimeReport"
#include "perimeter/third-part/LimeReport/limereport/lrpreviewreportwindow.h"

namespace Perimeter {
class AnalysisSvc
{
public:
    explicit AnalysisSvc();
    ~AnalysisSvc();

    void ThresholdAnalysis(int resultId,QVector<int>& dev,QVector<int>& mDev,QVector<int>& peDev,QVector<int>& peMDev,float& md,float& psd,float& VFI,int& GHT, float& p_md,float& p_psd);

    void ScreeningAnalysis(int resultId,int& dotSeen,int& dotWeakSeen,int& dotUnseen);
//    void DrawThreshold(int resultId,int imageSize,QVector<int>& dev,QVector<int>& mDev,QVector<int>& peDev,QVector<int>& peMDev);
//    void DrawScreening(int resultId,int imageSize);
//    void DrawDynamic(int resultId,int imageSzie);
    static AnalysisSvc* getSingleton();

    void drawPixScale(int range,QImage& img);

    void drawRoundCrossPixScale(int range,QImage& img);

    void drawText(QVector<int> values,QVector<QPointF> locs,int range,int OS_OD,QImage& img);             //db,dev,mdev

    void drawGray(QVector<int> values,QVector<QPointF> locs,int range,int innerRange,QImage& imgh);

    void drawPE(QVector<int> values,QVector<QPointF> locs,int range,QImage& img);                                   //totalPe,patternPe

    void drawDefectDepth(QVector<int> values,QVector<QPointF> locs,int range,QImage& img);

    void drawScreening(QVector<int> values,QVector<QPointF> locs,int range,int OS_OD,QImage& img);

    void drawDynamic(QVector<QPointF> values,int range,QImage& img);

    void drawFixationDeviation(QVector<int> values,QImage& img);

    QPoint convertDegLocToPixLoc(QPointF DegLoc,int range,QImage img);

private:
    static AnalysisSvc* singleton;

    LimeReport::ReportEngine* m_reportEngine;

    QJsonArray m_jsonArray;

    QVector<QPoint> m_pointLoc_30d;
    QVector<QPoint> m_pointLoc_60d;

    QVector<QVector<QVector<int>>> m_value_30d_cursorSize_ageCorrectionOrCursorColor;     //cursorSize!=2 ,it's CursorColor
    QVector<int> m_value_60d;

    QVector<int> m_pe_v5[2];
    QVector<int> m_pe_v2[2];
    QVector<int> m_pe_v1[2];
    QVector<int> m_pe_v05[2];

    QVector<QPoint> m_GHT1_RIGHT;
    QVector<QPoint> m_GHT2_RIGHT;
    QVector<QPoint> m_GHT3_RIGHT;
    QVector<QPoint> m_GHT4_RIGHT;
    QVector<QPoint> m_GHT5_RIGHT;
};
}
#endif // DIGRAM_PROVIDER_H
