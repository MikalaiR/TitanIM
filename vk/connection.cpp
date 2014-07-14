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

#include "connection.h"

Connection::Connection(const QString &clientId, const QString &clientSecret)
{
    _loginVars.grant_type = "password";
    _loginVars.client_id = clientId;
    _loginVars.client_secret = clientSecret;
    _loginVars.scope = "friends,photos,audio,video,docs,notes,status,wall,groups,messages,notifications";

    _urlServers.auth_server = "https://oauth.vk.com/token";
    _urlServers.api_server = "https://api.vk.com";

    _status = Offline;
    _isHttps = true;
    _isProcessing = false;

    tooManyRequestsTimer = new QTimer();
    tooManyRequestsTimer->setInterval(500);
    connect(tooManyRequestsTimer, SIGNAL(timeout()), this, SLOT(onTooManyRequestsTimerTick()));
}

Connection::~Connection()
{
    delete tooManyRequestsTimer;
}

void Connection::setStatus(const Status &status)
{
    if (_status != status)
    {
        _status = status;
        onStatusChanged(_status);
    }
}

bool Connection::isOnline() const
{
    return _status == Online;
}

bool Connection::isOffline() const
{
    return _status == Offline;
}

bool Connection::isHttps() const
{
    return _isHttps;
}

void Connection::connectToVk(const QString &username, const QString &password, const bool https)
{
    if (!isOffline() || username.isEmpty() || password.isEmpty())
    {
        return;
    }

    setHttpsMode(https);

    httpApi = new QNetworkAccessManager(this);
    connect(httpApi, SIGNAL(finished(QNetworkReply*)), this, SLOT(apiResponse(QNetworkReply*)));

    _loginVars.username = username;
    _loginVars.password = password;

    getToken();
}

void Connection::connectToVk(const int uid, const QString &token, const QString &secret)
{
    if (!isOffline() || token.isEmpty())
    {
        return;
    }

    httpApi = new QNetworkAccessManager(this);
    connect(httpApi, SIGNAL(finished(QNetworkReply*)), this, SLOT(apiResponse(QNetworkReply*)));

    _sessionVars.user_id = uid;
    _sessionVars.access_token = token;
    _sessionVars.secret = secret;

    setHttpsMode(_sessionVars.secret.isEmpty());

    setStatus(Online);
}

void Connection::disconnectVk()
{
    setStatus(Offline);
}

void Connection::setHttpsMode(const bool isHttps)
{
    if (_isHttps != isHttps)
    {
        _isHttps = isHttps;
        onHttpsModeChanged(_isHttps);
    }
}

void Connection::getToken()
{
    httpAuth = new QNetworkAccessManager(this);
    connect(httpAuth, SIGNAL(finished(QNetworkReply*)), this, SLOT(getTokenFinished(QNetworkReply*)));

    QString authUrl = QString("%1?grant_type=%2&client_id=%3&client_secret=%4&scope=%5&username=%6&password=%7&v=5.21")
            .arg(_urlServers.auth_server)
            .arg(_loginVars.grant_type)
            .arg(_loginVars.client_id)
            .arg(_loginVars.client_secret)
            .arg(_loginVars.scope)
            .arg(_loginVars.username)
            .arg(_loginVars.password);

    if (!_loginVars.captcha_sid.isEmpty())
    {
        authUrl += QString("&captcha_sid=%1&captcha_key=%2")
                .arg(_loginVars.captcha_sid)
                .arg(_loginVars.captcha_key);

        _loginVars.captcha_sid.clear();
        _loginVars.captcha_key.clear();
    }

    QNetworkRequest request;
    request.setUrl(authUrl);
    httpAuth->get(request);
}

void Connection::getTokenFinished(QNetworkReply *networkReply)
{
    if (networkReply->error() != QNetworkReply::NoError)
    {
        if (networkReply->error() == QNetworkReply::AuthenticationRequiredError)
        {
            QVariantMap error = Utils::parseJSON(networkReply->readAll()).toMap();
            networkReply->deleteLater();
            httpAuth->deleteLater();

            ErrorResponse *errorResponse = new ErrorResponse(error);
            onError(errorResponse);
        }
        else
        {
            networkReply->deleteLater();
            httpAuth->deleteLater();
            onError(ErrorResponse::ServerIsNotAvailable, "Server is not available");
        }

        return;
    }

    QVariantMap response = Utils::parseJSON(networkReply->readAll()).toMap();
    networkReply->deleteLater();

    if (response.contains("https_required"))
    {
        httpAuth->deleteLater();
        onError(ErrorResponse::HttpAuthorizationFailed, "HTTP authorization failed");
        return;
    }

    if (response.contains("access_token"))
    {
        loginSuccess(response);
    }
    else
    {
        loginFailure(response);
    }

    httpAuth->deleteLater();
}

void Connection::loginSuccess(const QVariantMap &response)
{
    _sessionVars.access_token = response.value("access_token").toString();
    _sessionVars.user_id = response.value("user_id").toInt();
    _sessionVars.expires_in = response.value("expires_in").toString();
    _sessionVars.secret = response.contains("secret") ? response.value("secret").toString() : "";

    if (!_sessionVars.access_token.isEmpty())
    {
        setStatus(Online);
    }
    else
    {
        onError(ErrorResponse::LoadTokenFailed, "Load token failed");
    }
}

