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

#ifndef SERVICEMSGITEMPRIVATE_H
#define SERVICEMSGITEMPRIVATE_H

#include <QObject>
#include <QSharedPointer>
#include "messagebase.h"

class ServiceMsgItemPrivate : public MessageBase
{
    Q_OBJECT

public:
    ServiceMsgItemPrivate();
    QString body() const;
    void setBody(const QString &body);

private:
    QString _body;
};

typedef QSharedPointer<ServiceMsgItemPrivate> ServiceMsgItem;

#endif // SERVICEMSGITEMPRIVATE_H
