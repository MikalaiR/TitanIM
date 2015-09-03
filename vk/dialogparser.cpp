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
    int unreadCount = item.contains("unread") ? item.value("unread").toInt() : 0;

    MessageItem message = MessageParser::parser(messageItem);
    ProfileItem profile = profiles->item(message->uid());

    GroupChatHandler *groupChatHandler = 0;

    if (messageItem.contains("chat_active"))
    {
        groupChatHandler = new GroupChatHandler(message->chatId());
        QVector<int> chatActive = Utils::toVectorInt(messageItem.value("chat_active").toList());

        foreach (int uid, chatActive) {
            groupChatHandler->addUser(profiles->item(uid));
        }

        groupChatHandler->setTitle(message->title());

        if (messageItem.contains("users_count"))
        {
            groupChatHandler->setUsersCount(messageItem.value("users_count").toInt());
        }

        groupChatHandler->setCover(messageItem.value("photo_100").toString());
        groupChatHandler->setAdminId(messageItem.value("admin_id").toInt());
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

    foreach (QVariant item, items)
    {
        DialogItem dialog = DialogItem::create();
        parser(item.toMap(), dialog.data(), profiles);
        dialogs->add(dialog);
    }

    Client::instance()->engine()->setMaxMsgId(dialogs->at(0)->message()->id());

    return dialogs;
}
