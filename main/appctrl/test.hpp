#include "perimeter/main/database/precompiled.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <iostream>
#include <QDebug>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include "perimeter/main/database/checkResult.h"
#include "perimeter/main/database/patient.h"
#include "perimeter/main/database/program.h"
#include "perimeter/main/database/patient.h"
#include <perimeter/main/model/Params.h>
#include <perimeter/main/model/utility.h>
#include <perimeter/main/model/programModel.h>
#include <perimeter/main/model/checkResultModel.h>
#include "qxpack/indcom/common/qxpack_ic_global.hxx"

class Test{
public:
    static void removeDataBase()
    {
        QFile::remove(R"(./test.sqlite)");
    }

    static void connectDataBase()
    {
        qx::QxSqlDatabase::getSingleton()->setDriverName("QSQLITE");
        qx::QxSqlDatabase::getSingleton()->setDatabaseName(R"(./test.sqlite)");
        qx::QxSqlDatabase::getSingleton()->setHostName("localhost");
        qx::QxSqlDatabase::getSingleton()->setUserName("root");
        qx::QxSqlDatabase::getSingleton()->setPassword("");
        qx::QxSqlDatabase::getSingleton()->setFormatSqlQueryBeforeLogging(true);
        qx::QxSqlDatabase::getSingleton()->setDisplayTimerDetails(true);
        qx::QxSqlDatabase::getSingleton()->setVerifyOffsetRelation(true);
    }

    static void createTable(){
        // Create all tables in database
        QSqlError daoError = qx::dao::create_table<Patient>();
        daoError = qx::dao::create_table<CheckResult>();
        daoError = qx::dao::create_table<Program>();
    }


    void static doStuff()
    {
        qDebug()<<"hehe";
        QSqlError daoError = qx::dao::create_table<CheckResult>();
    }

    static void displayInfo()
    {
        Program_List Program_List;
        Patient_List Patient_List;
        CheckResult_List CheckResult_List;

        qx_query query("select * from program");
        QSqlError daoError = qx::dao::execute_query(query, Program_List);
        query.query("select * from patient");
        daoError = qx::dao::execute_query(query, Patient_List);
        query.query("select * from checkResult");
        daoError = qx::dao::execute_query(query, CheckResult_List);


        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_id<<" "<<i->m_patientId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;

        std::cout<<"*****program_list****"<<std::endl;
        for(auto& i:Program_List)
            std::cout<<i->m_id<<" "<<int(i->m_category)<<" "<<int(i->m_type)<<" "<<i->m_name.toStdString()<<" "<<i->m_data.toStdString()<<std::endl;
        std::cout<<std::endl;

        std::cout<<"*****checkResult_list****"<<std::endl;
        for(auto& i:CheckResult_List)
            std::cout<<i->m_id<<" "<<" "<<i->m_params.toStdString()<<" "<<i->m_data.toStdString()<<" "
                     <<i->m_time.toString("yyyy-MM-dd HH:mm::ss").toStdString()<<" "<<i->m_patient->m_patientId.toStdString()<<" "<<i->m_program->m_id<<std::endl;

        std::cout<<std::endl;
    }

