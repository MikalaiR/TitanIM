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

#ifndef DIALOGSHANDLER_H
#define DIALOGSHANDLER_H

#include <QObject>
#include <QSortFilterProxyModel>
#include "dialogsmodel.h"
#include "vk/client.h"

class DialogsHandler : public QObject
{
    Q_OBJECT

public:
    DialogsHandler();
    ~DialogsHandler();
    DialogsModel* model() const;
    QSortFilterProxyModel* proxy() const;
    DialogItem dialog(const int index, const bool isProxyIndex=true) const;

private:
    DialogsModel *_model;
    QSortFilterProxyModel *_proxy;

protected slots:
    void onLongPollMessageAdded(const DialogItem dialog);
    void onUserStatusChanged(const int uid, const bool online);
};

#endif // DIALOGSHANDLER_H
