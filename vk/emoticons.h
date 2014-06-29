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
#include <QHash>
#include <QStringList>
#include <QFile>

class Emoticons : public QObject
{
    Q_OBJECT

public:
    static Emoticons *instance();
    static void destroy();
    void setCurrentTheme(const QString &path);
    QString fromEmoji(const QString &emojiText);

private:
    Emoticons();

private:
    static Emoticons *aInstance;
    QHash<QString,QString> _emojis;
};

#endif // EMOTICONS_H
