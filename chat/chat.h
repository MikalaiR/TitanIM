#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include "vk/dialogitem.h"
#include "chatmodel.h"

class Chat : public QObject
{
    Q_OBJECT

public:
    explicit Chat(const DialogItem dialog, const ProfileItem ownProfile);
    ~Chat();
    DialogItem dialog() const;
    ChatModel* model() const;

private:
    DialogItem _dialog;
    ChatModel *_model;
};

#endif // CHAT_H
