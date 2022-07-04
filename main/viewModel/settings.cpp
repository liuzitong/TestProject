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
        m_doubleName=jo["doubleName"].toBool();
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
        {"doubleName",m_doubleName}
    };
    QJsonDocument jsonDoc;
    jsonDoc.setObject(jo);
    jsonFile.write(jsonDoc.toJson());
    jsonFile.close();
}
}
