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

#ifndef UTILS_H
#define UTILS_H

#include <QJsonDocument>
#include <QCryptographicHash>
#include <QStringBuilder>
#include <QVariant>
#include <QMap>
#include <QUrl>
#include <QDateTime>
#include <QVector>
#include <QSound>
#include <QProcess>
#include "global.h"

class Utils
{
public:
    static QVariant parseJSON(const QByteArray &data);
    static QString getMd5String(const QString &source);
    static QString join(const QMap<QString, QString> &list, const QChar &separator);
    static QString join(const QList<QString> &list, const QChar &separator);
    static QString joinAndEscape(const QMap<QString, QString> &list, const QChar &separator);
    static QString toHtmlEscaped(const QString &str);
    static QString fromHtmlEscaped(const QString &str);
    static QString simplified(const QString &str);
    static QString dateToText(const QDateTime &dateTime);
    static QString dateToSection(const QDateTime &dateTime);
    static QString firstUpper(const QString &str);
    static QString pluralForm(const int n, const QString &form1, const QString &form2, const QString &form5);
    static QString lastSeenToString(const uint lastSeen, const Sex sex=Unknown);
    static QString peopleConversation(const int count);
    static QVector<int> toVectorInt(const QVariantList &list);
    static void playSound(const QString &fileName, const QString &cmd="aplay -q");
    static QDateTime currentDateTime();
    static void setServerDateTime(const QDateTime &dateTime);
    static QString fileSizeToText(int size);

private:
    static qint64 _lambdaServerTime;
};

#endif // UTILS_H
