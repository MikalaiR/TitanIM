#ifndef CHATS_H
#define CHATS_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QQmlComponent>
#include "chatshandler.h"
#include "vk/dialogitem.h"

class Chats : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QSortFilterProxyModel* currentChatModel READ currentChatModel CONSTANT)
    Q_PROPERTY(DialogItemPrivate* currentChatDialog READ currentChatDialog NOTIFY changedCurrentChat)

public:
    static Chats *instance();
    static void destroy();
    int currentChatId() const;
    Chat* currentChat() const;
    QSortFilterProxyModel* currentChatModel() const;
    DialogItemPrivate* currentChatDialog() const;

private:
    explicit Chats();
    ~Chats();

private:
    static Chats *aInstance;
    ChatsHandler *_chatsHandler;
    int _currentChatId;
    QSortFilterProxyModel *_proxy;
    DialogItemPrivate *_dialog;

protected:
    void setCurrentChat(const int id);

public slots:
    void openChat(const DialogItem dialog);

signals:
    void changedCurrentChat(const int id);
};

#endif // CHATS_H
