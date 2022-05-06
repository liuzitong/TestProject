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
    Q_PROPERTY(long id READ getID WRITE setID)
    Q_PROPERTY(int type READ getType WRITE setType)
    Q_PROPERTY(QString name READ getName WRITE setName);
    Q_PROPERTY(QObject* params READ getParams)
    Q_PROPERTY(QVariantList strategies READ getStrategies  WRITE setStrategies)
    Q_PROPERTY(QVariantList dots READ getDots WRITE setDots )
    Q_PROPERTY(int category READ getCategory WRITE setCategory)

public:
    Q_INVOKABLE explicit StaticProgramVM(const QVariantList &);
    Q_INVOKABLE virtual ~StaticProgramVM();
    Q_INVOKABLE void updateProgram();
    Q_INVOKABLE void insertProgram();
    Q_INVOKABLE void deleteProgram();

    long getID(){return m_id;}void setID(int value){m_id=value;}
    long getType(){return m_type;}void setType(int value){m_type=value;}
    QString getName(){return m_name;}void setName(QString value){m_name=value;}
    QObject* getParams(){return m_staticParamsVm;}
    QVariantList getStrategies(){return m_strategies;}void setStrategies(QVariantList value){m_strategies=value;}
    QVariantList getDots(){return m_dots;}void setDots(QVariantList value){m_dots=value;}
    int getCategory(){return m_category;}void setCategory(int value){m_category=value;}

private:
    long m_id;
    int m_type;
    QString m_name;
    StaticParamsVM* m_staticParamsVm;
    QVariantList m_strategies;
    QVariantList m_dots;
    int m_category;

    Program_ptr getProgramData();

//    QSharedPointer<ProgramModel<Type::ThreshHold>> programModel;
    Q_DISABLE_COPY(StaticProgramVM);
};

class MoveProgramVM:public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit MoveProgramVM(const QVariantList &);
    Q_INVOKABLE virtual ~MoveProgramVM();
    Q_INVOKABLE void updateProgram();
    Q_INVOKABLE void insertProgram();
    Q_INVOKABLE void deleteProgram();

    Q_PROPERTY(long id READ getID WRITE setID);
    Q_PROPERTY(int type READ getType WRITE setType);
    Q_PROPERTY(QString name READ getName WRITE setName);
    Q_PROPERTY(QObject* params READ getParams);
    Q_PROPERTY(QVariantList dots READ getDots WRITE setDots);
    Q_PROPERTY(int category READ getCategory WRITE setCategory)

    long getID(){return m_id;}void setID(int value){m_id=value;}
    long getType(){return m_type;}void setType(int value){m_type=value;}
    QString getName(){return m_name;}void setName(QString value){m_name=value;}
    QObject* getParams(){return m_moveParamsVm;}
    QVariantList getDots(){return m_dots;}void setDots(QVariantList value){m_dots=value;}
    int getCategory(){return m_category;}void setCategory(int value){m_category=value;}
private:
    long m_id;
    int m_type;
    QString m_name;
    MoveParamVM* m_moveParamsVm;
    QVariantList m_dots;
    int m_category;

    Program_ptr getProgramData();
//    QSharedPointer<ProgramModel<Type::Move>> programModel;
    Q_DISABLE_COPY(MoveProgramVM);
};
}


#endif // PROGRAMVM_H
