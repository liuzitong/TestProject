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
        baseLine,
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
        bool baseLine;
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
    Q_INVOKABLE explicit ProgressAnalysisListVm(const QVariantList &args);
    Q_INVOKABLE virtual ~ProgressAnalysisListVm() Q_DECL_OVERRIDE;
    Q_INVOKABLE void removeCheckResult(int resultId);
    Q_INVOKABLE void switchEye();
private:
    void generateDataList();
    Data getProgressData(CheckResult_ptr checkResult_ptr);
    int m_OS_OD;
    int m_patientId;
    QVector<Data> m_originalDataList;
    QVector<Data> m_currentDataList;



public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};
}

#endif // PROGRESSANALYSISLISTVM_H


