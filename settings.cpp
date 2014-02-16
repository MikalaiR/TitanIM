#include "settings.h"

Settings *Settings::aInstance = 0;

Settings *Settings::instance()
{
    if (!aInstance) aInstance = new Settings();
    return aInstance;
}

void Settings::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

Settings::Settings()
{
    if (QFile::exists(QCoreApplication::applicationDirPath() + "/settings.ini"))
    {
        _configDir = QCoreApplication::applicationDirPath();
    }
    else
    {
        _configDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + "/TitanIM";
    }

    if (QCoreApplication::applicationDirPath() == "/opt/titanim/bin")
    {
        _dataDir = "/opt/titanim/share";
    }
    else
    {
        _dataDir = QCoreApplication::applicationDirPath();
    }
}

QStringList Settings::uids()
{
    QDir dir(_configDir + "/profiles");
    QStringList uids = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    QString last = lastUid();

    if (uids.contains(last))
    {
        uids.swap(uids.indexOf(last), 0);
    }

    return uids;
}

void Settings::setCurrentUid(const QString &uid)
{
    _currentUid = uid;
    createDirs();
}

QString Settings::lastUid()
{
    return loadMain("profiles/last", "").toString();
}

void Settings::createDirs()
{
    QDir dir;

    if (!dir.exists(_configDir + "/profiles/" + _currentUid))
        dir.mkpath(_configDir + "/profiles/" + _currentUid);
    if (!dir.exists(_configDir + "/dictionaries"))
        dir.mkpath(_configDir + "/dictionaries");
}

QString Settings::configDir() const
{
    return _configDir;
}

QString Settings::dataDir() const
{
    return _dataDir;
}

void Settings::saveProfile(const QString &key, const QVariant &value)
{
    QSettings settings(_configDir + "/profiles/" + _currentUid + "/profilesettings.ini", QSettings::IniFormat);
    settings.setValue(key, value);
}

void Settings::saveMain(const QString &key, const QVariant &value)
{
    QSettings settingsMain(_configDir + "/settings.ini", QSettings::IniFormat);
    settingsMain.setValue(key, value);
}

QVariant Settings::loadProfile(const QString &key, const QVariant &defaultValue)
{
    QSettings settings(_configDir +"/profiles/" + _currentUid + "/profilesettings.ini", QSettings::IniFormat);
    return settings.value(key, defaultValue);
}

QVariant Settings::loadMain(const QString &key, const QVariant &defaultValue)
{
    QSettings settingsMain(_configDir + "/settings.ini", QSettings::IniFormat);
    return settingsMain.value(key, defaultValue);
}
