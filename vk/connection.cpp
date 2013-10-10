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
    _loginVars.scope = "friends,photos,audio,video,docs,notes,status,wall,groups,messages,notifications,nohttps";

    _urlServers.auth_server = QString("https://api.vk.com/oauth/token");
    _urlServers.api_server = "http://api.vk.com";

    _status = offline;
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

bool Connection::isOnline()
{
    return _status == online;
}

bool Connection::isOffline()
{
    return _status ==offline;
}

void Connection::connectToVk(const QString &username, const QString &password)
{
    if (!isOffline() || username.isEmpty() || password.isEmpty())
    {
        return;
    }

    http = new QNetworkAccessManager(this);
    connect(http, SIGNAL(finished(QNetworkReply*)), this, SLOT(apiResponse(QNetworkReply*)));

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

    _sessionVars.user_id = uid;
    _sessionVars.access_token = token;
    _sessionVars.secret = secret;
    setStatus(online);
}

void Connection::disconnectVk()
{
    setStatus(offline);
}

void Connection::getToken()
{
    httpAuth = new QNetworkAccessManager(this);
    connect(httpAuth, SIGNAL(finished(QNetworkReply*)), this, SLOT(getTokenFinished(QNetworkReply*)));

    QString authUrl = QString("%1?grant_type=%2&client_id=%3&client_secret=%4&scope=%5&username=%6&password=%7")
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
            onError(serverIsNotAvailable, "Server is not available");
        }

        return;
    }

    QVariantMap response = Utils::parseJSON(networkReply->readAll()).toMap();
    networkReply->deleteLater();

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
    _sessionVars.access_token = response["access_token"].toString();
    _sessionVars.secret = response["secret"].toString();
    _sessionVars.user_id = response["user_id"].toInt();
    _sessionVars.expires_in = response["expires_in"].toString();

    if (!_sessionVars.access_token.isEmpty())
    {
        setStatus(online);
    }
    else
    {
        onError(loadTokenFailed, "Load token failed");
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

    QString query = _queryList.head()->genQuery(_sessionVars.access_token, _sessionVars.secret);
    QUrl requestUrl = QUrl(_urlServers.api_server + query);

    QNetworkRequest request;
    request.setUrl(requestUrl);
    http->get(request);
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
            onError(serverIsNotAvailable, "Server is not available");
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
            //отправляем в обработчик глобальных ошибок
            //цикл выполнения пакетов остановлен
            onError(errorResponse);
            return;
        }
        else
        {
            //ошибка локальная - заполняем пакет ошибкой и забываем про него
            _queryList.dequeue()->setError(errorResponse);
        }
    }
    else
    {
        //заполняем пакет ответом от сервера ВК
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
        //запрос капчи был при выполнении пакета
        _queryList.head()->addParam("captcha_sid", sid);
        _queryList.head()->addParam("captcha_key", text);
        _isProcessing = false;
        execQuery();
    }
    else if (isOffline() && !text.isEmpty())
    {
        //запрос капчи был при авторизации
        _loginVars.captcha_sid = sid;
        _loginVars.captcha_key = text;
        getToken();
    }
    else
    {
        onError(userAuthorizationFailed, "User authorization failed");
    }
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
    case online:
    {
        onConnected();
        break;
    }

    case offline:
    {
        onDisconnected();
        break;
    }
    }
}

void Connection::onConnected()
{
    emit connected(_sessionVars.access_token, _sessionVars.secret, _sessionVars.user_id);
}

void Connection::onDisconnected()
{
    disconnect(http, SIGNAL(finished(QNetworkReply*)), this, SLOT(apiResponse(QNetworkReply*)));
    http->deleteLater();
    _queryList.clear();
    _isProcessing = false;

    emit disconnected();
}

void Connection::onError(const ErrorResponse *errorResponse)
{
    if (errorResponse->fatal())
    {
        setStatus(offline);
    }

    switch (errorResponse->code())
    {
    case captchaNeeded:
    {
        emit captcha(errorResponse->captchaSid(), errorResponse->captchaImg());
        break;
    }

    case tooManyRequestsPerSecond:
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
}

void Connection::onError(const Error &code, const QString &msg)
{
    ErrorResponse *errorResponse = new ErrorResponse(code, msg);
    onError(errorResponse);
}
