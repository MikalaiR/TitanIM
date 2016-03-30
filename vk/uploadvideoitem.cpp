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

#include "uploadvideoitem.h"

UploadVideoItem::UploadVideoItem(Connection *connection, QNetworkAccessManager *manager)
    : UploadItem(connection, manager)
{
}

void UploadVideoItem::upload(AttachmentItem item) const
{
    VideoItem video = qobject_cast<VideoItem>(item);

    if (!video->isUploading())
    {
        video->setUploadProgress(10);

        Packet *packet = new Packet("video.save");
        packet->addParam("is_private", 1);
        packet->setProperty("item", QVariant::fromValue(video));
        connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onVideoSave(const Packet*,QVariantMap)));
        _connection->appendQuery(packet);
    }
}

void UploadVideoItem::onVideoSave(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QUrl uploadUrl = response.value("upload_url").toUrl();

    VideoItem video = qvariant_cast<VideoItem>(sender->property("item"));
    video->setOwnerId(response.value("owner_id").toInt());

    UploadFile *uploadFile = new UploadFile(this, 10, 90);
    uploadFile->setProperty("item", QVariant::fromValue(video));
    connect(uploadFile, SIGNAL(finished(QByteArray)), this, SLOT(onUploadFileFinished(QByteArray)));
    connect(uploadFile, SIGNAL(uploadProgress(int)), video.data(), SLOT(setUploadProgress(int)));
    uploadFile->upload(uploadUrl, video->url().toLocalFile(), "video_file", _manager);

    emit nextItem();
    delete sender;
}

void UploadVideoItem::onUploadFileFinished(const QByteArray &result)
{
    QVariantMap response = Utils::parseJSON(result).toMap();

    UploadFile *uploadFile = qobject_cast<UploadFile*>(sender());

    VideoItem video = qvariant_cast<VideoItem>(uploadFile->property("item"));
    video->setId(response.value("video_id").toInt());

    if (response.contains("photo_130"))
    {
        video->setSrcBig(response.value("photo_130").toUrl());
    }

    video->setUploadProgress(100);

    emit finished();
    uploadFile->deleteLater();
}
