#include "check_svc.h"
#include <QApplication>
#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
namespace Perimeter{

class CheckSvcWorker : public QObject
{
    Q_OBJECT
public:
    explicit CheckSvcWorker()
    {
        m_timer=new QTimer (this) ;
        m_timer->setInterval(200);
        connect(m_timer,&QTimer::timeout,[&](){m_staticCheckResultModel->m_data.testTimespan++;emit checkResultChanged();});
    }
//    explicit CheckSvcWorker(int* m_checkState,PatientVm* patientVm,ProgramVm* programVm,CheckResultVm* checkResultVm):m_checkState(m_checkState),m_patientVm(patientVm),m_programVm(programVm),m_checkResultVm(checkResultVm){}
    virtual ~CheckSvcWorker() Q_DECL_OVERRIDE {}
public slots:
  void setCheckState(int value)
  {
      *m_checkState=value;
//      if(value==1)
//      {
//           m_timer.start();
//      }
//      else
//      {
//           m_timer.stop();
//      }
      emit checkStateChanged();
  }
  void doWork()
  {
      *m_checkState=0;
      while(true)
      {
//          QString result;
//          QApplication::processEvents();
          qDebug()<<*m_checkState;
          switch (*m_checkState)
          {
          case 0:{initialize();setCheckState(1);break;}
          case 1:{check();break;}
          case 2:
          {
              qDebug()<<("pausing");
              m_elapsedTimer.restart();
              while(m_elapsedTimer.elapsed()<=1000)
              {
                  QApplication::processEvents();
              }
              break;
          }
          case 3:{qDebug()<<("stopped");goto exit;}
          case 4:{/*m_checkResultVm->insertCheckResult()*/qDebug()<<("finished");m_staticCheckResultVm->insert();goto exit;}
          };
      }
      exit:
         m_timer->stop();
         qDebug()<<("work end");
  }
  void pause()
  {
      setCheckState(2);m_timer->stop();
  }

  void resume()
  {
      setCheckState(1);m_timer->start();
  }

  void stop()
  {
      setCheckState(3);m_timer->stop();
  }

signals:
    void checkStateChanged();
    void checkResultChanged();
    void checkedCountChanged(int count);
private:
  void initialize()
  {
      qDebug()<<("initializing");
      m_checkResultVm->setPatient_id(m_patientVm->getID());
      m_checkResultVm->setProgram_id(m_programVm->getID());
      checkedCount=0;
      int type=m_programVm->getType();
      if(type!=3)
      {
          m_staticCheckResultVm=static_cast<StaticCheckResultVm*>(m_checkResultVm);
          m_staticProgramVm=static_cast<StaticProgramVm*>(m_programVm);
          m_staticCheckResultModel=m_staticCheckResultVm->getModel();
          m_staticProgramModel=m_staticProgramVm->getModel();

          totalCount=m_staticProgramModel->m_data.dots.size();
          m_staticCheckResultModel->m_data.checkData.resize(totalCount,-1);
          m_staticCheckResultModel->m_params=m_staticProgramModel->m_params;
      }

      m_timer->start();
      emit checkedCountChanged(checkedCount);
      emit checkResultChanged();
  }

  void check(){
      qDebug()<<("checking");
//      auto val=m_checkResultVm->getResultData()->getFalseNegativeTestCount();
//      qDebug()<<val;
//      m_checkResultVm->getResultData()->setFalseNegativeTestCount(val+1);
//      QApplication::processEvents();




//      m_staticCheckResultModel->m_data.checkData[0]=3;

      auto seed=quint32((QTime::currentTime().msecsSinceStartOfDay()));
      qsrand(seed);
      m_staticCheckResultModel->m_data.checkData[checkedCount]=qrand()%40;
//      m_staticCheckResultModel->m_data.fixationDeviation[checkedCount]=5;

      int times=qrand()%5+1;
      for(int i=0;i<times;i++)
            m_staticCheckResultModel->m_data.fixationDeviation.push_back(qrand()%21-10);
      checkedCount++;
      m_elapsedTimer.restart();
      while(m_elapsedTimer.elapsed()<=1000)
      {
          QApplication::processEvents();
      }
      if(checkedCount>=totalCount){setCheckState(4);}
      emit checkResultChanged();
//      emit m_staticCheckResultVm->getResultData()->checkDataChanged();
      emit checkedCountChanged(checkedCount);
  }



  int checkedCount,totalCount;

  StaticProgramVm *m_staticProgramVm;
  StaticCheckResultVm* m_staticCheckResultVm;

  DynamicProgramVm *m_dynamicProgramVm;
  DynamicCheckResultVm* m_dynamicCheckResultVm;

  QSharedPointer<StaticCheckResultModel> m_staticCheckResultModel;
  QSharedPointer<DynamicCheckResultModel> m_dynamicCheckResultModel;
  QSharedPointer<StaticProgramModel> m_staticProgramModel;
  QSharedPointer<DynamicProgramModel> m_dynamicProgramModel;


public:
  QTimer* m_timer;
  QElapsedTimer m_elapsedTimer;
  int* m_checkState;
  PatientVm* m_patientVm;
  ProgramVm* m_programVm;
  CheckResultVm* m_checkResultVm;
};




CheckSvc::CheckSvc(QObject *parent)
{
    m_worker = new CheckSvcWorker();
    m_worker->moveToThread(&m_workerThread);
//    connect(&m_workerThread, &QThread::finished, m_worker, &QObject::deleteLater);
//    connect(&m_workerThread, &QThread::started, m_worker, &CheckSvcWorker::doWork);
    connect(m_worker,&CheckSvcWorker::checkResultChanged,this, &CheckSvc::checkResultChanged);
    connect(m_worker,&CheckSvcWorker::checkStateChanged,this, &CheckSvc::checkStateChanged);
    connect(m_worker,&CheckSvcWorker::checkedCountChanged,this, &CheckSvc::setCheckedCount);
    m_workerThread.start();
}

CheckSvc::~CheckSvc()
{
    m_workerThread.terminate();
    m_worker->deleteLater();
}

void CheckSvc::start()
{
    m_worker->m_patientVm=m_patientVm;
    m_worker->m_programVm=m_programVm;
    m_worker->m_checkResultVm=m_checkResultVm;
    m_worker->m_checkState=&m_checkState;
    setCheckState(0);
    QMetaObject::invokeMethod(m_worker,"doWork",Qt::QueuedConnection);

}

void CheckSvc::pause()
{
    QMetaObject::invokeMethod(m_worker,"pause",Qt::QueuedConnection);

}

void CheckSvc::resume()
{
    QMetaObject::invokeMethod(m_worker,"resume",Qt::QueuedConnection);


}

void CheckSvc::stop()
{
    QMetaObject::invokeMethod(m_worker,"stop",Qt::QueuedConnection);
}







}
#include "check_svc.moc"

