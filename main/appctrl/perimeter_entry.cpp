#include <QCoreApplication>
#include <QDebug>
#include <QMessageLogContext>
#include <QThread>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>

#include <QxOrm_Impl.h>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "perimeter/main/appctrl/perimeter_appctrl.hxx"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_memcntr.hxx"
#include "qxpack/indcom/common/qxpack_ic_global.hxx"
#include "qxpack/indcom/sys/qxpack_ic_eventloopbarrier.hxx"
#include "qxpack/indcom/sys/qxpack_ic_appdcl_priv.hxx"
#include "qxpack/indcom/ui_qml_base/qxpack_ic_ui_qml_api.hxx"
#include "perimeter/main/model/patient.h"



#include <cstdio>
#include <signal.h>

#if !defined(WIN32)
#include <unistd.h>
#endif
#if  defined(_MSC_VER )
#include <windows.h>
#endif

// ============================================================================
// handle the SIGTERM
// ============================================================================
static void  gSigTerm_Handler( int sig )
{
    if ( sig == SIGTERM ) {
        QCoreApplication::quit();
    }
}

// ============================================================================
// print out current memory counter
// ============================================================================
static void  gPrintMemCntr( const char *stage )
{
    qInfo("%s, current new counter = %d", stage, perimeter_curr_new_cntr );
    qInfo("%s, current qxpack ic counter=%d", stage, qxpack_ic_curr_new_cntr );
}

// ============================================================================
// the message handler
// ============================================================================
static void gMsgHandler( QtMsgType type, const QMessageLogContext &ctxt, const QString &msg )
{
    QString  fmt_str;
    uint t_id = reinterpret_cast<uint>( QThread::currentThread());
    char tmp[26]; QxPack::tailPath( ctxt.file, tmp, sizeof(tmp), 1 );

    switch( type ) {
    case QtDebugMsg : {
        fmt_str = QString("[Debug] 0x%1 %2 ( ln:%3, %4 %5 )\n")
                  .arg( t_id, 0, 16 ).arg( msg ).arg( ctxt.line ).arg( QString( ctxt.function )).arg( tmp );
    } break;

    case QtInfoMsg  : {
        fmt_str = QString("[Info ] 0x%1 %2 ( ln:%3, %4 %5 )\n")
                  .arg( t_id, 0, 16 ).arg( msg ).arg( ctxt.line ).arg( QString( ctxt.function )).arg( tmp );
    } break;

    case QtWarningMsg : {
        fmt_str = QString("[Warn ] 0x%1 %2 ( ln:%3, %4 %5 )\n")
                  .arg( t_id, 0, 16 ).arg( msg ).arg( ctxt.line ).arg( QString( ctxt.function )).arg( tmp );
    } break;

    case QtCriticalMsg : {
        char tmp[26];
        fmt_str = QString("[Criti] 0x%1 %2 ( ln:%3, %4 %5 )\n")
                  .arg( t_id, 0, 16 ).arg( msg ).arg( ctxt.line ).arg( QString( ctxt.function )).arg( tmp );
    } break;

    case QtFatalMsg : {
        fmt_str = QString("[Fatal] 0x%1 %2 ( ln:%3, %4, %5 )\n")
                  .arg( t_id, 0, 16 ).arg( msg ).arg( ctxt.line ).arg( QString( ctxt.function )).arg( tmp );
    }   abort();
    }

#if defined(_MSC_VER)
    OutputDebugStringW( reinterpret_cast<LPCWSTR>( fmt_str.constData()) );
#else
    std::fprintf( stderr, fmt_str.toUtf8().constData() );
#endif

}

// ============================================================================
// main entry
// ============================================================================
void test( );
int  main ( int argc, char *argv[] )
{
    int ret = 0;

    //handle the terminate signal
    signal( SIGTERM, & gSigTerm_Handler );
    qInstallMessageHandler( & gMsgHandler );

    // start the application
    gPrintMemCntr("pre-stage");
    {
        QGuiApplication app(argc, argv);

        Perimeter::AppCtrl *app_ctrl = new Perimeter::AppCtrl;
        app_ctrl->doInit();

        // --------------------------------------------------------------------
        // share the application controller into QML
        // --------------------------------------------------------------------
        QSharedPointer<QObject> s_app_ctrl( app_ctrl, [](QObject*){});
        QxPack::IcUiQmlApi::setAppCtrl( s_app_ctrl );

        // --------------------------------------------------------------------
        //  here create the main view
        // --------------------------------------------------------------------
        test();
        QQmlApplicationEngine *eng = new QQmlApplicationEngine;
        eng->addImportPath(QStringLiteral("qrc:/") );

        eng->load(QUrl(QLatin1String("qrc:/perimeter/main/view/application.qml")));

        gPrintMemCntr("enter eventloop stage");
        ret = app.exec();
        eng->deleteLater();

        QxPack::IcAppDclPriv::barrier( 32 );
        app_ctrl->doDeinit();
        delete app_ctrl;
        QxPack::IcAppDclPriv::barrier( 32 );
    }
    gPrintMemCntr("post-stage");


    return ret;
}


void test(  )
{

    QFile::remove(".\\qxDemo1.sqlite");

    qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
    qx::QxSqlDatabase::getSingleton()->setDatabaseName(".\\qxDemo1.sqlite");
    qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
    qx::QxSqlDatabase::getSingleton()->setUserName("root");
    qx::QxSqlDatabase::getSingleton()->setPassword("");
    qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
    qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);

    qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);

    //更具上面类型创建表
    QSqlError daoError = qx::dao::create_table<Patient>();

    Patient_ptr user_1, user_2;
    user_1.reset(new Patient);
    user_2.reset(new Patient);


    user_1->id=1;
    user_1->name="zhuxiaoming";
    user_1->age=18;
    user_1->capacity=99.9;

    user_2->id=2;
    user_2->name="qiuqiu";
    user_2->age=18;
    user_2->capacity=999.999;




    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
    bool bCommit = db.transaction();

    //猪小明入库
    daoError = qx::dao::insert(user_1, &db);
    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();

    //球球入库
    daoError = qx::dao::save(user_2);
    bCommit = !daoError.isValid();
    qAssert(bCommit);

    //通过SQL进行检索，映射到 typedef qx::QxCollection<int, User_ptr> List_user;中
    List_Patient list_user;
    qx_query storedProc("select * from user");
    daoError = qx::dao::execute_query(storedProc, list_user);

    List_Patient::iterator it = list_user.begin();

    qDebug() << "------------------华丽的分割线------------------";
    while(it != list_user.end()){

        qDebug() << "id:" << it.i->t().second->id;
        qDebug() << "name:" << it.i->t().second->name;
        qDebug() << "age:" << it.i->t().second->age;
        qDebug() << "capacity:" << it.i->t().second->capacity;
        it++;
    }
    qDebug() << "------------------华丽的分割线------------------";

    //修改下
    it = list_user.begin();
    while(it != list_user.end()){

        it.i->t().second->capacity = 59.9;
        it++;
    }

    qx::dao::update(list_user);


//    //新增及删除
//    User_ptr user_3;
//    user_3.reset(new User);

//    user_3->id = 100;
//    user_3->name = "闰土";
//    user_3->age = 19;
//    user_3->capacity = 99999.9999;
//    list_user.removeByKey(2);
//    list_user.insert(100, user_3);

//    qx::dao::save(user_3);
//    qx::dao::delete_by_id<User>(*user_2);


}
