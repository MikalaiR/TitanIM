#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QStandardPaths>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings *instance();
    static void destroy();
    QStringList uids();
    void setCurrentUid(const QString &uid);
    QString lastUid();
    QString configDir() const;
    QString dataDir() const;
    void saveProfile(const QString &key, const QVariant &value);
    void saveMain(const QString &key, const QVariant &value);
    QVariant loadProfile(const QString &key, const QVariant &defaultValue);
    QVariant loadMain(const QString &key, const QVariant &defaultValue);

private:
    Settings();
    void createDirs();

private:
    static Settings *aInstance;
    QString _configDir;
    QString _dataDir;
    QString _currentUid;
};

#endif // SETTINGS_H