void Connection::loginFailure(const QVariantMap &response)
{
    ErrorResponse *errorResponse = new ErrorResponse(response);
    onError(errorResponse);

    return;
}

void Connection::appendQuery(Packet *packet)
{
    if (isOffline())
    {
        return;
    }

    _queryList.enqueue(packet);

    if (!_isProcessing)
    {
        execQuery();
    }
}

void Connection::execQuery()
{
    if (isOffline() || _queryList.isEmpty() || _isProcessing)
    {
        return;
    }

    _isProcessing = true;

    Packet *currentPacket = _queryList.head();
    currentPacket->addParam("access_token", _sessionVars.access_token);
    currentPacket->signPacket(_sessionVars.secret);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(QUrl(_urlServers.api_server + currentPacket->urlPath()));
    httpApi->post(request, currentPacket->urlQuery());
}

void Connection::clearQuery(const ErrorResponse::Error &code, const QString &msg)
{
    while (!_queryList.isEmpty())
    {
        ErrorResponse *errorResponse = new ErrorResponse(code, msg);
        _queryList.dequeue()->setError(errorResponse);
    }

    _isProcessing = false;
}

void Connection::apiResponse(QNetworkReply *networkReply)
{
    if (isOffline())
    {
        networkReply->deleteLater();
        return;
    }

    if (networkReply->error() != QNetworkReply::NoError)
    {
        if (networkReply->error() == QNetworkReply::UnknownNetworkError)
        {
            networkReply->deleteLater();
            onError(ErrorResponse::ServerIsNotAvailable, "Server is not available");
        }
        else
        {
            networkReply->deleteLater();
            _isProcessing = false;
            execQuery();
        }

        return;
    }

    QVariantMap response = Utils::parseJSON(networkReply->readAll()).toMap();
    networkReply->deleteLater();

    if (response.contains("error"))
    {
        ErrorResponse *errorResponse = new ErrorResponse(response);

        if (errorResponse->global())
        {
            //error global. ExecQuery stop
            onError(errorResponse);
            return;
        }
        else
        {
            //error local
            _queryList.dequeue()->setError(errorResponse);
        }
    }
    else
    {
        _queryList.dequeue()->setResult(response);
    }

    _isProcessing = false;

    if (!_queryList.isEmpty())
    {
        execQuery();
    }
}

void Connection::setCaptcha(const QString &sid, const QString &text)
{
    if (!_queryList.isEmpty())
    {
        //captcha in packet
        _queryList.head()->addParam("captcha_sid", sid);
        _queryList.head()->addParam("captcha_key", text);
        _isProcessing = false;
        execQuery();
    }
    else if (isOffline())
    {
        //captcha in authorization
        _loginVars.captcha_sid = sid;
        _loginVars.captcha_key = text;
        getToken();
    }
    else
    {
        onError(ErrorResponse::UserAuthorizationFailed, "User authorization failed");
    }
}

void Connection::cancelCaptcha()
{
    clearQuery(ErrorResponse::CaptchaCanceled, "Captcha canceled");
}

void Connection::onTooManyRequestsTimerTick()
{
    tooManyRequestsTimer->stop();
    _isProcessing = false;
    if (!_queryList.isEmpty())
    {
        execQuery();
    }
}

void Connection::onStatusChanged(const Status &status)
{
    switch (status)
    {
    case Online:
    {
        onConnected();
        break;
    }

    case Offline:
    {
        onDisconnected();
        break;
    }
    }
}

void Connection::onHttpsModeChanged(const bool isHttps)
{
    if (isHttps)
    {
        _loginVars.scope.remove(",nohttps");
        _urlServers.api_server.replace("http://", "https://");
    }
    else
    {
        _loginVars.scope += ",nohttps";
        _urlServers.api_server.replace("https://", "http://");
    }
}

void Connection::onConnected()
{
    emit connected(_sessionVars.user_id, _sessionVars.access_token, _sessionVars.secret);
}

void Connection::onDisconnected()
{
    disconnect(httpApi, SIGNAL(finished(QNetworkReply*)), this, SLOT(apiResponse(QNetworkReply*)));
    httpApi->deleteLater();
    _queryList.clear();
    _isProcessing = false;

    emit disconnected();
}

void Connection::onError(const ErrorResponse *errorResponse)
{
    if (errorResponse->fatal())
    {
        setStatus(Offline);
    }

    switch (errorResponse->code())
    {
    case ErrorResponse::CaptchaNeeded:
    {
        emit captcha(errorResponse->captchaSid(), errorResponse->captchaImg());
        break;
    }

    case ErrorResponse::HttpAuthorizationFailed:
    {
        setHttpsMode(true);

        if (!_queryList.isEmpty())
        {
            _isProcessing = false;
            execQuery();
        }
        else
        {
            getToken();
        }

        break;
    }

    case ErrorResponse::ValidationRequired:
    {
        emit validation(errorResponse->validationUri());
        break;
    }

    case ErrorResponse::TooManyRequestsPerSecond:
    {
        tooManyRequestsTimer->start();
        break;
    }

    default:
    {
        emit error(errorResponse->code(), errorResponse->msg(), errorResponse->global(), errorResponse->fatal());
        break;
    }
    }

    delete errorResponse;
}

void Connection::onError(const ErrorResponse::Error &code, const QString &msg)
{
    ErrorResponse *errorResponse = new ErrorResponse(code, msg);
    onError(errorResponse);
}
