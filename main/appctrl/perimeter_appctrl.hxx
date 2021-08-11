#ifndef PERIMETER_APPCTRL_HXX
#define PERIMETER_APPCTRL_HXX

#include "perimeter/base/common/perimeter_def.h"
#include "qxpack/indcom/afw/qxpack_ic_appctrlbase.hxx"
#include <QObject>
#include <QString>
#include "perimeter/main/database/databaseSvc.h"

namespace Perimeter {

// ////////////////////////////////////////////////////////////////////////////
/*!
 * @brief  the application controller
 */
// ////////////////////////////////////////////////////////////////////////////
class PERIMETER_API  AppCtrl : public QxPack::IcAppCtrlBase {
    Q_OBJECT
    Q_PROPERTY( QObject*  objMgr READ  objMgrObj CONSTANT )
    Q_PROPERTY( QObject*  msgBus READ  msgBusObj CONSTANT )
    Q_PROPERTY( QString language READ  language WRITE setLanguage NOTIFY languageChanged)


public:
    //! ctor
    explicit AppCtrl( QObject *pa = Q_NULLPTR );

    //! dtor
    virtual ~AppCtrl() Q_DECL_OVERRIDE;

    //! objMgrObj
    QObject*  objMgrObj( ) const;

    //! msgBusObj
    QObject*  msgBusObj( ) const;

    QString language() {return m_language ;}

    void setLanguage(QString value){m_language=value;languageChanged(m_language);}

    Q_SIGNAL void  languageChanged(QString value);

protected:
    // override the name
    virtual QString   instanceName() const Q_DECL_OVERRIDE;

    // override the the initialization process
    virtual void      preInit( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcAppSettingsBase* createSingleton_AppSettings( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcMsgBus*  createSingleton_MsgBus( ) Q_DECL_OVERRIDE;
    virtual QxPack::IcObjMgr*  createSingleton_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      registerTypes_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      postInit( ) Q_DECL_OVERRIDE;

    // override the deinitialization process
    virtual void      preDeinit() Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_ObjMgr( ) Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_MsgBus( ) Q_DECL_OVERRIDE;
    virtual void      releaseSingleton_AppSettings( ) Q_DECL_OVERRIDE;
    virtual void      postDeinit() Q_DECL_OVERRIDE;

private:
    void *m_obj;
    QString m_language="Chinese";

    Q_DISABLE_COPY( AppCtrl )
};

}

#endif 

