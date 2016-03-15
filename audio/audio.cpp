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

#include "audio.h"

Audio *Audio::aInstance = 0;

Audio *Audio::instance()
{
    if (!aInstance) aInstance = new Audio();
    return aInstance;
}

void Audio::destroy()
{
    if (aInstance)
        delete aInstance, aInstance = 0;
}

Audio::Audio()
{
    _audioPacket = new AudioPacket(Client::instance()->connection());
    connect(_audioPacket, SIGNAL(audio(const AudioPacket*,AudioList)), SLOT(getAudioFinished(const AudioPacket*,AudioList)));
    connect(_audioPacket, SIGNAL(albums(const AudioPacket*,QVector<QPair<int,QString> >)), SLOT(getAlbumsFinished(const AudioPacket*,QVector<QPair<int,QString> >)));
    connect(_audioPacket, SIGNAL(recommendations(const AudioPacket*,AudioList)), this, SLOT(getRecommendFinished(const AudioPacket*,AudioList)));
    connect(_audioPacket, SIGNAL(popular(const AudioPacket*,AudioList)), this, SLOT(getPopularFinished(const AudioPacket*,AudioList)));
    connect(_audioPacket, SIGNAL(search(const AudioPacket*,AudioList)), this, SLOT(searchFinished(const AudioPacket*,AudioList)));

    _albums = new AudioAlbumsModel(this);
    _model = new AudioModel(this);

    _currentItems = AudioList::create();
    _currentIndex = -1;
    _currentAlbumId = -3;

    qRegisterMetaType<AudioModel*>("AudioModel*");
}

Audio::~Audio()
{
    delete _model;
    delete _albums;
}

AudioAlbumsModel *Audio::albums() const
{
    return _albums;
}

AudioModel *Audio::model() const
{
    return _model;
}

AudioItemPrivate *Audio::currentAudio() const
{
    if (_currentIndex != -1 && _currentItems->count() > _currentIndex)
    {
        return _currentItems->at(_currentIndex).data();
    }
    else
    {
        return 0;
    }
}

int Audio::currentIndex() const
{
    return _currentIndex;
}

int Audio::currentAlbumId() const
{
    return _currentAlbumId;
}

void Audio::setCurrentAlbumId(const int albumId)
{
    switch (albumId) {
    case -4:
        showCurrent();
        break;
    case -3:
        loadMyAudio();
        break;
    case -2:
        loadRecommend();
        break;
    case -1:
        loadPopular();
        break;
    default:
        loadAudioAlbum(albumId);
        break;
    }

    if (_currentAlbumId != albumId)
    {
        _currentAlbumId = albumId;
        emit currentAlbumIdChanged();
    }
}

void Audio::clear()
{
    pause();
    _albums->removeAll();
    _model->removeAll();
    setCurrentIndex(-1);
    _currentItems->clear();
    _currentAlbumId = -3;
}

QString Audio::durationToText(const int duration)
{
    return Utils::durationToText(duration);
}

void Audio::loadAlbums()
{
    _audioPacket->getAlbums();
}

void Audio::loadMyAudio()
{
    _audioPacket->load();
}

void Audio::loadAudioAlbum(const int albumId)
{
    _audioPacket->load(0, 100, 0, albumId);
}

void Audio::loadRecommend()
{
    _audioPacket->getRecommendations();
}

void Audio::loadPopular()
{
    _audioPacket->getPopular();
}

void Audio::showCurrent()
{
    _model->replaceAll(_currentItems);
}

void Audio::setCurrentIndex(const int row)
{
    _currentIndex = row;
    emit currentAudioChanged();
}

void Audio::setCurrentModelIndex(const int index)
{
    _currentItems->clear();
    _currentItems->add(_model->toList());

    setCurrentIndex(index);
}

void Audio::search(const QString &query)
{
    _audioPacket->getSearch(query);
}

void Audio::refreshCurrentAlbum()
{
    setCurrentAlbumId(_currentAlbumId);
}

void Audio::setPlaylist(const QVariantList list, const int index)
{
    _currentItems->clear();

    for (int i = 0; i < list.count(); i++)
    {
        AttachmentItem item = qvariant_cast<AttachmentItem>(list.at(i));
        _currentItems->add(qobject_cast<AudioItem>(item));
    }

    setCurrentIndex(index);
    showCurrent();
}

void Audio::prev()
{
    if (_currentIndex - 1 >= 0)
    {
        setCurrentIndex(_currentIndex - 1);
    }

    emit switchedAudio();
}

void Audio::pause()
{
    setCurrentIndex(_currentIndex);
}

void Audio::next()
{
    if (_currentItems->count() > _currentIndex + 1)
    {
        setCurrentIndex(_currentIndex + 1);
    }

    emit switchedAudio();
}

void Audio::getAudioFinished(const AudioPacket *sender, const AudioList &audio)
{
    _model->replaceAll(audio);
}

void Audio::getAlbumsFinished(const AudioPacket *sender, const QVector<QPair<int, QString> > &items)
{
    QVector<QPair<int, QString>> temp;
    temp.append(QPair<int, QString>(-4, tr("Current playlist")));
    temp.append(QPair<int, QString>(-3, tr("My music")));
    temp.append(QPair<int, QString>(-2, tr("Suggested music")));
    temp.append(QPair<int, QString>(-1, tr("Popular music")));

    temp.append(items);

    _albums->replaceAll(temp);
}

void Audio::getRecommendFinished(const AudioPacket *sender, const AudioList &audio)
{
    _model->replaceAll(audio);
}

void Audio::getPopularFinished(const AudioPacket *sender, const AudioList &audio)
{
    _model->replaceAll(audio);
}

void Audio::searchFinished(const AudioPacket *sender, const AudioList &audio)
{
    _model->replaceAll(audio);
}
