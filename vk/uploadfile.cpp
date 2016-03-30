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

UploadFile::UploadFile(QObject *parent, const int begin, const int end) :
    QObject(parent)
{
    _beginPercent = begin;
    _endPercent = end;
}

void UploadFile::upload(const QUrl &url, const QString &fileName, const QString &field, QNetworkAccessManager *nam)
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

    multiPart->append(httpPart);

    QNetworkRequest request(url);
    QNetworkReply *networkReply = nam->post(request, multiPart);

    auto uploadFinished = [=]()
    {
        file->close();

        delete multiPart;
        delete file;

        if (networkReply->error() != QNetworkReply::NoError)
        {
            //qDebug() << networkReply->errorString();
            networkReply->deleteLater();
            emit error();
            return;
        }

        QByteArray result = networkReply->readAll();
        networkReply->deleteLater();

        emit finished(result);
    };

    connect(networkReply, &QNetworkReply::finished, uploadFinished);
    connect(networkReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(uploadProgressHandler(qint64,qint64)));
}

void UploadFile::uploadProgressHandler(qint64 bytesSent, qint64 bytesTotal)
{
    if (bytesTotal == 0 || bytesTotal == -1)
    {
        return;
    }

    static int oldPercent = 0;
    int percent = (((float)bytesSent / bytesTotal) * (_endPercent - _beginPercent)) + _beginPercent;

    if (oldPercent != percent)
    {
        emit uploadProgress(percent);
    }

    oldPercent = percent;
}
