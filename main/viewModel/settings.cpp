#include "settings.h"
#include <QJsonArray>
#include <qfile.h>
#include <qjsondocument.h>
#include <QJsonObject>
#include <QDir>
#include <QDebug>
namespace Perimeter
{
Settings::Settings()
{
    QFile jsonFile("./Settings.json");
    if(jsonFile.open(QIODevice::ReadOnly))
    {
        QJsonParseError jsonParserError;
        auto JsonDoc = QJsonDocument::fromJson(jsonFile.readAll(),&jsonParserError);
        auto jo=JsonDoc.object();
        m_hospitalName=jo["hospitalName"].toString();
        m_language=jo["language"].toString();
        m_version=jo["version"].toString();
        m_deviceInfo=jo["deviceInfo"].toString();
        m_programUnlockPwd=jo["programUnlockPwd"].toString();
        m_defaultProgramId=jo["defaultProgramId"].toInt();
        m_defaultProgramType=jo["defaultProgramType"].toInt();

        if(m_language=="Default")
        {
            QLocale local = QLocale::system();
            QLocale::Language lang = local.language();
            qDebug()<<lang;
            lang==QLocale::Chinese? setDoubleName(false):setDoubleName(true);
        }
        else if(m_language=="Chinese")
        {
            setDoubleName(false);
        }
        else if(m_language=="English")
        {
            setDoubleName(true);
        }
        jsonFile.close();
    }
}

void Settings::save()
{
    QFile jsonFile("./Settings.json");
    jsonFile.open(QIODevice::WriteOnly);
    QJsonObject jo{
        {"hospitalName",m_hospitalName},
        {"language",m_language},
        {"version",m_version},
        {"deviceInfo",m_deviceInfo},
        {"programUnlockPwd",m_programUnlockPwd},
        {"defaultProgramId",m_defaultProgramId},
        {"defaultProgramType",m_defaultProgramType}
    };
    if(m_language=="Default")
    {
        QLocale local = QLocale::system();
        QLocale::Language lang = local.language();
        lang==QLocale::Chinese? setDoubleName(false):setDoubleName(true);
    }
    else if(m_language=="Chinese")
    {
        setDoubleName(false);
    }
    else if(m_language=="English")
    {
        setDoubleName(true);
    }
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jo);
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
}
}
