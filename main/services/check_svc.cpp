#include "check_svc.h"
#include <QThread>
namespace Perimeter{

class CheckSvcWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckSvcWorker(int& m_checkState,PatientVm* patientVm,QObject* programVm,CheckResultVm* checkResultVm);
    virtual ~CheckSvcWorker() Q_DECL_OVERRIDE;
public slots:
  void doWork()
  {
      while(true)
      {
          QString result;
          switch (m_checkState)
          {
          case 0:{initialize();m_checkState=1;break;}
          case 1:{check();break;}
          case 2:{qDebug()<<("pausing");thread()->msleep(100); break;}
          case 3:{qDebug()<<("stopped");goto exit;}
          case 4:{/*m_checkResultVm->insertCheckResult()*/qDebug()<<("finished");m_checkState=4;goto exit;}
          };
      }
      exit:
      thread()->terminate();
  }

private:
  void initialize(){qDebug()<<("initializing");}
  void check(){qDebug()<<("checking");thread()->msleep(100);}


  int& m_checkState;
  PatientVm* m_patientVm;
  QObject* m_programVm;
  CheckResultVm* m_checkResultVm;
};




void CheckSvc::start()
{
    qDebug("start");
    m_checkState=0;
    if(m_checkResultVm->getType()==0)
    {
        auto params=static_cast<StaticParamsVM*>(m_checkResultVm->getParams());
    }else
    {
        auto params=static_cast<MoveParamsVM*>(m_checkResultVm->getParams());
        qDebug()<<params->getBrightness();
    }
    CheckSvcWorker *worker = new CheckSvcWorker(m_checkState,m_patientVm,m_programVm,m_checkResultVm);
    worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(&m_workerThread, &QThread::started, worker, &CheckSvcWorker::doWork);
    m_workerThread.start();
}

void CheckSvc::pause()
{
    m_checkState=2;
}

void CheckSvc::resume()
{
    m_checkState=1;
}

void CheckSvc::stop()
{
    m_checkState=3;
}

CheckSvcWorker::CheckSvcWorker(int &m_checkState, PatientVm *patientVm, QObject *programVm, CheckResultVm *checkResultVm):m_checkState(m_checkState),m_patientVm(patientVm),m_programVm(programVm),m_checkResultVm(checkResultVm){}

CheckSvcWorker::~CheckSvcWorker() {}



}
#include "check_svc.moc"

