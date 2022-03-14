﻿#ifndef PROGRAMLISTMODELVM_H
#define PROGRAMLISTMODELVM_H
#include <QAbstractListModel>
#include <perimeter/main/database/program.h>
namespace Perimeter{


class ProgramListModelVm:public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit ProgramListModelVm(const QVariantList & category);
    Q_INVOKABLE virtual ~ProgramListModelVm() Q_DECL_OVERRIDE;
//    Q_INVOKABLE void getProgramListByCategory(int category);
//    Q_INVOKABLE QObject* getProgramByProgramId();
//    Q_INVOKABLE void addProgram();
private:
    Program_List m_list;
    Q_DISABLE_COPY(ProgramListModelVm);
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

};
}

#endif // PROGRAMLISTMODELH_H
