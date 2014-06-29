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

#include "emoticons.h"

Emoticons *Emoticons::aInstance = 0;

Emoticons *Emoticons::instance()
{
    if (!aInstance) aInstance = new Emoticons();
    return aInstance;
}

void Emoticons::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

Emoticons::Emoticons()
{
}

void Emoticons::setCurrentTheme(const QString &path)
{
    QString define = path + "define";
    QFile defineFile(define);

    if (!defineFile.open(QFile::ReadOnly))
    {
        return;
    }

    _emojis.clear();

    const QString data = defineFile.readAll();
    const QStringList &list = data.split("\n", QString::SkipEmptyParts);
    foreach(const QString &l, list)
    {
        const QStringList &parts = l.split("\t", QString::SkipEmptyParts);

        if (parts.count() < 2)
        {
            continue;
        }

        QString emojiPath = path + parts.at(0);
        QString emojiCode = parts.at(1);

        _emojis[emojiCode] = emojiPath;
    }
}

QString Emoticons::fromEmoji(const QString &emojiText)
{
    QString res = emojiText;

    for(int i = 0; i < res.count(); i++)
    {
        for(int j = 1; j < 5; j++) //todo optimize me
        {
            QString emoji = res.mid(i, j);

            if (!_emojis.contains(emoji))
            {
                continue;
            }

            QString path = _emojis.value(emoji);
            QString richText = QString("<img src=\"file://%1\"/>").arg(path);
            res.replace(i, j, richText);
            i += richText.count() - 1;
            break;
        }
    }

    return res;
}
