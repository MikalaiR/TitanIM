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
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QUrlQuery>
#include <QNetworkConfigurationManager>
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
    void connectToVk(const QString &username, const QString &password, const bool https=true);
    void connectToVk(const int uid, const QString &token, const QString &secret="");
    void disconnectVk();
    bool isAuthorized() const;
    bool isHttps() const;
    void appendQuery(Packet *packet);
    void setCaptcha(const QString &sid, const QString &text);
    void cancelCaptcha();
    SessionVars session() const;

private:
    void setHttpsMode(const bool isHttps);
    void getToken(const QString &version="5.21");
    void checkToken();
    void execQuery();
    void clearAllQuery(const ErrorResponse::Error &code, const QString &msg);
    void clearPerishableQuery(const ErrorResponse::Error &code, const QString &msg);

private:
    LoginVars _loginVars;
    SessionVars _sessionVars;
    UrlServers _urlServers;
    bool _isHttps;
    bool _isProcessing;
    bool _isAuthorized;
    QQueue<Packet*> _queryList;
    QTimer *tooManyRequestsTimer;
    QNetworkConfigurationManager *bearer;
    QNetworkAccessManager *httpAuth;
    QNetworkAccessManager *httpApi;

protected:
    void onHttpsModeChanged(const bool isHttps);
    void onError(const ErrorResponse *errorResponse);
    void onError(const ErrorResponse::Error &code, const QString &msg);

private slots:
    void getTokenFinished(QNetworkReply *networkReply);
    void successfullyToken(const Packet *sender, const QVariantMap &result);
    void unsuccessfullyToken(const Packet *sender, const ErrorResponse *errorResponse);
    void apiResponse(QNetworkReply *networkReply);
    void onNetworkOnlineChanged(const bool isOnline);
    void onTooManyRequestsTimerTick();

signals:
    void authorized(const int uid, const QString &token, const QString &secret);
    void logout(const int uid);
    void error(const ErrorResponse::Error &error, const QString &text, const bool global, const bool fatal);
    void captcha(const QString &captchaSid, const QString &captchaImg);
    void validation(const QString &validationUri);
    void networkOnlineChanged(bool isOnline);
};

#endif // CONNECTION_H
