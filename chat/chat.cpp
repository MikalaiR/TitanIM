#include "chat.h"

Chat::Chat(const DialogItem dialog, const ProfileItem ownProfile)
{
    _dialog = dialog;
    _model = new ChatModel(_dialog, ownProfile, this);
}

Chat::~Chat()
{
    delete _model;
}

DialogItem Chat::dialog() const
{
    return _dialog;
}

ChatModel *Chat::model() const
{
    return _model;
}
