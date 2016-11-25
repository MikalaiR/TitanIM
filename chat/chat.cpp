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

#include "chat.h"

int Chat::_internalMessageId = 0;

Chat::Chat()
{
    _countUnsent = 0;
    _outAttachments = 0;

    _sendMessageHandler = new SendMessageHandler(Client::instance()->connection());
    connect(_sendMessageHandler, &SendMessageHandler::sending, this, &Chat::onMessageSending);
    connect(_sendMessageHandler, &SendMessageHandler::successfullyMessageSent, this, &Chat::onSuccessfullyMessageSent);
    connect(_sendMessageHandler, &SendMessageHandler::unsuccessfullyMessageSent, this, &Chat::onUnsuccessfullyMessageSent);
}

Chat::Chat(const DialogItem dialog) : Chat()
{
    _dialog = dialog;
    connect(_dialog.data(), &DialogItemPrivate::newTyping, [this](const TypingItem typing){ addTyping(typing); });

    _model = new ChatModel(_dialog, this);
    connect(_model, &ChatModel::rowsAllReplaced, this, &Chat::onModelRowsAllReplaced);
    connect(_model, &ChatModel::selectedCountChanged, this, &Chat::selectedCountChanged);
}

Chat::~Chat()
{
    if (_outAttachments) delete _outAttachments;

    delete _sendMessageHandler;
    delete _model;
}

int Chat::id() const
{
    return _dialog->id();
}

bool Chat::isGroupChat() const
{
    return _dialog->isGroupChat();
}

bool Chat::isCurrent() const
{
    return _dialog->isCurrent();
}

bool Chat::isBusy() const
{
    return countUnsent() || outAttachmentsCount();
}

DialogItem Chat::dialog() const
{
    return _dialog;
}

ChatModel *Chat::model() const
{
    return _model;
}

int Chat::countUnsent() const
{
    return _countUnsent;
}

int Chat::countUnread() const
{
    return _dialog->unreadCount();
}

QString Chat::textMessage() const
{
    return _textMessage;
}

void Chat::setTextMessage(const QString &text)
{
    if (_textMessage == text)
    {
        return;
    }

    _textMessage = text;
    emit textMessageChanged(text);
}

AttachmentList *Chat::outAttachments() const
{
    return _outAttachments;
}

int Chat::outAttachmentsCount() const
{
    if (!_outAttachments || !_outAttachments->count())
    {
        return 0;
    }
    else
    {
        _outAttachments->count();
    }
}

int Chat::selectedCount() const
{
    return _model->selectedCount();
}

QString Chat::selectedCountStr() const
{
    int c = selectedCount();

    if (c == 0)
    {
        return "";
    }

    QString s = Utils::pluralForm(c, QObject::tr("message", "1"), QObject::tr("messages", "2"), QObject::tr("messages", "5"));
    return QString("%1 %2").arg(c).arg(s);
}

void Chat::addInMessage(const MessageItem message)
{
    _model->prepend(message, true);
}

void Chat::addOutMessage(const MessageItem message)
{
    _model->prepend(message, true);
}

void Chat::addTempOutMessageQueue(MessageItem message)
{
    _tempOutMessageQueue.append(message);
}

void Chat::addTyping(const TypingItem typing)
{
    typing->setId(--_internalMessageId);
    _model->prepend(typing);
}

void Chat::addTyping(const QList<TypingItem> &typingList)
{
    for (int i = 0; i < typingList.count(); i++)
    {
        addTyping(typingList.at(i));
    }
}

void Chat::addServiceMsg(const ServiceMsgItem serviceMsg)
{
    serviceMsg->setId(--_internalMessageId);
    _model->prepend(serviceMsg);
}

void Chat::sendMessage(const QString &text)
{
    if (text.isEmpty() && (!_outAttachments || !_outAttachments->count()))
    {
        return;
    }

    MessageItem message = MessageItem::create();
    message->setId(--_internalMessageId);
    message->setIsFake(true);

    if (isGroupChat())
    {
        message->setChatId(_dialog->groupChatHandler()->chatId());
        message->setUid(Client::instance()->uid());
    }
    else
    {
        message->setUid(_dialog->profile()->id());
    }

    message->setDate(Utils::currentDateTime());
    message->setIsUnread(true);
    message->setIsOut(true);
    message->setBody(text, false, false, true);

    message->setAttachments(_outAttachments);

    _outAttachments = 0;
    emit outAttachmentsChanged(_outAttachments);

    _model->prepend(message);
//    _dialog->setMessage(message); //todo

    _sendMessageHandler->send(message);
}

void Chat::sendTyping()
{
    if (!Settings::instance()->loadProfile("chat/autoRead", true).toBool() && countUnread())
    {
        markAsRead();
    }

    Packet *packet = new Packet("messages.setActivity");
    packet->addParam("user_id", _dialog->id());
    packet->addParam("type", "typing");
    Client::instance()->connection()->appendQuery(packet);
}

