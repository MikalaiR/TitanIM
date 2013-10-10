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

#ifndef ERRORRESPONSE_H
#define ERRORRESPONSE_H

#include <QObject>
#include <QVariant>
#include "global.h"

class ErrorResponse : public QObject
{
    Q_OBJECT

public:
    ErrorResponse(const QVariantMap &response);
    ErrorResponse(const Error &code, const QString &msg);
    Error code() const;
    QString msg() const;
    bool global() const;
    bool fatal() const;
    QString captchaSid() const;
    QString captchaImg() const;
    static bool isGlobal(const Error &code);
    static bool isFatal(const Error &code);

private:
    Error _code;
    QString _msg;
    bool _global;
    bool _fatal;
    QString _captchaSid;
    QString _captchaImg;
};

#endif // ERRORRESPONSE_H
