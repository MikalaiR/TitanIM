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

#ifndef EMOTICONS_H
#define EMOTICONS_H

#include <QObject>
#include <QSet>
#include <QStringList>
#include <QStringBuilder>
#include <QFile>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextStream>
#include "utils.h"

class Emoticons : public QObject
{
    Q_OBJECT

    static const int TOTAL_BLOCKS = 6;

public:
    static Emoticons *instance();
    static void destroy();
    void setCurrentTheme(const QString &path);
    Q_INVOKABLE QString fromEmoji(const QString &emojiText) const;
    Q_INVOKABLE QString toEmoji(const QString &richText) const;
    Q_INVOKABLE QString pathOf(const QString &code) const;
    Q_INVOKABLE QList<QString> emojiList(const int block=1);

private:
    Emoticons();
    ~Emoticons();

private:
    static Emoticons *aInstance;
    QString _path;
    QString _recentFile;
    QSet<QString> _emojis;
    QList<QString> _blocks[TOTAL_BLOCKS];

public slots:
    void setRecentFile(const QString &file);
    void saveRecent(const QString &file);
    void loadRecent(const QString &file);
    void addRecent(const QString &code);
};

#endif // EMOTICONS_H
