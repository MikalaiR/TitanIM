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

#ifndef DOCITEMPRIVATE_H
#define DOCITEMPRIVATE_H

#include <QObject>
#include <QUrl>
#include "attachmentitem.h"
#include "utils.h"

class DocItemPrivate : public Attachment
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT) //todo
    Q_PROPERTY(int size READ size CONSTANT)
    Q_PROPERTY(QString sizeStr READ sizeStr CONSTANT)
    Q_PROPERTY(QString ext READ ext CONSTANT)
    Q_PROPERTY(QUrl url READ url CONSTANT)
    Q_PROPERTY(QUrl thumb READ thumb CONSTANT)
    Q_PROPERTY(bool isImage READ isImage CONSTANT)

public:
    DocItemPrivate();
    QString title() const;
    void setTitle(const QString &title);
    int size() const;
    QString sizeStr() const;
    void setSize(const int size);
    QString ext() const;
    void setExt(const QString &ext);
    QUrl url() const;
    void setUrl(const QUrl &url);
    QUrl thumb() const;
    void setThumb(const QUrl &thumb);
    bool isImage() const;

private:
    QString _title;
    int _size;
    QString _sizeStr;
    QString _ext;
    QUrl _url;
    QUrl _thumb;
};

typedef QSharedPointer<DocItemPrivate> DocItem;

#endif // DOCITEMPRIVATE_H