void Chat::addAttachments(const QList<QUrl> &list, const bool asDoc)
{
    if (!_outAttachments)
    {
        _outAttachments = new AttachmentList();
    }

    for (int i = 0; i < list.count(); i++)
    {
        QUrl fileName = list.at(i);
        QFileInfo fileInfo(fileName.toString());
        QString title = fileInfo.fileName();

        int ext = -1;

        if (!asDoc)
        {
            QString extStr = Utils::firstUpper(fileName.toLocalFile().section('.', -1).toLower());
            ext = Attachment::metaEnumerator("Extension").keyToValue(extStr.toLatin1());
        }

        switch (ext) {
        case Attachment::Jpeg:
        case Attachment::Jpg:
        case Attachment::Png:
        case Attachment::Bmp:
        case Attachment::Gif:
        {
            PhotoItem photo = PhotoItem::create();
            photo->setUploadProgress(0);
            photo->setSrc(fileName);
            photo->setSrcBig(fileName);
            _outAttachments->add(photo);
            break;
        }

        case Attachment::Avi:
        case Attachment::Mpg:
        {
            VideoItem video = VideoItem::create();
            video->setUploadProgress(0);
            video->setTitle(title);
            video->setSrc(QUrl("images/upload_video.png"));
            video->setSrcBig(QUrl("images/upload_video.png"));
            video->setUrl(fileName);
            _outAttachments->add(video);
            break;
        }

        case Attachment::Mp3:
        {
            AudioItem audio = AudioItem::create();
            audio->setUploadProgress(0);
            audio->setTitle(fileInfo.baseName());
            audio->setUrl(fileName);
            _outAttachments->add(audio);
            break;
        }

        default:
        {
            DocItem doc = DocItem::create();
            doc->setUploadProgress(0);
            doc->setTitle(title);
            doc->setUrl(fileName);
            _outAttachments->add(doc);
            break;
        }
        }
    }

    emit outAttachmentsChanged(_outAttachments);
}

void Chat::addFwdMessages(const MessageList messages)
{
    if (messages->count() == 0)
        return;

    if (!_outAttachments)
    {
        _outAttachments = new AttachmentList();
    }

    FwdMsgItem fwdMsg = FwdMsgItem::create();
    fwdMsg->setMessages(messages);

    _outAttachments->add(fwdMsg);
    emit outAttachmentsChanged(_outAttachments);
}

void Chat::removeAttachment(const int index)
{
    if (!_outAttachments || !_outAttachments->count())
    {
        return;
    }

    _outAttachments->removeAt(index);
    emit outAttachmentsChanged(_outAttachments);
}

void Chat::markAsRead()
{
    Packet *packet = new Packet("messages.markAsRead");
    packet->addParam("user_id", _dialog->id());
    Client::instance()->connection()->appendQuery(packet);
}

void Chat::refreshHistory()
{
    _model->load(20);
}

void Chat::clearHistory()
{
    Packet *packet = new Packet("messages.deleteDialog");
    packet->addParam("user_id", _dialog->id());
    Client::instance()->connection()->appendQuery(packet);
}

MessageList Chat::getSelectedItems() const
{
    return _model->getSelectedItems();
}

void Chat::deleteSelectedItems()
{
    _model->deleteSelectedItems();
}

void Chat::copyTextSelectedItems()
{
    _model->copyTextSelectedItems();
}

void Chat::clearSelected()
{
    _model->clearSelected();
}

QString Chat::actionToString(const QString &author, const int act, const QString &arg, const int sex)
{
    return Utils::actionToString(author, act, arg, sex);
}

void Chat::onMessageSending(const int internalMid)
{
    _countUnsent++;
}

void Chat::onSuccessfullyMessageSent(const int internalMid, const int serverMid)
{
    _countUnsent--;

    if (countUnsent() == 0 && _tempOutMessageQueue.count())
    {
        for (int i = 0; i < _tempOutMessageQueue.count(); i++)
        {
            _model->prepend(_tempOutMessageQueue.at(i), true);
        }

        _tempOutMessageQueue.clear();
    }
}

void Chat::onUnsuccessfullyMessageSent(const int internalMid)
{
    _countUnsent--;

    if (countUnsent() == 0 && _tempOutMessageQueue.count())
    {
        for (int i = 0; i < _tempOutMessageQueue.count(); i++)
        {
            _model->prepend(_tempOutMessageQueue.at(i), true);
        }

        _tempOutMessageQueue.clear();
    }
}

void Chat::onModelRowsAllReplaced()
{
    if (_dialog->typingHandler())
    {
        addTyping(_dialog->typingHandler()->toList());
    }
}
