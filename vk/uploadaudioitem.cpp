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

#include "uploadaudioitem.h"

UploadAudioItem::UploadAudioItem(Connection *connection, QNetworkAccessManager *manager)
    : UploadItem(connection, manager)
{
}

void UploadAudioItem::upload(AttachmentItem item) const
{
    AudioItem audio = qobject_cast<AudioItem>(item);

    if (!audio->isUploading())
    {
        audio->setUploadProgress(10);

        Packet *getUploadServer = new Packet("audio.getUploadServer");
        getUploadServer->setProperty("item", QVariant::fromValue(audio));
        connect(getUploadServer, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onGetUploadServerFinished(const Packet*,QVariantMap)));
        _connection->appendQuery(getUploadServer);
    }
}

void UploadAudioItem::onGetUploadServerFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QUrl uploadUrl = response.value("upload_url").toUrl();
    AudioItem audio = qvariant_cast<AudioItem>(sender->property("item"));

    UploadFile *uploadFile = new UploadFile(this, 10, 90);
    uploadFile->setProperty("item", QVariant::fromValue(audio));
    connect(uploadFile, SIGNAL(finished(QByteArray)), this, SLOT(onUploadFileFinished(QByteArray)));
    connect(uploadFile, SIGNAL(uploadProgress(int)), audio.data(), SLOT(setUploadProgress(int)));
    uploadFile->upload(uploadUrl, audio->url().toLocalFile(), "file", _manager);

    emit nextItem();
    delete sender;
}

void UploadAudioItem::onUploadFileFinished(const QByteArray &result)
{
    QVariantMap response = Utils::parseJSON(result).toMap();

    UploadFile *uploadFile = qobject_cast<UploadFile*>(sender());
    AudioItem audio = qvariant_cast<AudioItem>(uploadFile->property("item"));

    Packet *packet = new Packet("audio.save");
    packet->addParam("server", response.value("server").toString());
    packet->addParam("audio", response.value("audio").toString());
    packet->addParam("hash", response.value("hash").toString());
    packet->setProperty("item", QVariant::fromValue(audio));
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onSaveMessagesAudioFinished(const Packet*,QVariantMap)));
    connect(packet, SIGNAL(error(const Packet*,const ErrorResponse*)), this, SLOT(onSaveMessagesAudioError(const Packet*,const ErrorResponse*)));

    _connection->appendQuery(packet);

    uploadFile->deleteLater();
}

void UploadAudioItem::onSaveMessagesAudioFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    AudioItem audio = qvariant_cast<AudioItem>(sender->property("item"));
    audio->setId(response.value("id").toInt());
    audio->setOwnerId(response.value("owner_id").toInt());
    audio->setArtist(response.value("artist").toString());
    audio->setTitle(response.value("title").toString());
    audio->setDuration(response.value("duration").toInt());
    audio->setUrl(response.value("url").toUrl());

    audio->setUploadProgress(100);

    emit finished();
    delete sender;
}

void UploadAudioItem::onSaveMessagesAudioError(const Packet *sender, const ErrorResponse *errorResponse)
{
    AudioItem audio = qvariant_cast<AudioItem>(sender->property("item"));
    audio->setUploadError(errorResponse->msg());
    audio->setUploadProgress(-1);

    emit finished();
    delete sender;
}

