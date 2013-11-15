#ifndef ROSTERPACKET_H
#define ROSTERPACKET_H

#include <QObject>
#include "connection.h"
#include "profileparser.h"

class RosterPacket : public QObject
{
    Q_OBJECT

public:
    RosterPacket(Connection *connection);
    void load(const int offset, const int count);
    int offset() const;
    int count() const;
    QString fields() const;
    void setFields(const QString &fields);
    bool needFavorites() const;
    void setNeedFavorites(const bool needFavorites);

private:
    Connection *_connection;
    int _offset;
    int _count;
    QString _fields;
    bool _needFavorites;

private slots:
    void loadFinished(const Packet *sender, const QVariantMap &response);

signals:
    void roster(const RosterPacket *sender, const ProfileList *roster);
};

#endif // ROSTERPACKET_H
