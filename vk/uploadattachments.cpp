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

#include "uploadattachments.h"

UploadAttachments::UploadAttachments(Connection *connection)
{
    _connection = connection;
}

void UploadAttachments::setAttachments(AttachmentList *attachments)
{
    _attachments = attachments;
    _index = 0;
    _countUpload = attachments->count();
}

void UploadAttachments::upload()
{
    if (_index >= _attachments->count())
    {
        if (_countUpload == 0)
        {
            emit finished();
        }

        return;
    }

    switch (_attachments->at(_index)->attachmentType()) {
    case Attachment::photo:
    {
        PhotoItem photo = qobject_cast<PhotoItem>(_attachments->at(_index));

        if (!photo->isUploading())
        {
            photo->setUploadProgress(10);

            Packet *getUploadServer = new Packet("photos.getMessagesUploadServer");
            getUploadServer->setId(_index);
            connect(getUploadServer, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onGetUploadServerFinished(const Packet*,QVariantMap)));
            _connection->appendQuery(getUploadServer);
        }

        break;
    }

    default:
    {
        _countUpload--;
        _index++;
        upload();
        break;
    }
    }
}

void UploadAttachments::onGetUploadServerFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    int id = sender->id();
    QUrl uploadUrl = response.value("upload_url").toUrl();

    PhotoItem photo = qobject_cast<PhotoItem>(_attachments->at(id));

    UploadFile *uploadFile = new UploadFile(this);
    uploadFile->setId(id);
    connect(uploadFile, SIGNAL(finished(int,QByteArray)), this, SLOT(onUploadFileFinished(int,QByteArray)));
    uploadFile->upload(uploadUrl, photo->src().toLocalFile(), "photo");

    photo->setUploadProgress(photo->uploadProgress() + 10);
    _index++;
    upload();

    delete sender;
}

void UploadAttachments::onUploadFileFinished(const int id, const QByteArray &result)
{
    QVariantMap response = Utils::parseJSON(result).toMap();

    Packet *saveMessagesPhoto = new Packet("photos.saveMessagesPhoto");
    saveMessagesPhoto->addParam("server", response.value("server").toString());
    saveMessagesPhoto->addParam("photo", response.value("photo").toString());
    saveMessagesPhoto->addParam("hash", response.value("hash").toString());
    saveMessagesPhoto->setId(id);
    connect(saveMessagesPhoto, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onSaveMessagesPhotoFinished(const Packet*,QVariantMap)));

    PhotoItem photo = qobject_cast<PhotoItem>(_attachments->at(id));
    photo->setUploadProgress(photo->uploadProgress() + 70);

    _connection->appendQuery(saveMessagesPhoto);

    UploadFile *uploadFile = qobject_cast<UploadFile*>(sender());
    if (uploadFile) delete uploadFile;
}

void UploadAttachments::onSaveMessagesPhotoFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toList().value(0).toMap();
    int id = sender->id();

    PhotoItem photo = qobject_cast<PhotoItem>(_attachments->at(id));
    photo->setId(response.value("id").toInt());
    photo->setOwnerId(response.value("owner_id").toInt());
    photo->setSrcBig(response.value("src_big").toUrl());

    photo->setUploadProgress(100);

    _countUpload--;

    if (_countUpload == 0)
    {
        emit finished();
    }

    delete sender;
}
