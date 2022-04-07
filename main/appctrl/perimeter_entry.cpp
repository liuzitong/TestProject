#include <QCoreApplication>
#include <QDebug>
#include <QMessageLogContext>
#include <QThread>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSharedPointer>

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
#include "perimeter/main/appctrl/test.hpp"


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


//std::fprintf( stderr, fmt_str.toUtf8().constData() );

#if defined(_MSC_VER)
    OutputDebugStringW( reinterpret_cast<LPCWSTR>( fmt_str.constData()) );
#else
    std::fprintf( stderr, fmt_str.toUtf8().constData() );
#endif
}


#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/binary_object.hpp>
#include <perimeter/main/model/Params.h>
#include <perimeter/main/model/utility.h>
#include <perimeter/main/model/programModel.h>

//void save()
//{
//    std::ofstream file("archive.xml");
//    std::stringstream str;
//    boost::archive::xml_oarchive oa(file);
//    float array[] = { 34.2, 78.1, 22.221, 1.0, -910.88 };
//    std::list<float> L1(array, array + 5);
//    std::vector<float> V1(array, array + 5);
//    oa<< BOOST_SERIALIZATION_NVP(L1);
//    oa<< BOOST_SERIALIZATION_NVP(V1);

////    std::cout << str.str() << std::endl;
//}


//void load()
//{
//    std::ifstream file("archive.xml");
//    boost::archive::xml_iarchive ia(file);
//    std::list<float> L2;
//    ia >> BOOST_SERIALIZATION_NVP(L2); // No size/range needed

//    std::vector<float> V2;
//    ia >> BOOST_SERIALIZATION_NVP(V2); // No size/range needed

//    std::ostream_iterator<float> oi(std::cout, " ");
//    std::copy(L2.begin(), L2.end(), oi);
//    std::copy(V2.begin(), V2.end(), oi);
//}

//std::string  global_str;
//void save2()
//{
//    StaticParams param{{{3,2},0},{}};
//    using strategy=StaticParams::CommonParams::Strategy;
//    StaticProgramData<Type::ThreshHold> data{{strategy::fullThreshold,strategy::fastInterative},{{3.2f,2.5f},{4.5f,1.2f}}};
//    std::stringstream ss;
//    {                                           //必须括号主动调用析构函数,不然写入不全
//        boost::archive::xml_oarchive oa(ss);
//        oa<< BOOST_SERIALIZATION_NVP(param);
//    }
//    global_str=ss.str();
//    std::cout<<ss.str().c_str()<<std::endl;
//}

//void load2()
//{
//    StaticParams t;
//    std::stringstream ss(global_str);
//    std::cout<<ss.str()<<std::endl;
//    boost::archive::xml_iarchive ia(ss);
//    ia>> BOOST_SERIALIZATION_NVP(t);
//    std::cout<<t.commonParams.Range[0]<<std::endl;
//    std::cout<<std::endl;
//}


int  main1 ( int argc, char *argv[] )
{
    using StaticStrategy=StaticParams::CommonParams::Strategy;
    using StaticStrategyMode=StaticParams::CommonParams::StrategyMode;
    using MoveStrategy=MoveParams::Strategy;
    ProgramModel<Type::Move> pm;
    pm.m_name="Straight Line";
    pm.m_type=Type::Move;
//    pm.m_params=
//    {{{0,80},StaticStrategy::Standard,StaticStrategyMode::ageRelated,CursorColor::white,CursorSize::I,BackGroundColor::white,
//     0,1,500,0,0,FixationViewSelection::centerPoint,EyeMoveAlarmMode::dontAlarm,1},
//     {180,1000,20,20,10,10,10,10}};
    pm.m_params={{0,30},MoveStrategy::straightLine,CursorColor::white,CursorSize::I,BackGroundColor::white,
                1,0,MoveParams::MoveMethod::_4Lines,MoveParams::MoveDistance::_5,EyeMoveAlarmMode::dontAlarm};

//    pm.m_data.strategies={StaticStrategy::oneStage,StaticStrategy::twoStages,StaticStrategy::quantifyDefects,StaticStrategy::singleStimulation};
    pm.m_data.dots={
//        {42,85},{42,85},{48,60},{48,120},{60,265},{60,275},{62,240},{62,300},
//        {66,30},{66,160},{75,5},{75,175},{75,185},{75,210},{75,330},{75,355}


    };
    pm.m_category=Category::Move;

    auto pp=pm.ModelToDB();
    std::cout<<pp->m_params.toStdString()<<std::endl;
    std::cout<<pp->m_data.toStdString()<<std::endl;






//    ProgramModel<Type::Screening> pm2;
//    pm2.m_type=Type::Screening;pm2.m_params={{{3,2},0},{}};
//    pm2.m_data.strategies={StaticStrategy::oneStage,StaticStrategy::twoStages};
//    pm2.m_data.dots={{44,3},{55,2}};
//    pm2.m_category=Category::Screening;
//    pm2.m_name="screening";
//    auto pp2=pm2.ModelToDB();

//    ProgramModel<Type::Move> pm3;
//    pm3.m_type=Type::Move;
//    pm3.m_params={{3,5},FixationViewSelection::centerPoint};
//    pm3.m_data.dots={{44,3},{55,2}};
//    pm3.m_name="move";
//    pm3.m_category=Category::Move;
//    auto pp3=pm3.ModelToDB();



//    Test::removeDataBase();
    Test::connectDataBase();
//    Test::createTable();
//    Test::createEntityData();
//    Test::GetEntityData();
//    Test::displayInfo();

    QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
    bool bCommit = db.transaction();
    QSqlError daoError = qx::dao::insert(pp, &db);
//    daoError = qx::dao::insert(pp2, &db);
//    daoError = qx::dao::insert(pp3, &db);
    bCommit = (bCommit && ! daoError.isValid());
    qAssert(bCommit);
    db.commit();

    return  0;
}


// ============================================================================
// main entry
// ============================================================================

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
        QQmlApplicationEngine *eng = new QQmlApplicationEngine;
        eng->addImportPath(QStringLiteral("qrc:/") );

        eng->load(QUrl(QLatin1String("qrc:/perimeter/main/view/Application.qml")));
        qmlRegisterSingletonType(QUrl("qrc:/perimeter/main/view/Utils/CusUtils.qml"), "perimeter.main.view.Utils", 1, 0, "CusUtils");

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

