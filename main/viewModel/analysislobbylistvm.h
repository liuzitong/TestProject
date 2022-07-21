#ifndef ANALYSISLOBBYLISTVM_H
#define ANALYSISLOBBYLISTVM_H
#include <QAbstractListModel>
#include <perimeter/main/services/analysis_svc.h>

namespace Perimeter{
class AnalysisLobbyListVm:public QAbstractListModel
{
    enum LobbyAnalysisRoles
    {
        year=Qt::UserRole+1,
        monthDay,
        simpleCheckResult,
    };

    struct Data
    {
        QString year;
        QString monthDay;
        QObject* simpleCheckResult=nullptr;
    };

    Q_OBJECT
public:
    AnalysisLobbyListVm();
    Q_INVOKABLE explicit AnalysisLobbyListVm(const QVariantList &args);
    Q_INVOKABLE virtual ~AnalysisLobbyListVm() Q_DECL_OVERRIDE;
    Q_INVOKABLE void refreshData();


public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
private:
    void generateModelListData(CheckResult_List checkResult_list);
    QList<Data> m_dataList;
    int m_age;
    float m_imageHeight;
    QString drawImage(CheckResult_ptr checkResult_ptr);

    void drawPatternPE(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawDefectDepth(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawScreening(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawDynamic(CheckResultModel<Type::Dynamic>& checkResultModel,ProgramModel<Type::Dynamic>& programModel);

};
}

#endif // ANALYSISLOBBYLISTVM_H
