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
            std::cout<<i->m_id<<" "<<i->m_patinetId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;

        std::cout<<"*****program_list****"<<std::endl;
        for(auto& i:Program_List)
            std::cout<<i->m_id<<" "<<int(i->m_category)<<" "<<int(i->m_type)<<" "<<i->m_name.toStdString()<<" "<<i->m_data.toStdString()<<std::endl;
        std::cout<<std::endl;

        std::cout<<"*****checkResult_list****"<<std::endl;
        for(auto& i:CheckResult_List)
            std::cout<<i->m_id<<" "<<" "<<i->m_params.toStdString()<<" "<<i->m_data.toStdString()<<" "
                     <<i->m_time.toString("yyyy-MM-dd HH:mm::ss").toStdString()<<" "<<i->m_patient->m_patinetId.toStdString()<<" "<<i->m_program->m_id<<std::endl;
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

        CheckResult_ptr checkResult(new CheckResult(5,"params1","data1",QDateTime::currentDateTime(),Patient_List.front(),Program_List.front()));
        QSqlError daoError = qx::dao::insert(checkResult);

    }

    static void query()
    {
        Patient_List Patient_List;
        qx_query query("select * from patient where name=:name");
        query.bind(":name","yangzhiqun");
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);;

        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_patinetId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
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
            std::cout<<i->m_patinetId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
        std::cout<<std::endl;

        query.clear();
        query.where("name").isEqualTo("yangzhiqun");
        daoError = qx::dao::fetch_by_query(query, Patient_List);;

        std::cout<<"*****patient_list*****"<<std::endl;
        for(auto& i:Patient_List)
            std::cout<<i->m_patinetId.toStdString()<<" "<<i->m_name.toStdString()<<" "<<i->m_birthDate.toString("yyyy-MM-dd").toStdString()<<" "<<std::endl;
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
        program_2.reset(new Program(Program::Type::ThreshHold,"30-2","program2 params","program2 data"));

        program_1->m_id=1;
        program_1->m_data="program1data";
        program_1->m_name="24-2";
        program_1->m_type=Program::Type::ThreshHold;
        program_1->m_params="ttt";
        program_1->m_data="bbbb";

        patient_1.reset(new Patient("5001","lzt",Patient::sex::male,QDate::currentDate()));
        patient_2.reset(new Patient("5003","yangzhiqun",Patient::sex::female,QDate::currentDate()));


        checkResult_1.reset(new CheckResult(1,"params1","data1",QDateTime::currentDateTime(),patient_1,program_1));
        checkResult_2.reset(new CheckResult(2,"params2","data2",QDateTime::currentDateTime(),patient_2,program_1));
        checkResult_3.reset(new CheckResult(3,"params3","data3",QDateTime::currentDateTime(),patient_1,program_2));
        checkResult_4.reset(new CheckResult(4,"params4","data4",QDateTime::currentDateTime(),patient_2,program_2));

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
};
