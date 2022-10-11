#include "check_svc.h"
#include <QApplication>
#include <QThread>
namespace Perimeter{

class CheckSvcWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckSvcWorker(int* m_checkState,PatientVm* patientVm,QObject* programVm,CheckResultVm* checkResultVm):m_checkState(m_checkState),m_patientVm(patientVm),m_programVm(programVm),m_checkResultVm(checkResultVm){}
    virtual ~CheckSvcWorker() Q_DECL_OVERRIDE {}
public slots:
  void setCheckState(int value)
  {
      *m_checkState=value;
      emit checkStateChanged();
  }
  void doWork()
  {
      while(true)
      {
          QString result;
          QApplication::processEvents();
          qDebug()<<*m_checkState;
          switch (*m_checkState)
          {
          case 0:{initialize();setCheckState(1);break;}
          case 1:{check();break;}
          case 2:{qDebug()<<("pausing");thread()->msleep(50); break;}
          case 3:{qDebug()<<("stopped");goto exit;}
          case 4:{/*m_checkResultVm->insertCheckResult()*/qDebug()<<("finished");goto exit;}
          };
      }
      exit:
      thread()->terminate();
  }
signals:
    void checkStateChanged();
    void checkResultChanged();
private:
  void initialize(){qDebug()<<("initializing");}
  void check(){
      qDebug()<<("checking");
//      auto val=m_checkResultVm->getResultData()->getFalseNegativeTestCount();
//      qDebug()<<val;
//      m_checkResultVm->getResultData()->setFalseNegativeTestCount(val+1);
//      QApplication::processEvents();
      thread()->msleep(50);
//      if(val>=100){setCheckState(4);}
      emit checkResultChanged();
  }


  int* m_checkState;
  PatientVm* m_patientVm;
  QObject* m_programVm;
  CheckResultVm* m_checkResultVm;
};




void CheckSvc::start()
{
    qDebug("start");
    if(m_checkResultVm->getType()!=2)
    {
//        auto params=static_cast<StaticParamsVM*>(m_checkResultVm->getParams());
    }else
    {
//        auto params=static_cast<DynamicParamsVM*>(m_checkResultVm->getParams());
//        qDebug()<<params->getBrightness();
    }
    CheckSvcWorker *worker = new CheckSvcWorker(&m_checkState,m_patientVm,m_programVm,m_checkResultVm);
    worker->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(&m_workerThread, &QThread::started, worker, &CheckSvcWorker::doWork);
    connect(this, &CheckSvc::setCheckState,worker,&CheckSvcWorker::setCheckState);
    connect(worker,&CheckSvcWorker::checkResultChanged,this, &CheckSvc::checkResultChanged);
    connect(worker,&CheckSvcWorker::checkStateChanged,this, &CheckSvc::checkStateChanged);
    m_checkState=0;
    m_workerThread.start();
}

void CheckSvc::pause()
{
    setCheckState(2);
}

void CheckSvc::resume()
{
    setCheckState(1);

    qDebug()<<m_checkState;
}

void CheckSvc::stop()
{
    setCheckState(3);
}







}
#include "check_svc.moc"

