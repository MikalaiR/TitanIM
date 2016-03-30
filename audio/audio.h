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

#ifndef AUDIO_H
#define AUDIO_H

#include <QObject>
#include "vk/client.h"
#include "vk/audiopacket.h"
#include "audioalbumsmodel.h"
#include "audiomodel.h"

class Audio : public QObject
{
    Q_OBJECT

    Q_PROPERTY(AudioAlbumsModel* albums READ albums CONSTANT)
    Q_PROPERTY(AudioModel* model READ model CONSTANT)
    Q_PROPERTY(AudioItemPrivate* currentAudio READ currentAudio NOTIFY currentAudioChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentAudioChanged)
    Q_PROPERTY(int currentAlbumId READ currentAlbumId WRITE setCurrentAlbumId NOTIFY currentAlbumIdChanged)

public:
    static Audio *instance();
    static void destroy();
    AudioAlbumsModel* albums() const;
    AudioModel* model() const;
    AudioItemPrivate* currentAudio() const;
    int currentIndex() const;
    int currentAlbumId() const;
    void setCurrentAlbumId(const int albumId);
    void clear();
    Q_INVOKABLE QString durationToText(const int duration);

private:
    Audio();
    ~Audio();
    void setCurrentIndex(const int index);

private:
    static Audio *aInstance;
    AudioAlbumsModel *_albums;
    AudioModel *_model;
    AudioPacket *_audioPacket;
    AudioList _currentItems;
    int _currentIndex;
    int _currentAlbumId;

public slots:
    void loadAlbums();
    void loadMyAudio();
    void loadAudioAlbum(const int albumId);
    void loadRecommend();
    void loadPopular();
    void showCurrent();
    void setPlaylist(const QVariantList list, const int index=0);
    void setCurrentModelIndex(const int row);
    void search(const QString &query);
    void refreshCurrentAlbum();
    void prev();
    void pause();
    void next();

protected slots:
    void getAudioFinished(const AudioPacket *sender, const AudioList &audio);
    void getAlbumsFinished(const AudioPacket *sender, const QVector<QPair<int, QString>> &items);
    void getRecommendFinished(const AudioPacket *sender, const AudioList &audio);
    void getPopularFinished(const AudioPacket *sender, const AudioList &audio);
    void searchFinished(const AudioPacket *sender, const AudioList &audio);

signals:
    void currentAudioChanged();
    void currentAlbumIdChanged();
    void switchedAudio();
};

#endif // AUDIO_H
