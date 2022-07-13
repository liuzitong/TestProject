#include "testclass.h"

#include <QJsonArray>
#include <qfile.h>
#include <qjsondocument.h>
#include <QJsonObject>
#include <perimeter/main/model/checkResultModel.h>
#include <perimeter/main/viewModel/checkResultVm.h>
#include "perimeter/main/viewModel/settings.h"
namespace Perimeter {
void TestClass::test()
{

    using Params=StaticParams::CommonParams;
    CheckResultModel<Type::Move> checkResultModel;
    checkResultModel.m_type=Type::Move;
    checkResultModel.m_OS_OD=0;
    checkResultModel.m_params={{}};
    checkResultModel.m_data={0,2,10,4,10,3,10,3,6,100,50,30,30,{4,8,10,11,15},{{-9,30},{30,20},{30,-10},{-20,-10}}};
    checkResultModel.m_time=QDateTime::currentDateTime();
    checkResultModel.m_patient_id=1;
    checkResultModel.m_program_id=1;
    auto ptr=checkResultModel.ModelToDB();

    qx::dao::insert(ptr);


}
}
