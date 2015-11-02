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

#ifndef LINKITEMPRIVATE_H
#define LINKITEMPRIVATE_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"

class LinkItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT) //todo signal
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)

public:
    LinkItemPrivate();
    QString title() const;
    void setTitle(const QString &title);
    QString description() const;
    void setDescription(const QString &description);
    QUrl url() const;
    void setUrl(const QUrl &url);

private:
    QString _title;
    QString _description;
    QUrl _url;
};

typedef QSharedPointer<LinkItemPrivate> LinkItem;

#endif // LINKITEMPRIVATE_H
