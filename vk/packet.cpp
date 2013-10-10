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

#include "packet.h"

Packet::Packet(QString method)
{
    _method = method;
}

int Packet::id() const
{
    return _id;
}

void Packet::setId(const int id)
{
    _id = id;
}

QString Packet::method() const
{
    return _method;
}

QString Packet::dataUser() const
{
    return _dataUser;
}

void Packet::setDataUser(const QString &dataUser)
{
    _dataUser = dataUser;
}

QString Packet::genQuery(const QString &token, const QString &secret)
{
    addParam("access_token", token);

    QString query = QString("/method/%1?%2")
            .arg(_method)
            .arg(Utils::join(_paramsPacket, '&'));

    QString sig = QString("&sig=%1")
            .arg(Utils::getMd5String(query + secret));

    query = QString("/method/%1?%2")
            .arg(_method)
            .arg(Utils::joinAndEscape(_paramsPacket, '&'));

    return query + sig;
}

void Packet::addParam(const QString &key, const QString &value)
{
    _paramsPacket[key] = value;
}

void Packet::addParam(const QString &key, const int value)
{
    addParam(key, QString::number(value));
}

QString Packet::operator[](QString key)
{
    if (_paramsPacket.contains(key))
    {
        return _paramsPacket[key];
    }
    else
    {
        return "";
    }
}

QVariantMap Packet::result() const
{
    return _result;
}

void Packet::setResult(const QVariantMap &result)
{
    _result = result;
    emit finished(_result);
}

void Packet::setError(const ErrorResponse *errorResponse)
{
    emit error(errorResponse->code(), errorResponse->msg(), errorResponse->global(), errorResponse->fatal());
}

bool Packet::contains(const QString &key)
{
    return _paramsPacket.contains(key);
}
