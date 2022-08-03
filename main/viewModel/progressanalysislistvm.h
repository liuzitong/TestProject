#ifndef PROGRESSANALYSISLISTVM_H
#define PROGRESSANALYSISLISTVM_H
#include <QAbstractListModel>
#include <QString>
#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/programVm.h>

namespace Perimeter{
class progressAnalysisListVm:public QAbstractListModel
{
    enum ProgressAnalysisRoles
    {
        checkResultId=Qt::UserRole+1,
        dateTime,
        program,
        strategy,
        md,
        psd,
        fixationLossRate,
        falsePositiveRate,
        falseNegativeRate
    };


    struct Data
    {
        int checkResultId;
        QString dateTime;
        QString program;
        QString strategy;
        float md;
        float psd;
        float fixationLossRate;
        float falsePositiveRate;
        float falseNegativeRate;
    };

    Q_OBJECT
public:
    Q_INVOKABLE explicit progressAnalysisListVm(const QVariantList &args);
    Q_INVOKABLE virtual ~progressAnalysisListVm() Q_DECL_OVERRIDE;
    Q_INVOKABLE void removeCheckResult(int checkResultId);
private:
    void initialize();

    QVector<Data> m_originalDataList;
    QVector<Data> m_currentDataList;

    QVector<QPointF> m_dotList;
    QVector<int> m_staticValues;
    PatientVm* m_patient=nullptr;
    StaticProgramVM* m_program_30_2=nullptr;
    StaticProgramVM* m_program_24_2=nullptr;

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

    float m_md,m_psd;

    int m_patientAge;


    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
};
}

#endif // PROGRESSANALYSISLISTVM_H


