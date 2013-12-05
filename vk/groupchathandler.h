#ifndef GROUPCHATHANDLER_H
#define GROUPCHATHANDLER_H

#include <QObject>
#include <QStringList>
#include "connection.h"
#include "profileparser.h"

class GroupChatHandler : public QObject
{
    Q_OBJECT

public:
    GroupChatHandler(const int chatId);
    int chatId() const;
    QVector<int> chatActive();
    void setChatActive(const QVector<int> &chatActive);
    void addUser(ProfileItem profile);
    QStringList avatars() const;
    int usersCount() const;
    void setUsersCount(const int usersCount);
    void setCover(const QString &cover);
    QString title() const;
    void setTitle(const QString &title);
    int adminId() const;
    void setAdminId(const int adminId);
    bool isCover() const;
    void getAllFields(Connection *connection);

private:
    int _chatId;
    QVector<int> _chatActive;
    ProfileList _users;
    QStringList _avatars;
    int _usersCount;
    QString _cover;
    QString _title;
    int _adminId;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &result);

signals:
    void propertyChanged(const int chatId, const QString &propertyName);
};

#endif // GROUPCHATHANDLER_H
