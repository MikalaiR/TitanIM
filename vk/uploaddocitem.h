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

#ifndef UPLOADDOCITEM_H
#define UPLOADDOCITEM_H

#include <QObject>
#include "uploaditem.h"
#include "docitem.h"

class UploadDocItem : public UploadItem
{
    Q_OBJECT

public:
    UploadDocItem(Connection *connection, QNetworkAccessManager *manager);
    void upload(AttachmentItem item) const;

protected slots:
    void onGetUploadServerFinished(const Packet *sender, const QVariantMap &result);
    void onUploadFileFinished(const QByteArray &result);
    void onSaveMessagesDocFinished(const Packet *sender, const QVariantMap &result);
};

#endif // UPLOADDOCITEM_H
