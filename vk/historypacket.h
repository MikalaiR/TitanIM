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

#ifndef HISTORYPACKET_H
#define HISTORYPACKET_H

#include <QObject>
#include "connection.h"
#include "messageparser.h"

class HistoryPacket : public QObject
{
    Q_OBJECT

public:
    explicit HistoryPacket(Connection *connection);
    void load(const int id, const int offset, const int count);
    int offset() const;
    int count() const;
    int serverCount() const;

private:
    Connection *_connection;
    int _offset;
    int _count;
    int _serverCount;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void history(const HistoryPacket *sender, const int id, const MessageList &messages);
};

#endif // HISTORYPACKET_H
