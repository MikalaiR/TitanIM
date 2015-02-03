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

QStringList Utils::_translitCyr = (QStringList()
                           << "а" << "б" << "в" << "г" << "д" << "е" << "ё" << "ж" << "з" << "и" << "й"
                           << "к" << "л" << "м" << "н" << "о" << "п" << "р" << "с" << "т" << "у" << "ф"
                           << "х" << "ц" << "ч" << "ш" << "щ" << "ъ" << "ы" << "ь" << "э" << "ю" << "я");

QStringList Utils::_translitLat = (QStringList()
                           << "a" << "b" << "v" << "g" << "d" << "e" << "yo" << "zh" << "z" << "i" << "j"
                           << "k" << "l" << "m" << "n" << "o" << "p" << "r" << "s" << "t" << "u" << "f"
                           << "h" << "c" << "ch" << "sh" << "sch" << "'" << "y" << "'" << "e" << "yu" << "ya");

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

QString Utils::join(const QList<QString> &list, const QChar &separator)
{
    QString s;
    QListIterator<QString> i(list);
    while (i.hasNext())
    {
        s.append(i.next() % separator);
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

QString Utils::toHtmlEscaped(const QString &str)
{
    QString text;
    const int len = str.length();
    text.reserve(int(len * 1.1));

    for (int i = 0; i < len; ++i) {
        if (str.at(i) == QLatin1Char('<'))
            text += QLatin1String("&lt;");
        else if (str.at(i) == QLatin1Char('>'))
            text += QLatin1String("&gt;");
        else if (str.at(i) == QLatin1Char('&'))
            text += QLatin1String("&amp;");
        else if (str.at(i) == QLatin1Char('"'))
            text += QLatin1String("&quot;");
        else
            text += str.at(i);
    }

    text.squeeze();
    return text;
}

QString Utils::fromHtmlEscaped(const QString &str)
{
    QString text = str;

    text.replace("&lt;", "<");
    text.replace("&gt;", ">");
    text.replace("&quot;", "\"");
    text.replace("&amp;", "&");

    return text;
}

QString Utils::simplified(const QString &str)
{
    QString text = str.simplified();

    text.replace("<!--StartFragment--> ", "<!--StartFragment-->");
    text.replace(" <!--EndFragment-->", "<!--EndFragment-->");

    return text;
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

QString Utils::fileSizeToText(int size)
{
    static QString si[9] = {QObject::tr("Bytes"),
                           QObject::tr("KB"),
                           QObject::tr("MB"),
                           QObject::tr("GB"),
                           QObject::tr("TB")};

    int i;
    for (i = 0; size >= 1024 && i < 4; i++)
        size = size / 1024;

    return QString("%1 %2").arg(size).arg(si[i]);
}

QString Utils::toTranslit(const QString &str)
{
    QString result = str;

    for (int i = 0; i < _translitCyr.count(); i++)
    {
        result.replace(_translitCyr.at(i), _translitLat.at(i));
    }

    return result;
}

QString Utils::fromTranslit(const QString &str)
{
    QString result = str;

    for (int i = 0; i < _translitCyr.count(); i++)
    {
        result.replace(_translitLat.at(i), _translitCyr.at(i));
    }

    return result;
}
