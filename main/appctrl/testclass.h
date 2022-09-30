#ifndef TESTCLASS_H
#define TESTCLASS_H
#include "perimeter/base/common/perimeter_def.h"
#include <QByteArray>
namespace Perimeter {
class TestClass:public QObject
{
    Q_OBJECT
public:
    TestClass()=default;
    ~TestClass()=default;
    Q_INVOKABLE void test();
    void createPatientData();
    void createCheckResultData();

    void createDynamicCheckResultData();
};
}

#endif // TESTCLASS_H
