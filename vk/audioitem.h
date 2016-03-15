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

#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"
#include "utils.h"

class AudioItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString artist READ artist CONSTANT) //todo signal
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(int duration READ duration CONSTANT)
    Q_PROPERTY(QString durationStr READ durationStr CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    AudioItemPrivate();
    QString artist() const;
    void setArtist(const QString &artist);
    QString title() const;
    void setTitle(const QString &title);
    int duration() const;
    QString durationStr() const;
    void setDuration(const int duration);
    QUrl url() const;
    void setUrl(const QUrl &url);

private:
    QString _artist;
    QString _title;
    int _duration;
    QUrl _url;
};

typedef QSharedPointer<AudioItemPrivate> AudioItem;

#endif // AUDIOITEM_H
