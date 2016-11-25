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
#include "messageitem.h"
#include "profileitem.h"

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

QString Utils::toSmartLinks(const QString &str, bool *changed)
{
    QString text = str;
    bool myChanged = false;

    QRegExp rx("(https?://[_a-zA-Z0-9./~:?#!$&'+,;=%-]+)");
    rx.setCaseSensitivity(Qt::CaseInsensitive);

    int pos = 0;

    while ((pos = rx.indexIn(text, pos)) != -1)
    {
        QString link = QString("<a href=\"%1\">%1</a>").arg(rx.cap(1));
        text.replace(pos, rx.matchedLength(), link);
        pos += link.length();
        myChanged = true;
    }

    if (changed != 0)
    {
        *changed = myChanged;
    }

    return text;
}

QString Utils::fromSmartLinks(const QString &str)
{
    QString text = str;

    QRegExp rx("\\<a[^\\>]*href\\s*=\\s*\"([^\"]*)\"[^\"]*\\>");
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    text.replace(rx, "\\1");

    return text;
}

QString Utils::simplified(const QString &str)
{
    QString text = str.simplified();

    text.replace("<!--StartFragment--> ", "<!--StartFragment-->");
    text.replace(" <!--EndFragment-->", "<!--EndFragment-->");

    return text;
}

QString Utils::toPlainText(const QString &richText)
{
    QTextDocument textDocument;
    textDocument.setHtml(richText);
    return textDocument.toPlainText();
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
        return form5;
    }

    if (n2 > 1 && n2 < 5)
    {
        return form2;
    }

    if (n2 == 1)
    {
        return form1;
    }

    return form5;
}

QString Utils::lastSeenToString(const uint lastSeen, const int sex)
{
    if (lastSeen == 0)
    {
        return "";
    }

    QDateTime dateTime = QDateTime::fromTime_t(lastSeen);

    QString prefix = (sex == ProfileItemPrivate::Woman) ? QObject::tr("last seen", "woman") : QObject::tr("last seen", "man");
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
            return QString("%1 %2 %3 %4")
                    .arg(prefix)
                    .arg(minutes)
                    .arg(suffix)
                    .arg(QObject::tr("ago"));
        }

        if (minutes < 240)
        {
            int hours = minutes / 60;

            QString suffix = pluralForm(hours, QObject::tr("hour", "1"), QObject::tr("hours", "2"), QObject::tr("hours", "5"));
            return QString("%1 %2 %3 %4")
                    .arg(prefix)
                    .arg(hours)
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

    QString s = pluralForm(count, QObject::tr("member", "1"), QObject::tr("members", "2"), QObject::tr("members", "5"));

    return QString("%1 %2").arg(count).arg(s);
}

QString Utils::actionToString(const QString &author, const int act, const QString &arg, const int sex)
{
    QString text;
    bool isWoman = (sex == ProfileItemPrivate::Woman);

    switch (act) {
    case MessageItemPrivate::Chat_photo_update:
    {
        text = isWoman ? QObject::tr("changed conversation cover", "woman")
                       : QObject::tr("changed conversation cover", "man");
        break;
    }

    case MessageItemPrivate::Chat_photo_remove:
    {
        text = isWoman ? QObject::tr("deleted conversation cover", "woman")
                       : QObject::tr("deleted conversation cover", "man");
        break;
    }

    case MessageItemPrivate::Chat_create:
    {
        text = QString("%1 «%2»")
                .arg(isWoman ? QObject::tr("created the chat", "woman")
                             : QObject::tr("created the chat", "man"))
                .arg(arg);
        break;
    }

    case MessageItemPrivate::Chat_title_update:
    {
        text = QString("%1 «%2»")
                .arg(isWoman ? QObject::tr("changed the conversation name to", "woman")
                             : QObject::tr("changed the conversation name to", "man"))
                .arg(arg);
        break;
    }

    case MessageItemPrivate::Chat_invite_user:
    {
        text = QString("%1 %2")
                .arg(isWoman ? QObject::tr("invited", "woman")
                             : QObject::tr("invited", "man"))
                .arg(arg);
        break;
    }

    case MessageItemPrivate::Chat_kick_user:
    {
        text = QString("%1 %2")
                .arg(isWoman ? QObject::tr("kicked", "woman")
                             : QObject::tr("kicked", "man"))
                .arg(arg);
        break;
    }

    case MessageItemPrivate::Chat_invite_self:
    {
        text = isWoman ? QObject::tr("returned to the conversation", "woman")
                       : QObject::tr("returned to the conversation", "man");
        break;
    }

    case MessageItemPrivate::Chat_kick_self:
    {
        text = isWoman ? QObject::tr("left the conversation", "woman")
                       : QObject::tr("left the conversation", "man");
        break;
    }
    }

    return QString("%1 %2").arg(author).arg(text);
}

QString Utils::actionToString(const int act)
{
    QString text;

    switch (act) {
    case MessageItemPrivate::Chat_photo_update:
    {
        text = QObject::tr("chat photo updated");
        break;
    }

    case MessageItemPrivate::Chat_photo_remove:
    {
        text = QObject::tr("chat photo removed");
        break;
    }

    case MessageItemPrivate::Chat_create:
    {
        text = QObject::tr("chat created");
        break;
    }

    case MessageItemPrivate::Chat_title_update:
    {
        text = QObject::tr("chat name changed");
        break;
    }

    case MessageItemPrivate::Chat_invite_user:
    {
        text = QObject::tr("user invited");
        break;
    }

    case MessageItemPrivate::Chat_kick_user:
    {
        text = QObject::tr("user kicked out");
        break;
    }

    case MessageItemPrivate::Chat_invite_self:
    {
        text = QObject::tr("user invited", "returned to the conversation");
        break;
    }

    case MessageItemPrivate::Chat_kick_self:
    {
        text = QObject::tr("user has left", "left the conversation");
        break;
    }
    }

    return text;
}

QVector<int> Utils::toVectorInt(const QVariantList &list)
{
    QVector<int> v;

    for (QVariant item: list) {
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

QString Utils::fileSizeToText(qint64 size)
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

bool Utils::applicationIsActive()
{
    return QGuiApplication::focusWindow() != 0;
}

QString Utils::durationToText(const int duration)
{
    int min = duration / 60;
    int sec = duration - (min * 60);
    return QString("%1:%2").arg(min).arg(QString().number(sec).rightJustified(2, '0'));
}
