#include "progressanalysislistvm.h"
#include <perimeter/main/database/checkResult.h>

Perimeter::progressAnalysisListVm::progressAnalysisListVm(const QVariantList &args)
{

    int patientId=args[0].toInt();
    qx_query query;
    CheckResult_List checkResult_List;
    query.where("patient_id").isEqualTo(patientId);
    qx::dao::fetch_by_query(query,checkResult_List);
    m_patient=new PatientVm(QVariantList{patientId});
    m_patientAge=m_patient->getAge();
    m_program_24_2=new StaticProgramVM(QVariantList{1});
    m_program_30_2=new StaticProgramVM(QVariantList{4});


}

Perimeter::progressAnalysisListVm::~progressAnalysisListVm()
{
    if(m_patient!=nullptr) delete m_patient;
    m_patient=nullptr;
}

void Perimeter::progressAnalysisListVm::removeCheckResult(int checkResultId)
{


}

void Perimeter::progressAnalysisListVm::initialize()
{
    QFile jsonFile("./data.json");
    if( !jsonFile.open(QIODevice::ReadOnly)) {qDebug() << "read file error!";return;}
    QJsonParseError jsonParserError;
    auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
    m_jsonArray=JsonDoc.array();
    jsonFile.close();

}

int Perimeter::progressAnalysisListVm::rowCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant Perimeter::progressAnalysisListVm::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
