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

#include "rosterhandler.h"

RosterHandler::RosterHandler()
{
    _model = new RosterModel(this);

    _proxy = new QSortFilterProxyModel(_model);
    _proxy->setDynamicSortFilter(true);
    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setSourceModel(_model);

    connect(Client::instance()->longPoll(), SIGNAL(userStatusChanged(int,bool)), this, SLOT(onUserStatusChanged(int,bool)));
    connect(Client::instance()->longPoll(), SIGNAL(resumed()), this, SLOT(getFriendsOnline()));

    qRegisterMetaType<RosterModel*>("RosterModel*");
}

RosterHandler::~RosterHandler()
{
    delete _proxy;
    delete _model;
}

RosterModel *RosterHandler::model() const
{
    return _model;
}

QSortFilterProxyModel *RosterHandler::proxy() const
{
    return _proxy;
}

ProfileItem RosterHandler::profile(const int index, const bool isProxyIndex) const
{
    if (isProxyIndex)
    {
        QModelIndex proxyIndex = _proxy->index(index, 0);
        QModelIndex modelIndex = _proxy->mapToSource(proxyIndex);

        return _model->at(modelIndex);
    }
    else
    {
        return _model->at(index);
    }
}

int RosterHandler::indexOf(const int id, const bool proxyIndex) const
{
    int index = _model->indexOf(id);

    if (proxyIndex)
    {
        QModelIndex modelIndex = _model->index(index, 0);
        QModelIndex proxyIndex = _proxy->mapFromSource(modelIndex);

        return proxyIndex.row();
    }
    else
    {
        return index;
    }
}

ProfileItem RosterHandler::item(const int id) const
{
    return _model->item(id);
}

void RosterHandler::getFriendsOnline()
{
    Packet *packet = new Packet("friends.getOnline");
    packet->addParam("order", "hints");
    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(onFriendsOnline(const Packet*,QVariantMap)));
    Client::instance()->connection()->appendQuery(packet);
}

void RosterHandler::setFilterWildcard(const QString &pattern)
{
    _proxy->setFilterRegExp(QString("%1|%2|%3")
                            .arg(pattern)
                            .arg(Utils::toTranslit(pattern))
                            .arg(Utils::fromTranslit(pattern)));
}

void RosterHandler::onUserStatusChanged(const int uid, const bool online)
{
    int i = _model->indexOf(uid);

    if (i > -1)
    {
        _model->at(i)->setOnline(online);
    }
}

void RosterHandler::onFriendsOnline(const Packet *sender, const QVariantMap &result)
{
    QVariantList response = result.value("response").toList();
    QSet<int> uids;

    foreach (QVariant item, response)
    {
        uids.insert(item.toInt());
    }

    for (int i = 0; i < _model->rowCount(); i++)
    {
        ProfileItem profile = _model->at(i);
        profile->setOnline(uids.contains(profile->id()));
    }

    delete sender;
}
