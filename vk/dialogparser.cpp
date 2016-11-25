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

#include "dialogparser.h"

void DialogParser::parser(const QVariantMap &item, DialogItemPrivate *dialog, const ProfileList &profiles)
{
    QVariantMap messageItem = item.contains("message") ? item.value("message").toMap() : item;

    if (messageItem.isEmpty())
    {
        if (profiles->count() == 1 && !profiles->at(0)->id() == dialog->id())
        {
            dialog->setProfile(Client::instance()->engine()->getProfile(dialog->id()));
            dialog->createStructure();
        }

        return;
    }

    int unreadCount = item.contains("unread") ? item.value("unread").toInt() : 0;

    MessageItem message = MessageParser::parser(messageItem);
    profiles->add(Client::instance()->profile());
    ProfileItem profile = profiles->item(message->uid());

    GroupChatHandler *groupChatHandler = 0;

    if (messageItem.contains("chat_active"))
    {
        groupChatHandler = new GroupChatHandler(message->chatId());
        groupChatHandler->setAdminId(messageItem.value("admin_id").toInt());
        int selfUid = Client::instance()->uid();

        QVector<int> chatActive = Utils::toVectorInt(messageItem.value("chat_active").toList());
        if (chatActive.count() && chatActive.at(0) != selfUid)
        {
            chatActive.append(selfUid);
        }

        ProfileList chatActiveList = ProfileList::create();
        for (int uid: chatActive)
        {
            chatActiveList->add(profiles->item(uid));
        }

        groupChatHandler->setUsers(chatActiveList);
        groupChatHandler->setTitle(message->title());

        if (messageItem.contains("users_count") && chatActive.count())
        {
            groupChatHandler->setUsersCount(messageItem.value("users_count").toInt());
        }
        else
        {
            if (message->action() == MessageItemPrivate::Chat_kick_self && message->isOut())
            {
                groupChatHandler->setLeft(true);
            }
            else
            {
                groupChatHandler->setKicked(true);
            }
        }

        groupChatHandler->setCover(messageItem.value("photo_100").toString());
    }

    dialog->beginChangeGroupProperties();

    dialog->setProfile(profile);
    dialog->setMessage(message);
    dialog->setGroupChatHandler(groupChatHandler);
    dialog->setUnreadCount(unreadCount);

    dialog->endChangeGroupProperties();
}

void DialogParser::parser(const QVariantMap &item, DialogItem dialog, const ProfileList &profiles)
{
    parser(item, dialog.data(), profiles);
}

DialogItem DialogParser::parser(const QVariantMap &item, const ProfileList &profiles)
{
    DialogItem dialog = DialogItem::create();
    parser(item, dialog.data(), profiles);

    return dialog;
}

DialogList DialogParser::parser(const QVariantList &items, const ProfileList &profiles)
{
    DialogList dialogs = DialogList::create();

    for (QVariant item: items)
    {
        DialogItem dialog = DialogItem::create();
        parser(item.toMap(), dialog.data(), profiles);
        dialogs->add(dialog);
    }

    Client::instance()->engine()->setMaxMsgId(dialogs->at(0)->message()->id());

    return dialogs;
}
