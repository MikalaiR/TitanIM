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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QQueue>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "packet.h"
#include "errorresponse.h"
#include "global.h"
#include "utils.h"

class Connection : public QObject
{
    Q_OBJECT

public:
    Connection(const QString &clientId, const QString &clientSecret);
    ~Connection();
    bool isOnline();
    bool isOffline();
    void connectToVk(const QString &username, const QString &password);
    void connectToVk(const int uid, const QString &token, const QString &secret);
    void disconnectVk();
    void appendQuery(Packet *packet);
    void setCaptcha(const QString &sid, const QString &text);

private:
    void setStatus(const Status &status);
    void getToken();
    void loginSuccess(const QVariantMap &response);
    void loginFailure(const QVariantMap &response);
    void execQuery();

private:
    LoginVars _loginVars;
    SessionVars _sessionVars;
    UrlServers _urlServers;
    Status _status;
    bool _isProcessing;
    QQueue<Packet*> _queryList;
    QTimer *tooManyRequestsTimer;
    QNetworkAccessManager *httpAuth;
    QNetworkAccessManager *http;

protected:
    void onStatusChanged(const Status &status);
    void onConnected();
    void onDisconnected();
    void onError(const ErrorResponse *errorResponse);
    void onError(const Error &code, const QString &msg);

private slots:
    void getTokenFinished(QNetworkReply *networkReply);
    void apiResponse(QNetworkReply *networkReply);
    void onTooManyRequestsTimerTick();

signals:
    void connected(const QString &token, const QString &secret, const int uid);
    void disconnected();
    void error(const Error &error, const QString &text, const bool global, const bool fatal);
    void captcha(const QString &captchaSid, const QString &captchaImg);
};

#endif // CONNECTION_H
