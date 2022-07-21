#include "analysislobbylistvm.h"
#include <perimeter/main/database/patient.h>
#include <perimeter/main/database/program.h>
#include <perimeter/main/database/checkResult.h>
#include <perimeter/main/model/programModel.h>
#include <perimeter/main/model/checkResultModel.h>

namespace Perimeter{
class SimpleCheckResultList:public QAbstractListModel
{
    enum SimpleCheckResultListRoles
    {
        checkResultId=Qt::UserRole+1,
        picName,
    };

    struct Data
    {
        int checkResultId;
        QString picName;
    };
    Q_OBJECT
    // QAbstractItemModel interface
public:
    SimpleCheckResultList()=default;
//    SimpleCheckResultList(QVector<int> checkResultIdList,QVector<QString> picNameList):
//        m_checkResultIdList(checkResultIdList),m_picNameList(picNameList){}
    ~SimpleCheckResultList()=default;

    virtual int rowCount(const QModelIndex &parent) const override
    {
        return m_dataList.count();
    }
    virtual QVariant data(const QModelIndex &index, int role) const override
    {
//        QVector<QString> picName={"dBDiagram.bmp","defectDepth.bmp","gray.bmp","PatterDeviation.bmp"};
        switch (role)
        {
            case SimpleCheckResultListRoles::checkResultId:return m_dataList[index.row()].checkResultId;
            case SimpleCheckResultListRoles::picName:return m_dataList[index.row()].picName;
        }
        return QVariant();
    }
    virtual QHash<int, QByteArray> roleNames() const override
    {
        QHash<int, QByteArray> roles;
        roles[checkResultId] = "checkResultId";
        roles[picName] = "picName";
        return roles;
    }
public:
    QVector<Data> m_dataList;

};








AnalysisLobbyListVm::AnalysisLobbyListVm()
{

}
AnalysisLobbyListVm::AnalysisLobbyListVm(const QVariantList &args)
{
    int patient_id=args[0].toInt();
//    m_report=args[1].toInt();
    m_imageHeight=args[1].toFloat();


    qx_query query("select * from Patient");
    Patient_List patient_List;
    QSqlError daoError = qx::dao::execute_query(query, patient_List);
    auto birthDate=patient_List.first()->m_birthDate;

    auto currentDate=QDateTime::currentDateTime().date();
    m_age = currentDate.year()- birthDate.year();
    if (currentDate.month() < birthDate.month() || (currentDate.month() ==birthDate.month() && currentDate.day() < birthDate.day()))
        { m_age--;}

//    int age=patient_List.

//    query.query("select * from Program");
//    Program_List program_List;
//    daoError = qx::dao::execute_query(query, program_List);
//    QVariantList programID;

//    for(auto &pp: program_List)
//    {
//        auto reports=Utility::QStringToEntity<std::vector<int>>(pp->m_report);
//        if(std::find(reports.begin(),reports.end(),m_report)!=reports.end())
//        {
//            programID.push_back(QVariant(int(pp->m_id)));
//        }
//    }

    query.query("select * from CheckResult").where("patient_id").isEqualTo(patient_id);
    CheckResult_List checkResult_list;
    daoError = qx::dao::execute_query(query, checkResult_list);

    generateModelListData(checkResult_list);

    m_dataList.clear();
}

void AnalysisLobbyListVm::generateModelListData(CheckResult_List checkResult_list)
{
    QVector<QDate> dateList;
    for(auto &i:checkResult_list)
    {
        if(!dateList.contains(i->m_time.date()))
        {dateList.append(i->m_time.date());}
    }

    std::sort(dateList.begin(),dateList.end(),[](const QDate& d1,const QDate& d2){
        return  !(d1<d2);
    });

    for(int i=0;i<dateList.length();i++ )
    {
        Data data;
        data.year=QString::number(dateList[i].year());
        data.monthDay=QString::number(dateList[i].month())+"/"+QString::number(dateList[i].day());
        SimpleCheckResultList* simpleCheckResultList=new SimpleCheckResultList();
        for(int i;i<checkResult_list.length();i++)
        {
            if(checkResult_list[i]->m_time.date()==dateList[i])
            {
                auto imageFileName=drawImage(checkResult_list[i]);
                simpleCheckResultList->m_dataList.append({checkResult_list[i]->m_id,imageFileName});
                data.simpleCheckResult=simpleCheckResultList;
            }
        }
        m_dataList.append(data);
    }
}

QString AnalysisLobbyListVm::drawImage(CheckResult_ptr checkResult_ptr)
{
    QString os_od=checkResult_ptr->m_OS_OD?"OS":"OD";
    QString checkTime=checkResult_ptr->m_time.time().toString("HH-mm-ss");
    qx_query query("select * from Program");
    query.where("program_id").isEqualTo(int(checkResult_ptr->m_program->m_id));
    Program_List program_List;
    QSqlError daoError = qx::dao::execute_query(query, program_List);
    auto program_ptr=program_List.first();
    QString programName=program_ptr->m_name;
    QImage image(m_imageHeight*0.8,m_imageHeight,QImage::Format_ARGB32);
    QImage diagram(m_imageHeight*0.7,m_imageHeight*0.7,QImage::Format_ARGB32);
    auto program_reports= Utility::QStringToEntity<std::vector<int>>(program_ptr->m_report);
//    if(std::find(program_reports.begin(),program_reports.end(),0))
//    {

//    }

    if(std::find(program_reports.begin(),program_reports.end(),0)!=program_reports.end()||
        std::find(program_reports.begin(),program_reports.end(),2)!=program_reports.end())        //常规分析和总览显示概率图
    {

    }
    else if(std::find(program_reports.begin(),program_reports.end(),1)!=program_reports.end())      //三合一显示量化缺损
    {

    }




//    switch ()
//    {
//    case 0:

//        break;
//    case 1:break;
//    case 2:break;
//    case 3:break;
//    }
    return "";

}

void AnalysisLobbyListVm::drawPatternPE(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{

}

void AnalysisLobbyListVm::drawDefectDepth(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{

}

void AnalysisLobbyListVm::drawScreening(CheckResultModel<Type::ThreshHold>& checkResultModel,ProgramModel<Type::ThreshHold>& programModel)
{

}

void AnalysisLobbyListVm::drawDynamic(CheckResultModel<Type::Dynamic>& checkResultModel,ProgramModel<Type::Dynamic>& programModel)
{

}




AnalysisLobbyListVm::~AnalysisLobbyListVm()
{
    for(auto& i:m_dataList)
    {
        if(i.simpleCheckResult!=nullptr)
        {
            auto temp=static_cast<SimpleCheckResultList*>(i.simpleCheckResult);
            if(temp!=nullptr) delete temp;
        }
    }
}

void AnalysisLobbyListVm::refreshData()
{

}

int AnalysisLobbyListVm::rowCount(const QModelIndex &parent) const
{
    return m_dataList.length();
}

QVariant AnalysisLobbyListVm::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case year:return m_dataList[index.row()].year;
    case monthDay:return m_dataList[index.row()].monthDay;
    case simpleCheckResult:return QVariant::fromValue(m_dataList[index.row()].simpleCheckResult);
    }
    return QVariant();
}

QHash<int, QByteArray> AnalysisLobbyListVm::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[year] = "year";
    roles[monthDay]="monthDay";
    roles[simpleCheckResult] = "simpleCheckResult";
    return roles;
}






//template<Type type>
//void AnalysisLobbyListVm::drawImage(CheckResultModel<type> checkResultModel)
//{

//}
}
#include "analysislobbylistvm.moc"
