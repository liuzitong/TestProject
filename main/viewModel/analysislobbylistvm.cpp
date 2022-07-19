#include "analysislobbylistvm.h"
namespace Perimeter{
class PicListVm:public QAbstractListModel
{
    enum PicListRoles
    {
        picName=Qt::UserRole+1,
    };

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override
    {
        return 8;
    }
    virtual QVariant data(const QModelIndex &index, int role) const override
    {
        QVector<QString> picName={"dBDiagram.bmp","defectDepth.bmp","gray.bmp","PatterDeviation.bmp"};
        switch (role)
        {
            case PicListRoles::picName:return picName[index.row()%4];
        }
        return QVariant();
    }
    virtual QHash<int, QByteArray> roleNames() const override
    {
        QHash<int, QByteArray> roles;
        roles[picName] = "picName";
        return roles;
    }
};








AnalysisLobbyListVm::AnalysisLobbyListVm()
{

}
AnalysisLobbyListVm::AnalysisLobbyListVm(const QVariantList &)
{

}

void AnalysisLobbyListVm::refreshData()
{

}

int AnalysisLobbyListVm::rowCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant AnalysisLobbyListVm::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case year:return "2008";
    case monthDay:return "05/06";
    case picList:return QVariant::fromValue(new PicListVm());
    }
    return QVariant();
}

QHash<int, QByteArray> AnalysisLobbyListVm::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[year] = "year";
    roles[monthDay]="monthDay";
    roles[picList] = "picList";
    return roles;
}
}
//#include "analysislobbylistvm.moc"
