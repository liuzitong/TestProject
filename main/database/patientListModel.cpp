#include "patientListModel.h"
#include "patientObj.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QList>
#include "roles.h"
namespace Perimeter{
class PERIMETER_API PatientListModelPriv
{

private:
//    QList<PatientObj> m_list;
    PatientListModel* m_parent;
    Patient_List m_list;
public:
    PatientListModelPriv(PatientListModel* pa)
    {
        m_parent=pa;
    }
    inline int RowCount(){return m_list.count();}
    QVariant data(const QModelIndex &idx, int role) const
    {
//        int index=idx.row();
//        Patient_ptr pp=m_list[index];
//        if ( role == Qt::DisplayRole )
//        {
//            PatientObj pb(pp);
//            return pb;

//        } else {
//            return QVariant( 0 );
//        }
    }

    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roles;
           roles[id] = "id";
           roles[name] = "name";
           roles[birthDate] = "birthDate";
           roles[sex]="sex";
           return roles;
    }
};

PatientListModel::PatientListModel()
{
    m_obj = perimeter_new( PatientListModelPriv, this );
}

int PatientListModel::rowCount(const QModelIndex &) const
{
    return m_obj->RowCount();
}

QVariant PatientListModel::data(const QModelIndex &idx, int role) const
{
    return m_obj->data(idx,role);
}

QHash<int, QByteArray> PatientListModel::roleNames() const
{
    return m_obj->roleNames();
}

bool PatientListModel::setData(const QModelIndex &, const QVariant &, int)
{

}

Qt::ItemFlags PatientListModel::flags(const QModelIndex &) const
{

}
}
