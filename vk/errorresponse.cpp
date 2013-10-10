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

#include "errorresponse.h"

ErrorResponse::ErrorResponse(const QVariantMap &response)
{
    try
    {
        QVariantMap error;

        switch (response["error"].type()) {
        case QVariant::String:
        {
            QVariant tempError = response["error"];

            QString errorString = tempError.toString().toLower();

            if (errorString.contains("captcha"))
            {
                _code = captchaNeeded;
            }
            else if (errorString.contains("invalid_client"))
            {
                _code = userAuthorizationFailed;
            }
            else
            {
                _code = unknownErrorOccured;
            }

            _msg = tempError.toString();

            error = response;
            break;
        }

        case QVariant::Map:
        {
            QVariantMap tempError = response["error"].toMap();

            _code = (Error)tempError["error_code"].toInt();
            _msg = tempError["error_msg"].toString();

            error = tempError;
            break;
        }

        default:
        {
            throw;
            break;
        }
        }

        if (_code == captchaNeeded)
        {
            _captchaSid = error["captcha_sid"].toString();
            _captchaImg = error["captcha_img"].toString();
        }
    }
    catch(...)
    {
        _code = unknownErrorOccured;
        _msg = "Unknown error occurred";
    }

    _global = isGlobal(_code);
    _fatal = isFatal(_code);
}

ErrorResponse::ErrorResponse(const Error &code, const QString &msg)
{
    _code = code;
    _msg = msg;
    _global = isGlobal(_code);
    _fatal = isFatal(_code);
}

Error ErrorResponse::code() const
{
    return _code;
}

QString ErrorResponse::msg() const
{
    return _msg;
}

bool ErrorResponse::global() const
{
    return _global;
}

bool ErrorResponse::fatal() const
{
    return _fatal;
}

QString ErrorResponse::captchaSid() const
{
    return _captchaSid;
}

QString ErrorResponse::captchaImg() const
{
    return _captchaImg;
}

bool ErrorResponse::isGlobal(const Error &code)
{
    if (code == loadTokenFailed ||
        code == timeoutLongPollServer ||
        code == serverIsNotAvailable ||
        code == unknownErrorOccured ||
        code == applicationIsDisabled ||
        code == unknownMethodPassed ||
        code == incorrectSignature ||
        code == userAuthorizationFailed ||
        code == tooManyRequestsPerSecond ||
        code == captchaNeeded)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool ErrorResponse::isFatal(const Error &code)
{
    if (code == loadTokenFailed ||
        code == applicationIsDisabled ||
        code == userAuthorizationFailed)
    {
        return true;
    }
    else
    {
        return false;
    }
}
