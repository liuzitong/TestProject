#ifndef PROGRAMVM_H
#define PROGRAMVM_H
#include <QObject>
#include <perimeter/main/model/programModel.h>
#include <perimeter/main/viewModel/paramsVm.h>
#include <QVector>
#include <QPoint>
#include <perimeter/main/database/program.h>
#include <qsharedpointer.h>
namespace Perimeter
{
class StaticProgramVM:public QObject
{
    Q_OBJECT
    Q_INVOKABLE explicit StaticProgramVM(const QVariantList &);
    Q_INVOKABLE virtual ~StaticProgramVM() Q_DECL_OVERRIDE=default;
private:
    long m_id;
    int m_type;
    QString m_name;
    StaticParamsVM* m_staticParamsVm;
    QVector<int> m_strategies;
    QVector<QPoint> m_dots;

    QSharedPointer<ProgramModel<Type::ThreshHold>> programModel;
};

class MoveProgramVM:public QObject
{
    Q_OBJECT
    Q_INVOKABLE explicit MoveProgramVM(const QVariantList &);
    Q_INVOKABLE virtual ~MoveProgramVM() Q_DECL_OVERRIDE=default;
private:
    long m_id;
    int m_type;
    QString m_name;
    MoveParamVM* m_moveParamsVm;
    QVector<QPoint> m_dots;

    QSharedPointer<ProgramModel<Type::Move>> programModel;
};
}


#endif // PROGRAMVM_H
