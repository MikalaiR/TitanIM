#include "groupchathandler.h"

GroupChatHandler::GroupChatHandler(const int chatId)
{
    _chatId = chatId;
    _id = _chatId + GROUP_CHAT_OFFSET;
    _users = ProfileList::create();
    _cover.clear();
    _usersCount = 0;
    _adminId = 0;
}

int GroupChatHandler::id() const
{
    return _id;
}

int GroupChatHandler::chatId() const
{
    return _chatId;
}

void GroupChatHandler::addUser(ProfileItem profile)
{
    _users->add(profile);
    _avatars.append(profile->photoMediumRect());
    emit propertyChanged(_chatId, "users");
}

ProfileItem GroupChatHandler::user(const int uid)
{
    return _users->item(uid);
}

QStringList GroupChatHandler::avatars() const
{
    return isCover() ? QStringList(_cover) : _avatars;
}

int GroupChatHandler::usersCount() const
{
    return _usersCount;
}

void GroupChatHandler::setUsersCount(const int usersCount)
{
    if (_usersCount != usersCount)
    {
        _usersCount = usersCount;
        emit propertyChanged(_chatId, "usersCount");
    }
}

void GroupChatHandler::setCover(const QString &cover)
{
    if (_cover != cover)
    {
        _cover = cover;
        emit propertyChanged(_chatId, "cover");
    }
}

QString GroupChatHandler::title() const
{
    return _title;
}

void GroupChatHandler::setTitle(const QString &title)
{
    if (_title != title)
    {
        _title = title;
        emit propertyChanged(_chatId, "title");
    }
}

int GroupChatHandler::adminId() const
{
    return _adminId;
}

void GroupChatHandler::setAdminId(const int adminId)
{
    if (_adminId != adminId)
    {
        _adminId = adminId;
        emit propertyChanged(_chatId, "adminId");
    }
}

bool GroupChatHandler::isCover() const
{
    return !(_cover.isEmpty());
}

void GroupChatHandler::getAllFields(Connection *connection)
{
    Packet *packet = new Packet("messages.getChat");
    packet->addParam("chat_id", _chatId);
    packet->addParam("fields", "photo_medium_rec");

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    connection->appendQuery(packet);
}

void GroupChatHandler::loadFinished(const Packet *sender, const QVariantMap &result)
{
    QVariantMap response = result.value("response").toMap();

    _title = response.value("title").toString();
    _adminId = response.value("adminId").toInt();
    _cover = response.value("photo_50").toString();
    ProfileList profiles = ProfileParser::parser(response.value("users").toList());

    _users->add(profiles->toVector());

    emit propertyChanged(_chatId, "users");
    delete sender;
}
