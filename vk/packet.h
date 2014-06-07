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

#ifndef PACKET_H
#define PACKET_H

#include <QObject>
#include <QDebug>
#include <QMap>
#include "errorresponse.h"
#include "utils.h"

class Packet : public QObject
{
    Q_OBJECT

public:
    Packet(const QString &method, const QString &version="5.21");
    ~Packet();
    int id() const;
    void setId(const int id);
    QString method() const;
    QString dataUser() const;
    void setDataUser(const QString &dataUser);
    QVariantMap result() const;
    QVariantMap response() const;
    ErrorResponse *errorResponse() const;
    void signPacket(const QString &secret);
    QString urlPath() const;
    QByteArray urlQuery() const;
    void addParam(const QString &key, const QString &value);
    void addParam(const QString &key, const int value);
    void removeParam(const QString &key);
    QString value(const QString &key);
    void setResult(const QVariantMap &result);
    void setError(ErrorResponse *errorResponse);
    bool contains(const QString &key);

private:
    int _id;
    QString _method;
    QMap<QString, QString> _paramsPacket;
    QString _dataUser;
    QVariantMap _result;
    ErrorResponse *_errorResponse;

signals:
    void finished(const Packet *sender, const QVariantMap &result);
    void error(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal);
};

QDebug operator<<(QDebug dbg, const Packet &packet);

#endif // PACKET_H
