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

#include "dialogshandler.h"

DialogsHandler::DialogsHandler()
{
    _model = new DialogsModel(this);

    _proxy = new QSortFilterProxyModel(_model);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setSortRole(DialogsModel::dateRole);
    _proxy->sort(0, Qt::DescendingOrder);
    _proxy->setSourceModel(_model);

    connect(Client::instance()->longPoll(), SIGNAL(messageInAdded(DialogItem)), this, SLOT(onLongPollMessageAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(messageOutAdded(DialogItem)), this, SLOT(onLongPollMessageAdded(DialogItem)));
}

DialogsHandler::~DialogsHandler()
{
    delete _proxy;
    delete _model;
}

DialogsModel *DialogsHandler::model() const
{
    return _model;
}

QSortFilterProxyModel *DialogsHandler::proxy() const
{
    return _proxy;
}

DialogItem DialogsHandler::dialog(const int index, const bool isProxyIndex) const
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

void DialogsHandler::onLongPollMessageAdded(const DialogItem dialog)
{
    int id = dialog->id();
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->setMessage(dialog->message());
    }
    else
    {
        _model->append(dialog);
    }
}
