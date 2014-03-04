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

#ifndef UPLOADATTACHMENTS_H
#define UPLOADATTACHMENTS_H

#include <QObject>
#include "connection.h"
#include "uploadfile.h"
#include "attachmentlist.h"
#include "photoitem.h"

class UploadAttachments : public QObject
{
    Q_OBJECT

public:
    explicit UploadAttachments(Connection *connection);
    void setAttachments(AttachmentList *attachments);
    void upload();

private:
    Connection *_connection;
    AttachmentList *_attachments;
    int _index;
    int _countUpload;

protected slots:
    void onGetUploadServerFinished(const Packet *sender, const QVariantMap &result);
    void onUploadFileFinished(const int id, const QByteArray &result);
    void onSaveMessagesPhotoFinished(const Packet *sender, const QVariantMap &result);

signals:
    void finished();
};

#endif // UPLOADATTACHMENTS_H