    static void updateInfo()
    {
        Patient_List Patient_List;
        qx_query query("select * from patient");
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);
        for(auto& i:Patient_List)
        {
            if(i->m_name=="yangzhiqun")
            {
                i->m_birthDate=QDate(1960,5,18);
            }
        }
        qx::dao::update(Patient_List);
    }

    static void insertData()
    {
        Patient_List Patient_List;
        qx_query query("select * from patient where name=:name");
        query.bind(":name","yangzhiqun");
        qx::dao::execute_query(query,Patient_List);

        Program_List Program_List;
        query.query("select * from program where name=:name");
        query.bind(":name","30-2");
        qx::dao::execute_query(query,Program_List);

//        CheckResult_ptr checkResult(new CheckResult(5,"params1","data1",QDateTime::currentDateTime(),Patient_List.front(),Program_List.front()));
//        QSqlError daoError = qx::dao::insert(checkResult);

    }

    static void query()
    {
        Patient_List Patient_List;
        qx_query query("select * from patient where name=:name");
        query.bind(":name","yangzhiqun");
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);;

        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_patientId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;
    }

    static void query2()
    {
        Patient_List Patient_List;
        qx_query query;QSqlError daoError ;
        query.where("name").isEqualTo("lzt");
        daoError = qx::dao::fetch_by_query(query, Patient_List);;

        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_patientId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;

        query.clear();
        query.where("name").isEqualTo("yangzhiqun");
        daoError = qx::dao::fetch_by_query(query, Patient_List);;

        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_patientId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;
    }

    static void DeleteData()
    {
        Patient_List Patient_List;
        CheckResult_List CheckResult_List1;
        CheckResult_List CheckResult_List2;
        qx_query query("select * from patient where name=:name");
        query.bind(":name","yangzhiqun");
        QSqlError daoError =  qx::dao::execute_query(query,Patient_List);
        query.query("select* from checkResult");
        daoError = qx::dao::execute_query(query,CheckResult_List1);
        for(auto & patient:Patient_List)
        {
            for(auto &checkResult:CheckResult_List1)
            {
                if(checkResult->m_patient->m_id==patient->m_id)
                    CheckResult_List2.append(checkResult);
            }
        }
        qx::dao::delete_by_id(CheckResult_List2);
    }

    static void createData()
    {

        Patient_ptr patient_1,patient_2;
        CheckResult_ptr checkResult_1,checkResult_2,checkResult_3,checkResult_4;
        Program_ptr program_1,program_2;
        program_1.reset(new Program);
        program_2.reset(new Program(4,"30-2","program2 params","program2 data"));

        program_1->m_id=1;
        program_1->m_data="program1data";
        program_1->m_name="24-2";
        program_1->m_type=4;
        program_1->m_params="ttt";
        program_1->m_data="bbbb";

        patient_1.reset(new Patient("5001","lzt",Patient::sex::male,QDate::currentDate()));
        patient_2.reset(new Patient("5003","yangzhiqun",Patient::sex::female,QDate::currentDate()));


//        checkResult_1.reset(new CheckResult(1,"params1","data1",QDateTime::currentDateTime(),patient_1,program_1));
//        checkResult_2.reset(new CheckResult(2,"params2","data2",QDateTime::currentDateTime(),patient_2,program_1));
//        checkResult_3.reset(new CheckResult(3,"params3","data3",QDateTime::currentDateTime(),patient_1,program_2));
//        checkResult_4.reset(new CheckResult(4,"params4","data4",QDateTime::currentDateTime(),patient_2,program_2));

        QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
        bool bCommit = db.transaction();

        QSqlError daoError;
        daoError = qx::dao::insert(program_1, &db);
        daoError = qx::dao::insert(program_2, &db);
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();

        bCommit = db.transaction();
        daoError = qx::dao::insert(patient_1, &db);
        daoError = qx::dao::insert(patient_2, &db);
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();

        bCommit = db.transaction();
        daoError = qx::dao::insert(checkResult_1, &db);
        daoError = qx::dao::insert(checkResult_2, &db);
        daoError = qx::dao::insert(checkResult_3, &db);
        daoError = qx::dao::insert(checkResult_4, &db);
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();


    }


    static void  createEntityData()
    {
        ProgramModel<Type::ThreshHold> pm;
        pm.m_name="30-2";
        using strategy=StaticParams::CommonParams::Strategy;
        pm.m_data.strategies={strategy::fullThreshold,strategy::fastInterative};
        pm.m_data.dots={{2,3},{55,2}};
        pm.m_category=Category::Custom;
        auto pm_ptr=pm.ModelToDB();

        Patient_ptr pp_ptr(new Patient("50022","lzt",Patient::sex::male,QDate::currentDate()));

        CheckResultModel<Type::ThreshHold> crm;
        crm.m_type=Type::ThreshHold;
        crm.m_data={1,2,3,4,10,5,3,2,3,1,3,{3,2,3,2},{11,223,11,22,33,22},{2,4,2,3,5,2,33,55,32,33}};
        crm.m_time=QDateTime::currentDateTime();
        auto cr_ptr=crm.ModelToDB();

        QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
        bool bCommit = db.transaction();

        QSqlError daoError;
        daoError = qx::dao::insert(pp_ptr, &db);
        daoError = qx::dao::insert(pm_ptr, &db);
        daoError = qx::dao::insert(cr_ptr, &db);
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();
    }

    static void  GetEntityData()
    {
        CheckResult_List CheckResult_List;

        qx_query query("select * from checkResult");
        QSqlError daoError = qx::dao::execute_query(query, CheckResult_List);

//        std::cout<<"*****checkResult_list****"<<std::endl;
//        for(auto& i:CheckResult_List)
//            std::cout<<i->m_id<<" "<<" "<<i->m_params.toStdString()<<" "<<i->m_data.toStdString()<<" "
//                     <<i->m_time.toString("yyyy-MM-dd HH:mm::ss").toStdString()<<" "<<i->m_patient->m_patientId.toStdString()<<" "<<i->m_program->m_id<<std::endl;

        auto checkResult_ptr=CheckResult_List.front();
        std::cout<<checkResult_ptr->m_params.toStdString()<<std::endl;
        std::cout<<checkResult_ptr->m_data.toStdString()<<std::endl;

        query.clear();
        query.where("program_id").isEqualTo(int(checkResult_ptr->m_program->m_id));
        daoError = qx::dao::fetch_by_query(query, checkResult_ptr->m_program);;

        std::cout<<checkResult_ptr->m_program->m_name.toStdString()<<std::endl;


        query.clear();
        query.where("id").isEqualTo(int(checkResult_ptr->m_patient->m_id));
        daoError = qx::dao::fetch_by_query(query, checkResult_ptr->m_patient);

        std::cout<<checkResult_ptr->m_patient->m_name.toStdString()<<std::endl;


    }

    static void createPatientsData()
    {
        QxPack::IcLCG  lcg;
        Patient_ptr patient_ptr,patient_1,patient_2;
        CheckResult_ptr checkResult_1,checkResult_2,checkResult_3,checkResult_4;
        Program_ptr program_1,program_2;
        program_1.reset(new Program);
        program_2.reset(new Program(2,2,"30-2","program2 params","program2 data"));

        program_1->m_id=1;
        program_1->m_data="program1data";
        program_1->m_name="24-2";
        program_1->m_type=3;
        program_1->m_params="ttt";
        program_1->m_data="bbbb";
        patient_1.reset(new Patient("5001","lzt",Patient::sex::male,QDate::fromString("1988-05-11","yyyy-MM-dd")));
        patient_2.reset(new Patient("5003","yangzhiqun",Patient::sex::female,QDate::fromString("1988-05-11","yyyy-MM-dd")));

//        checkResult_1.reset(new CheckResult(1,"params1","data1",QDateTime::currentDateTime(),patient_1,program_1));
//        checkResult_2.reset(new CheckResult(2,"params2","data2",QDateTime::currentDateTime(),patient_2,program_1));
//        checkResult_3.reset(new CheckResult(3,"params3","data3",QDateTime::currentDateTime(),patient_1,program_2));
//        checkResult_4.reset(new CheckResult(4,"params4","data4",QDateTime::currentDateTime(),patient_2,program_2));

        QSqlDatabase db = qx::QxSqlDatabase::getDatabase();
        bool bCommit = db.transaction();

        QSqlError daoError;
        daoError = qx::dao::insert(program_1, &db);
        daoError = qx::dao::insert(program_2, &db);

        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();

        bCommit = db.transaction();
    //    daoError = qx::dao::insert(patient_1, &db);
    //    daoError = qx::dao::insert(patient_2, &db);
        for(int i=0;i<100;i++)
        {
            QDate date = QDate::currentDate();
            date=date.addYears(-(lcg.value()%80)-1).addMonths(-(lcg.value()%12)).addDays(-(lcg.value()%31));
            qDebug()<<QString("year:%1,month:%2,day:%3").arg(date.year()).arg(date.month()).arg(date.day());
            QDateTime time = QDateTime::currentDateTime();
            int year,month,day;
            year=-lcg.value()%(time.date().year()-date.year());
            month=-lcg.value()%12;
            day=-lcg.value()%31;
            time = time.addYears(year).addMonths(month).addDays(day);
            qDebug()<<QString("year:%1,month:%2,day:%3").arg(time.date().year()).arg(time.date().month()).arg(time.date().day());

            QString nm = QString("%1%2%3").arg( QChar( ( lcg.value() % 26 ) + 'A' ))
                             .arg( QChar( ( lcg.value() % 26 ) + 'a' ))
                             .arg( QChar( ( lcg.value() % 26 ) + 'a' ));
            QString id = QString("PID_%1").arg( lcg.value() % 10000 );
            patient_ptr.reset(new Patient(id ,nm,Patient::sex(lcg.value()%3),date,time));
            daoError = qx::dao::insert(patient_ptr, &db);
        }
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();

        bCommit = db.transaction();
        daoError = qx::dao::insert(checkResult_1, &db);
        daoError = qx::dao::insert(checkResult_2, &db);
        daoError = qx::dao::insert(checkResult_3, &db);
        daoError = qx::dao::insert(checkResult_4, &db);
        bCommit = (bCommit && ! daoError.isValid());
        qAssert(bCommit);
        db.commit();
    }

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



};
