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
    void initialize();
    void generateModelListData(CheckResult_List checkResult_list);
    QString drawImage(CheckResult_ptr checkResult_ptr);
    void initData(CheckResultModel<Type::ThreshHold>& checkResultModel);
    void drawPatternPE(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawDefectDepth(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawScreening(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel);
    void drawDynamic(CheckResultModel<Type::Dynamic>& checkResultModel,ProgramModel<Type::Dynamic>& programModel);
    void drawPixScale(int range);
    void drawRoundCrossPixScale(int range);
    QPoint convertDegLocToPixLoc(QPointF DegLoc,int range);
    int getIndex(QPointF& dot,QVector<QPoint>& pointLoc,int os_od);

    QList<Data> m_dataList;
//    int m_age;
    float m_imageHeight;
    QImage m_image;
    int m_imageSerial=0;

    QJsonArray m_jsonArray;
    QVector<QPoint> m_pointLoc_30d;
    QVector<QPoint> m_pointLoc_60d;
    QVector<int> m_value_30d;
    QVector<int> m_value_60d;

    int m_patientAge;
    int m_age_correction;

    QVector<int> m_v5;
    QVector<int> m_v2;
    QVector<int> m_v1;
    QVector<int> m_v05;

    QString m_imageSavePath;
};
}

#endif // ANALYSISLOBBYLISTVM_H
