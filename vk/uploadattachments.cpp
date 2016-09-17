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
    _manager = new QNetworkAccessManager(this);

    _uploadPhoto = new UploadPhotoItem(connection, _manager);
    connect(_uploadPhoto, &UploadPhotoItem::nextItem, this, &UploadAttachments::onNextUpload);
    connect(_uploadPhoto, &UploadPhotoItem::finished, this, &UploadAttachments::onUploadFinished);

    _uploadDoc = new UploadDocItem(connection, _manager);
    connect(_uploadDoc, &UploadPhotoItem::nextItem, this, &UploadAttachments::onNextUpload);
    connect(_uploadDoc, &UploadPhotoItem::finished, this, &UploadAttachments::onUploadFinished);

    _uploadAudio = new UploadAudioItem(connection, _manager);
    connect(_uploadAudio, &UploadPhotoItem::nextItem, this, &UploadAttachments::onNextUpload);
    connect(_uploadAudio, &UploadPhotoItem::finished, this, &UploadAttachments::onUploadFinished);

    _uploadVideo = new UploadVideoItem(connection, _manager);
    connect(_uploadVideo, &UploadPhotoItem::nextItem, this, &UploadAttachments::onNextUpload);
    connect(_uploadVideo, &UploadPhotoItem::finished, this, &UploadAttachments::onUploadFinished);
}

UploadAttachments::~UploadAttachments()
{
    delete _uploadPhoto;
    delete _uploadDoc;
    delete _uploadAudio;
    delete _uploadVideo;
    delete _manager;
}

void UploadAttachments::setAttachments(const int id, AttachmentList *attachments)
{
    _id = id;
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
            checkAttach();
        }

        return;
    }

    switch (_attachments->at(_index)->attachmentType()) {
    case Attachment::Photo:
    {
        _uploadPhoto->upload(_attachments->at(_index));
        break;
    }

    case Attachment::Doc:
    {
        _uploadDoc->upload(_attachments->at(_index));
        break;
    }

    case Attachment::Audio:
    {
        _uploadAudio->upload(_attachments->at(_index));
        break;
    }

    case Attachment::Video:
    {
        _uploadVideo->upload(_attachments->at(_index));
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

int UploadAttachments::id() const
{
    return _id;
}

void UploadAttachments::onNextUpload()
{
    _index++;
    upload();
}

void UploadAttachments::onUploadFinished()
{
    _countUpload--;

    if (_countUpload == 0)
    {
        checkAttach();
    }
}

void UploadAttachments::checkAttach()
{
    for (int i = 0; i < _attachments->count(); i++)
    {
        if (_attachments->at(i)->isUploadError())
        {
            emit error();
            return;
        }
    }

    emit finished();
}
