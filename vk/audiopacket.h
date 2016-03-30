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

#ifndef AUDIOPACKET_H
#define AUDIOPACKET_H

#include <QObject>
#include "connection.h"
#include "audioparser.h"

class AudioPacket : public QObject
{
    Q_OBJECT

public:
    AudioPacket(Connection *connection);
    void load(const int offset=0, const int count=100, const int ownerId=0, const int albumId=0);
    void getAlbums(const int offset=0, const int count=100, const int ownerId=0);
    void getRecommendations(const int offset=0, const int count=100);
    void getPopular(const int offset=0, const int count=100);
    void getSearch(const QString &query, const int offset=0, const int count=100);

private:
    Connection *_connection;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &result);
    void getAlbumsFinished(const Packet *sender, const QVariantMap &result);
    void getRecommendationsFinished(const Packet *sender, const QVariantMap &result);
    void getPopularFinished(const Packet *sender, const QVariantMap &result);
    void searchFinished(const Packet *sender, const QVariantMap &result);

signals:
    void audio(const AudioPacket *sender, const AudioList &audio);
    void albums(const AudioPacket *sender, const QVector<QPair<int, QString>> &items);
    void recommendations(const AudioPacket *sender, const AudioList &audio);
    void popular(const AudioPacket *sender, const AudioList &audio);
    void search(const AudioPacket *sender, const AudioList &audio);
};

#endif // AUDIOPACKET_H
