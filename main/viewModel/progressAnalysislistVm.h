#ifndef PROGRESSANALYSISLISTVM_H
#define PROGRESSANALYSISLISTVM_H
#include <QAbstractListModel>
#include <QString>
#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/programVm.h>
#include <perimeter/main/model/checkResultModel.h>

namespace Perimeter{


class ProgressAnalysisListVm:public QAbstractListModel
{
    enum ProgressAnalysisRoles
    {
        checkResultId=Qt::UserRole+1,
        dateTime,
        program,
        strategy,
        centerDotCheck,
        md,
        p_md,
        psd,
        p_psd,
        GHT,
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
        bool centerDotCheck;
        float md;
        float p_md;
        float psd;
        float p_psd;
        int GHT;
        float fixationLossRate;
        float falsePositiveRate;
        float falseNegativeRate;


        QVector<int> values,dev,mDev,peDev,peMDev;
        QVector<QPointF> locs;

    };

    Q_OBJECT
    Q_PROPERTY(int selectedResultId READ getSelectedResultId WRITE setSelectedResultId NOTIFY selectedResultIdChanged)
    Q_PROPERTY(int selectedIndex READ getSelectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
public:
    Q_INVOKABLE explicit ProgressAnalysisListVm(const QVariantList &args);
    Q_INVOKABLE virtual ~ProgressAnalysisListVm() Q_DECL_OVERRIDE;
    Q_INVOKABLE QObject* getProgressBaseLine(int imageSize,bool isReport);
    Q_INVOKABLE QVariantList getThreeFollowUps(int index,int imageSize,bool isReport);
    Q_INVOKABLE QVariant getSingleProgress(int index,int imageSize,bool isReport);
    Q_INVOKABLE QVariant getData(int index,QString name);

    Q_INVOKABLE void removeCheckResult(int index);
    Q_INVOKABLE void switchEye();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void showReport(int report);




private:
    int getSelectedResultId(){return m_selectedResultId;}void setSelectedResultId(int value){m_selectedResultId=value;emit selectedResultIdChanged();}Q_SIGNAL void selectedResultIdChanged();
    int getSelectedIndex(){return m_selectedIndex;}void setSelectedIndex(int value){m_selectedIndex=value;setSelectedResultId(m_currentDataList[m_selectedIndex].checkResultId);emit selectedIndexChanged();}Q_SIGNAL void selectedIndexChanged();
    void generateDataList();
    Data getProgressData(CheckResult_ptr checkResult_ptr);
    int m_OS_OD;
    int m_selectedResultId=0;
    int m_selectedIndex=0;
    int m_patientId;
//    QVector<QVector<int>> m_mDevList;
//    QVector<QVector<QPointF>> m_locsList;
    QVector<QDateTime> m_dateTimeList;
    QVector<Data> m_originalDataList;
    QVector<Data> m_currentDataList;
    QString m_previewFolder="./previewImage/";
    QString m_reportFolder="./reportImage/";

    float m_avgMd,m_progressSpeedBase,m_progressSpeedDeviation;
    int m_slopeType;

public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};
}

#endif // PROGRESSANALYSISLISTVM_H


