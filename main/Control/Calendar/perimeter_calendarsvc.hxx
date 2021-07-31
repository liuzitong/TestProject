#ifndef ALBIO_CALENDARSVC_HXX
#define ALBIO_CALENDARSVC_HXX

#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_svcbase.hxx"

namespace Perimeter {

class PERIMETER_API CalendarSvc : public QxPack::IcSvcBase
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CalendarSvc( );
    virtual ~CalendarSvc( ) Q_DECL_OVERRIDE;

    QObject   * yearObj ( ) const;
    QObject   * monthObj( ) const;
    QObject   * dayObj  ( ) const;
    QString     dateStr ( ) const;

private:
    void *m_obj;
    Q_DISABLE_COPY( CalendarSvc )
};

}

#endif // ALBIO_CALENDARSVC_HXX
