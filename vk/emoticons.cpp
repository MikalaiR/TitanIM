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
    _recentFile.clear();
}

Emoticons::~Emoticons()
{
    if (!_recentFile.isEmpty())
    {
        saveRecent(_recentFile);
    }
}

void Emoticons::setCurrentTheme(const QString &path)
{
    QString define = path + "define";
    QFile defineFile(define);

    if (!defineFile.open(QFile::ReadOnly))
    {
        return;
    }

    _path = path;
    _emojis.clear();

    const QString data = defineFile.readAll();
    const QStringList &blocks = data.split("\n", QString::SkipEmptyParts);
    int _blockIndex = 1;

    foreach(const QString &g, blocks)
    {
        const QStringList &list = g.split("\t", QString::SkipEmptyParts);

        foreach(const QString &code, list)
        {
            _emojis.insert(code);
            _blocks[_blockIndex].append(code);
        }

        if (_blockIndex < TOTAL_BLOCKS - 1)
        {
            _blockIndex++;
        }
    }

    defineFile.close();
}

QString Emoticons::fromEmoji(const QString &emojiText) const
{
    QString res = emojiText;

    for(int i = 0; i < res.count(); i++)
    {
        for(int j = 1; j < 5; j++) //todo optimize me. Finite State Machine
        {
            QString emoji = res.mid(i, j);

            if (!_emojis.contains(emoji))
            {
                continue;
            }

            QString richText = QString("<img src=\"%1\"/>").arg(pathOf(emoji));
            res.replace(i, j, richText);
            i += richText.count() - 1;
            break;
        }
    }

    return res;
}

QString Emoticons::toEmoji(const QString &richText) const
{
    QString res = richText;

    QRegExp rx("\\<img[^\\>]*src\\s*=\\s*\"([^\"]*)\"[^\\>]*\\>");
    int pos = 0;

    while ((pos = rx.indexIn(res, pos)) != -1)
    {
        QFileInfo fileInfo(rx.cap(1));
        QByteArray byteArray = QByteArray::fromHex(fileInfo.baseName().toLatin1());
        QString emoji = QString::fromUtf8(byteArray.constData());
        res.replace(pos, rx.matchedLength(), emoji);
        pos += emoji.length();
    }

    QTextDocument textDocument;
    textDocument.setHtml(res);

    return textDocument.toPlainText();
}

QString Emoticons::pathOf(const QString &code) const
{
    return "file://" % _path % code.toUtf8().toHex().toUpper() % ".png";
}

QList<QString> Emoticons::emojiList(const int block)
{
    return _blocks[block];
}

void Emoticons::setRecentFile(const QString &file)
{
    _recentFile = file;
    loadRecent(_recentFile);
}

void Emoticons::saveRecent(const QString &file)
{
    QFile recentFile(file);

    if (!recentFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&recentFile);
    out << Utils::join(_blocks[0], '\t');

    recentFile.close();
}

void Emoticons::loadRecent(const QString &file)
{
    QFile recentFile(file);

    if (!recentFile.open(QFile::ReadOnly))
    {
        return;
    }

    _blocks[0].clear();

    const QString data = recentFile.readAll();
    const QStringList &list = data.split("\t", QString::SkipEmptyParts);

    foreach(const QString &code, list)
    {
        _blocks[0].append(code);
    }

    recentFile.close();
}

void Emoticons::addRecent(const QString &code)
{
    _blocks[0].removeOne(code);
    _blocks[0].prepend(code);
}
