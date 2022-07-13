#ifndef CheckSvc_H
#define CheckSvc_H

#include <perimeter/main/viewModel/patientVm.h>
#include <perimeter/main/viewModel/programVm.h>
#include <perimeter/main/viewModel/checkResultVm.h>

namespace Perimeter{

class CheckSvc: public QObject
{
    Q_OBJECT
    Q_PROPERTY(PatientVm* patient READ getPatient WRITE setPatient)
    Q_PROPERTY(QObject* program READ getProgram WRITE setProgram)
    Q_PROPERTY(CheckResultVm* checkResult READ getCheckResult WRITE setCheckResult NOTIFY checkResultChanged)
    Q_PROPERTY(int checkState READ getCheckState WRITE setCheckState NOTIFY checkStateChanged)
public:
    explicit CheckSvc(QObject *parent = nullptr){};
    ~CheckSvc()=default;
    Q_INVOKABLE void start();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void resume();
    Q_INVOKABLE void stop();

public:
    PatientVm* getPatient(){return m_patientVm;}void setPatient(PatientVm* value){m_patientVm=value;}
    QObject* getProgram(){return m_programVm;}void setProgram(QObject* value){m_programVm=value;}
    CheckResultVm* getCheckResult(){return m_checkResultVm;}void setCheckResult(CheckResultVm* value){m_checkResultVm=value;} Q_SIGNAL void checkResultChanged();
    int getCheckState(){return m_checkState;}void setCheckState(int value){m_checkState=value;emit checkStateChanged();}Q_SIGNAL void checkStateChanged();
//public slots:
//  void handleResults(const QString &);
//signals:
//  void operate(const QString &);

private:
    QThread m_workerThread;
    int m_checkState;                        //0:start,1:checking,2:pausing,3:abort,4:finished
    PatientVm* m_patientVm;
    QObject* m_programVm;
    CheckResultVm* m_checkResultVm;
};
}
#endif // CheckSvc_H
