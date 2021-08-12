#include "patientListModel.h"
#include "patientVm.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include <QList>
#include "roles.h"
namespace Perimeter{
#define T_PrivPtr( o )  perimeter_objcast( PatientListModelPriv*, o )
class PERIMETER_API PatientListModelPriv
{
private:
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
        int index=idx.row();
        Patient_ptr pp=m_list[index];
        switch (role)
        {
        case (PatientRoles::Id):return (int)pp->m_id;
        case (PatientRoles::patientId): return pp->m_patinetId;
        case (PatientRoles::name): return pp->m_name;
        case (PatientRoles::birthDate): return pp->m_birthDate.toString("yyyy-MM-dd");
        case (PatientRoles::sex): return pp->m_sex;
        default:return QVariant();
        }
    }

    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roles;
           roles[Id] = "Id";
           roles[patientId] = "patientId";
           roles[name] = "name";
           roles[birthDate] = "birthDate";
           roles[sex]="sex";
           return roles;
    }
    void setPatientList(Patient_List patient_list)
    {
        m_list=patient_list;
    }
};

PatientListModel::PatientListModel()
{
    m_obj = perimeter_new( PatientListModelPriv, this );
}

void PatientListModel::setPatientList(Patient_List patient_list)
{
    T_PrivPtr( m_obj )->setPatientList(patient_list);
}

int PatientListModel::rowCount(const QModelIndex &) const
{
    return T_PrivPtr( m_obj )->RowCount();
}

QVariant PatientListModel::data(const QModelIndex &idx, int role) const
{
    return T_PrivPtr( m_obj )->data(idx,role);
}

QHash<int, QByteArray> PatientListModel::roleNames() const
{
    return T_PrivPtr( m_obj )->roleNames();
}

//bool PatientListModel::setData(const QModelIndex &, const QVariant &, int)
//{

//}

//Qt::ItemFlags PatientListModel::flags(const QModelIndex &) const
//{

//}
}
