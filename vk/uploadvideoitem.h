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

#ifndef UPLOADVIDEOITEM_H
#define UPLOADVIDEOITEM_H

#include <QObject>
#include "uploaditem.h"
#include "videoitem.h"

class UploadVideoItem : public UploadItem
{
    Q_OBJECT

public:
    UploadVideoItem(Connection *connection, QNetworkAccessManager *manager);
    void upload(AttachmentItem item) const;

protected slots:
    void onVideoSave(const Packet *sender, const QVariantMap &result);
    void onUploadFileFinished(const QByteArray &result);
};

#endif // UPLOADVIDEOITEM_H
