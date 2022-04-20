#include "testclass.h"
namespace Perimeter {

void TestClass::hello()
{
    qDebug()<<"hello world";
}

void TestClass::testObj(QByteArray o)
{
    qDebug()<<o.length();
    qDebug()<<int(o[0]);
    qDebug()<<int(o[1]);
    qDebug()<<int(o[2]);
    qDebug()<<int(o[3]);
}
}
