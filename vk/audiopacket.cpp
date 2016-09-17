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

#include "audiopacket.h"

AudioPacket::AudioPacket(Connection *connection)
{
    _connection = connection;
}

void AudioPacket::load(const int offset, const int count, const int ownerId, const int albumId)
{
    Packet *packet = new Packet("audio.get");
    packet->addParam("offset", offset);
    packet->addParam("count", count);

    if (ownerId != 0)
    {
        packet->addParam("owner_id", ownerId);
    }

    if (albumId != 0)
    {
        packet->addParam("album_id", albumId);
    }

    connect(packet, &Packet::finished, this, &AudioPacket::loadFinished);
    _connection->appendQuery(packet);
}

void AudioPacket::getAlbums(const int offset, const int count, const int ownerId)
{
    Packet *packet = new Packet("audio.getAlbums");
    packet->addParam("offset", offset);
    packet->addParam("count", count);

    if (ownerId != 0)
    {
        packet->addParam("owner_id", ownerId);
    }

    connect(packet, &Packet::finished, this, &AudioPacket::getAlbumsFinished);
    _connection->appendQuery(packet);
}

void AudioPacket::getRecommendations(const int offset, const int count)
{
    Packet *packet = new Packet("audio.getRecommendations");
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    connect(packet, &Packet::finished, this, &AudioPacket::getRecommendationsFinished);
    _connection->appendQuery(packet);
}

void AudioPacket::getPopular(const int offset, const int count)
{
    Packet *packet = new Packet("audio.getPopular");
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    connect(packet, &Packet::finished, this, &AudioPacket::getPopularFinished);
    _connection->appendQuery(packet);
}

void AudioPacket::getSearch(const QString &query, const int offset, const int count)
{
    Packet *packet = new Packet("audio.search");
    packet->addParam("q", query);
    packet->addParam("auto_complete", 1);
    packet->addParam("search_own", 1);
    packet->addParam("offset", offset);
    packet->addParam("count", count);
    connect(packet, &Packet::finished, this, &AudioPacket::searchFinished);
    _connection->appendQuery(packet);
}

void AudioPacket::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList items = response.value("items").toList();

    AudioList audioList = AudioParser::parser(items);

    emit audio(this, audioList);
    delete sender;
}

void AudioPacket::getAlbumsFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList items = response.value("items").toList();

    QVector<QPair<int, QString>> albumsList;

    foreach (QVariant item, items)
    {
        albumsList.append(QPair<int, QString>(item.toMap().value("id").toInt(), item.toMap().value("title").toString()));
    }

    emit albums(this, albumsList);
    delete sender;
}

void AudioPacket::getRecommendationsFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList items = response.value("items").toList();

    AudioList audioList = AudioParser::parser(items);

    emit recommendations(this, audioList);
    delete sender;
}

void AudioPacket::getPopularFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantList items = result.value("response").toList();

    AudioList audioList = AudioParser::parser(items);

    emit popular(this, audioList);
    delete sender;
}

void AudioPacket::searchFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();
    QVariantList items = response.value("items").toList();

    AudioList audioList = AudioParser::parser(items);

    emit search(this, audioList);
    delete sender;
}
