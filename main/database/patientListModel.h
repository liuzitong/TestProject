#ifndef PATIENTMODELLIST_H
#define PATIENTMODELLIST_H
#include "perimeter/base/common/perimeter_def.h"
#include <QAbstractListModel>
//#include "patientVm.h"
#include "patient.h"

namespace Perimeter{
class PERIMETER_API PatientListModel:public QAbstractListModel
{
    Q_OBJECT
//    Q_PROPERTY(int currentValue READ current WRITE setCurrent NOTIFY currentChanged)
public:
    PatientListModel();
//    int current();
//    void setCurrent();
    void setPatientList(Patient_List patient_list);
//    Q_SIGNAL void currentChanged();

    // ========================================================================
    // override functions
    // ========================================================================
    //! the total records number
    virtual int        rowCount( const QModelIndex &)  const Q_DECL_OVERRIDE;

    //! return the data object by index and role
    virtual QVariant   data    ( const QModelIndex &idx, int role ) const Q_DECL_OVERRIDE;

    //! return all role id ( and names ) supported by this model
    //! @note we used roles like "id","name","dob", so view can use name to access it.
    virtual QHash<int,QByteArray>  roleNames( ) const Q_DECL_OVERRIDE;

//    //! directly set data object at spec. index
//    virtual bool       setData ( const QModelIndex &, const QVariant &, int ) Q_DECL_OVERRIDE;

//    //! return flags ( characteristics )
//    virtual Qt::ItemFlags  flags ( const QModelIndex & ) const Q_DECL_OVERRIDE;

private:
    void* m_obj;
    Q_DISABLE_COPY(PatientListModel);
};
}
#endif // PATIENTMODELLIST_H
