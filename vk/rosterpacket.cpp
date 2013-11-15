#include "rosterpacket.h"

RosterPacket::RosterPacket(Connection *connection)
{
    _connection = connection;
    _needFavorites = false;
}

void RosterPacket::load(const int offset, const int count)
{
    Packet *packet;
    _offset = offset;
    _count = count;

    if (_offset == 0 && _needFavorites)
    {
        packet = new Packet("execute");

        QString script = "var favorites=API.friends.get({\"fields\":\"" + _fields + "\",\"count\":5,\"order\":\"hints\"});"
                + "var friends=API.friends.get({\"fields\":\"" + _fields + "\",\"offset\":" + QString::number(_offset)
                + ",\"count\":" + QString::number(_count) + ",\"order\":\"name\"});"
                + "return {\"favorites\":favorites, \"friends\":friends};";

        packet->addParam("code", script);
    }
    else
    {
        packet = new Packet("friends.get");

        packet->addParam("fields", _fields);
        packet->addParam("offset", _offset);
        packet->addParam("count", _count);
        packet->addParam("order", "name");
    }

    connect(packet, SIGNAL(finished(const Packet*,QVariantMap)), this, SLOT(loadFinished(const Packet*,QVariantMap)));
    _connection->appendQuery(packet);
}

int RosterPacket::offset() const
{
    return _offset;
}

int RosterPacket::count() const
{
    return _count;
}

QString RosterPacket::fields() const
{
    return _fields;
}

void RosterPacket::setFields(const QString &fields)
{
    _fields = fields;
}

bool RosterPacket::needFavorites() const
{
    return _needFavorites;
}

void RosterPacket::setNeedFavorites(const bool needFavorites)
{
    _needFavorites = needFavorites;
}

void RosterPacket::loadFinished(const Packet *sender, const QVariantMap &response)
{
    ProfileList *profiles;

    if (sender->method() == "execute")
    {
        profiles = new ProfileList();

        foreach (QVariant item, response.value("favorites").toMap().value("items").toList())
        {
            ProfileItem *profile = ProfileParser::parser(item.toMap());
            profile->setAlphabet(tr("Favorites"));
            profiles->add(profile);
        }

        profiles->add(ProfileParser::parser(response.value("friends").toMap().value("items").toList()));
    }
    else
    {
        profiles = ProfileParser::parser(response.value("items").toList());
    }

    emit roster(this, profiles);
}
