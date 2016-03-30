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

#include <QtGui/QGuiApplication>
#include <QJsonDocument>
#include <QCryptographicHash>
#include <QStringBuilder>
#include <QTextDocument>
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
    static QString toSmartLinks(const QString &str, bool *changed=0);
    static QString fromSmartLinks(const QString &str);
    static QString simplified(const QString &str);
    static QString toPlainText(const QString &richText);
    static QString dateToText(const QDateTime &dateTime);
    static QString dateToSection(const QDateTime &dateTime);
    static QString firstUpper(const QString &str);
    static QString pluralForm(const int n, const QString &form1, const QString &form2, const QString &form5);
    static QString lastSeenToString(const uint lastSeen, const int sex=0);
    static QString peopleConversation(const int count);
    static QString actionToString(const QString &author, const int act, const QString &arg, const int sex);
    static QString actionToString(const int act);
    static QVector<int> toVectorInt(const QVariantList &list);
    static void playSound(const QString &fileName, const QString &cmd="aplay -q");
    static QDateTime currentDateTime();
    static void setServerDateTime(const QDateTime &dateTime);
    static QString fileSizeToText(qint64 size);
    static QString toTranslit(const QString &str);
    static QString fromTranslit(const QString &str);
    static bool applicationIsActive();
    static QString durationToText(const int duration);

private:
    static qint64 _lambdaServerTime;
    static QStringList _translitCyr;
    static QStringList _translitLat;
};

#endif // UTILS_H
