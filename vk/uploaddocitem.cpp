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

#include "uploaddocitem.h"

UploadDocItem::UploadDocItem(Connection *connection, QNetworkAccessManager *manager)
    : UploadItem(connection, manager)
{
}

void UploadDocItem::upload(AttachmentItem item) const
{
    DocItem doc = qobject_cast<DocItem>(item);

    if (!doc->isUploading())
    {
        doc->setUploadProgress(10);

        Packet *getUploadServer = new Packet("docs.getUploadServer");
        getUploadServer->setProperty("item", QVariant::fromValue(doc));
        connect(getUploadServer, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onGetUploadServerFinished(const Packet*,QVariantMap)));
        _connection->appendQuery(getUploadServer);
    }
}

void UploadDocItem::onGetUploadServerFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QUrl uploadUrl = response.value("upload_url").toUrl();
    DocItem doc = qvariant_cast<DocItem>(sender->property("item"));

    UploadFile *uploadFile = new UploadFile(this, 10, 90);
    uploadFile->setProperty("item", QVariant::fromValue(doc));
    connect(uploadFile, SIGNAL(finished(QByteArray)), this, SLOT(onUploadFileFinished(QByteArray)));
    connect(uploadFile, SIGNAL(uploadProgress(int)), doc.data(), SLOT(setUploadProgress(int)));
    uploadFile->upload(uploadUrl, doc->url().toLocalFile(), "file", _manager);

    emit nextItem();
    delete sender;
}

void UploadDocItem::onUploadFileFinished(const QByteArray &result)
{
    QVariantMap response = Utils::parseJSON(result).toMap();
    UploadFile *uploadFile = qobject_cast<UploadFile*>(sender());
    DocItem doc = qvariant_cast<DocItem>(uploadFile->property("item"));

    Packet *packet = new Packet("docs.save");
    packet->addParam("file", response.value("file").toString());
    packet->setProperty("item", QVariant::fromValue(doc));
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onSaveMessagesDocFinished(const Packet*,QVariantMap)));

    _connection->appendQuery(packet);

    uploadFile->deleteLater();
}

void UploadDocItem::onSaveMessagesDocFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toList().value(0).toMap();

    DocItem doc = qvariant_cast<DocItem>(sender->property("item"));
    doc->setId(response.value("id").toInt());
    doc->setOwnerId(response.value("owner_id").toInt());
    doc->setTitle(response.value("title").toString());
    doc->setSize(response.value("size").toInt());
    doc->setExt(response.value("ext").toString());
    doc->setUrl(response.value("url").toUrl());

    if (response.contains("photo_130"))
    {
        doc->setThumb(response.value("photo_130").toUrl());
    }

    doc->setUploadProgress(100);

    emit finished();
    delete sender;
}

