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
    _unreadDialogs = 0;

    _model = new DialogsModel(this);
    connect(_model, SIGNAL(unreadDialogs(int)), this, SLOT(setUnreadDialogs(int)));

    _proxy = new QSortFilterProxyModel(_model);
    _proxy->setDynamicSortFilter(true);
    _proxy->setSortCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    _proxy->setSortRole(DialogsModel::dateRole);
    _proxy->sort(0, Qt::DescendingOrder);
    _proxy->setSourceModel(_model);

    connect(Client::instance()->longPoll(), SIGNAL(messageInAdded(DialogItem)), this, SLOT(onLongPollMessageInAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(messageOutAdded(DialogItem)), this, SLOT(onLongPollMessageOutAdded(DialogItem)));
    connect(Client::instance()->longPoll(), SIGNAL(userStatusChanged(int,bool)), this, SLOT(onUserStatusChanged(int,bool)));
    connect(Client::instance()->longPoll(), SIGNAL(unreadDialogs(int)), this, SLOT(setUnreadDialogs(int)));

    qRegisterMetaType<DialogsModel*>("DialogsModel*");
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

int DialogsHandler::unreadDialogs() const
{
    return _unreadDialogs;
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

int DialogsHandler::indexOf(const int id, const bool proxyIndex) const
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

void DialogsHandler::incUnreadDialogs()
{
    setUnreadDialogs(_unreadDialogs + 1);
}

void DialogsHandler::decUnreadDialogs()
{
    setUnreadDialogs(_unreadDialogs - 1);
}

void DialogsHandler::setUnreadDialogs(const int unreadDialogs)
{
    if (_unreadDialogs != unreadDialogs)
    {
        _unreadDialogs = unreadDialogs;
        emit unreadDialogsChanged(_unreadDialogs);
    }
}

void DialogsHandler::onLongPollMessageInAdded(const DialogItem dialog)
{
    int id = dialog->id();
    int i = _model->indexOf(id);

    if (i > -1)
    {
        _model->at(i)->setMessage(dialog->message());
        _model->at(i)->incUnreadDialogs();
    }
    else
    {
        dialog->incUnreadDialogs();
        dialog->getMessage(Client::instance()->connection()); //update unread
        _model->append(dialog);
    }

    static QString fileName = Settings::instance()->dataDir() + "/sounds/message.wav";
    static QString cmd = Settings::instance()->loadMain("main/cmdSound", "aplay -q").toString();
    Utils::playSound(fileName, cmd);
}

void DialogsHandler::onLongPollMessageOutAdded(const DialogItem dialog)
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

void DialogsHandler::onUserStatusChanged(const int uid, const bool online)
{
    int i = _model->indexOf(uid);

    if (i > -1)
    {
        _model->at(i)->profile()->setOnline(online);
    }
}
