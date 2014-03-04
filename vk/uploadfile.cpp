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

#include "uploadfile.h"

UploadFile::UploadFile(QObject *parent) :
    QObject(parent)
{
    _id = 0;

    _manager = new QNetworkAccessManager(this);
    connect(_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));
}

int UploadFile::id() const
{
    return _id;
}

void UploadFile::setId(const int id)
{
    _id = id;
}

void UploadFile::upload(const QUrl &url, const QString &fileName, const QString &field)
{
    if (!QFile::exists(fileName))
        return;

    QFileInfo fileInfo(fileName);

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart httpPart;

    QString dispositionHeader = QString("form-data; name=\"%1\"; filename=\"%2\"")
            .arg(field)
            .arg(fileInfo.fileName());

    httpPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(dispositionHeader));

    QMimeDatabase mimeDatabase;
    QMimeType mimeType;
    mimeType = mimeDatabase.mimeTypeForFile(fileInfo);

    httpPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(mimeType.name()));

    QFile *file = new QFile(fileName);
    file->open(QIODevice::ReadOnly);
    httpPart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(httpPart);

    QNetworkRequest request(url);
    QNetworkReply *networkReply = _manager->post(request, multiPart);
    multiPart->setParent(networkReply);
}

void UploadFile::uploadFinished(QNetworkReply *networkReply)
{
    if (networkReply->error() != QNetworkReply::NoError)
    {
        //qDebug() << networkReply->errorString(); //todo emit error(_id);
        networkReply->deleteLater();
        return;
    }

    QByteArray result = networkReply->readAll();
    networkReply->deleteLater();

    emit finished(_id, result);
}
