#include "PatientVm.h"
#include "perimeter/main/database/patient.h"
#include "perimeter/base/common/perimeter_def.h"
#include "perimeter/base/common/perimeter_memcntr.hxx"
#include "perimeter/main/model/rx.h"
#include "perimeter/main/model/utility.h"
#include <QDate>
#define T_PrivPtr( o )  perimeter_objcast( Patient*, o )

namespace Perimeter {
class RxVm:public QObject
{
    Q_OBJECT
    Q_PROPERTY(float rx1_r READ rx1_r WRITE setRx1_r NOTIFY rx1_rChanged)
    Q_PROPERTY(float rx2_r READ rx2_r WRITE setRx2_r NOTIFY rx2_rChanged)
    Q_PROPERTY(float rx3_r READ rx3_r WRITE setRx3_r NOTIFY rx3_rChanged)
    Q_PROPERTY(float rx1_l READ rx1_l WRITE setRx1_l NOTIFY rx1_lChanged)
    Q_PROPERTY(float rx2_l READ rx2_l WRITE setRx2_l NOTIFY rx2_lChanged)
    Q_PROPERTY(float rx3_l READ rx3_l WRITE setRx3_l NOTIFY rx3_lChanged)

public:
    Q_INVOKABLE explicit RxVm(Rx rx)
    {
        m_rx1_l=rx.rx1_l;
        m_rx2_l=rx.rx2_l;
        m_rx3_l=rx.rx3_l;

        m_rx1_r=rx.rx1_r;
        m_rx2_r=rx.rx2_r;
        m_rx3_r=rx.rx3_r;
    }

    Q_INVOKABLE virtual ~RxVm() Q_DECL_OVERRIDE=default;
    Rx getData()
    {
        return Rx{m_rx1_l,m_rx2_l,m_rx3_l,m_rx1_r,m_rx2_r,m_rx3_r};
    }

    float rx1_r(){return m_rx1_r;}void setRx1_r(float value){m_rx1_r=value;}Q_SIGNAL void rx1_rChanged(float value);
    float rx2_r(){return m_rx2_r;}void setRx2_r(float value){m_rx2_r=value;}Q_SIGNAL void rx2_rChanged(float value);
    float rx3_r(){return m_rx3_r;}void setRx3_r(float value){m_rx3_r=value;}Q_SIGNAL void rx3_rChanged(float value);

    float rx1_l(){return m_rx1_l;}void setRx1_l(float value){m_rx1_l=value;}Q_SIGNAL void rx1_lChanged(float value);
    float rx2_l(){return m_rx2_l;}void setRx2_l(float value){m_rx2_l=value;}Q_SIGNAL void rx2_lChanged(float value);
    float rx3_l(){return m_rx3_l;}void setRx3_l(float value){m_rx3_l=value;}Q_SIGNAL void rx3_lChanged(float value);

private:
    float m_rx1_r,m_rx2_r,m_rx3_r,m_rx1_l,m_rx2_l,m_rx3_l;
};


void PatientVm::hello()
{
    qDebug()<<"patient says hello.";
}




PatientVm::PatientVm(const QVariantList & args)
{
    qDebug()<<"constructor";
    if(args.count()==0)
    {
        return;
    }
    else
    {
        Patient_List Patient_List;
        Patient_ptr patient_ptr;
        int id=args[0].toInt();
        qx_query query("select * from patient where id=:id");
        query.bind(":id",id);
        QSqlError daoError = qx::dao::execute_query(query, Patient_List);
        if(Patient_List.count()==0)
            return;
        else
        {
            patient_ptr=Patient_List.first();
            m_id=patient_ptr->m_id;
            m_patientID=patient_ptr->m_patientId;
            m_name=patient_ptr->m_name;
            m_sex=int(patient_ptr->m_sex);
            m_rx=new RxVm(Utility::QStringToEntity<Rx>(patient_ptr->m_rx));
            m_birthDate=patient_ptr->m_birthDate;
            m_lastUpdate=patient_ptr->m_lastUpdate;

        }
    }


}

PatientVm::~PatientVm()
{
    if(m_rx!=nullptr) {delete m_rx;m_rx=nullptr;}
}

void PatientVm::update()
{
    m_lastUpdate=QDateTime::currentDateTime();
    QString rx=Utility::entityToQString(((RxVm*)m_rx)->getData());
    auto patient_ptr=QSharedPointer<Patient>(new Patient(m_id,m_patientID,m_name,Patient::sex(m_sex),m_birthDate,rx,m_lastUpdate));
    qx::dao::update(patient_ptr);
}

void PatientVm::insert()
{
    m_lastUpdate=QDateTime::currentDateTime();
    QString rx=Utility::entityToQString(((RxVm*)m_rx)->getData());
    m_lastUpdate=QDateTime::currentDateTime();
    auto patient_ptr=QSharedPointer<Patient>(new Patient(m_patientID,m_name,Patient::sex(m_sex),m_birthDate,rx,m_lastUpdate));
    qx::dao::insert(patient_ptr);
}






//PatientVm &PatientVm::operator=(const PatientVm &other)
//{
//    this->m_patient=other.m_patient;
//    return *this;
//}

//PatientVm::PatientVm(PatientVm &&other)
//{
//    this->m_patient=other.m_patient;
//    other.m_patient=nullptr;
//}

//PatientVm::PatientVm(const PatientVm &other)
//{
//    this->m_patient=other.m_patient;
//}

//PatientVm::~PatientVm()
//{
//    if(m_patient!=nullptr)
//        perimeter_delete(m_patient,Patient);
//}

long PatientVm::getID()
{
    return  m_id;
}

void PatientVm::setID(long value)
{
    m_id=value;
}

QString PatientVm::getPatientID()
{
   return  m_patientID;
}

void PatientVm::setPatientID(QString value)
{
    m_patientID=value;
}

QString PatientVm::getName()
{
    return  m_name;
}

void PatientVm::setName(QString value)
{
   m_name=value;
}

int PatientVm::getSex()
{
    return m_sex;
}

void PatientVm::setSex(int value)
{
    m_sex=value;
}

QString PatientVm::getBirthDate()
{
    return  m_birthDate.toString("yyyy-MM-dd");
}

void PatientVm::setBirthDate(QString date)
{
    QList<QString> list=date.split("-");
    int year=list[0].toInt();
    int month=list[1].toInt();
    int day=list[2].toInt();
    m_birthDate.setDate(year,month,day);
    qDebug()<<year;
    qDebug()<<month;
    qDebug()<<day;
}

QDateTime PatientVm::getLastUpdate()
{
    return m_lastUpdate;
}

void Perimeter::PatientVm::setLastUpdate(QDateTime value)
{
    m_lastUpdate=value;
}

int PatientVm::getAge() const
{
    int age;
    auto currentDate=QDateTime::currentDateTime().date();
    age = currentDate.year()- m_birthDate.year();
    if (currentDate.month() < m_birthDate.month() || (currentDate.month() == m_birthDate.month() && currentDate.day() < m_birthDate.day())) { age--;}
    return age;
}

}

#include "patientVm.moc"
