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

Packet::Packet(const QString &method, const QString &version)
{
    _method = method;
    _id = 0;
    _paramsPacket.clear();
    _isPerishable = false;
    _errorResponse = 0;

    addParam("v", version);
}

Packet::~Packet()
{
    if (_errorResponse)
    {
        delete _errorResponse;
    }
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

bool Packet::isPerishable() const
{
    return _isPerishable;
}

void Packet::setPerishable(const bool isPerishable)
{
    _isPerishable = isPerishable;
}

QString Packet::dataUser() const
{
    return _dataUser;
}

void Packet::setDataUser(const QString &dataUser)
{
    _dataUser = dataUser;
}

void Packet::signPacket(const QString &secret)
{
    if (!secret.isEmpty())
    {
        removeParam("sig");

        QString query = QString("%1?%2")
                .arg(urlPath())
                .arg(Utils::join(_paramsPacket, '&'));

        QString sig = Utils::getMd5String(query + secret);

        addParam("sig", sig);
    }
}

QString Packet::urlPath() const
{
    return QString("/method/%1").arg(_method);
}

QByteArray Packet::urlQuery() const
{
    return Utils::joinAndEscape(_paramsPacket, '&').toLatin1();
}

void Packet::addParam(const QString &key, const QString &value)
{
    _paramsPacket[key] = value;
}

void Packet::addParam(const QString &key, const int value)
{
    addParam(key, QString::number(value));
}

void Packet::removeParam(const QString &key)
{
    _paramsPacket.remove(key);
}

QString Packet::value(const QString &key)
{
    return _paramsPacket.value(key);
}

QVariantMap Packet::result() const
{
    return _result;
}

QVariantMap Packet::response() const
{
    return _result.value("response").toMap();
}

ErrorResponse *Packet::errorResponse() const
{
    return _errorResponse;
}

void Packet::setResult(const QVariantMap &result)
{
    _result = result;

    static const QMetaMethod finishedSignal = QMetaMethod::fromSignal(&Packet::finished);

    if (isSignalConnected(finishedSignal)) {
        emit finished(this, _result);
    }
    else
    {
        deleteLater();
    }
}

void Packet::setError(ErrorResponse *errorResponse)
{
    _errorResponse = errorResponse;

    static const QMetaMethod errorSignal = QMetaMethod::fromSignal(&Packet::error);

    if (isSignalConnected(errorSignal)) {
        emit error(this, _errorResponse);
    }
    else
    {
        deleteLater();
    }
}

void Packet::setError(const ErrorResponse::Error &code, const QString &msg)
{
    ErrorResponse *error = new ErrorResponse(code, msg);
    setError(error);
}

bool Packet::contains(const QString &key)
{
    return _paramsPacket.contains(key);
}

QDebug operator <<(QDebug dbg, const Packet &packet)
{
    dbg.nospace() << "Packet ("
                  << "\n  METHOD: "   << packet.method()
                  << "\n  QUERY: "    << packet.urlQuery()
                  << "\n  RESULT: "   << packet.result()
                  << "\n)";
    return dbg.space();
}
