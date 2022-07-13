#include "check_svc.h"

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
          case 0:initialize();break;
          case 1:check();break;
          case 2:printf("pausing");thread()->sleep(100); break;
          case 3:printf("stopped");return;
          case 4:/*m_checkResultVm->insertCheckResult()*/printf("finished");m_checkState=4;return;
          };
      }

  }

private:
  void initialize(){printf("initializing");}
  void check(){printf("checking");thread()->sleep(100);}
////signals:
//  void resultReady();
private:
  int& m_checkState;
  PatientVm* m_patientVm;
  QObject* m_programVm;
  CheckResultVm* m_checkResultVm;
};

CheckSvcWorker::CheckSvcWorker(int &m_checkState, PatientVm *patientVm, QObject *programVm, CheckResultVm *checkResultVm):m_checkState(m_checkState),m_patientVm(patientVm),m_programVm(programVm),m_checkResultVm(checkResultVm){}

CheckSvcWorker::~CheckSvcWorker()
{

}


void CheckSvc::start()
{
    qDebug("start");
//    m_checkState=0;
//    CheckSvcWorker *worker = new CheckSvcWorker(m_checkState,m_patientVm,m_programVm,m_checkResultVm);
//    worker->moveToThread(&m_workerThread);
////    connect(this, &CheckSvc::operate, worker, &CheckSvcWorker::doWork);
////    connect(worker, &CheckSvcWorker::resultReady, this, &CheckSvc::handleResults);
////    connect(worker, &CheckSvcWorker::resultReady, &m_workerThread, &QThread::finished);
//    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
//    connect(&m_workerThread, &QThread::start, worker, &CheckSvcWorker::doWork);
//    m_workerThread.start();
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


}
#include "check_svc.moc"

