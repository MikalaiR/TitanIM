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
#include <QMap>
#include "global.h"
#include "errorresponse.h"
#include "utils.h"

class Packet : public QObject
{
    Q_OBJECT

public:
    Packet(QString method);
    int id() const;
    void setId(const int id);
    QString method() const;
    QString dataUser() const;
    void setDataUser(const QString &dataUser);
    QVariantMap result() const;
    QString genQuery(const QString &token, const QString &secret);
    void addParam(const QString &key, const QString &value);
    void addParam(const QString &key, const int value);
    QString operator[](QString key);
    void setResult(const QVariantMap &result);
    void setError(const ErrorResponse *errorResponse);
    bool contains(const QString &key);

private:
    int _id;
    QString _method;
    QMap<QString, QString> _paramsPacket;
    QString _dataUser;
    QVariantMap _result;

signals:
    void finished(const QVariantMap &result);
    void error(const Error &error, const QString &text, const bool global, const bool fatal);
};

#endif // PACKET_H
