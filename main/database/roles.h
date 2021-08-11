#ifndef ROLES_H
#define ROLES_H
#include <QAbstractListModel>

namespace Perimeter{

enum PatientRoles
{
    id = Qt::UserRole + 1,
    name,
    birthDate,
    sex,
};

}
#endif // ROLES_H
