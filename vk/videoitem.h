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

#ifndef VIDEOITEMPRIVATE_H
#define VIDEOITEMPRIVATE_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class VideoItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT) //todo signal
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(int duration READ duration CONSTANT)
    Q_PROPERTY(QString durationStr READ durationStr CONSTANT)
    Q_PROPERTY(QUrl src READ src CONSTANT)
    Q_PROPERTY(QUrl srcBig READ srcBig CONSTANT)
    Q_PROPERTY(QString accessKey READ accessKey CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    VideoItemPrivate();
    QString title() const;
    void setTitle(const QString &title);
    QString description() const;
    void setDescription(const QString &description);
    int duration() const;
    QString durationStr() const;
    void setDuration(const int duration);
    QUrl src() const;
    void setSrc(const QUrl &src);
    QUrl srcBig() const;
    void setSrcBig(const QUrl &srcBig);
    QString accessKey() const;
    void setAccessKey(const QString &accessKey);
    QUrl url() const;

private:
    QString _title;
    QString _description;
    int _duration;
    QUrl _src;
    QUrl _srcBig;
    QString _accessKey;
};

typedef QSharedPointer<VideoItemPrivate> VideoItem;

#endif // VIDEOITEMPRIVATE_H
