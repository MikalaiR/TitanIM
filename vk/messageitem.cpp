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

#include "messageitem.h"
#include "messageparser.h"

MessageItemPrivate::MessageItemPrivate()
{
    setMessageType(Text);

    _isError = false;
    _deliveryReport = false;
    _chatId = 0;
    _emoji = false;
    _link = false;
    _action = None;
    _actionMid = 0;
    _attachments = 0;
    _isLoading = false;
}

MessageItemPrivate::~MessageItemPrivate()
{
    if (_attachments) delete _attachments;
}

int MessageItemPrivate::fromId() const
{
    return isGroupChat() ? _chatId + GROUP_CHAT_OFFSET : _uid;
}

void MessageItemPrivate::setFromId(const int fromId)
{
    if (fromId > GROUP_CHAT_OFFSET)
    {
        setChatId(fromId - GROUP_CHAT_OFFSET);
    }
    else
    {
        setUid(fromId);
    }
}

uint MessageItemPrivate::unixtime() const
{
    return _date.toTime_t();
}

bool MessageItemPrivate::isUnread() const
{
    return _isUnread;
}

void MessageItemPrivate::setIsUnread(const bool isUnread)
{
    if (_isUnread != isUnread)
    {
        _isUnread = isUnread;
        emitPropertyChanged("isUnread");
    }
}

bool MessageItemPrivate::isOut() const
{
    return _isOut;
}

void MessageItemPrivate::setIsOut(const bool isOut)
{
    if (_isOut != isOut)
    {
        _isOut = isOut;
        emitPropertyChanged("isOut");
    }
}

bool MessageItemPrivate::isError() const
{
    return _isError;
}

void MessageItemPrivate::setIsError(const bool isError)
{
    if (_isError != isError)
    {
        _isError = isError;
        emitPropertyChanged("isError");
    }
}

bool MessageItemPrivate::isLoading() const
{
    return _isLoading;
}

void MessageItemPrivate::setIsLoading(const bool isLoading)
{
    if (_isLoading != isLoading)
    {
        _isLoading = isLoading;
        emitPropertyChanged("isLoading");
    }
}

bool MessageItemPrivate::isSending() const
{
    return _id < 0;
}

QString MessageItemPrivate::title() const
{
    return _title;
}

void MessageItemPrivate::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emitPropertyChanged("title");
    }
}

QString MessageItemPrivate::body() const
{
    return _body;
}

QString MessageItemPrivate::plainBody() const
{
    QString res = Emoticons::instance()->toEmoji(_body);
    res = Utils::toPlainText(res);
    return Utils::fromHtmlEscaped(res);
}

QString MessageItemPrivate::shortBody() const
{
    if (!_body.isEmpty())
    {
        QString text = _body;

        if (_link)
        {
            text = Utils::fromSmartLinks(text);
        }

        if (!_emoji)
        {
            text = text.left(60);
        }

        text.replace("<br>", " ");

        return text;
    }
    else if (_attachments && _attachments->count())
    {
        return _attachments->description();
    }
    else
    {
        return "...";
    }
}

void MessageItemPrivate::setBody(const QString &body, const bool emoji, const bool escaped, const bool simplified)
{
    if (escaped)
    {
        _body = Utils::toHtmlEscaped(body);
    }
    else
    {
        _body = body;
    }

    _body.replace("\n", "<br>");
    _body = Utils::toSmartLinks(_body, &_link);

    if (emoji)
    {
        _body = Emoticons::instance()->fromEmoji(_body);
    }

    if (simplified)
    {
        _body = Utils::simplified(_body);
    }

    _emoji = emoji;

    emitPropertyChanged("body");
}

int MessageItemPrivate::chatId() const
{
    return _chatId;
}

void MessageItemPrivate::setChatId(const int chatId)
{
    if (_chatId != chatId)
    {
        _chatId = chatId;
        emitPropertyChanged("chatId");
    }
}

bool MessageItemPrivate::isGroupChat() const
{
    return _chatId == 0 ? false : true;
}

bool MessageItemPrivate::emoji() const
{
    return _emoji;
}

MessageItemPrivate::Action MessageItemPrivate::action() const
{
    return _action;
}

void MessageItemPrivate::setAction(const Action action)
{
    if (_action != action)
    {
        _action = action;
        emitPropertyChanged("action");
    }

    setBody(Utils::actionToString(_action), false, false, false);
}

void MessageItemPrivate::setAction(const QString &action)
{
    if (action.isEmpty())
        return;

    int index = staticMetaObject.indexOfEnumerator("Action");
    QMetaEnum metaEnum = staticMetaObject.enumerator(index);
    Action act = (Action)metaEnum.keyToValue(Utils::firstUpper(action).toUtf8());

    if (_uid == _actionMid)
    {
        act = (act == Chat_invite_user) ? Chat_invite_self : Chat_kick_self;
    }

    setAction(act);
}

int MessageItemPrivate::actionMid() const
{
    return _actionMid;
}

void MessageItemPrivate::setActionMid(const int actionMid)
{
    if (_actionMid != actionMid)
    {
        _actionMid = actionMid;
        emitPropertyChanged("actionMid");
    }
}

QString MessageItemPrivate::actionText() const
{
    return _actionText;
}

void MessageItemPrivate::setActionText(const QString &actionText)
{
    if (_actionText != actionText)
    {
        _actionText = actionText;
        emitPropertyChanged("actionText");
    }
}

AttachmentList* MessageItemPrivate::attachments() const
{
    return _attachments;
}

void MessageItemPrivate::setAttachments(AttachmentList *attachments)
{
    if (_attachments != attachments)
    {
        if (_attachments) delete _attachments;

        _attachments = attachments;
        emitPropertyChanged("attachments");
    }
}

void MessageItemPrivate::getAllFields(Connection *connection)
{
    setIsLoading(true);

    Packet *packet = new Packet("execute.messagesGetById");
    packet->addParam("mid", _id);

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    connection->appendQuery(packet);
}

void MessageItemPrivate::loadFinished(const Packet *sender, const QVariantMap &result)
{
    setIsLoading(false);

    QVariantMap response = result.value("response").toMap();
    MessageParser::parser(response, this);

    delete sender;
}
