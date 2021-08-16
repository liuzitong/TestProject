#ifndef ROLES_H
#define ROLES_H
#include <QAbstractListModel>

namespace Perimeter{

enum PatientRoles
{
    Id=Qt::UserRole + 1,
    patientId ,
    name,
    birthDate,
    sex,
    lastUpdate
};

}
#endif // ROLES_H
