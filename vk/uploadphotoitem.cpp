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

#include "uploadphotoitem.h"

UploadPhotoItem::UploadPhotoItem(Connection *connection, QNetworkAccessManager *manager)
    : UploadItem(connection, manager)
{
}

void UploadPhotoItem::upload(AttachmentItem item) const
{
    PhotoItem photo = qobject_cast<PhotoItem>(item);

    if (!photo->isUploading())
    {
        photo->setUploadProgress(10);

        Packet *getUploadServer = new Packet("photos.getMessagesUploadServer");
        getUploadServer->setProperty("item", QVariant::fromValue(photo));
        connect(getUploadServer, &Packet::finished, this, &UploadPhotoItem::onGetUploadServerFinished);
        _connection->appendQuery(getUploadServer);
    }
}

void UploadPhotoItem::onGetUploadServerFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QUrl uploadUrl = response.value("upload_url").toUrl();
    PhotoItem photo = qvariant_cast<PhotoItem>(sender->property("item"));

    UploadFile *uploadFile = new UploadFile(this, 10, 80);
    uploadFile->setProperty("item", QVariant::fromValue(photo));
    connect(uploadFile, &UploadFile::finished, this, &UploadPhotoItem::onUploadFileFinished);
    connect(uploadFile, &UploadFile::uploadProgress, photo.data(), &PhotoItemPrivate::setUploadProgress);
    uploadFile->upload(uploadUrl, photo->src().toLocalFile(), "photo", _manager);

    emit nextItem();
    delete sender;
}

void UploadPhotoItem::onUploadFileFinished(const QByteArray &result)
{
    QVariantMap response = Utils::parseJSON(result).toMap();
    UploadFile *uploadFile = qobject_cast<UploadFile*>(sender());
    PhotoItem photo = qvariant_cast<PhotoItem>(uploadFile->property("item"));

    Packet *saveMessagesPhoto = new Packet("photos.saveMessagesPhoto");
    saveMessagesPhoto->addParam("server", response.value("server").toString());
    saveMessagesPhoto->addParam("photo", response.value("photo").toString());
    saveMessagesPhoto->addParam("hash", response.value("hash").toString());
    saveMessagesPhoto->setProperty("item", QVariant::fromValue(photo));
    connect(saveMessagesPhoto, &Packet::finished, this, &UploadPhotoItem::onSaveMessagesPhotoFinished);

    photo->setUploadProgress(photo->uploadProgress() + 10);

    _connection->appendQuery(saveMessagesPhoto);

    uploadFile->deleteLater();
}

void UploadPhotoItem::onSaveMessagesPhotoFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toList().value(0).toMap();

    PhotoItem photo = qvariant_cast<PhotoItem>(sender->property("item"));
    photo->setId(response.value("id").toInt());
    photo->setOwnerId(response.value("owner_id").toInt());
    photo->setSrcBig(response.value("src_big").toUrl());

    photo->setUploadProgress(100);

    emit finished();
    delete sender;
}

