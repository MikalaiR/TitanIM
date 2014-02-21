#include "authorization.h"

Authorization::Authorization()
{
    connect(Client::instance()->connection(), SIGNAL(connected(int,QString,QString)), this, SLOT(onConnected(int,QString,QString)));
    connect(Client::instance()->connection(), SIGNAL(disconnected()), this, SLOT(onDisconnected()));
    connect(Client::instance()->connection(), SIGNAL(error(Error,QString,bool,bool)), this, SLOT(onError(Error,QString,bool,bool)));
}

void Authorization::connectToVk()
{
    if (Settings::instance()->lastUid().isEmpty())
    {
        emit showAuthPage();
    }
    else
    {
        emit showMainPage();

        QString uid = Settings::instance()->lastUid();
        Settings::instance()->setCurrentUid(uid);
        QString token = Settings::instance()->loadProfile("main/token", "").toString();
        QString secret = Settings::instance()->loadProfile("main/secret", "").toString();

        Client::instance()->connection()->connectToVk(uid.toInt(), token, secret);
    }
}

void Authorization::connectToVk(const QString &username, const QString &password)
{
    Client::instance()->connection()->connectToVk(username, password);
}

void Authorization::onConnected(const int uid, const QString &token, const QString &secret)
{
    qDebug() << "connecting" << " " << token << " " << secret;

    Settings::instance()->setCurrentUid(QString::number(uid));
    Settings::instance()->saveProfile("main/uid", uid);
    Settings::instance()->saveProfile("main/token", token);
    Settings::instance()->saveProfile("main/secret", secret);
    Settings::instance()->saveMain("profiles/last", uid);

    emit showMainPage();
}

void Authorization::onDisconnected()
{
    qDebug() << "disconnecting";
}

void Authorization::onError(const Error &error, const QString &text, const bool global, const bool fatal)
{
    qDebug() << error << text;

    if (error == UserAuthorizationFailed || error == LoadTokenFailed)
    {
        //todo remove token and secret
        emit showAuthPage();
    }
}
