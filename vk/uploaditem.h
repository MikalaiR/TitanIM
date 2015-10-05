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

#ifndef UPLOADITEM_H
#define UPLOADITEM_H

#include <QObject>
#include "connection.h"
#include "attachmentitem.h"
#include "uploadfile.h"

class UploadItem : public QObject
{
    Q_OBJECT

public:
    UploadItem(Connection *connection, QNetworkAccessManager *manager);
    virtual void upload(AttachmentItem item) const = 0;

protected:
    Connection *_connection;
    QNetworkAccessManager *_manager;

signals:
    void nextItem();
    void finished();
};

#endif // UPLOADITEM_H
