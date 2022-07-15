#ifndef DIGRAM_PROVIDER_H
#define DIGRAM_PROVIDER_H

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
class AnalysisSvc : public QObject
{
    Q_OBJECT

public:
    explicit AnalysisSvc(QObject *parent = nullptr);
    ~AnalysisSvc();
    Q_INVOKABLE QObject* runProcess(int report,PatientVm* patient,CheckResultVm* checkResult,QObject* program,QVariant diagramWidth);
    Q_INVOKABLE QPointF getClickDot(float MouseX,float MouseY,float width,float height);
    Q_INVOKABLE QPointF getPixFromPoint(QPointF point,float width,float height);
    Q_INVOKABLE int getSelectedDotIndex(){return m_selectedDotIndex;};
    Q_INVOKABLE void showReport(int report);


private:
    QImage m_image;
//    QSize m_imageSizeLarge=QSize(720,720);
//    QImage m_imageLarge=QImage(m_imageSize, QImage::Format_RGB32);
    int m_report;   //0,30-2
    int m_os_od;
    int m_range;
    int m_innerRange;
    int m_selectedDotIndex;
    int m_programType;

    LimeReport::ReportEngine* m_reportEngine=nullptr;

    QVector<QPointF> m_dotList;
    QVector<int> m_staticValues;
    QVector<QPointF> m_dynamicValues;
    CheckResultVm* m_checkResult=nullptr;
    PatientVm* m_patient=nullptr;
    QObject* m_program=nullptr;

    QJsonArray m_jsonArray;

    QVector<QPoint> m_pointLoc_30d;
    QVector<QPoint> m_pointLoc_60d;
    QVector<int> m_value_30d;
    QVector<int> m_value_60d;
    int m_age_correction;

    QVector<int> m_v5;
    QVector<int> m_v2;
    QVector<int> m_v1;
    QVector<int> m_v05;

    QVector<QPoint> m_GHT1_RIGHT;
    QVector<QPoint> m_GHT2_RIGHT;
    QVector<QPoint> m_GHT3_RIGHT;
    QVector<QPoint> m_GHT4_RIGHT;
    QVector<QPoint> m_GHT5_RIGHT;

    QVector<int> m_sv;
    QVector<int> m_dev;
    QVector<int> m_mDev;
    QVector<int> m_peDev;
    QVector<int> m_peMDev;

    QVector<int> m_vfiRingStandard;
    QVector<int> m_vfiRingTest;

    float m_VFI_Weight[5]={3.29f,1.28f,0.79f,0.57f,0.43f};
    float m_md,m_psd,m_VFI,m_p_md,m_p_psd;
    int m_GHT;
    int m_dotSeen,m_dotWeakSeen,m_dotUnseen;

    int m_patientAge;

    QString m_imageSavePath;
    bool m_isPreview;
    int m_previewDiagramWidth;





private:
    void drawPixScale();
    void drawRoundCrossPixScale();

    void drawDevDBText(QVector<int> values);
    void drawTotalDeviation();
    void drawPatternDeviation();

    void drawPE(QVector<int> values);
    void drawTotalPE();
    void drawPatternPE();

    void drawDBDiagram();
    void drawGrayDiagram();
    void drawDefectDepthDiagram();

    void drawScreening();
    void drawDynamic();

    void drawFixationDeviation();

    void staticAnalysis();
    void dynamicAnalysis();




    QPoint convertDegLocToPixLoc(QPointF DegLoc);
    void DrawDiagram();
    void DrawReportDiagram();



};
}
#endif // DIGRAM_PROVIDER_H
