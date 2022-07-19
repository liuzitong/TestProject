#ifndef ANALYSISLOBBYLISTVM_H
#define ANALYSISLOBBYLISTVM_H
#include <QAbstractListModel>

namespace Perimeter{
class AnalysisLobbyListVm:public QAbstractListModel
{
    enum LobbyAnalysisRoles
    {
        year=Qt::UserRole+1,
        monthDay,
        picList,
    };

    Q_OBJECT
public:
    AnalysisLobbyListVm();
    Q_INVOKABLE explicit AnalysisLobbyListVm(const QVariantList &);
    Q_INVOKABLE virtual ~AnalysisLobbyListVm() Q_DECL_OVERRIDE=default;
    Q_INVOKABLE void refreshData();

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;
};
}

#endif // ANALYSISLOBBYLISTVM_H
