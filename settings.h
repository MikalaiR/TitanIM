/*
    Copyright (c) 2013 by Ruslan Nazarov <818151@gmail.com>

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************
*/

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
