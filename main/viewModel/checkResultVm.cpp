#include "checkResultVm.h"
#include "paramsVm.h"
namespace Perimeter
{

StaticCheckResultVm::StaticCheckResultVm(const QVariantList &args)
{
    int checkResult_id=args[0].toInt();
    CheckResult_ptr checkResult_ptr(new CheckResult());
    checkResult_ptr->m_id=checkResult_id;
    qx::dao::fetch_by_id(checkResult_ptr);
    m_data.reset(new StaticCheckResultModel(checkResult_ptr));
    CheckResultVm::setData(m_data.data());

    m_params.reset(new StaticParamsVM(&m_data->m_params));
    m_resultData.reset(new StaticResultDataVm(&m_data->m_data));
}

void StaticCheckResultVm::insert()
{
    auto sp=m_data->ModelToDB();
    qx::dao::insert(sp);
}

void StaticCheckResultVm::update()
{
    auto sp=m_data->ModelToDB();
    qx::dao::update(sp);
}

DynamicCheckResultVm::DynamicCheckResultVm(const QVariantList &args)
{
    int checkResult_id=args[0].toInt();
    CheckResult_ptr checkResult_ptr(new CheckResult());
    checkResult_ptr->m_id=checkResult_id;
    qx::dao::fetch_by_id(checkResult_ptr);
    m_data.reset(new DynamicCheckResultModel(checkResult_ptr));
    CheckResultVm::setData(m_data.data());

    m_params.reset(new DynamicParamsVM(&m_data->m_params));
    m_resultData.reset(new DynamicResultDataVm(&m_data->m_data));
}

void DynamicCheckResultVm::insert()
{
    auto sp=m_data->ModelToDB();
    qx::dao::insert(sp);
}

void DynamicCheckResultVm::update()
{
    auto sp=m_data->ModelToDB();
    qx::dao::update(sp);
}


}
