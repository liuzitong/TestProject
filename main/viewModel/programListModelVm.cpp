#include "programListModelVm.h"

namespace Perimeter{

enum ProgramRoles
{
    Id=Qt::UserRole + 1,
    program_id,
    name,
    category
};

ProgramListModelVm::ProgramListModelVm(const QVariantList & list)
{
    int category=list[0].toInt();
    qx_query query("select program_id,name from program where category=:category");
    query.bind(":category",category);
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    beginResetModel();
    m_list=program_List;
    endResetModel();
}

int ProgramListModelVm::rowCount(const QModelIndex &) const
{
    return m_list.count();
}

QVariant ProgramListModelVm::data(const QModelIndex &index, int role) const
{
    switch (role) {
        case name:return m_list[index.row()]->m_name;
    default:return QVariant();
    }
}

}
