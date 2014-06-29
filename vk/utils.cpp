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

#include "utils.h"

qint64 Utils::_lambdaServerTime = 0;

QVariant Utils::parseJSON(const QByteArray &data)
{
    QJsonDocument json = QJsonDocument::fromJson(data);
    return json.toVariant();
}

QString Utils::getMd5String(const QString &source)
{
    return QString(QCryptographicHash::hash(source.toUtf8(), QCryptographicHash::Md5).toHex());
}

QString Utils::join(const QMap<QString, QString> &list, const QChar &separator)
{
    QString s;
    QMapIterator<QString, QString> i(list);
    while (i.hasNext())
    {
        i.next();
        s.append(i.key() % "=" % i.value() % separator);
    }

    s.resize(s.length() - 1);
    return s;
}

QString Utils::joinAndEscape(const QMap<QString, QString> &list, const QChar &separator)
{
    QString s;
    QMapIterator<QString, QString> i(list);
    while (i.hasNext())
    {
        i.next();
        s.append(i.key() % "=" % QUrl::toPercentEncoding(i.value()) % separator);
    }

    s.resize(s.length() - 1);
    return s;
}

QString Utils::decode(const QString &str)
{
    QString temp = str;
    temp = temp.replace("<br>", "\n"); //todo vk api5
    temp = temp.replace("&amp;", "&");
    temp = temp.replace("&lt;", "<");
    temp = temp.replace("&gt;", ">");
    temp = temp.replace("&quot;", "\"");
    temp = temp.replace("&#33;", "!");
    temp = temp.replace("&#036;", "$");
    temp = temp.replace("&#092;", "\\");
    temp = temp.replace("&#39;", "'");

    return temp;
}

QString Utils::dateToText(const QDateTime &dateTime)
{
    static QString m[12] = {QObject::tr("Jan"),
                            QObject::tr("Feb"),
                            QObject::tr("Mar"),
                            QObject::tr("Apr"),
                            QObject::tr("May"),
                            QObject::tr("Jun"),
                            QObject::tr("Jul"),
                            QObject::tr("Aug"),
                            QObject::tr("Sep"),
                            QObject::tr("Oct"),
                            QObject::tr("Nov"),
                            QObject::tr("Dec")};

    int days = dateTime.daysTo(currentDateTime());

    switch (days)
    {
    case 0:
        return dateTime.time().toString("hh:mm");

    case 1:
        return QObject::tr("yesterday");

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return dateTime.date().toString("ddd");

    default:
        if (dateTime.date().year() == QDate::currentDate().year())
        {
            return QString("%1 %2").arg(dateTime.date().day()).arg(m[dateTime.date().month() - 1]);
        }
        else
        {
            return dateTime.date().toString("dd.MM.yy");
        }
    }
}

QString Utils::dateToSection(const QDateTime &dateTime)
{
    int days = dateTime.daysTo(currentDateTime());
    QString dateStr = dateTime.date().toString("d MMMM");

    switch (days)
    {
    case 0:
        return QObject::tr("Today, %1").arg(dateStr);

    case 1:
        return QObject::tr("Yesterday, %1").arg(dateStr);

    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return firstUpper(dateTime.date().toString("dddd, %1").arg(dateStr));

    default:
        if (dateTime.date().year() == QDate::currentDate().year())
        {
            return dateStr;
        }
        else
        {
            return dateTime.date().toString("d MMMM yyyy");
        }
    }
}

QString Utils::firstUpper(const QString &str)
{
    QString result = str;
    result[0] = result.at(0).toUpper();

    return result;
}

QString Utils::pluralForm(const int n, const QString &form1, const QString &form2, const QString &form5)
{
    int n1 = n % 100;
    int n2 = n1 % 10;

    if (n1 > 10 && n1 < 20)
    {
        return QString("%1 %2").arg(n).arg(form5);
    }

    if (n2 > 1 && n2 < 5)
    {
        return QString("%1 %2").arg(n).arg(form2);
    }

    if (n2 == 1)
    {
        return QString("%1 %2").arg(n).arg(form1);
    }

    return QString("%1 %2").arg(n).arg(form5);
}

QString Utils::lastSeenToString(const uint lastSeen, const Sex sex)
{
    if (lastSeen == 0)
    {
        return QObject::tr("offline");
    }

    QDateTime dateTime = QDateTime::fromTime_t(lastSeen);

    QString prefix = (sex == Woman) ? QObject::tr("last seen", "woman") : QObject::tr("last seen", "man");
    int days = dateTime.daysTo(currentDateTime());

    switch (days) {
    case 0:
    {
        int minutes = dateTime.secsTo(currentDateTime()) / 60;

        if (minutes == 0)
        {
            return QString("%1 %2")
                    .arg(prefix)
                    .arg(QObject::tr("just now"));
        }

        if (minutes < 60)
        {
            QString suffix = pluralForm(minutes, QObject::tr("minute", "1"), QObject::tr("minutes", "2"), QObject::tr("minutes", "5"));
            return QString("%1 %2 %3")
                    .arg(prefix)
                    .arg(suffix)
                    .arg(QObject::tr("ago"));
        }

        if (minutes < 240)
        {
            int hours = minutes / 60;

            QString suffix = pluralForm(hours, QObject::tr("hour", "1"), QObject::tr("hours", "2"), QObject::tr("hours", "5"));
            return QString("%1 %2 %3")
                    .arg(prefix)
                    .arg(suffix)
                    .arg(QObject::tr("ago"));
        }
        else
        {
            return QString("%1 %2 %3").arg(prefix).arg(QObject::tr("today at")).arg(dateTime.toString("h:mm"));
        }

        break;
    }

    case 1:
        return QString("%1 %2 %3").arg(prefix).arg(QObject::tr("yesterday at")).arg(dateTime.toString("h:mm"));
        break;

    default:
        return QString("%1 %2").arg(prefix).arg(dateTime.toString("d MMMM yyyy h:mm"));
    }
}

QString Utils::peopleConversation(const int count)
{
    if (count == 0)
    {
        return QObject::tr("no members");
    }

    return pluralForm(count, QObject::tr("member", "1"), QObject::tr("members", "2"), QObject::tr("members", "5"));
}

QVector<int> Utils::toVectorInt(const QVariantList &list)
{
    QVector<int> v;

    foreach (QVariant item, list) {
        v.append(item.toInt());
    }

    return v;
}

void Utils::playSound(const QString &fileName, const QString &cmd)
{
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
    QSound::play(fileName);
#else
    QProcess::startDetached(QString("%1 %2").arg(cmd).arg(fileName));
#endif
}

QDateTime Utils::currentDateTime()
{
    return QDateTime::currentDateTime().addSecs(_lambdaServerTime);
}

void Utils::setServerDateTime(const QDateTime &dateTime)
{
    _lambdaServerTime = QDateTime::currentDateTime().secsTo(dateTime);
}
