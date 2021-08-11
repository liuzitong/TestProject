#include "databaseSvc.h"
#include "checkResult.h"
#include "patient.h"
#include "program.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"


namespace Perimeter {
class PERIMETER_API databaseSvcPriv
{
private:
    databaseSvc *m_parent;
public :
    explicit databaseSvcPriv( databaseSvc * pa)
    {
        m_parent=pa;
    }
    ~databaseSvcPriv( ){};
};

databaseSvc::databaseSvc()
{
    m_obj=perimeter_new( databaseSvcPriv, this );
}

databaseSvc::~databaseSvc()
{
    perimeter_delete( m_obj, databaseSvcPriv );
}

}
