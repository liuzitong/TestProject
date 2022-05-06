#ifndef CHECKRESULTVM_H
#define CHECKRESULTVM_H
#include <QObject>
namespace Perimeter
{

class CheckResultVm: public QObject
{
public:


    CheckResultVm();
};

class StaticCheckResultVm:public CheckResultVm
{

};


class MoveCheckResultVm:public CheckResultVm
{

};
}
#endif // CHECKRESULTVM_H
