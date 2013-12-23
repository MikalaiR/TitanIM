#include "dialogparser.h"

DialogItem DialogParser::parser(const QVariantMap &item, const ProfileList &profiles)
{
    DialogItem dialog = DialogItem::create();

    MessageItem message = MessageParser::parser(item);
    ProfileItem profile = profiles->item(message->uid());

    GroupChatHandler *groupChatHandler = 0;
    if (item.contains("chat_active"))
    {
        groupChatHandler = new GroupChatHandler(message->chatId());
        QVector<int> chatActive = Utils::toVectorInt(item.value("chat_active").toList());

        foreach (int uid, chatActive) {
            groupChatHandler->addUser(profiles->item(uid));
        }

        groupChatHandler->setTitle(message->title());
        groupChatHandler->setUsersCount(item.value("users_count").toInt());
        groupChatHandler->setCover(item.value("photo_50").toString());
        groupChatHandler->setAdminId(item.value("admin_id").toInt());
    }

    dialog->setProfile(profile);
    dialog->setMessage(message);
    dialog->setGroupChatHandler(groupChatHandler);

    return dialog;
}

DialogList DialogParser::parser(const QVariantList &items, const ProfileList &profiles)
{
    DialogList dialogs = DialogList::create();

    foreach (QVariant item, items)
    {
        DialogItem dialog = parser(item.toMap(), profiles);
        dialogs->add(dialog);
    }

    return dialogs;
}
