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

void RosterHandler::clear()
{
    _model->removeAll();
}

void RosterHandler::setFilterWildcard(const QString &pattern)
{
    _proxy->setFilterRegExp(QString("%1|%2|%3")
                            .arg(pattern)
                            .arg(Utils::toTranslit(pattern))
                            .arg(Utils::fromTranslit(pattern)));
}

