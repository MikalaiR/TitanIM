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
#include "attachmentlist.h"
#include "uploadphotoitem.h"
#include "uploaddocitem.h"
#include "uploadaudioitem.h"
#include "uploadvideoitem.h"

class UploadAttachments : public QObject
{
    Q_OBJECT

public:
    explicit UploadAttachments(Connection *connection);
    ~UploadAttachments();
    void setAttachments(const int id, AttachmentList *attachments);
    void upload();
    int id() const;

private:
    AttachmentList *_attachments;
    int _id;
    int _index;
    int _countUpload;
    QNetworkAccessManager *_manager;
    UploadPhotoItem *_uploadPhoto;
    UploadDocItem *_uploadDoc;
    UploadAudioItem *_uploadAudio;
    UploadVideoItem *_uploadVideo;

protected slots:
    void onNextUpload();
    void onUploadFinished();
    void checkAttach();

signals:
    void finished();
    void error();
};

#endif // UPLOADATTACHMENTS_H
