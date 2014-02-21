#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QObject>
#include "vk/client.h"
#include "settings.h"

class Authorization : public QObject
{
    Q_OBJECT

public:
    Authorization();

public slots:
    void connectToVk();
    void connectToVk(const QString &username, const QString &password);

private slots:
    void onConnected(const int uid, const QString &token, const QString &secret);
    void onDisconnected();
    void onError(const Error &error, const QString &text, const bool global, const bool fatal);

signals:
    void showAuthPage();
    void showMainPage();
};

#endif // AUTHORIZATION_H
