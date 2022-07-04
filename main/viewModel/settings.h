#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
namespace Perimeter
{
class Settings :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hospitalName READ getHospitalName WRITE setHospitalName NOTIFY hospitalNameChanged)
    Q_PROPERTY(QString language READ getLanguage WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString version READ getVersion WRITE setVersion NOTIFY versionChanged)
    Q_PROPERTY(QString deviceInfo READ getDeviceInfo WRITE setDeviceInfo NOTIFY deviceInfoChanged)
    Q_PROPERTY(bool doubleName READ getDoubleName WRITE setDoubleName NOTIFY doubleNameChanged)

public:
    Settings();
    Q_INVOKABLE void save();
    QString getHospitalName(){return m_hospitalName;}void setHospitalName(QString value){m_hospitalName=value;emit hospitalNameChanged(value);}Q_SIGNAL void hospitalNameChanged(QString value);
    QString getLanguage(){return m_language;}void setLanguage(QString value){m_language=value;emit languageChanged(value);} Q_SIGNAL void languageChanged(QString value);
    QString getVersion(){return m_version;}void setVersion(QString value){m_version=value;emit versionChanged(value);}Q_SIGNAL void versionChanged(QString value);
    QString getDeviceInfo(){return m_deviceInfo;}void setDeviceInfo(QString value){m_deviceInfo=value;emit deviceInfoChanged(value);}Q_SIGNAL void deviceInfoChanged(QString value);
    bool getDoubleName(){return m_doubleName;}void setDoubleName(bool value){m_doubleName=value;emit doubleNameChanged(value);}Q_SIGNAL void doubleNameChanged(bool value);

public:
    QString m_hospitalName;
    QString m_language;
    QString m_version;
    QString m_deviceInfo;
    bool m_doubleName;
};
}
#endif // SETTINGS_H
